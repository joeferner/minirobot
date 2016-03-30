import React from 'react';
import Peripheral from './Peripheral.jsx';

export default class PeripheralsList extends React.Component {
  handlePeripheralClick(id) {
    this.props.onPeripheralClick(id);
  }
  
  _mapPeripherals(callback) {
    return Object.keys(this.props.peripherals).map((key) => {
      var peripheral = this.props.peripherals[key];
      peripheral.name = peripheral.name || 'not set';
      return callback(key, peripheral);
    });
  }

  render() {
    return (
      <ul>
        { 
          this._mapPeripherals((key, peripheral) => {
            return (<Peripheral onClick={this.handlePeripheralClick.bind(this, peripheral.id)} key={peripheral.id} peripheral={peripheral} />)
          })
        }
      </ul>
    );
  }
}
