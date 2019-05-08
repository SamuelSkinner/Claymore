#!/usr/bin/env node
var b = require('bonescript');
var app = require('http').createServer(handler);
var io = require('/home/debian/web_example/node_modules/socket.io').listen(app);
var fs = require('fs');

var htmlPage = '/var/lib/cloud9/claymore/BUAROTC_claymore.html';
app.listen(8085);

var method = 0;
var boom = 0;
var res = 0;


function handler(req, res) {
    fs.readFile(htmlPage, function(err, data) {
        if (err) {
            res.writeHead(500);
            return res.end('Error loading file: ' + htmlPage);
        }
        res.writeHead(200);
        res.end(data);
    });
}

function onConnect(socket) {
    socket.on('method_choice', handleMethodChoice);
    socket.on('explosion', handleExplosion);
}

function handleMethodChoice(message) {
    method = message;
    fs.writeFile('/var/lib/cloud9/claymore/Method.txt', method, (err) => {
        if (err) throw err;
    });
}

function handleExplosion(message) {
    boom = message;
    fs.writeFile('/var/lib/cloud9/claymore/Trigger.txt', boom, (err) => {
        if (err) throw err;
    });
}

io.sockets.on('connection', onConnect);