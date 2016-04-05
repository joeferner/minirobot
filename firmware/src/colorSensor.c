
#include "colorSensor.h"
#include <tcs3472/tcs3472.h>
#include <utils/utils.h>
#include <utils/timer.h>
#include <string.h>
#include <stdlib.h>

#define _COLOR_SENSOR_LED_PERIOD() ((int32_t)COLOR_SENSOR_LED_TIMER.Init.Period)
#define DIFF(a, b) (abs((int32_t)a - (int32_t)b))

TCS3472 tcs3472;
PeriodicTimer tcs3472Timer;
TCS3472_Color tcs3472LastColorData;

bool _colorSensor_hasChanges(TCS3472_Color* last, TCS3472_Color* current);

HAL_StatusTypeDef colorSensor_setup() {
  DEBUG_OUT("colorSensor setup\n");
  returnNonOKHALStatus(TCS3472_setup(&tcs3472, &COLOR_SENSOR_I2C, TCS34725_I2C_ADDRESS));
  returnNonOKHALStatus(TCS3472_setIntegrationTime(&tcs3472, TCS3472_INTEGRATION_TIME));
  returnNonOKHALStatus(TCS3472_setGain(&tcs3472, TCS3472_REG_CONTROL_AGAIN_1X));
  returnNonOKHALStatus(TCS3472_enable(&tcs3472));
  periodicTimer_setup(&tcs3472Timer, TCS3472_INTEGRATION_TIMEMS);

  colorSensor_setLedBrightness(50);
  returnNonOKHALStatus(HAL_TIM_PWM_Start(&COLOR_SENSOR_LED_TIMER, COLOR_SENSOR_LED_TIMER_CH));

  return HAL_OK;
}

HAL_StatusTypeDef colorSensor_tick() {
  if (periodicTimer_hasElapsed(&tcs3472Timer)) {
    TCS3472_Color tcs3472ColorData;
    ColorSensorData colorSensorData;

    returnNonOKHALStatus(TCS3472_readColor(&tcs3472, &tcs3472ColorData));

    if (_colorSensor_hasChanges(&tcs3472LastColorData, &tcs3472ColorData)) {
      memcpy(&tcs3472LastColorData, &tcs3472ColorData, sizeof(TCS3472_Color));
      colorSensorData.c = tcs3472ColorData.c;
      colorSensorData.r = tcs3472ColorData.r;
      colorSensorData.g = tcs3472ColorData.g;
      colorSensorData.b = tcs3472ColorData.b;
      onColorSensorData(&colorSensorData);
    }
  }
  return HAL_OK;
}

HAL_StatusTypeDef colorSensor_setLedBrightness(uint8_t brightness) {
  uint32_t clampedBrightness = clamp(brightness, 0, 100);
  uint32_t totalPeriod = _COLOR_SENSOR_LED_PERIOD();
  uint32_t pulse = totalPeriod * clampedBrightness / 100;
  __HAL_TIM_SET_COMPARE(&COLOR_SENSOR_LED_TIMER, COLOR_SENSOR_LED_TIMER_CH, pulse);
  return HAL_OK;
}

HAL_StatusTypeDef colorSensor_setGain(uint8_t gain) {
  TCS3472_Gain tcsGain;
  switch (gain) {
  case 1:
    tcsGain = TCS3472_REG_CONTROL_AGAIN_1X;
    break;
  case 4:
    tcsGain = TCS3472_REG_CONTROL_AGAIN_4X;
    break;
  case 16:
    tcsGain = TCS3472_REG_CONTROL_AGAIN_16X;
    break;
  case 60:
    tcsGain = TCS3472_REG_CONTROL_AGAIN_60X;
    break;
  default:
    DEBUG_OUT("invalid gain %d\n", gain);
    return HAL_ERROR;
  }
  return TCS3472_setGain(&tcs3472, tcsGain);
}

bool _colorSensor_hasChanges(TCS3472_Color* last, TCS3472_Color* current) {
  if (DIFF(last->r, current->r) > COLOR_SENSOR_CHANGE_THRESHOLD) {
    return true;
  }
  if (DIFF(last->g, current->g) > COLOR_SENSOR_CHANGE_THRESHOLD) {
    return true;
  }
  if (DIFF(last->b, current->b) > COLOR_SENSOR_CHANGE_THRESHOLD) {
    return true;
  }
  if (DIFF(last->c, current->c) > COLOR_SENSOR_CHANGE_THRESHOLD) {
    return true;
  }
  return false;
}
