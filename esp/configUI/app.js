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