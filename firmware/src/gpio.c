
#include "gpio.h"

HAL_StatusTypeDef gpio_setup() {
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  return HAL_OK;
}

void HAL_GPIO_EXTI_Callback(uint16_t pin) {
  switch (pin) {
  case FEELER_LEFT_PIN:
    onFeeler(FEELER_LEFT, HAL_GPIO_ReadPin(FEELER_LEFT_PORT, FEELER_LEFT_PIN) == GPIO_PIN_RESET);
    break;
  case FEELER_RIGHT_PIN:
    onFeeler(FEELER_RIGHT, HAL_GPIO_ReadPin(FEELER_RIGHT_PORT, FEELER_RIGHT_PIN) == GPIO_PIN_RESET);
    break;
  case WHEEL_LEFT_PIN:
    onWheelSensor(WHEEL_SENSOR_LEFT, HAL_GPIO_ReadPin(WHEEL_LEFT_PORT, WHEEL_LEFT_PIN) == GPIO_PIN_RESET);
    break;
  case WHEEL_RIGHT_PIN:
    onWheelSensor(WHEEL_SENSOR_RIGHT, HAL_GPIO_ReadPin(WHEEL_RIGHT_PORT, WHEEL_RIGHT_PIN) == GPIO_PIN_RESET);
    break;
  case LINE_LEFT_IN_PIN:
    onLineSensor(LINE_SENSOR_LEFT_IN, HAL_GPIO_ReadPin(LINE_LEFT_IN_PORT, LINE_LEFT_IN_PIN) == GPIO_PIN_RESET);
    break;
  case LINE_LEFT_OUT_PIN:
    onLineSensor(LINE_SENSOR_LEFT_OUT, HAL_GPIO_ReadPin(LINE_LEFT_OUT_PORT, LINE_LEFT_OUT_PIN) == GPIO_PIN_RESET);
    break;
  case LINE_RIGHT_IN_PIN:
    onLineSensor(LINE_SENSOR_RIGHT_IN, HAL_GPIO_ReadPin(LINE_RIGHT_IN_PORT, LINE_RIGHT_IN_PIN) == GPIO_PIN_RESET);
    break;
  case LINE_RIGHT_OUT_PIN:
    onLineSensor(LINE_SENSOR_RIGHT_OUT, HAL_GPIO_ReadPin(LINE_RIGHT_OUT_PORT, LINE_RIGHT_OUT_PIN) == GPIO_PIN_RESET);
    break;
  default:
    DEBUG_OUT("unhandled GPIO EXTI pin %d\n", pin);
    break;
  }
}

void EXTI0_IRQHandler() {
  HAL_GPIO_EXTI_IRQHandler(WHEEL_LEFT_PIN);
}

void EXTI1_IRQHandler() {
  HAL_GPIO_EXTI_IRQHandler(WHEEL_RIGHT_PIN);
}

void EXTI2_IRQHandler() {
}

void EXTI3_IRQHandler() {
}

void EXTI4_IRQHandler() {
}

void EXTI9_5_IRQHandler() {
  HAL_GPIO_EXTI_IRQHandler(FEELER_LEFT_PIN);
  HAL_GPIO_EXTI_IRQHandler(FEELER_RIGHT_PIN);
}

void EXTI15_10_IRQHandler() {
  HAL_GPIO_EXTI_IRQHandler(LINE_RIGHT_OUT_PIN);
  HAL_GPIO_EXTI_IRQHandler(LINE_RIGHT_IN_PIN);
  HAL_GPIO_EXTI_IRQHandler(LINE_LEFT_IN_PIN);
  HAL_GPIO_EXTI_IRQHandler(LINE_LEFT_OUT_PIN);
}
