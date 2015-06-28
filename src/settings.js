var data_success = function(e) {
	console.log('Successfully set settings.');
};

var data_error = function(e) {
    console.log('Failed to send settings to Pebble. ' + e.error);
};

var send_settings = function() {
	Pebble.sendAppMessage(
			{"KEY_DATE_FORMAT": localStorage.getItem("KEY_DATE_FORMAT"),
			"KEY_MONTH_ZERO": parseInt(localStorage.getItem("KEY_MONTH_ZERO"), 10),
			"KEY_DAY_ZERO": parseInt(localStorage.getItem("KEY_DAY_ZERO"), 10),
			"KEY_BATTERY_ON": parseInt(localStorage.getItem("KEY_BATTERY_ON"), 10),
			"KEY_PERCENT_SIGN": parseInt(localStorage.getItem("KEY_PERCENT_SIGN"), 10),
			"KEY_WEEKDAY_ON": parseInt(localStorage.getItem("KEY_WEEKDAY_ON"), 10),
			"KEY_WEEKDAY_NAMED": parseInt(localStorage.getItem("KEY_WEEKDAY_NAMED"), 10),
			"KEY_WEEKDAY_START": parseInt(localStorage.getItem("KEY_WEEKDAY_START"), 10),
			"KEY_WEEKDAY_LANG": localStorage.getItem("KEY_WEEKDAY_LANG"),
			"KEY_COLOR_FONT": localStorage.getItem("KEY_COLOR_FONT"),
			"KEY_COLOR_BACKGROUND": localStorage.getItem("KEY_COLOR_BACKGROUND")},
			data_success, data_error
		);
};

Pebble.addEventListener ('ready',function(e){
	console.log('Loading previous settings. (if any)');
	send_settings();
});

Pebble.addEventListener('showConfiguration', function(e) {
	var url = 'http://www-personal.umich.edu/~jinmoy/yahc/aplite.html';
	// comment out until it works
	/*
	var watch = Pebble.getActiveWatchInfo();
	if (watch && watch.platform == 'basalt'){
		url = 'http://www-personal.umich.edu/~jinmoy/yahc/basalt.html';
	}
	*/
	
	var settings = {'date_format': localStorage.getItem("KEY_DATE_FORMAT"),
					'month_zero': localStorage.getItem("KEY_MONTH_ZERO"),
					'day_zero': localStorage.getItem("KEY_DAY_ZERO"),
					'battery_on': localStorage.getItem("KEY_BATTERY_ON"),
					'percent_sign': localStorage.getItem("KEY_PERCENT_SIGN"),
					'weekday_on': localStorage.getItem("KEY_WEEKDAY_ON"),
					'weekday_named': localStorage.getItem("KEY_WEEKDAY_NAMED"),
					'weekday_start': localStorage.getItem("KEY_WEEKDAY_START"),
					'weekday_lang': localStorage.getItem("KEY_WEEKDAY_LANG")};
					
	if (localStorage.getItem("KEY_COLOR_FONT")){
		settings['color_scheme'] = localStorage.getItem("KEY_COLOR_FONT") +
			'/'	+ localStorage.getItem("KEY_COLOR_BACKGROUND");
	}
	
	var prefix = '?';
	for (var option in settings) {
		// check the option is not null
		if (settings.hasOwnProperty(option) && settings[option]) {
			url += prefix.concat(option, '=', settings[option]);
		}
		prefix = '&';
	}
	Pebble.openURL(url);
});

Pebble.addEventListener("webviewclosed",
	function(e) {
		if (!e.response){ return; }    
		//Get JSON dictionary
		var configuration = JSON.parse(decodeURIComponent(e.response));
		console.log("Configuration window returned: " + JSON.stringify(configuration));
 
		var color_pair = configuration.color_scheme.split('/');
		
		// store settings to local storage
		localStorage.setItem("KEY_DATE_FORMAT", configuration.date_format);
		localStorage.setItem("KEY_MONTH_ZERO", configuration.month_zero);
		localStorage.setItem("KEY_DAY_ZERO", configuration.day_zero);
		localStorage.setItem("KEY_BATTERY_ON", configuration.battery_on);
		localStorage.setItem("KEY_PERCENT_SIGN", configuration.percent_sign);
		localStorage.setItem("KEY_WEEKDAY_ON", configuration.weekday_on);
		localStorage.setItem("KEY_WEEKDAY_NAMED", configuration.weekday_named);
		localStorage.setItem("KEY_WEEKDAY_START", configuration.weekday_start);
		localStorage.setItem("KEY_WEEKDAY_LANG", configuration.weekday_lang);
		localStorage.setItem("KEY_COLOR_FONT", color_pair[0]);
		localStorage.setItem("KEY_COLOR_BACKGROUND", color_pair[1]);
		send_settings();
	}
);


