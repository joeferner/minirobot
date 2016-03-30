import React from 'react';
import Es6Promise from 'es6-promise';
import qwest from 'qwest';
import PeripheralsList from './PeripheralsList.jsx';
import ConnectedPeripheral from './ConnectedPeripheral.jsx';
const socket = io();
qwest.setDefaultDataType('json');

Es6Promise.polyfill();

export default class App extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      peripherals: {},
      selectedPeripheralId: null
    };

    socket.on('connect', () => {
      console.log('socket connect');
    });
    socket.on('event', (message) => {
      this.onWebSocketMessage(message);
    });
    socket.on('disconnect', () => {
      console.log('socket disconnect');
    });
    
    this.refreshPeripheralsList();
  }

  onWebSocketMessage(message) {
    console.log('socket event', message);
    if (message.type === 'disconnect') {
      this.setState({'selectedPeripheralId': null});
    } else if (message.type === 'discover') {
      this.refreshPeripheralsList();
    }
  }

  refreshPeripheralsList() {
    qwest.get('/ble/peripherals')
      .then((xhr, peripherals) => {
        this.setState({'peripherals': peripherals});
	    });
  }
  
  handlePeripheralClick(peripheralId) {
    qwest.post('/ble/connect', { peripheralId: peripheralId })
      .then(() => {
        this.setState({'selectedPeripheralId': peripheralId});
      });
  }
  
  handleDisconnect() {
    qwest.post('/ble/disconnect', { peripheralId: this.state.selectedPeripheralId });
  }

  render() {
    if (this.state.selectedPeripheralId) {
      var selectedPeripheral = this.state.peripherals[this.state.selectedPeripheralId];
      return (
        <div>
          <ConnectedPeripheral onDisconnectClick={this.handleDisconnect.bind(this)} peripheral={selectedPeripheral} />
        </div>
      );
    } else {
      return (
        <div>
          <PeripheralsList onPeripheralClick={this.handlePeripheralClick.bind(this)} peripherals={this.state.peripherals} />
        </div>
      );
    }
  }
}
