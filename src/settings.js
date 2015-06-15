
Pebble.addEventListener('showConfiguration', function(e) {
	// comment out until it works
	/*
	var watch = Pebble.getActiveWatchInfo();
	if (watch && watch.platform == 'basalt'){
		Pebble.openURL('http://www-personal.umich.edu/~jinmoy/yahc/aplite.html');
	} else {
		Pebble.openURL('http://www-personal.umich.edu/~jinmoy/yahc/basalt.html');
	}
	*/
	Pebble.openURL('http://www-personal.umich.edu/~jinmoy/yahc/aplite.html');
});

var data_success = function(e) {
    console.log('Successfully sent settings');
};

var data_error = function(e) {
    console.log('Failed to send settings to Pebble. ' + e.error);
};

Pebble.addEventListener("webviewclosed",
  function(e) {
	if (!e.response){ return; }    
    //Get JSON dictionary
    var configuration = JSON.parse(decodeURIComponent(e.response));
    console.log("Configuration window returned: " + JSON.stringify(configuration));
 
    Pebble.sendAppMessage(
		{"KEY_DATE_FORMAT": configuration.date_format,
		"KEY_MONTH_ZERO": configuration.month_zero,
		"KEY_DAY_ZERO": configuration.day_zero,
		"KEY_BATTERY_ON": configuration.battery_on,
		"KEY_PERCENT_SIGN": configuration.percent_sign,
		"KEY_WEEKDAY_ON": configuration.weekday_on,
		"KEY_WEEKDAY_NAMED": configuration.named,
		"KEY_WEEKDAY_START": configuration.weekday_start},
      data_success, data_error
    );
  }
);


