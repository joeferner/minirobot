
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

#define TCS3472_DEBUG
#define COLOR_SENSOR_CHANGE_THRESHOLD 10
#define COLOR_SENSOR_I2C              hi2c1
#define COLOR_SENSOR_LED_TIMER        htim4
#define COLOR_SENSOR_LED_TIMER_CH     TIM_CHANNEL_2
#define TCS3472_INTEGRATION_TIME      0xd5 /* 101ms */
#define TCS3472_INTEGRATION_TIMEMS    101
#define TCS3472_GAIN                  TCS3472_REG_CONTROL_AGAIN_1X

#ifdef DEBUG
#  define DEBUG_OUT(format, ...) printf("%s:%d: " format, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#  define DEBUG_OUT(format, ...)
#endif

extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

#endif
