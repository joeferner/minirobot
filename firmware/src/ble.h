
#ifndef _BLE_H_
#define _BLE_H_

#include <platform_config.h>
#include <stdbool.h>
#include "exti.h"
#include "colorSensor.h"

typedef struct __attribute__((__packed__)) {
  uint8_t colorSensorGain;
  uint8_t colorSensorLedBrightness;
}
BLE_Configuration;

HAL_StatusTypeDef ble_setup();
void ble_tick();
bool ble_debugProcessLine(const char* line);

/**
 * batteryLevel 0 - 100
 */
void ble_updateBatteryLevel(uint8_t batteryLevel);
void ble_updateFeeler(Feeler feeler, bool active);
void ble_updateLineSensor(LineSensor lineSensor, bool active);
void ble_updateCompass(uint16_t heading);
void ble_updateColorSensorData(ColorSensorData* colorData);
void ble_updateColorSensorGain(uint8_t colorSensorGain);
void ble_updateColorSensorLedBrightness(uint8_t colorSensorLedBrightness);

extern void onBLESetMotor(int8_t left, int8_t right);
extern void onBLEWriteConfiguration(BLE_Configuration* configuration);

#endif
