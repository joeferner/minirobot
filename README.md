# Minirobot

# Microcontroller Pin Requirements

| Name                    | Requirements                    | Pin                   |
| ----------------------- | ------------------------------- | --------------------- |
| Debug                   | UART TX/RX                      | UART1-PA9/PA10        |
| Wheel encoder left      | EXTI                            | PB0                   |
| Wheel encoder right     | EXTI                            | PB1                   |
| Wheel servo left        | PWM                             | TIM3-PA6              |
| Wheel servo right       | PWM                             | TIM3-PA7              |
| Line follow left outer  | EXTI                            | PB12                  |
| Line follow right outer | EXTI                            | PB14                  |
| Line follow left inner  | EXTI                            | PB13                  |
| Line follow right inner | EXTI                            | PB15                  |
| I<sup>2</sup>C1         | SCL/SDA                         | PB8/PB9               |
| Compass (ST-LIS3MDL)    | I<sup>2</sup>C (0x1E)           | I<sup>2</sup>C1       |
| Color Sensor (TCS3472)  | I<sup>2</sup>C (0x29)           | I<sup>2</sup>C1       |
| Color Sensor Interrupt  | GPIO IN                         | PB6                   |
| Color LED               | PWM                             | TIM4-PB7              |
| Bluetooth (RN4020)      | USART TX/RX/CTS/RTS             | UART2-PA2/PA3/PA0/PA1 |
| Bluetooth MLDP          | GPIO OUT                        | PC1                   |
| Bluetooth WakeSW        | GPIO OUT                        | PC2                   |
| Bluetooth WakeHW        | GPIO OUT                        | PC3                   |
| Bluetooth WS            | GPIO IN                         | PA4                   |
| Bluetooth Connection    | GPIO IN                         | PA5                   |
| Bluetooth MLDPEV        | GPIO IN                         | PC4                   |
| Battery level           | ADC                             | VBAT                  |
