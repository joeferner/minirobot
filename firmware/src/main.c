
#include <pinout.h>

volatile GPIO_PinState lastState;

void sleep_ms(uint32_t ms);
void sleep_us(uint32_t us);

void setup() {
  lastState = GPIO_PIN_RESET;
}

void loop() {
  GPIO_PinState newState = lastState == GPIO_PIN_RESET ? GPIO_PIN_SET : GPIO_PIN_RESET;
  HAL_GPIO_WritePin(DEBUG_GPIO_PORT, DEBUG_GPIO_PIN, newState);
  lastState = newState;
  sleep_ms(500);
}

void sleep_ms(uint32_t ms) {
  volatile uint32_t i;
  for (i = ms; i != 0; i--) {
    sleep_us(1000);
  }
}

void sleep_us(uint32_t us) {
  volatile uint32_t i;
  for (i = ((SystemCoreClock / 8000000) * us); i != 0; i--) {}
}
