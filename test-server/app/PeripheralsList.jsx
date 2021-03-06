import React from 'react';
import qwest from "qwest";
import {Link} from "react-router";

export default class PeripheralsList extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      peripherals: []
    }
  }

  componentDidMount() {
    this._socketioMessageEventListener = this.onWebSocketMessage.bind(this);
    document.addEventListener('socketio.message', this._socketioMessageEventListener);
    this.refreshPeripherals();
  }

  componentWillUnmount() {
    document.removeEventListener('socketio.message', this._socketioMessageEventListener);
  }

  onWebSocketMessage(e) {
    var message = e.detail;
    if (message.type === 'discover') {
      this.forceUpdate();
    }
  }

  refreshPeripherals() {
    qwest.get('/ble/peripherals')
        .then((xhr, peripherals) => {
          this.setState({'peripherals': peripherals});
        });
  }
  
  _mapPeripherals(callback) {
    return Object.keys(this.state.peripherals).map((key) => {
      var peripheral = this.state.peripherals[key];
      peripheral.name = peripheral.name || peripheral.id;
      return callback(key, peripheral);
    });
  }

  renderPeripheral(key, peripheral) {
    return (
        <li key={key}>
          <Link to={`peripheral/${peripheral.id}`}>{peripheral.name}</Link>
        </li>
    );
  }

  render() {
    return (
      <ul>
        { 
          this._mapPeripherals((key, peripheral) => {
            return this.renderPeripheral(key, peripheral);
          })
        }
      </ul>
    );
  }
}
