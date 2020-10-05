const express = require('express');
const app = express();
app.use(express.static('./'));

var server = app.listen(8080, function() {
    
});

app.get('/', function(req, res) {res.sendFile('./index.html')});
app.get('/config', function(req, res) {res.sendFile('config.html')});
app.get('/style_index.css', function(req, res) {res.sendFile('style_index.css')});
app.get('/style_config.css', function(req, res) {res.sendFile('style_config.css')});
app.get('/config.js', function(req, res) {res.sendFile('config.js')});
app.get('/indes.js', function(req, res) {res.sendFile('index.js')});

app.get('/get_config', function(req, res){
    console.log("/get_config called");
    var content = `{ "hostname":"esphost", "wifi-ssid":"mySSID", "wifi-password":"mywifi-pass", "broker-address":"broker-address", "broker-user":"broker-user", "broker-password":"broker-password", "broker-update-interval":1000 }`;

    res.statusCode = 200;
    res.set('Content-Type: application/json')
    res.send(content);
});

app.get('/get_sensors', function(req, res){
    console.log("/get_sensors called");
    var content = `{ "temperature":24.5, "humidity":45.9}`;

    res.statusCode = 200;
    res.set('Content-Type: application/json')
    res.send(content);
});