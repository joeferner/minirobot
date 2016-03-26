
#include "gpio.h"

HAL_StatusTypeDef gpio_setup() {
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
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
  default:
    DEBUG_OUT("unhandled GPIO EXTI pin %d\n", pin);
    break;
  }
}

void EXTI0_IRQHandler() {
}

void EXTI1_IRQHandler() {
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
}
