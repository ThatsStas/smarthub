#include <string>

const std::string website = R"(
<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <title>Countdown Timer</title>
        <!-- <link rel="stylesheet" href="css/style.css" /> -->
        <!-- <script src="js/script.js" defer></script> -->
    </head>

<body>
<h1> Welcome to the ESP8266 </h1>

</body>

</html>
)";

const std::string config_html = R"(
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

const std::string config_css = R"(
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

const std::string config_js = R"(
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


// function get_data() {
//     var request = new XMLHttpRequest();
//     request.open('GET', 'http://127.0.0.1:8080/get_config', true);
//     console.log("Trying to get json");

//     request.onload = function() {
//     if (request.status >= 200 && request.status < 400) {
//         // Success!
//         var data = JSON.parse(request.responsevalue);

//         // var json_data = JSON.parse(data);
//         console.log("Data obtained from esp: " + data[0]);
    
//         document.getElementById('hostname').value = data.hostname;
//     } else {
//         // We reached our target server, but it returned an error

//     }

//     request.send();
//     };

//     request.onerror = function() {
//     // There was a connection error of some sort
//     };
// }

get_data();

)";