/* eslint no-console: 0 */

const path = require('path');
const express = require('express');
const webpack = require('webpack');
const webpackMiddleware = require('webpack-dev-middleware');
const webpackHotMiddleware = require('webpack-hot-middleware');
const config = require('./webpack.config.js');
import Ble from "./lib/ble";
import bodyParser from "body-parser";

const ble = new Ble();

const port = process.env.PORT || 3000;
const app = express();
const server = require('http').Server(app);
const io = require('socket.io')(server);

const compiler = webpack(config);
const middleware = webpackMiddleware(compiler, {
    publicPath: config.output.publicPath,
    contentBase: 'src',
    stats: {
        colors: true,
        hash: false,
        timings: true,
        chunks: false,
        chunkModules: false,
        modules: false
    }
});

app.use(middleware);
app.use(bodyParser.json());
app.use(function (req, res, next) {
    console.log(req.originalUrl);
    return next();
});

function peripheralToJson(peripheral) {
    return {
        id: peripheral.id,
        name: peripheral.advertisement.localName
    };
}

app.get('/ble/peripherals', function (req, res) {
    var results = {};
    var peripherals = ble.getPeripherals();

    Object.keys(peripherals).map((key) => {
        var peripheral = peripherals[key];
        results[peripheral.id] = peripheralToJson(peripheral);
    });

    res.send(results);
});

app.post('/ble/connect', function (req, res, next) {
    var peripheralId = req.body.peripheralId;
    if (!peripheralId) {
        return next(new Error('peripheralId is required'));
    }
    var peripheral = ble.getPeripheral(peripheralId);
    if (!peripheral) {
        return res.status(404).send({message: 'Could not find peripheral with id ' + peripheralId});
    }

    ble.connectToPeripheral(peripheralId, function (err, blePeripheral) {
        if (err) {
            return next(err);
        }
        res.send(peripheralToJson(peripheral));
    });
});

app.post('/ble/disconnect', function (req, res, next) {
    var peripheralId = req.body.peripheralId;
    if (!peripheralId) {
        return next(new Error('peripheralId is required'));
    }
    var peripheral = ble.getPeripheral(peripheralId);
    if (!peripheral) {
        return res.status(404).send({message: 'Could not find peripheral with id ' + peripheralId});
    }

    ble.disconnectPeripheral(peripheralId, function (err) {
        if (err) {
            return next(err);
        }
        res.send({});
    });
});

app.get('/ble/:peripheralId/batteryLevel', function (req, res, next) {
    var peripheralId = req.params.peripheralId;
    if (!peripheralId) {
        return next(new Error('peripheralId is required'));
    }
    var peripheral = ble.getConnectedPeripheral(peripheralId);
    if (!peripheral) {
        return res.status(404).send({message: 'Could not find connected peripheral with id ' + peripheralId});
    }
    peripheral.getBatteryLevel((err, batteryLevel) => {
        if (err) {
            return next(err);
        }
        //console.log('got battery level:', batteryLevel);
        res.send({batteryLevel: batteryLevel});
    });
});

app.get('/ble/:peripheralId/sensors', function (req, res, next) {
    var peripheralId = req.params.peripheralId;
    if (!peripheralId) {
        return next(new Error('peripheralId is required'));
    }
    var peripheral = ble.getConnectedPeripheral(peripheralId);
    if (!peripheral) {
        return res.status(404).send({message: 'Could not find connected peripheral with id ' + peripheralId});
    }
    peripheral.getSensors((err, sensorData) => {
        if (err) {
            return next(err);
        }
        //console.log('got sensor data:', sensorData);
        res.send(sensorData);
    });
});

app.get('/ble/:peripheralId/get-motors', function (req, res, next) {
    var peripheralId = req.params.peripheralId;
    if (!peripheralId) {
        return next(new Error('peripheralId is required'));
    }
    var peripheral = ble.getConnectedPeripheral(peripheralId);
    if (!peripheral) {
        return res.status(404).send({message: 'Could not find connected peripheral with id ' + peripheralId});
    }
    peripheral.getMotors((err, motorData) => {
        if (err) {
            return next(err);
        }
        //console.log('got motor data:', motorData);
        res.send(motorData);
    });
});

app.post('/ble/:peripheralId/set-motors', function (req, res, next) {
    var peripheralId = req.params.peripheralId;
    if (!peripheralId) {
        return next(new Error('peripheralId is required'));
    }
    var peripheral = ble.getConnectedPeripheral(peripheralId);
    if (!peripheral) {
        return res.status(404).send({message: 'Could not find connected peripheral with id ' + peripheralId});
    }

    var left = req.body.left;
    if (typeof left == 'undefined') {
        return next(new Error('left is required'));
    }
    var right = req.body.right;
    if (typeof right == 'undefined') {
        return next(new Error('right is required'));
    }

    //console.log('setting motor data: left:', left, ', right:', right);
    peripheral.setMotor(left, right, (err) => {
        if (err) {
            return next(err);
        }
        //console.log('set motor data');
        res.send({});
    });
});

app.get('/ble/:peripheralId/get-configuration', function (req, res, next) {
    var peripheralId = req.params.peripheralId;
    if (!peripheralId) {
        return next(new Error('peripheralId is required'));
    }
    var peripheral = ble.getConnectedPeripheral(peripheralId);
    if (!peripheral) {
        return res.status(404).send({message: 'Could not find connected peripheral with id ' + peripheralId});
    }
    peripheral.getConfiguration((err, configuration) => {
        if (err) {
            return next(err);
        }
        //console.log('got configuration data:', configuration);
        res.send(configuration);
    });
});

app.post('/ble/:peripheralId/set-configuration', function (req, res, next) {
    var peripheralId = req.params.peripheralId;
    if (!peripheralId) {
        return next(new Error('peripheralId is required'));
    }
    var peripheral = ble.getConnectedPeripheral(peripheralId);
    if (!peripheral) {
        return res.status(404).send({message: 'Could not find connected peripheral with id ' + peripheralId});
    }

    console.log('setting configuration data:', req.body);
    peripheral.setConfiguration(req.body, (err) => {
        if (err) {
            return next(err);
        }
        //console.log('set configuration data');
        res.send({});
    });
});

app.use(webpackHotMiddleware(compiler));

server.listen(port, '0.0.0.0', function onStart(err) {
    if (err) {
        console.log(err);
        return;
    }
    console.info('==> Listening http://127.0.0.1:%s/', port);
});

ble.on('discover', function (peripheralId) {
    io.emit('event', {type: 'discover', peripheralId: peripheralId});
});

ble.on('disconnect', function (peripheralId) {
    io.emit('event', {type: 'disconnect', peripheralId: peripheralId});
});

io.on('connection', function (socket) {
    console.log('ws connection');
});

