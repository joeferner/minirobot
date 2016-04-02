import EventEmitter from "events";
import Locks from "locks";

const CHARACTERISTIC_UUID_BATTERY_LEVEL = '2a19';
const SERVICE_UUID = '5a6c53b8470d467d9531f53de5f31e21';
const CHARACTERISTIC_UUID_CONFIGURATION = '82552f9e365049aba0129338954accbc';
const CHARACTERISTIC_UUID_MOTORS = 'b5fb41dc8f5d4ec4b323c9d1bd2b3308';
const CHARACTERISTIC_UUID_SENSORS = 'd8e7746ce8d6473aa157265888a3b153';

function clamp(value, min, max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

function scaleMotorValue(value) {
    value = clamp(value, -100.0, 100.0);
    return value;
}

function isValidColorSensorGain(gain) {
    return !!(gain == 1 || gain == 4 || gain == 16 || gain == 60);

}

function isValidColorSensorLedBrightness(brightness) {
    return !!(brightness >= 0 && brightness <= 100);

}

export default class BlePeripheral extends EventEmitter {
    constructor(peripheral, services, characteristics) {
        super();
        this._lock = Locks.createMutex();
        this._peripheral = peripheral;
        if (process.env.TEST) {
            this._testBatteryLevel = 0;
            //setTimeout(this._handleDisconnect.bind(this), 1000);
            return;
        }

        this._peripheral.once('disconnect', () => {
            this._handleDisconnect();
        });
        characteristics.forEach((characteristic) => {
            if (characteristic.uuid === CHARACTERISTIC_UUID_BATTERY_LEVEL) {
                this._batteryLevelCharacteristic = characteristic;
            } else if (characteristic.uuid === CHARACTERISTIC_UUID_MOTORS) {
                this._motorsCharacteristic = characteristic;
            } else if (characteristic.uuid === CHARACTERISTIC_UUID_SENSORS) {
                this._sensorsCharacteristic = characteristic;
            } else if (characteristic.uuid === CHARACTERISTIC_UUID_CONFIGURATION) {
                this._configurationCharacteristic = characteristic;
            }
        });
        console.log(peripheral, services, characteristics);
    }

    _handleDisconnect() {
        console.log('peripheral disconnected', this._peripheral.id);
        this.emit('disconnect', this._peripheral.id);
        this._peripheral = null; // after a disconnect this object is no longer valid
    }

    getBatteryLevel(callback) {
        if (process.env.TEST) {
            this._testBatteryLevel++;
            if (this._testBatteryLevel > 100) {
                this._testBatteryLevel = 0;
            }
            return callback(null, this._testBatteryLevel);
        }

        if (!this._batteryLevelCharacteristic) {
            return callback(new Error('Could not find battery level characteristic'));
        }
        this._readCharacteristic(this._batteryLevelCharacteristic, (err, data) => {
            if (err) {
                return callback(err);
            }
            console.log('battery level characteristic', data);
            return callback(null, data[0]);
        });
    }

    getSensors(callback) {
        if (process.env.TEST) {
            return callback(null, {
                lineLeftOuter: false,
                lineLeftInner: false,
                lineRightOuter: false,
                lineRightInner: false,
                feelerLeft: false,
                feelerRight: false,
                compass: 240,
                color: {
                    red: 0,
                    green: 0,
                    blue: 255,
                    clear: 0
                }
            });
        }

        if (!this._sensorsCharacteristic) {
            return callback(new Error('Could not find sensor characteristic'));
        }
        this._readCharacteristic(this._sensorsCharacteristic, (err, data) => {
            if (err) {
                return callback(err);
            }
            console.log('sensor characteristic', data);
            var firstByte = data.readUInt8(0);
            var sensorData = {
                lineLeftOuter: (firstByte & 0x01) ? true : false,
                lineLeftInner: (firstByte & 0x02) ? true : false,
                lineRightOuter: (firstByte & 0x04) ? true : false,
                lineRightInner: (firstByte & 0x08) ? true : false,
                feelerLeft: (firstByte & 0x10) ? true : false,
                feelerRight: (firstByte & 0x20) ? true : false,
                compass: data.readUInt16LE(1),
                color: {
                    red: data.readUInt16LE(3),
                    green: data.readUInt16LE(5),
                    blue: data.readUInt16LE(7),
                    clear: data.readUInt16LE(9)
                }
            };
            return callback(null, sensorData);
        });
    }

    getMotors(callback) {
        if (process.env.TEST) {
            return callback(null, {
                left: 0,
                right: 0
            });
        }

        if (!this._sensorsCharacteristic) {
            return callback(new Error('Could not find motor characteristic'));
        }
        this._readCharacteristic(this._motorsCharacteristic, (err, data) => {
            if (err) {
                return callback(err);
            }
            console.log('motor characteristic', data);
            var motorData = {
                left: data.readInt8(0),
                right: data.readInt8(1)
            };
            return callback(null, motorData);
        });
    }

    getConfiguration(callback) {
        if (process.env.TEST) {
            return callback(null, {
                colorSensorGain: 1,
                colorSensorLedBrightness: 50
            });
        }

        if (!this._configurationCharacteristic) {
            return callback(new Error('Could not configuration characteristic'));
        }
        this._readCharacteristic(this._configurationCharacteristic, (err, data) => {
            if (err) {
                return callback(err);
            }
            console.log('configuration characteristic', data);
            var configuration = {
                colorSensorGain: data.readUInt8(0),
                colorSensorLedBrightness: data.readUInt8(1)
            };
            return callback(null, configuration);
        });
    }

    setConfiguration(configuration, callback) {
        if (!isValidColorSensorGain(configuration.colorSensorGain)) {
            return callback(new Error('Invalid color sensor gain: ' + configuration.colorSensorGain));
        }
        if (!isValidColorSensorLedBrightness(configuration.colorSensorLedBrightness)) {
            return callback(new Error('Invalid color sensor brightness: ' + configuration.colorSensorLedBrightness));
        }

        if (process.env.TEST) {
            console.log('set configuration:', configuration);
            return callback();
        }

        if (!this._configurationCharacteristic) {
            return callback(new Error('Could not find configuration characteristic'));
        }
        var data = new Buffer(2);
        data.writeUInt8(configuration.colorSensorGain, 0);
        data.writeUInt8(configuration.colorSensorLedBrightness, 1);
        this._writeCharacteristic(this._configurationCharacteristic, data, callback);
    }

    setMotor(left, right, callback) {
        if (process.env.TEST) {
            console.log('set motors left: ' + left + ', right: ' + right);
            return callback();
        }

        if (!this._motorsCharacteristic) {
            return callback(new Error('Could not find motor characteristic'));
        }
        var data = new Buffer(2);
        data.writeInt8(scaleMotorValue(left), 0);
        data.writeInt8(scaleMotorValue(right), 1);
        this._writeCharacteristic(this._motorsCharacteristic, data, callback);
    }

    _readCharacteristic(characteristic, callback) {
        console.log('_readCharacteristic waiting for lock:', characteristic.uuid);
        this._lock.timedLock(1000, () => {
            console.log('_readCharacteristic:', characteristic.uuid);
            return characteristic.read((err, data) => {
                this._lock.unlock();
                console.log('_readCharacteristic release:', characteristic.uuid, data);
                return callback(err, data);
            });
        });
    }

    _writeCharacteristic(characteristic, data, callback) {
        console.log('_writeCharacteristic waiting for lock:', characteristic.uuid);
        this._lock.timedLock(1000, () => {
            console.log('_writeCharacteristic:', characteristic.uuid, data);
            return characteristic.write(data, false, (err, data) => {
                this._lock.unlock();
                console.log('_writeCharacteristic release:', characteristic.uuid);
                return callback(err, data);
            });
        });
    }
}