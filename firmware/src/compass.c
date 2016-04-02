
#include "compass.h"
#include <lis3mdl/lis3mdl.h>
#include <utils/timer.h>
#include <utils/utils.h>
#include <utils/math.h>

LIS3MDL compass;
PeriodicTimer compass_timer;

HAL_StatusTypeDef _compass_readXYHeading(uint16_t* heading);
int16_t _compass_scale(int32_t value, int32_t min, int32_t max);

HAL_StatusTypeDef compass_setup() {
  DEBUG_OUT("compass setup\n");

  returnNonOKHALStatus(LIS3MDL_setup(&compass, &COMPASS_I2C, LIS3MDL_ADDRESS1));
  returnNonOKHALStatus(LIS3MDL_reset(&compass));
  returnNonOKHALStatus(LIS3MDL_enableTemperature(&compass, false));
  returnNonOKHALStatus(LIS3MDL_setPerformance(&compass, LIS3MDL_PERFORMANCE_HIGH));
  returnNonOKHALStatus(LIS3MDL_setDateRate(&compass, LIS3MDL_DATA_RATE_80_HZ));
  returnNonOKHALStatus(LIS3MDL_setScale(&compass, LIS3MDL_SCALE_4_GAUSS));
  returnNonOKHALStatus(LIS3MDL_setMode(&compass, LIS3MDL_MODE_CONTINUOUS));

  periodicTimer_setup(&compass_timer, COMPASS_TIMER_MS);

  return HAL_OK;
}

HAL_StatusTypeDef compass_tick() {
  HAL_StatusTypeDef s;
  uint16_t heading;
  uint8_t compassStatus;

  if (!periodicTimer_hasElapsed(&compass_timer)) {
    return HAL_OK;
  }

  s = LIS3MDL_readStatus(&compass, &compassStatus);
  if (s != HAL_OK) {
    DEBUG_OUT("could not read compass status: %d\n", s);
    return s;
  }
  if (testBits(compassStatus, LIS3MDL_STATUS_ZYXDA)) {
    s = _compass_readXYHeading(&heading);
    if (s != HAL_OK) {
      DEBUG_OUT("could not read XY heading: %d\n", s);
      return s;
    }
    onCompassChange(heading);
  }

  return HAL_OK;
}

HAL_StatusTypeDef _compass_readXYHeading(uint16_t* heading) {
  HAL_StatusTypeDef status;
  int16_t x, y;
  int16_t scaledX, scaledY;

  status = LIS3MDL_readAxis(&compass, LIS3MDL_AXIS_X, &x);
  if (status != HAL_OK) {
    return status;
  }

  status = LIS3MDL_readAxis(&compass, LIS3MDL_AXIS_Y, &y);
  if (status != HAL_OK) {
    return status;
  }

  scaledX = _compass_scale(x, compass.min[LIS3MDL_AXIS_X], compass.max[LIS3MDL_AXIS_X]);
  scaledY = _compass_scale(y, compass.min[LIS3MDL_AXIS_Y], compass.max[LIS3MDL_AXIS_Y]);

  *heading = 360 - trig_int16_atan2deg(scaledY, scaledX);
  //DEBUG_OUT("%d, %d, %d, %d, %d, %d, %d, %d, %d\n", x, scaledX, compass.min[LIS3MDL_AXIS_X], compass.max[LIS3MDL_AXIS_X], y, scaledY, compass.min[LIS3MDL_AXIS_Y], compass.max[LIS3MDL_AXIS_Y], *heading);

  return HAL_OK;
}

int16_t _compass_scale(int32_t value, int32_t min, int32_t max) {
  value = value - ((max + min) / 2); // center around mid point
  value = value * (20000 / (max - min)); // scale from -10000 to 10000
  return value;
}
