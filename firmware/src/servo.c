
#include "servo.h"
#include <stdlib.h>
#include <string.h>
#include <utils/utils.h>

#define _SERVO_PERIOD() ((int32_t)SERVO_TIMER.Init.Period)

TIM_HandleTypeDef _servo_timHandle;

HAL_StatusTypeDef _servo_setPulse(uint32_t channel, uint32_t pulse);
HAL_StatusTypeDef _servo_setSpeed(uint32_t channel, int16_t speed);
uint32_t _servo_calculatePulse(int16_t speed);

HAL_StatusTypeDef servo_setup() {
  DEBUG_OUT("servo setup\n");

  returnNonOKHALStatus(servo_allStop());
  returnNonOKHALStatus(HAL_TIM_PWM_Start(&SERVO_TIMER, SERVO_LEFT_CH));
  returnNonOKHALStatus(HAL_TIM_PWM_Start(&SERVO_TIMER, SERVO_RIGHT_CH));

  return HAL_OK;
}

HAL_StatusTypeDef servo_tick() {
  return HAL_OK;
}

HAL_StatusTypeDef servo_allStop() {
  return servo_setSpeed(0, 0);
}

HAL_StatusTypeDef servo_setSpeed(int16_t leftSpeed, int16_t rightSpeed) {
  HAL_StatusTypeDef resultLeft = _servo_setSpeed(SERVO_LEFT_CH, leftSpeed);
  HAL_StatusTypeDef resultRight = _servo_setSpeed(SERVO_RIGHT_CH, rightSpeed);
  if (resultLeft != HAL_OK) {
    return resultLeft;
  }
  if (resultRight != HAL_OK) {
    return resultRight;
  }
  return HAL_OK;
}

HAL_StatusTypeDef servo_setLeftSpeed(int16_t leftSpeed) {
  return _servo_setSpeed(SERVO_LEFT_CH, leftSpeed);
}

HAL_StatusTypeDef servo_setRightSpeed(int16_t rightSpeed) {
  return _servo_setSpeed(SERVO_RIGHT_CH, rightSpeed);
}

HAL_StatusTypeDef _servo_setSpeed(uint32_t channel, int16_t speed) {
  uint32_t pulse = _servo_calculatePulse(speed);
  return _servo_setPulse(channel, pulse);
}

uint32_t _servo_calculatePulse(int16_t speed) {
  if (speed == 0) {
    return 0;
  }
  int32_t clampedSpeed = clamp(speed, -100, 100);

  // PPM - assuming the period is set to 20ms we need to generate a pulse between
  // 0.5ms/2.5% (full reverse) and 2.5ms/12.5% (full forward) with stop being the
  // midpoint 1.5ms/7.5%
  int32_t totalPeriod = _SERVO_PERIOD();
  int32_t workingPeriod = (totalPeriod * (125 - 25)) / 10 / 100; // = totalPeriod * (12.5% - 2.5%) = totalPeriod * (12.5 - 2.5)/100
  int32_t zeroOffset = (totalPeriod * 25) / 10 / 100; // = totalPeriod * 2.5% = totalPeriod * 2.5/100
  int32_t speedPercent = (clampedSpeed + 100) / 2;
  int32_t pulse = zeroOffset + (workingPeriod * speedPercent / 100);
  return pulse;
}

HAL_StatusTypeDef _servo_setPulse(uint32_t channel, uint32_t pulse) {
  __HAL_TIM_SET_COMPARE(&SERVO_TIMER, channel, pulse);
  return HAL_OK;
}

bool servo_debugProcessLine(const char* line) {
  HAL_StatusTypeDef result;
  char* ch;
  if (line[0] == 'l' || line[1] == 'L') {
    int16_t newLeftSpeed = atoi(line + 1);
    result = servo_setLeftSpeed(newLeftSpeed);
    if (result == HAL_OK) {
      printf("OK left %d\n", newLeftSpeed);
    } else {
      printf("FAIL %d\n", result);
    }
    return true;
  } else if (line[0] == 'r' || line[1] == 'R') {
    int16_t newRightSpeed = atoi(line + 1);
    result = servo_setRightSpeed(newRightSpeed);
    if (result == HAL_OK) {
      printf("OK right %d\n", newRightSpeed);
    } else {
      printf("FAIL %d\n", result);
    }
    return true;
  } else if ((ch = strchr(line, ',')) != NULL) {
    char leftStr[100];
    strncpy(leftStr, line, ch - line);
    int16_t leftSpeed = atoi(leftStr);
    int16_t rightSpeed = atoi(ch + 1);
    result = servo_setSpeed(leftSpeed, rightSpeed);
    if (result == HAL_OK) {
      printf("OK %d, %d\n", leftSpeed, rightSpeed);
    } else {
      printf("FAIL %d\n", result);
    }
    return true;
  } else if (strcmp(line, "stop") == 0) {
    result = servo_allStop();
    if (result == HAL_OK) {
      printf("OK all stop\n");
    } else {
      printf("FAIL %d\n", result);
    }
    return true;
  }
  return false;
}
