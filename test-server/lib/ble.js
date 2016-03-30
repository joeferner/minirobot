import EventEmitter from 'events';
import noble from 'noble';
import BlePeripheral from './blePeripheral';

export default class Ble extends EventEmitter {
  constructor() {
    super();
    this._peripherals = {};
    this._connectedPeripherals = {};
    
    noble.on('stateChange', (state) => {
      console.info('noble stateChange', state);
      if (state === 'poweredOn') {        
        noble.startScanning([], true);
      } else {
        noble.stopScanning();
      }
    });
    
    noble.on('discover', (peripheral) => {
      if (!this._peripherals[peripheral.id]) {
        this._peripherals[peripheral.id] = peripheral;
        this.emit('discover', peripheral.id);
      }
    });
  }
  
  getPeripherals() {
    return this._peripherals;
  }
  
  getPeripheral(id) {
    return this._peripherals[id];
  }
  
  getConnectedPeripheral(id) {
    return this._connectedPeripherals[id];
  }
  
  connectToPeripheral(id, callback) {
    var peripheral = this.getPeripheral(id);
    
    peripheral.connect((err) => {
      if (err) {
        return callback(err);
      }
      peripheral.discoverAllServicesAndCharacteristics((err, services, characteristics) => {
        if (err) {
          peripheral.disonnect();
          return callback(err);
        }
        var blePeripheral = new BlePeripheral(peripheral, services, characteristics);
        this._connectedPeripherals[peripheral.id] = blePeripheral;
        blePeripheral.on('disconnect', () => {
          delete this._connectedPeripherals[peripheral.id];
          this.emit('disconnect', peripheral.id);
        });
        return callback(null, blePeripheral);
      });
    });
  }
}
