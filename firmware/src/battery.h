
#ifndef _BATTERY_H_
#define _BATTERY_H_

#include <platform_config.h>

HAL_StatusTypeDef battery_setup();
void battery_tick();

/**
 * level 0 - 100
 */
extern void onBatteryLevelUpdate(uint8_t level);

#endif
