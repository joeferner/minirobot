import React from "react";
import styles from "./App.css";
import qwest from "qwest";

const BATTERY_REFRESH_INTERVAL = 3000;
const SENSORS_REFRESH_INTERVAL = 1000;

export default class ConnectedPeripheral extends React.Component {
    constructor(props) {
        super(props);

        this.peripheral = {

        };

        this.state = {
            batteryLevel: 'unknown',
            sensorData: {
                lineLeftOuter: false,
                lineLeftInner: false,
                lineRightOuter: false,
                lineRightInner: false,
                feelerLeft: false,
                feelerRight: false,
                compass: 0,
                compassRadians: 0,
                color: {
                    red: 0,
                    green: 0,
                    blue: 0,
                    clear: 0
                }
            },
            motor: {
                left: 0,
                right: 0
            }
        };
    }

    componentDidMount() {
        this._socketioMessageEventListener = this.onWebSocketMessage.bind(this);
        document.addEventListener('socketio.message', this._socketioMessageEventListener);

        qwest.post('/ble/connect', { peripheralId: this.props.params.peripheralId })
            .then((xhr, peripheral) => {
                console.log('connected', peripheral);
                this.peripheral = peripheral;
                this.refreshBattery();
                this.refreshSensors();
                this.refreshMotor();
            });
    }

    componentWillUnmount() {
        clearTimeout(this._refreshBatteryTimeout);
        clearTimeout(this._refreshSensorsTimeout);
        document.removeEventListener('socketio.message', this._socketioMessageEventListener);
    }

    onWebSocketMessage(e) {
        var message = e.detail;
        if (message.type === 'disconnect') {
            this.handleDisconnected();
        }
    }

    refreshBattery() {
        qwest.get('/ble/' + this.props.params.peripheralId + '/batteryLevel')
            .then((xhr, data) => {
                this.setState({'batteryLevel': data.batteryLevel});
                this._refreshBatteryTimeout = setTimeout(this.refreshBattery.bind(this), BATTERY_REFRESH_INTERVAL);
            });
    }

    refreshMotor() {
        qwest.get('/ble/' + this.props.params.peripheralId + '/get-motors')
            .then((xhr, data) => {
                this.setState({'motor': data});
            });
    }

    refreshSensors() {
        qwest.get('/ble/' + this.props.params.peripheralId + '/sensors')
            .then((xhr, data) => {
                data.colorSwatchStyle = {
                    backgroundColor: 'rgb(' + data.color.red + ',' + data.color.green + ',' + data.color.blue + ')'
                };
                data.compassRadians = (data.compass - 90) * (3.1415 / 180.0);
                this.setState({'sensorData': data});
                this._refreshSensorsTimeout = setTimeout(this.refreshSensors.bind(this), SENSORS_REFRESH_INTERVAL);
            });
    }

    onMotorSet() {
        qwest.post('/ble/' + this.props.params.peripheralId + '/set-motors', this.state.motor)
            .then((xhr, data) => {

            });
    }

    handleMotorLeftChange(event) {
        var leftValue = event.target.value;
        this.setState({
            motor: {
                left: leftValue,
                right: this.state.motor.right
            }
        });
    }

    handleMotorRightChange(event) {
        var rightValue = event.target.value;
        this.setState({
            motor: {
                left: this.state.motor.left,
                right: rightValue
            }
        });
    }

    handleDisconnect() {
        qwest.post('/ble/disconnect', { peripheralId: this.props.params.peripheralId })
            .then(() => {
                this.handleDisconnected();
            });
    }

    handleDisconnected() {
        console.log('disconnected');
        this.context.router.replace('/');
    }

    render() {
        return (
            <div className={styles['connected-peripheral']}>
                <div className={styles['column']}>
                    <div className={styles.label}>ID</div>
                    <div>{this.peripheral.id}</div>

                    <div className={styles.label}>Name</div>
                    <div>{this.peripheral.name}</div>

                    <div className={styles.actions}>
                        <button onClick={() => this.handleDisconnect()}>Disconnect</button>
                    </div>
                </div>
                <div className={styles['column']}>
                    <div className={styles.label}>Battery Level</div>
                    <div>{this.state.batteryLevel}</div>

                    <div className={styles.label}>Feelers</div>
                    <div>
                        {this.state.sensorData.feelerLeft ? '1' : '0'}
                        {this.state.sensorData.feelerRight ? '1' : '0'}
                    </div>

                    <div className={styles.label}>Line Sensor</div>
                    <div>
                        {this.state.sensorData.lineLeftOuter ? '1' : '0'}
                        {this.state.sensorData.lineLeftInner ? '1' : '0'}
                        {this.state.sensorData.lineRightInner ? '1' : '0'}
                        {this.state.sensorData.lineRightOuter ? '1' : '0'}
                    </div>

                    <div className={styles.label}>Compass</div>
                    <div>
                        {this.state.sensorData.compass}
                        <svg className={styles['compass']}>
                            <circle cx="50" cy="50" r="49" stroke="black" strokeWidth="1" fill="none"/>
                            <line x1="50" y1="50"
                                  x2={50 + (Math.cos(this.state.sensorData.compassRadians) * 45)}
                                  y2={50 + (Math.sin(this.state.sensorData.compassRadians) * 45)}
                                  style={{stroke: "rgb(255,0,0)", strokeWidth: 3}}/>
                        </svg>
                    </div>

                    <div className={styles.label}>Color</div>
                    <div>
                        [rgbc] =&gt;
                        [{this.state.sensorData.color.red}, {this.state.sensorData.color.green}, {this.state.sensorData.color.blue}, {this.state.sensorData.color.clear}]
                        <div className={styles['color-swatch']} style={this.state.sensorData.colorSwatchStyle}></div>
                    </div>
                </div>

                <div className={styles['column']}>
                    <div className={styles.label}>Motors</div>
                    <div>
                        <div>
                            Left: <input value={this.state.motor.left} onChange={this.handleMotorLeftChange.bind(this)}/>
                        </div>
                        <div>
                            Right: <input value={this.state.motor.right} onChange={this.handleMotorRightChange.bind(this)}/>
                        </div>
                        <button onClick={this.onMotorSet.bind(this)}>Set</button>
                        <button onClick={this.refreshMotor.bind(this)}>Refresh</button>
                    </div>
                </div>
            </div>
        );
    }
}

ConnectedPeripheral.contextTypes = {
    router: React.PropTypes.object.isRequired
};
