
#ifndef _SERVO_H_
#define _SERVO_H_

#include <platform_config.h>
#include <stdbool.h>

HAL_StatusTypeDef servo_setup();
HAL_StatusTypeDef servo_tick();
HAL_StatusTypeDef servo_allStop();
HAL_StatusTypeDef servo_setSpeed(int16_t leftSpeed, int16_t rightSpeed);
HAL_StatusTypeDef servo_setLeftSpeed(int16_t leftSpeed);
HAL_StatusTypeDef servo_setRightSpeed(int16_t rightSpeed);
bool servo_debugProcessLine(const char* line);

#endif
