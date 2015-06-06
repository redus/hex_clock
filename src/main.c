#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_date_layer, *s_time_layer, *s_battery_layer;
static GFont s_font, s_battery_font;


// battery text
static void battery_handler(BatteryChargeState charge_state) {
	static char battery_text[4] = "64";

	/*
	if (charge_state.is_charging) {
		snprintf(battery_text, sizeof(battery_text), "charging");
	} else {
    	snprintf(battery_text, sizeof(battery_text), "%d%%", charge_state.charge_percent);
 	}
	*/
	
	snprintf(battery_text, sizeof(battery_text), "%x", charge_state.charge_percent);
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
static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
	static char time[16];
	char hr[4], min[4], sec[4];
	itoa(tick_time->tm_hour, hr, 16);
	itoa(tick_time->tm_min, min, 16);
	itoa(tick_time->tm_sec, sec, 16);
	snprintf(time, sizeof("00:00:00"), "%s:%s:%s", hr, min, sec);
	text_layer_set_text(s_time_layer, time);
	
	static char date[16];
	char day[4], mon[4], year[4];
	itoa(tick_time->tm_year % 100, year, 16);
	itoa((tick_time->tm_mon) + 1, mon, 16);
	itoa(tick_time->tm_mday, day, 16);
	snprintf(date, sizeof("99.01.01"), "%s|%s|%s", year, mon, day);
	text_layer_set_text(s_date_layer, date);
}

static void main_window_load(Window *window){
	window_set_background_color(s_main_window, GColorBlack);
	s_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SQUARE_34));
	s_battery_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SQUARE_16));
	
	// battery layer above date layer
	s_battery_layer = text_layer_create(GRect(36, 30, 72, 20));
	text_layer_set_background_color(s_battery_layer, GColorClear);
	text_layer_set_text_color(s_battery_layer, COLOR_FALLBACK(GColorGreen, GColorWhite));
	text_layer_set_font(s_battery_layer, s_battery_font);
	text_layer_set_text_alignment(s_battery_layer, GTextAlignmentCenter);
	battery_handler(battery_state_service_peek());
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_battery_layer));
	
	// date layer on top
	s_date_layer = text_layer_create(GRect(0, 44, 144, 36));
	text_layer_set_background_color(s_date_layer, GColorClear);
 	text_layer_set_text_color(s_date_layer, COLOR_FALLBACK(GColorGreen, GColorWhite));
	text_layer_set_font(s_date_layer, s_font);
	text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
	
	// time layer on bottom
	s_time_layer = text_layer_create(GRect(0, 78, 144, 36));
	text_layer_set_background_color(s_time_layer, GColorClear);
  	text_layer_set_text_color(s_time_layer, COLOR_FALLBACK(GColorGreen, GColorWhite));
	text_layer_set_font(s_time_layer, s_font);
	text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window){
	text_layer_destroy(s_date_layer);
	text_layer_destroy(s_time_layer);
	text_layer_destroy(s_battery_layer);
}

static void init(){
	// register w tick_timer_service
	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
	battery_state_service_subscribe(battery_handler);
	
	s_main_window = window_create();
	window_set_window_handlers(s_main_window, (WindowHandlers){
		.load = main_window_load,
		.unload = main_window_unload
	});
	window_stack_push(s_main_window, true); //animated
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