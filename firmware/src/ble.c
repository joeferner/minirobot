
#include "ble.h"
#include <rn4020/rn4020.h>
#include <utils/utils.h>
#include <string.h>
#include <version.h>

const uint8_t BLE_MINIROBOT_SERVICE_UUID[] = {0x5a, 0x6c, 0x53, 0xb8, 0x47, 0x0d, 0x46, 0x7d, 0x95, 0x31, 0xf5, 0x3d, 0xe5, 0xf3, 0x1e, 0x21};
const uint8_t BLE_MINIROBOT_CHARACTERISTIC_CONFIGURATION_UUID[] = {0x82, 0x55, 0x2f, 0x9e, 0x36, 0x50, 0x49, 0xab, 0xa0, 0x12, 0x93, 0x38, 0x95, 0x4a, 0xcc, 0xbc};
const uint8_t BLE_MINIROBOT_CHARACTERISTIC_MOTORS_UUID[] = {0xb5, 0xfb, 0x41, 0xdc, 0x8f, 0x5d, 0x4e, 0xc4, 0xb3, 0x23, 0xc9, 0xd1, 0xbd, 0x2b, 0x33, 0x08};
const uint8_t BLE_MINIROBOT_CHARACTERISTIC_SENSORS_UUID[] = {0xd8, 0xe7, 0x74, 0x6c, 0xe8, 0xd6, 0x47, 0x3a, 0xa1, 0x57, 0x26, 0x58, 0x88, 0xa3, 0xb1, 0x53};

typedef struct __attribute__((__packed__)) {
  uint8_t lineLeftOuter: 1;
  uint8_t lineLeftInner: 1;
  uint8_t lineRightOuter: 1;
  uint8_t lineRightInner: 1;
  uint8_t feelerLeft: 1;
  uint8_t feelerRight: 1;
  uint16_t compass;
  uint16_t colorRed;
  uint16_t colorGreen;
  uint16_t colorBlue;
  uint16_t colorClear;
}
BLE_SensorData;

typedef struct __attribute__((__packed__)) {
  int8_t left;
  int8_t right;
}
BLE_MotorData;

RN4020 rn4020;

uint8_t _ble_batteryLevel;
BLE_Configuration _ble_configurationData;
BLE_SensorData _ble_sensorData;
BLE_MotorData _ble_motorData;

HAL_StatusTypeDef ble_setup() {
  DEBUG_OUT("ble setup\n");

  _ble_batteryLevel = 0;
  _ble_configurationData.colorSensorGain = 0;
  _ble_configurationData.colorSensorLedBrightness = 0;

  uint32_t supportedServices = RN4020_SERVICE_DEVICE_INFORMATION
                               | RN4020_SERVICE_BATTERY
                               | RN4020_SERVICE_USER_DEFINED;
  uint32_t features = RN4020_FEATURE_AUTO_MLDP_DISABLE
                      | RN4020_FEATURE_DO_NOT_SAVE_BONDING
                      | RN4020_FEATURE_ENABLE_OTA
                      | RN4020_FEATURE_REAL_TIME_READ;

  rn4020.uart = &BLE_UART;
  rn4020.wakeswPort = BLE_WAKESW_PORT;
  rn4020.wakeswPin = BLE_WAKESW_PIN;
  rn4020.wakehwPort = BLE_WAKEHW_PORT;
  rn4020.wakehwPin = BLE_WAKEHW_PIN;
  returnNonOKHALStatus(RN4020_setup(&rn4020));
  returnNonOKHALStatus(RN4020_resetToFactoryDefaults(&rn4020));
  returnNonOKHALStatus(RN4020_clearPrivate(&rn4020));
  returnNonOKHALStatus(RN4020_setSupportedServices(&rn4020, supportedServices));
  returnNonOKHALStatus(RN4020_setSupportedFeatures(&rn4020, features));
  returnNonOKHALStatus(RN4020_addPrivateService(&rn4020, BLE_MINIROBOT_SERVICE_UUID));
  returnNonOKHALStatus(RN4020_addPrivateCharacteristic(
                         &rn4020,
                         BLE_MINIROBOT_CHARACTERISTIC_CONFIGURATION_UUID,
                         RN4020_PRIVATE_CHARACTERISTIC_PROPERTY_READ | RN4020_PRIVATE_CHARACTERISTIC_PROPERTY_WRITE,
                         sizeof(BLE_Configuration),
                         RN4020_PRIVATE_CHARACTERISTIC_SECURITY_NONE
                       ));
  returnNonOKHALStatus(RN4020_addPrivateCharacteristic(
                         &rn4020,
                         BLE_MINIROBOT_CHARACTERISTIC_SENSORS_UUID,
                         RN4020_PRIVATE_CHARACTERISTIC_PROPERTY_READ,
                         sizeof(BLE_SensorData),
                         RN4020_PRIVATE_CHARACTERISTIC_SECURITY_NONE
                       ));
  returnNonOKHALStatus(RN4020_addPrivateCharacteristic(
                         &rn4020,
                         BLE_MINIROBOT_CHARACTERISTIC_MOTORS_UUID,
                         RN4020_PRIVATE_CHARACTERISTIC_PROPERTY_READ | RN4020_PRIVATE_CHARACTERISTIC_PROPERTY_WRITE,
                         sizeof(BLE_MotorData),
                         RN4020_PRIVATE_CHARACTERISTIC_SECURITY_NONE
                       ));
  returnNonOKHALStatus(RN4020_setDeviceNameWithMAC(&rn4020, BLE_DEVICE_NAME));
  returnNonOKHALStatus(RN4020_reset(&rn4020));
  returnNonOKHALStatus(RN4020_advertise(&rn4020));
  returnNonOKHALStatus(RN4020_refreshHandleLookup(&rn4020));

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
  _ble_batteryLevel = clamp((uint32_t)batteryLevel * RN4020_BATTERY_MAX_LEVEL / BATTERY_MAX_VALUE, 0, RN4020_BATTERY_MAX_LEVEL);
}

void ble_updateFeeler(Feeler feeler, bool active) {
  switch (feeler) {
  case FEELER_LEFT:
    _ble_sensorData.feelerLeft = active ? 1 : 0;
    break;
  case FEELER_RIGHT:
    _ble_sensorData.feelerRight = active ? 1 : 0;
    break;
  }
}

void ble_updateLineSensor(LineSensor lineSensor, bool active) {
  switch (lineSensor) {
  case LINE_SENSOR_LEFT_OUT:
    _ble_sensorData.lineLeftOuter = active ? 1 : 0;
    break;
  case LINE_SENSOR_LEFT_IN:
    _ble_sensorData.lineLeftInner = active ? 1 : 0;
    break;
  case LINE_SENSOR_RIGHT_OUT:
    _ble_sensorData.lineRightOuter = active ? 1 : 0;
    break;
  case LINE_SENSOR_RIGHT_IN:
    _ble_sensorData.lineRightInner = active ? 1 : 0;
    break;
  }
}

void ble_updateCompass(uint16_t heading) {
  _ble_sensorData.compass = heading;
}

void ble_updateColorSensorData(ColorSensorData* colorData) {
  _ble_sensorData.colorRed = colorData->r;
  _ble_sensorData.colorGreen = colorData->g;
  _ble_sensorData.colorBlue = colorData->b;
  _ble_sensorData.colorClear = colorData->c;
}

void ble_updateColorSensorGain(uint8_t colorSensorGain) {
  _ble_configurationData.colorSensorGain = colorSensorGain;
}

void ble_updateColorSensorLedBrightness(uint8_t colorSensorLedBrightness) {
  _ble_configurationData.colorSensorLedBrightness = colorSensorLedBrightness;
}

void RN4020_onRealTimeRead(RN4020* rn4020, uint16_t characteristicHandle) {
  RN4020_handleLookupItem* handleLookupItem = RN4020_lookupHandle(rn4020, characteristicHandle);
  if (handleLookupItem == NULL) {
    return;
  }

  if (RN4020_isHandleLookupItemUUIDEqual16(handleLookupItem, RN4020_BATTERY_LEVEL_UUID)) {
    RN4020_battery_setLevel(rn4020, _ble_batteryLevel);
    return;
  }

  if (RN4020_isHandleLookupItemUUIDEqual16(handleLookupItem, RN4020_DEV_INFO_MANUFACTURER_NAME)) {
    RN4020_writeServerCharacteristicHandle(rn4020, handleLookupItem->handle, (const uint8_t*)BLE_MANUFACTURER_NAME, strlen(BLE_MANUFACTURER_NAME));
    return;
  }

  if (RN4020_isHandleLookupItemUUIDEqual16(handleLookupItem, RN4020_DEV_INFO_MODEL_NUMBER)) {
    RN4020_writeServerCharacteristicHandle(rn4020, handleLookupItem->handle, (const uint8_t*)BLE_MODEL_NUMBER, strlen(BLE_MODEL_NUMBER));
    return;
  }

  if (RN4020_isHandleLookupItemUUIDEqual16(handleLookupItem, RN4020_DEV_INFO_SERIAL_NUMBER)) {
    RN4020_writeServerCharacteristicHandle(rn4020, handleLookupItem->handle, (const uint8_t*)BLE_SERIAL_NUMBER, strlen(BLE_SERIAL_NUMBER));
    return;
  }

  if (RN4020_isHandleLookupItemUUIDEqual16(handleLookupItem, RN4020_DEV_INFO_HARDWARE_REVISION)) {
    RN4020_writeServerCharacteristicHandle(rn4020, handleLookupItem->handle, (const uint8_t*)BLE_HARDWARE_REVISION, strlen(BLE_HARDWARE_REVISION));
    return;
  }

  if (RN4020_isHandleLookupItemUUIDEqual16(handleLookupItem, RN4020_DEV_INFO_FIRMWARE_REVISION)) {
    RN4020_writeServerCharacteristicHandle(rn4020, handleLookupItem->handle, (const uint8_t*)GIT_HASH, 20);
    return;
  }

  if (RN4020_isHandleLookupItemUUIDEqual16(handleLookupItem, RN4020_DEV_INFO_SOFTWARE_REVISION)) {
    RN4020_writeServerCharacteristicHandle(rn4020, handleLookupItem->handle, (const uint8_t*)BLE_SOFTWARE_REVISION, strlen(BLE_SOFTWARE_REVISION));
    return;
  }

  if (RN4020_isHandleLookupItemUUIDEqual16(handleLookupItem, RN4020_DEV_INFO_SYSTEM_ID)) {
    RN4020_writeServerCharacteristicHandle(rn4020, handleLookupItem->handle, (const uint8_t*)BLE_SYSTEM_ID, strlen(BLE_SYSTEM_ID));
    return;
  }

  if (RN4020_isHandleLookupItemUUIDEqual128(handleLookupItem, BLE_MINIROBOT_CHARACTERISTIC_CONFIGURATION_UUID)) {
    RN4020_writeServerCharacteristicHandle(rn4020, handleLookupItem->handle, (const uint8_t*)&_ble_configurationData, sizeof(_ble_configurationData));
    return;
  }

  if (RN4020_isHandleLookupItemUUIDEqual128(handleLookupItem, BLE_MINIROBOT_CHARACTERISTIC_SENSORS_UUID)) {
    RN4020_writeServerCharacteristicHandle(rn4020, handleLookupItem->handle, (const uint8_t*)&_ble_sensorData, sizeof(_ble_sensorData));
    return;
  }

  if (RN4020_isHandleLookupItemUUIDEqual128(handleLookupItem, BLE_MINIROBOT_CHARACTERISTIC_MOTORS_UUID)) {
    RN4020_writeServerCharacteristicHandle(rn4020, handleLookupItem->handle, (const uint8_t*)&_ble_motorData, sizeof(_ble_motorData));
    return;
  }

  printf("unhandled real time read: ");
  for (int i = 0; i < handleLookupItem->characteristicUUIDLength; i++) {
    uint8_t z = handleLookupItem->characteristicUUID[i];
    printf("%02X", z);
  }
  printf("\n");
}

void RN4020_onWrite(RN4020* rn4020, uint16_t characteristicHandle, uint8_t* data, uint8_t dataLength) {
  RN4020_handleLookupItem* handleLookupItem = RN4020_lookupHandle(rn4020, characteristicHandle);
  if (handleLookupItem == NULL) {
    return;
  }

  if (RN4020_isHandleLookupItemUUIDEqual128(handleLookupItem, BLE_MINIROBOT_CHARACTERISTIC_CONFIGURATION_UUID)) {
    BLE_Configuration* configuration = (BLE_Configuration*)data;
    onBLEWriteConfiguration(configuration);
    memcpy((uint8_t*)&_ble_configurationData, configuration, sizeof(_ble_configurationData));
    return;
  }

  if (RN4020_isHandleLookupItemUUIDEqual128(handleLookupItem, BLE_MINIROBOT_CHARACTERISTIC_MOTORS_UUID)) {
    BLE_MotorData* motorData = (BLE_MotorData*)data;
    onBLESetMotor(motorData->left, motorData->right);
    return;
  }

  printf("unhandled write: ");
  for (int i = 0; i < handleLookupItem->characteristicUUIDLength; i++) {
    uint8_t z = handleLookupItem->characteristicUUID[i];
    printf("%02X", z);
  }
  printf("\n");
}

void RN4020_connectedStateChanged(RN4020* rn4020, bool connected) {
  DEBUG_OUT("%s\n", connected ? "connected" : "disconnected");
  if (!connected) {
    HAL_StatusTypeDef ret;
    ret = RN4020_removeBond(rn4020);
    if (ret != HAL_OK) {
      DEBUG_OUT("could not remove bond after disconnect %d\n", ret);
    }

    ret = RN4020_advertise(rn4020);
    if (ret != HAL_OK) {
      DEBUG_OUT("could not advertise after disconnect %d\n", ret);
      return;
    }
  }
}
