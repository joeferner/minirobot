import React from 'react';
import styles from './App.css';
import qwest from 'qwest';

const BATTERY_REFRESH_INTERVAL = 3000;

export default class ConnectedPeripheral extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            batteryLevel: 'unknown'
        };
    }

    componentDidMount() {
        this._refreshBatteryInterval = setInterval(this.refreshBattery.bind(this), BATTERY_REFRESH_INTERVAL);
        this.refreshBattery();
    }

    componentWillUnmount() {
        clearTimeout(this._refreshBatteryInterval);
    }

    refreshBattery() {
        qwest.get('/ble/' + this.props.peripheral.id + '/batteryLevel')
            .then((xhr, data) => {
                this.setState({'batteryLevel': data.batteryLevel});
            });
    }

    render() {
        return (
            <div>
                <button onClick={this.props.onDisconnectClick}>Disconnect</button>

                <div className={styles.label}>ID</div>
                <div>{this.props.peripheral.id}</div>

                <div className={styles.label}>Name</div>
                <div>{this.props.peripheral.name}</div>

                <div className={styles.label}>Battery Level</div>
                <div>{this.state.batteryLevel}</div>
            </div>
        );
    }
}
