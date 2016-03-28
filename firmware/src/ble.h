
#ifndef _BLE_H_
#define _BLE_H_

#include <platform_config.h>
#include <stdbool.h>

HAL_StatusTypeDef ble_setup();
void ble_tick();
bool ble_debugProcessLine(const char* line);

/**
 * batteryLevel 0 - 100
 */
void ble_updateBatteryLevel(uint8_t batteryLevel);

#endif
