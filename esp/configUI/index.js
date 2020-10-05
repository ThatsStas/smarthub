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

function set_data() {
    getJSON("get_sensors", 
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

setInterval(set_data, 1000);