import React from 'react';

export default class Peripheral extends React.Component {
  render() {
    return (
      <li onClick={this.props.onClick}>
        {this.props.peripheral.name}
      </li>
    );
  }
}
