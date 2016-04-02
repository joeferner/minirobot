
#ifndef _COMPASS_H_
#define _COMPASS_H_

#include <platform_config.h>

HAL_StatusTypeDef compass_setup();
HAL_StatusTypeDef compass_tick();

extern void onCompassChange(uint16_t heading);

#endif
