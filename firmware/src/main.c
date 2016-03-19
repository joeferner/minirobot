
#include "platform_config.h"
#include <utils/debug.h>
#include <utils/time.h>
#include <string.h>

#define DEBUG_OUT(format, ...) printf("%s:%d: " format, __FILE__, __LINE__, ##__VA_ARGS__)
//#define DEBUG_OUT(format, ...)

volatile GPIO_PinState lastState;

void setup() {
  debug_setup();
  sleep_ms(100);
  DEBUG_OUT("setup\n");
  
  lastState = GPIO_PIN_RESET;
  
  DEBUG_OUT("setup complete\n");
  DEBUG_OUT("> ");
}

void loop() {
  debug_tick();
  
  GPIO_PinState newState = lastState == GPIO_PIN_RESET ? GPIO_PIN_SET : GPIO_PIN_RESET;
  HAL_GPIO_WritePin(DEBUG_GPIO_PORT, DEBUG_GPIO_PIN, newState);
  lastState = newState;
  sleep_ms(500);
}

void debug_processLine(const char* line) {
  if (strlen(line) == 0) {
  } else {
    DEBUG_OUT("invalid debug command: %s\n", line);
  }
  DEBUG_OUT("> ");
}