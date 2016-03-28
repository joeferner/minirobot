
#include "ble.h"
#include <rn4020/rn4020.h>
#include <utils/utils.h>

RN4020 rn4020;

HAL_StatusTypeDef ble_setup() {
  DEBUG_OUT("ble setup\n");

  uint32_t supportedServices = RN4020_SERVICE_DEVICE_INFORMATION
    | RN4020_SERVICE_BATTERY
    | RN4020_SERVICE_USER_DEFINED;
  uint32_t features = RN4020_FEATURE_AUTO_MLDP_DISABLE 
    | RN4020_FEATURE_DO_NOT_SAVE_BONDING
    | RN4020_FEATURE_ENABLE_OTA;
  
  rn4020.uart = &BLE_UART;
  rn4020.wakeswPort = BLE_WAKESW_PORT;
  rn4020.wakeswPin = BLE_WAKESW_PIN;
  rn4020.wakehwPort = BLE_WAKEHW_PORT;
  rn4020.wakehwPin = BLE_WAKEHW_PIN;
  returnNonOKHALStatus(RN4020_setup(&rn4020));
  returnNonOKHALStatus(RN4020_resetToFactoryDefaults(&rn4020));
  returnNonOKHALStatus(RN4020_setSupportedServices(&rn4020, supportedServices));
  returnNonOKHALStatus(RN4020_setSupportedFeatures(&rn4020, features));
  returnNonOKHALStatus(RN4020_setDeviceNameWithMAC(&rn4020, BLE_DEVICE_NAME));
  returnNonOKHALStatus(RN4020_reset(&rn4020));
  returnNonOKHALStatus(RN4020_advertise(&rn4020));
  
  return HAL_OK;
}

void ble_tick() {
  RN4020_tick(&rn4020);
}

bool ble_debugProcessLine(const char* line) {
  RN4020_send(&rn4020, line);
  return true;
}

void ble_updateBatteryLevel(uint8_t batteryLevel) {
  uint8_t scaledBatteryLevel = clamp((uint32_t)batteryLevel * RN4020_BATTERY_MAX_LEVEL / BATTERY_MAX_VALUE, 0, RN4020_BATTERY_MAX_LEVEL);
  RN4020_battery_setLevel(&rn4020, scaledBatteryLevel);
}