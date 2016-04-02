
#ifndef _PLATFORM_CONFIG_H_
#define _PLATFORM_CONFIG_H_

#include <pinout.h>

#define DEBUG

#ifdef DEBUG
#  define UTILS_DEBUG /* print assert statements */
#endif
#define DEBUG_UART       huart1
#define MAX_TIMEOUT      0xffff

//#define LIS3MDL_DEBUG
#define COMPASS_I2C      hi2c1
#define COMPASS_TIMER_MS 20

#define SERVO_TIMER      htim3
#define SERVO_LEFT_CH    TIM_CHANNEL_1
#define SERVO_RIGHT_CH   TIM_CHANNEL_2

//#define TCS3472_DEBUG
#define COLOR_SENSOR_CHANGE_THRESHOLD 10
#define COLOR_SENSOR_I2C              hi2c1
#define COLOR_SENSOR_LED_TIMER        htim4
#define COLOR_SENSOR_LED_TIMER_CH     TIM_CHANNEL_2
#define TCS3472_INTEGRATION_TIME      0xd5 /* 101ms */
#define TCS3472_INTEGRATION_TIMEMS    101
#define TCS3472_GAIN                  TCS3472_REG_CONTROL_AGAIN_1X

#define RN4020_DEBUG
#define BLE_UART              huart2
#define BLE_DEVICE_NAME       "Minirobot"
#define BLE_MANUFACTURER_NAME "joeferner"
#define BLE_MODEL_NUMBER      "1.0"
#define BLE_SERIAL_NUMBER     "000001"
#define BLE_HARDWARE_REVISION "2016-02-23"
#define BLE_SOFTWARE_REVISION "1.0"
#define BLE_SYSTEM_ID         BLE_DEVICE_NAME

#define BATTERY_UPDATE_INTERVAL_MS 5000
#define BATTERY_MAX_VALUE          100
#define BATTERY_ADC                hadc1

#ifdef DEBUG
#  define DEBUG_OUT(format, ...) printf("%s:%d: " format, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#  define DEBUG_OUT(format, ...)
#endif

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern ADC_HandleTypeDef hadc1;

#endif
