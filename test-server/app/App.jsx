import React from "react";
import Es6Promise from "es6-promise";
import qwest from "qwest";
import PeripheralsList from "./PeripheralsList.jsx";
import Peripheral from "./Peripheral.jsx";
import {Router, Route, hashHistory} from "react-router";
const socket = io();
qwest.setDefaultDataType('json');

Es6Promise.polyfill();

export default class App extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            peripherals: {}
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
    }

    onWebSocketMessage(message) {
        console.log('socket event', message);
        document.dispatchEvent(new CustomEvent('socketio.message', { detail: message }));
        if (message.type === 'discover') {
            this.forceUpdate();
        }
    }

    render() {
        return (
            <Router history={hashHistory}>
                <Route path="/" component={PeripheralsList} />
                <Route path="peripheral/:peripheralId" component={Peripheral} />
            </Router>
        );
    }
}
