import React from "react";
import styles from "./App.css";
import qwest from "qwest";

const BATTERY_REFRESH_INTERVAL = 3000;
const SENSORS_REFRESH_INTERVAL = 1000;

function scaleSensorColorTo256(color, clear) {
    return parseInt((color / clear) * 256);
}

export default class ConnectedPeripheral extends React.Component {
    constructor(props) {
        super(props);

        this.peripheral = {};

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
                left: 'unknown',
                right: 'unknown'
            },
            configuration: {
                colorSensorGain: 'unknown',
                colorSensorLedBrightness: 'unknown'
            }
        };
    }

    componentDidMount() {
        this._socketioMessageEventListener = this.onWebSocketMessage.bind(this);
        document.addEventListener('socketio.message', this._socketioMessageEventListener);

        qwest.post('/ble/connect', {peripheralId: this.props.params.peripheralId})
            .then((xhr, peripheral) => {
                console.log('connected', peripheral);
                this.peripheral = peripheral;
                this.refreshBattery();
                this.refreshSensors();
                this.refreshMotor();
                this.refreshConfiguration();
            });
    }

    componentWillUnmount() {
        //clearTimeout(this._refreshBatteryTimeout);
        //clearTimeout(this._refreshSensorsTimeout);
        document.removeEventListener('socketio.message', this._socketioMessageEventListener);
    }

    onWebSocketMessage(e) {
        var message = e.detail;
        if (message.type === 'disconnect') {
            this.handleDisconnected();
        } else if (message.type === 'sensorChange') {
            this._setSensorDataState(message.data);
        } else if (message.type === 'batteryLevelChange') {
            this.setState({batteryLevel: message.data});
        }
    }

    _setSensorDataState(data) {
        var red = scaleSensorColorTo256(data.color.red, data.color.clear);
        var green = scaleSensorColorTo256(data.color.green, data.color.clear);
        var blue = scaleSensorColorTo256(data.color.blue, data.color.clear);
        data.colorSwatchStyle = {
            backgroundColor: 'rgb(' + red + ',' + green + ',' + blue + ')'
        };
        data.compassRadians = (data.compass - 90) * (3.1415 / 180.0);
        this.setState({sensorData: data});
    }

    refreshBattery() {
        qwest.get('/ble/' + this.props.params.peripheralId + '/batteryLevel')
            .then((xhr, data) => {
                this.setState({batteryLevel: data.batteryLevel});
                //this._refreshBatteryTimeout = setTimeout(this.refreshBattery.bind(this), BATTERY_REFRESH_INTERVAL);
            });
    }

    refreshMotor() {
        qwest.get('/ble/' + this.props.params.peripheralId + '/get-motors')
            .then((xhr, data) => {
                this.setState({motor: data});
            });
    }

    refreshConfiguration() {
        qwest.get('/ble/' + this.props.params.peripheralId + '/get-configuration')
            .then((xhr, data) => {
                this.setState({configuration: data});
            });
    }

    refreshSensors() {
        qwest.get('/ble/' + this.props.params.peripheralId + '/sensors')
            .then((xhr, data) => {
                this._setSensorDataState(data);
                //this._refreshSensorsTimeout = setTimeout(this.refreshSensors.bind(this), SENSORS_REFRESH_INTERVAL);
            });
    }

    onMotorSet() {
        qwest.post('/ble/' + this.props.params.peripheralId + '/set-motors', this.state.motor)
            .then((xhr, data) => {

            });
    }

    onConfigurationSet() {
        qwest.post('/ble/' + this.props.params.peripheralId + '/set-configuration', this.state.configuration)
            .then((xhr, data) => {

            });
    }

    handleMotorLeftChange(event) {
        var leftValue = parseInt(event.target.value);
        this.setState({
            motor: {
                left: leftValue,
                right: this.state.motor.right
            }
        });
    }

    handleMotorRightChange(event) {
        var rightValue = parseInt(event.target.value);
        this.setState({
            motor: {
                left: this.state.motor.left,
                right: rightValue
            }
        });
    }

    handleColorSensorGainChange(event) {
        var gain = parseInt(event.target.value);
        this.setState({
            configuration: {
                colorSensorGain: gain,
                colorSensorLedBrightness: this.state.configuration.colorSensorLedBrightness
            }
        });
    }

    handleColorSensorLedBrightnessChange(event) {
        var colorSensorLedBrightness = parseInt(event.target.value);
        this.setState({
            configuration: {
                colorSensorGain: this.state.configuration.colorSensorGain,
                colorSensorLedBrightness: colorSensorLedBrightness
            }
        });
    }

    handleDisconnect() {
        qwest.post('/ble/disconnect', {peripheralId: this.props.params.peripheralId})
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
                            Left: <input value={this.state.motor.left}
                                         onChange={this.handleMotorLeftChange.bind(this)}/>
                        </div>
                        <div>
                            Right: <input value={this.state.motor.right}
                                          onChange={this.handleMotorRightChange.bind(this)}/>
                        </div>
                        <button onClick={this.onMotorSet.bind(this)}>Set</button>
                        <button onClick={this.refreshMotor.bind(this)}>Refresh</button>
                    </div>
                </div>

                <div className={styles['column']}>
                    <div className={styles.label}>Color Sensor Gain</div>
                    <div>
                        <select value={this.state.configuration.colorSensorGain} onChange={this.handleColorSensorGainChange.bind(this)}>
                            <option value="">Not Set</option>
                            <option value="1">x1</option>
                            <option value="4">x4</option>
                            <option value="16">x16</option>
                            <option value="60">x60</option>
                        </select>
                    </div>

                    <div className={styles.label}>Color Sensor LED Brightness</div>
                    <div>
                        <input value={this.state.configuration.colorSensorLedBrightness}
                               onChange={this.handleColorSensorLedBrightnessChange.bind(this)}/>
                    </div>

                    <button onClick={this.onConfigurationSet.bind(this)}>Set</button>
                    <button onClick={this.refreshConfiguration.bind(this)}>Refresh</button>
                </div>
            </div>
        );
    }
}

ConnectedPeripheral.contextTypes = {
    router: React.PropTypes.object.isRequired
};
