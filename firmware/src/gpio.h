
#ifndef _GPIO_H_
#define _GPIO_H_

#include <platform_config.h>
#include <stdbool.h>

typedef enum {
  FEELER_LEFT,
  FEELER_RIGHT
} Feeler;

typedef enum {
  WHEEL_SENSOR_LEFT,
  WHEEL_SENSOR_RIGHT
} WheelSensor;

typedef enum {
  LINE_SENSOR_LEFT_OUT,
  LINE_SENSOR_LEFT_IN,
  LINE_SENSOR_RIGHT_OUT,
  LINE_SENSOR_RIGHT_IN
} LineSensor;

HAL_StatusTypeDef gpio_setup();

extern void onFeeler(Feeler feeler, bool active);
extern void onWheelSensor(WheelSensor wheelSensor, bool active);
extern void onLineSensor(LineSensor lineSensor, bool active);

#endif
