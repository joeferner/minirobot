# Minirobot

# Microcontroller Pin Requirements

| Name                    | Requirements                    |
| ----------------------- | ------------------------------- |
| Debug                   | UART TX/RX                      |
| Wheel encoder left      | EXTI                            |
| Wheel encoder right     | EXTI                            |
| Wheel servo left        | PWM                             |
| Wheel servo right       | PWM                             |
| Line follow left outer  | EXTI                            |
| Line follow right outer | EXTI                            |
| Line follow left inner  | EXTI                            |
| Line follow right inner | EXTI                            |
| Compass (ST-LIS3MDL)    | I<sup>2</sup>C (0x1E)           |
| Bluetooth (RN4020)      | USART TX/RX/CTS/RTS<br>GPIO x 6 |
| Battery level           | ADC                             |
