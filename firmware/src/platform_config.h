
#ifndef _PLATFORM_CONFIG_H_
#define _PLATFORM_CONFIG_H_

#include <pinout.h>

#define UTILS_DEBUG
#define DEBUG_UART       huart1
#define MAX_TIMEOUT      0xffff

#define DEBUG_LIS3MDL
#define COMPASS_I2C      hi2c1
#define COMPASS_TIMER_MS 20

#define DEBUG_OUT(format, ...) printf("%s:%d: " format, __FILE__, __LINE__, ##__VA_ARGS__)
//#define DEBUG_OUT(format, ...)

extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c1;

#endif
