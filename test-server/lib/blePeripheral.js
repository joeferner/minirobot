import EventEmitter from 'events';

const CHARACTERISTIC_UUID_BATTERY_LEVEL = '2a19';

export default class BlePeripheral extends EventEmitter {
  constructor(peripheral, services, characteristics) {
    super();
    this._peripheral = peripheral;
    if (process.env.TEST) {
      this._testBatteryLevel = 0;
      return;
    }

    this._peripheral.on('disconnect', () => {
      this.emit('disconnect');
    });
    characteristics.forEach((characteristic) => {
      if (characteristic.uuid === CHARACTERISTIC_UUID_BATTERY_LEVEL) {
        this._batteryLevelCharacteristic = characteristic;      
      }
    });
    console.log(peripheral, services, characteristics);
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
    this._batteryLevelCharacteristic.read((err, data) => {
      if (err) {
        return callback(err);
      }
      console.log('battery level characteristic', data);
      return callback(null, data[0]);
    });
  }
}