

var getJSON = function(url, callback) {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);
    xhr.responseType = 'json';
    xhr.onload = function() {
      var status = xhr.status;
      if (status === 200) {
      callback(null, xhr.response);
    } else {
      callback(status, xhr.response);
    }
  };
  xhr.send();
};

function get_data() {
    getJSON("get_config", 
    function(err, data) {
        if (err !== null) {
          alert('Something went wrong: ' + err);
        } else {
            var json_data = JSON.parse(data);
            console.log(json_data);
            document.getElementById('hostname').value = json_data['hostname'];
            document.getElementById('wifi-ssid').value = json_data['wifi-ssid'];
            document.getElementById('wifi-password').value = json_data['wifi-password'];
            document.getElementById('broker-address').value = json_data['broker-address'];
            document.getElementById('broker-user').value = json_data['broker-user'];
            document.getElementById('broker-password').value = json_data['broker-password'];
            document.getElementById('broker-topic').value = json_data['broker-topic'];
            
            document.getElementById('broker-update-inverval').value = json_data['broker-update-interval'];

        }
    });
};

function send_config() {
  console.log('clicked');
  var xhr = new XMLHttpRequest();
  xhr.open("POST", "localhost:8080", true);

  xhr.setRequestHeader('Content-Type', 'application/json');
  xhr.send("{\"test\":\"abc\"}")
}

function setup() {
  document.getElementById('submit').conclick = send_config;
};


setup();

get_data();