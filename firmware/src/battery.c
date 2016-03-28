
#include "battery.h"
#include <utils/timer.h>
#include <utils/utils.h>

#define ADC_MAX_VALUE 0xfff

PeriodicTimer batteryTimer;

void _battery_update();
HAL_StatusTypeDef _battery_startConversion();

HAL_StatusTypeDef battery_setup() {
  DEBUG_OUT("battery setup\n");
  periodicTimer_setup(&batteryTimer, BATTERY_UPDATE_INTERVAL_MS);
  return _battery_startConversion();
}

void battery_tick() {
  if (periodicTimer_hasElapsed(&batteryTimer)) {
    _battery_update();
  }
}

void _battery_update() {
  HAL_StatusTypeDef ret = HAL_ADC_PollForConversion(&BATTERY_ADC, 100);
  if (ret != HAL_OK) {
    DEBUG_OUT("failed to poll for battery level\n");
    _battery_startConversion();
    return;
  }
  uint32_t value = clamp(HAL_ADC_GetValue(&BATTERY_ADC) * BATTERY_MAX_VALUE / ADC_MAX_VALUE, 0, BATTERY_MAX_VALUE);
  _battery_startConversion();
  onBatteryLevelUpdate(value);
}

HAL_StatusTypeDef _battery_startConversion() {
  HAL_StatusTypeDef ret = HAL_ADC_Start(&BATTERY_ADC);
  if (ret != HAL_OK) {
    DEBUG_OUT("failed to start next conversion\n");
  }
  return ret;
}
