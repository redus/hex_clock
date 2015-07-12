#include <pebble.h>
#include <translate.h>

static Window *s_main_window;
static TextLayer *s_date_layer, *s_time_layer, *s_battery_layer, *s_day_layer;
static GColor s_background_color, s_font_color;
static GFont s_font, s_small_font;
static char *date_format = "yymmdd";
static Locale locale;
static int weekday_start = 0;
// flags used as int instaed of bool
// because translating js bool from appmessage to c value is hassle.
static int day_zero = 0,
	month_zero = 0,
	battery_on = 1,
	percent_sign = 1,
	weekday_on = 1,
	weekday_named = 0;

enum Settings {KEY_DATE_FORMAT, KEY_MONTH_ZERO, KEY_DAY_ZERO, KEY_BATTERY_ON,
			   KEY_PERCENT_SIGN, KEY_WEEKDAY_ON, KEY_WEEKDAY_NAMED, KEY_WEEKDAY_START,
			  KEY_WEEKDAY_LANG, KEY_COLOR_FONT, KEY_COLOR_BACKGROUND};

static GColor set_color(char* value){
	// default to black
	if (strcmp(value, "w") == 0){
		return GColorWhite;
	} else if (strcmp(value, "g") == 0){
		return COLOR_FALLBACK(GColorGreen, GColorWhite);
	} else if (strcmp(value, "bl") == 0){
		return COLOR_FALLBACK(GColorCobaltBlue, GColorBlack);
	} else if (strcmp(value, "p") == 0){
		return COLOR_FALLBACK(GColorImperialPurple, GColorBlack);
	} else {
		return GColorBlack;
	}
}

// load settings from persistent storage
static void load_settings(){
	if (persist_exists(KEY_DATE_FORMAT)){
		persist_read_string(KEY_DATE_FORMAT, date_format, sizeof("yymmdd"));
	} 
	if (persist_exists(KEY_MONTH_ZERO)){
		month_zero = persist_read_int(KEY_MONTH_ZERO);
	} 
	if (persist_exists(KEY_DAY_ZERO)){
		day_zero = persist_read_int(KEY_DAY_ZERO);
	}
	if (persist_exists(KEY_BATTERY_ON)){
		battery_on = persist_read_int(KEY_BATTERY_ON);
	} 
	if (persist_exists(KEY_PERCENT_SIGN)){
		percent_sign = persist_read_int(KEY_PERCENT_SIGN);
	}
	if (persist_exists(KEY_WEEKDAY_ON)){
		weekday_on = persist_read_int(KEY_WEEKDAY_ON);
	} 
	if (persist_exists(KEY_WEEKDAY_NAMED)){
		weekday_named = persist_read_int(KEY_WEEKDAY_NAMED);
	}
	if (persist_exists(KEY_WEEKDAY_START)){
		weekday_start = persist_read_int(KEY_WEEKDAY_START);
	}
	
	if (persist_exists(KEY_WEEKDAY_LANG)){
		char* temp = "";
		persist_read_string(KEY_WEEKDAY_LANG, temp, sizeof("ENF"));
		locale = set_locale(temp);
		// APP_LOG(APP_LOG_LEVEL_DEBUG, "what is the locale: %s", locale);
	} else {
		locale = set_locale("EN");
	}
	if (persist_exists(KEY_COLOR_FONT)){
		char* temp = "";
		persist_read_string(KEY_COLOR_FONT, temp, PERSIST_STRING_MAX_LENGTH);
		s_font_color = set_color(temp);
	} else {
		s_font_color = set_color("g");
	}
	if (persist_exists(KEY_COLOR_BACKGROUND)){
		char* temp = "";
		persist_read_string(KEY_COLOR_BACKGROUND, temp, PERSIST_STRING_MAX_LENGTH);
		s_background_color = set_color(temp);
	} else {
		s_background_color = set_color("b");
	}
}

// REQ: battery_on is true
// battery text
static void battery_handler(BatteryChargeState charge_state) {
	static char battery_text[4] = "64";
	if (percent_sign){
		snprintf(battery_text, sizeof(battery_text), "%x%%", charge_state.charge_percent);
	} else {
		snprintf(battery_text, sizeof(battery_text), "%x", charge_state.charge_percent);
	}
  	text_layer_set_text(s_battery_layer, battery_text);
}

/**
 * Ansi C "itoa" based on Kernighan & Ritchie's "Ansi C":
 */
static void strreverse(char* begin, char* end) {
	char aux;
	while(end>begin)
		aux=*end, *end--=*begin, *begin++=aux;
}
	
static void itoa(int value, char* str, int base) {
	static char num[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char* wstr=str;
	// int sign;
	int count = 0;
	
	// Validate base
	if (base<2 || base>35){ *wstr='\0'; return; }

	// ignore sign, always positive
	// if ((sign=value) < 0) value = -value;
	
	// Conversion. Number is reversed.
	do{
		*wstr++ = num[value%base]; 
		count++;
	} while(value/=base);
	
	// if only one digit was converted, append '0'
	if(count == 1) *wstr++ = '0';
	// if(sign < 0) *wstr++='-';
	*wstr='\0';
	
	// Reverse string
	strreverse(str,wstr-1);
}

static void set_date(char *date, const char *year, const char *mon, const char *day){
	if (strcmp(date_format, "mmddyy") == 0){
		snprintf(date, sizeof("01|23|99"), "%s|%s|%s", mon, day, year);
	} else if (strcmp(date_format, "ddmmyy") == 0){
		snprintf(date, sizeof("23|01|99"), "%s|%s|%s", day, mon, year);
	} else {
		snprintf(date, sizeof("99|01|23"), "%s|%s|%s", year, mon, day);
	}
	text_layer_set_text(s_date_layer, date);
}
static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
	static char time[16];
	char hr[4], min[4], sec[4];
	
	if (clock_is_24h_style()){
		itoa(tick_time->tm_hour, hr, 16);
	} else {
		itoa(tick_time->tm_hour / 12, hr, 16);
	}
	itoa(tick_time->tm_min, min, 16);
	itoa(tick_time->tm_sec, sec, 16);
	snprintf(time, sizeof("00:00:00"), "%s:%s:%s", hr, min, sec);
	text_layer_set_text(s_time_layer, time);
	
	// can't divide into two separate tick handlers
	static char date[16];
	char day[4], mon[4], year[4];
	itoa(tick_time->tm_year % 100, year, 16);
	if (month_zero){
		itoa(tick_time->tm_mon, mon, 16);
	} else {
		itoa((tick_time->tm_mon) + 1, mon, 16);
	}
	if (day_zero){
		itoa((tick_time->tm_mday) - 1, day, 16);
	} else {
		itoa(tick_time->tm_mday, day, 16);
	}
	set_date(date, year, mon, day); 
	
	if (!weekday_on){ return;}
	
	static char weekday[4];
	if (weekday_named){
		text_layer_set_text(s_day_layer, get_weekday(tick_time->tm_wday, locale));
	} else {
		itoa((tick_time->tm_wday - weekday_start + 7) % 7, weekday, 16);
		text_layer_set_text(s_day_layer, weekday);
	}
}

static void main_window_load(Window *window){
	s_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SQUARE_34));
	s_small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SQUARE_16));
	window_set_background_color(s_main_window, s_background_color);
	
	// battery layer above date layer
	s_battery_layer = text_layer_create(GRect(36, 30, 72, 20));
	text_layer_set_background_color(s_battery_layer, GColorClear);
	text_layer_set_text_color(s_battery_layer, s_font_color);
	text_layer_set_font(s_battery_layer, s_small_font);
	text_layer_set_text_alignment(s_battery_layer, GTextAlignmentCenter);
	battery_handler(battery_state_service_peek());
	layer_set_hidden(text_layer_get_layer(s_battery_layer), !battery_on);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_battery_layer));
	
	// date layer on top
	s_date_layer = text_layer_create(GRect(0, 44, 144, 36));
	text_layer_set_background_color(s_date_layer, GColorClear);
 	text_layer_set_text_color(s_date_layer, s_font_color);
	text_layer_set_font(s_date_layer, s_font);
	text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
	
	// time layer on bottom
	s_time_layer = text_layer_create(GRect(0, 78, 144, 36));
	text_layer_set_background_color(s_time_layer, GColorClear);
  	text_layer_set_text_color(s_time_layer, s_font_color);
	text_layer_set_font(s_time_layer, s_font);
	text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
	
	// day layer below time
	s_day_layer = text_layer_create(GRect(36, 115, 72, 20));
	text_layer_set_background_color(s_day_layer, GColorClear);
  	text_layer_set_text_color(s_day_layer, s_font_color);
	text_layer_set_font(s_day_layer, s_small_font);
	text_layer_set_text_alignment(s_day_layer, GTextAlignmentCenter);
	layer_set_hidden(text_layer_get_layer(s_day_layer), !weekday_on);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_day_layer));
	
	// load time
	time_t temp = time(NULL);
 	tick_handler(localtime(&temp), SECOND_UNIT);
}

static void main_window_unload(Window *window){
	text_layer_destroy(s_date_layer);
	text_layer_destroy(s_time_layer);
	text_layer_destroy(s_battery_layer);
	text_layer_destroy(s_day_layer);
}

char* app_message_error_text(AppMessageResult reason){
	char* log_message;
	
	switch(reason){
		case APP_MSG_SEND_TIMEOUT:
			log_message = "Timed out. (APP_MSG_SEND_TIMEOUT)";
		break;
		case APP_MSG_SEND_REJECTED:
			log_message = "Rejected sent data with NACK. (APP_MSG_SEND_REJECTED)";
		break;
		case APP_MSG_NOT_CONNECTED:
			log_message = "Not connected to web. (APP_MSG_NOT_CONNECTED)";
		break;
		case APP_MSG_APP_NOT_RUNNING:
			log_message = "Local app not running. (APP_MSG_APP_NOT_RUNNING)";
		break;
		case APP_MSG_INVALID_ARGS:
			log_message = "Invalid arguments given. (APP_MSG_INVALID_ARGS)";
		break;
		case APP_MSG_BUSY:
			log_message = "Pending message(s). (APP_MSG_BUSY)";
		break;
		case APP_MSG_BUFFER_OVERFLOW:
			log_message = "Buffer overflow. (APP_MSG_BUFFER_OVERFLOW)";
		break;
		case APP_MSG_ALREADY_RELEASED:
			log_message = "Resource released. (APP_MSG_ALREADY_RELEASED)";
		break;
		case APP_MSG_CALLBACK_ALREADY_REGISTERED:
			log_message = "Callback already registered. (APP_MSG_CALLBACK_ALREADY_REGISTERED)";
		break;
		case APP_MSG_CALLBACK_NOT_REGISTERED:
			log_message = "Callback not registered. (APP_MSG_CALLBACK_NOT_REGISTERED)";
		break;
		case APP_MSG_OUT_OF_MEMORY:
			log_message = "Out of memory. (APP_MSG_OUT_OF_MEMORY)";
		break;
		case APP_MSG_CLOSED:
			log_message = "AppMessage closed. (APP_MSG_CLOSED)";
		break;
		case APP_MSG_INTERNAL_ERROR:
			log_message = "Internal OS error (APP_MSG_INTERNAL_ERROR)";
		break;
		default:
			log_message = "Unknown reason";
	}
	return log_message;
}

// convert JS 'True/False' to bool values
static bool js_bool(char* value){
	return strcmp(value, "True") == 0;
}

static void refresh_background_color(char* value){
	s_background_color = set_color(value);
	window_set_background_color(s_main_window, s_background_color);
}

static void refresh_font_color(char* value){
	s_font_color = set_color(value);
	text_layer_set_text_color(s_date_layer, s_font_color);
	text_layer_set_text_color(s_time_layer, s_font_color);
	text_layer_set_text_color(s_battery_layer, s_font_color);
	text_layer_set_text_color(s_day_layer, s_font_color);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
	Tuple *t = dict_read_first(iterator);

	// Process all pairs present
	while(t != NULL) {
		switch (t->key) {
			case KEY_DATE_FORMAT: 
				date_format = t->value->cstring;
				persist_write_string(KEY_DATE_FORMAT, date_format);
			break;
			case KEY_MONTH_ZERO:
				month_zero = t->value->int8;
				persist_write_int(KEY_MONTH_ZERO, month_zero);
				// APP_LOG(APP_LOG_LEVEL_DEBUG, "Month zero: %d", month_zero);
			break;
			case KEY_DAY_ZERO:
				day_zero = t->value->int8; 
				persist_write_int(KEY_DAY_ZERO, day_zero);
				// APP_LOG(APP_LOG_LEVEL_DEBUG, "Day zero: %d", day_zero);
			break;
			case KEY_BATTERY_ON:
				battery_on = t->value->int8; 
				persist_write_int(KEY_BATTERY_ON, battery_on);
				layer_set_hidden(text_layer_get_layer(s_battery_layer), !battery_on);
			break;
			case KEY_PERCENT_SIGN:
				percent_sign = t->value->int8; 
				persist_write_int(KEY_PERCENT_SIGN, percent_sign);
				battery_handler(battery_state_service_peek());
			break;
			case KEY_WEEKDAY_ON: 
				weekday_on = t->value->int8; 
				persist_write_int(KEY_WEEKDAY_ON, weekday_on);
				layer_set_hidden(text_layer_get_layer(s_day_layer), !weekday_on);
			break;
			case KEY_WEEKDAY_NAMED:
				// APP_LOG(APP_LOG_LEVEL_DEBUG, "named value %s", t->value->cstring);
				weekday_named = t->value->int8; 
				persist_write_int(KEY_WEEKDAY_NAMED, weekday_named);
			break;
			case KEY_WEEKDAY_START:
				weekday_start = t->value->int8;
				persist_write_int(KEY_WEEKDAY_START, weekday_start);
				// APP_LOG(APP_LOG_LEVEL_DEBUG, "weekday start %d", weekday_start);
			break;
			case KEY_WEEKDAY_LANG:
				locale = set_locale(t->value->cstring);
				persist_write_string(KEY_WEEKDAY_LANG, t->value->cstring);
			break;
			case KEY_COLOR_BACKGROUND:
				persist_write_string(KEY_COLOR_BACKGROUND, t->value->cstring);
				refresh_background_color(t->value->cstring);
			break;
			case KEY_COLOR_FONT:
				persist_write_string(KEY_COLOR_FONT, t->value->cstring);
				refresh_font_color(t->value->cstring);
			break;
			default:
				APP_LOG(APP_LOG_LEVEL_ERROR, "Unrecognized key %d with value %s",
						(int) t->key, t->value->cstring);
		}
		t = dict_read_next(iterator);
	}
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped: %s", app_message_error_text(reason));
}

static void init(){
	load_settings();
	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
	battery_state_service_subscribe(battery_handler);
	
	s_main_window = window_create();
	window_set_window_handlers(s_main_window, (WindowHandlers){
		.load = main_window_load,
		.unload = main_window_unload
	});
	window_stack_push(s_main_window, true); //animated
	
	app_message_register_inbox_received(inbox_received_callback);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	app_message_register_inbox_dropped(inbox_dropped_callback);
}

static void deinit(){
	window_destroy(s_main_window);
	tick_timer_service_unsubscribe();
	battery_state_service_unsubscribe();
}

int main(void){
	init();
	app_event_loop();
	deinit();
}