
#ifndef _BLE_H_
#define _BLE_H_

#include <platform_config.h>
#include <stdbool.h>
#include "exti.h"
#include "colorSensor.h"

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

extern void onBLESetMotor(int8_t left, int8_t right);

#endif
