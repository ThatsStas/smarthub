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