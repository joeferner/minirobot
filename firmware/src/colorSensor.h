
#ifndef _COLOR_SENSOR_H_
#define _COLOR_SENSOR_H_

#include <platform_config.h>

typedef struct {
  uint16_t c;
  uint16_t r;
  uint16_t g;
  uint16_t b;
} ColorSensorData;

HAL_StatusTypeDef colorSensor_setup();
HAL_StatusTypeDef colorSensor_tick();

/**
 * brightness range 0-100
 */
HAL_StatusTypeDef colorSensor_setLedBrightness(uint8_t brightness);

/**
 * gain: 1, 4, 16, 60
 */
HAL_StatusTypeDef colorSensor_setGain(uint8_t gain);

extern void onColorSensorData(ColorSensorData* colorData);

#endif
