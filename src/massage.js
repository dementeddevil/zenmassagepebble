Pebble.addEventListener('ready', function(e) {
  
});

Pebble.addEventListener('showConfiguration', function(e) {
  // Show configuration page
  // TODO: Pass user's timeline user key to config endpoint
  Pebble.openURL('http://azurewebsites/zenmassage/pebble/index');
});

Pebble.addEventListener('webviewclosed', function(e) {
  // Decode and parse config data as JSON
  var config_data = JSON.parse(decodeURIComponent(e.response));
  console.log('configuration window returned: ' + e.response);
  
  // Prepare AppMessage payload
  var dict = {
    'KEY_SESSION_DBLCLK_30': config_data.sessionOptions['toggle-fast30'],
    'KEY_SESSION_DBLCLK_60': config_data.sessionOptions['toggle-fast60'],
    'KEY_VIBE_EVERY_10': config_data.sessionOptions['toggle-vibe10'],
    'KEY_VIBE_EVERY_30': config_data.sessionOptions['toggle-vibe30'],
    'KEY_VIBE_LAST_15': config_data.sessionOptions['toggle-vibe-last15']
  };
  
  // Send settings to Pebble watchapp
  Pebble.sendAppMessage(dict, function() {
    config.log('Sent config data to Pebble');
  }, function() {
    console.log('Failed to send config data!');
  });
});