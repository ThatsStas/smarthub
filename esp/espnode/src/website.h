#include <string>
const String index_html = R"(
    <!DOCTYPE html>
    <html lang="en">
        <head>
            <meta charset="UTF-8" />
            <meta name="viewport" content="width=device-width, initial-scale=1.0" />
            <!-- <title>ESPNodeConfigurator</title> -->
            <link rel="stylesheet" href="style_index.css" />
            <script src="index.js" defer></script>
        </head>
    <body>
    <h1>ESPNode </h1>
        <div class="content">
            <div class="temperature">
                <div class="content-el">Temperature:</div>
                <div class="quantities">
                    <p id="temperature-p"></p>
                    <p>&deg;C</p>
                </div>
            </div>
            <div class="humidity">
                <div class="content-el">Humidity:</div>
                <div class="quantities">
                    <p id="humidity-p"></p>
                    <p>%</p>
                </div>
            </div>
        </div>
        
    </body></html>
)";

const String index_js = R"(
    var getJSON = function(url, callback) {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);
    // xhr.responseType = 'json';
    xhr.onload = function() {
            var status = xhr.status;
            if (status === 200) {
                callback(null, xhr.response);
            } else {
                callback(status, xhr.response);
            }
        };
        xhr.send(200);
    };

    function get_sensors() {
        getJSON("sensors", 
        function(err, data) {
            if (err !== null) {
            alert('Something went wrong: ' + err);
            } else {
                var json_data = JSON.parse(data);
                console.log(json_data);
                document.getElementById('temperature-p').innerHTML = json_data["temperature"];
                document.getElementById('humidity-p').innerHTML = json_data["humidity"];

            }
        });
    };

    // set_data();

    setInterval(get_sensors, 5000);
)";

const String index_css = R"(
    * {
        box-sizing: border-box;
        align-items: center;

    }

    body {
        background-size: cover;
        display: flex;
        flex-direction: column;
        background-position: center center;
        align-items: center;
        min-height: 100vh;
        font-family: sans-serif;
        margin: 0;
        align-items: center;

    }

    .temperature, .humidity {
        font-size: 2rem;
        border: 1px solid black;
        border-radius: 5px;
        width: 500px;

        margin: 1rem;
    }

    .quantities, .content-el {
        margin-top: 1rem;
        text-align: center;
        align-items: center;
        display: flex;
        flex-direction: row;

        flex-wrap: column;
        justify-content: center;
    }
)";

const String config_html = R"(
<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <!-- <title>ESPNodeConfigurator</title> -->
        <link rel="stylesheet" href="style_config.css" />
        <script src="config.js" defer></script>

    </head>

<body>
<h1> Welcome to the ESP8266 </h1>
<div class="config">
    <span> Hostname: </span>
    <input
        class="hostname"
        type="text"
        id="hostname"
        placeholder="hostname"
        autocomplete="off"
    />

    <span> WiFi SSID: </span>
    <input
        class="ssid"
        type="text"
        id="wifi-ssid"
        placeholder="SSID"
        autocomplete="off"
        
    />

    <span> WiFI Password: </span>
    <input
        class="wifi-password"
        type="password"
        id="wifi-password"
        placeholder="***"
        autocomplete="off"
    />

    <span> Broker Address: </span>
    <input
        class="broker-address"
        type="text"
        id="broker-address"
        placeholder="broker address"
        autocomplete="off"
    />
    
    <span> Broker User: </span>
    <input
        class="broker-user"
        type="text"
        id="broker-user"
        placeholder="broker user"
        autocomplete="off"
    />

    <span> Broker Password: </span>
    <input
        class="broker-password"
        type="password"
        id="broker-password"
        placeholder="***"
        autocomplete="off"
    />

    <span> Broker Topic: </span>
    <input
        class="broker-topic"
        type="text"
        id="broker-topic"
        placeholder="topic"
        autocomplete="off"
    />
        
    <span> Update Interval: </span>
    <input
        class="broker-update-inverval"
        type="number"
        id="broker-update-inverval"
        placeholder="Interval"
        autocomplete="off"
    />

    <button id="submit" class="submit-button">Sumbit</button>
</div>
</body>

</html>
)";

const String config_css = R"(
* {
    box-sizing: border-box;
    align-items: center;
    /* display: grid; */

}

body {
    background-size: cover;
    background-position: center center;
    display: flex;
    flex-direction: column;
    align-items: center;
    min-height: 100vh;
    font-family: sans-serif;
    margin: 0;
}

h1 {
    font-size: large;
}

.broker-update-interval, .submit-button {
    display: flex;
}

input {
    margin-bottom: 1rem;
}

input, .submit-button {
    border-radius: 5px;
    border: 1px solid black;
    text-align: center;
    display: block;
    width: 100%;
}

span{
    display: table-row;
}

.submit-button {
    margin-top: 2rem;
}

)";

const String config_js = R"(


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
            console.log(data);
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
  xhr.open("POST", "localhost:8080/sensors", true);

  xhr.setRequestHeader('Content-Type', 'application/json');
  xhr.send("{\"test\":\"abc\"}")
}

function setup() {
  document.getElementById('submit').conclick = send_config;
};


setup();

get_data();
)";