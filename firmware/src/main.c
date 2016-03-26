
#include "platform_config.h"
#include "compass.h"
#include "servo.h"
#include "gpio.h"
#include <utils/debug.h>
#include <utils/time.h>
#include <utils/utils.h>
#include <string.h>

typedef enum {
  DEBUG_MODE_NOT_SET,
  DEBUG_MODE_SERVO
} _DebugMode;

void _printPrompt();

_DebugMode _debugMode;

void setup() {
  _debugMode = DEBUG_MODE_NOT_SET;
  debug_setup();
  sleep_ms(100);
  DEBUG_OUT("setup\n");

  assertNonOKHALStatus(compass_setup());
  assertNonOKHALStatus(servo_setup());
  assertNonOKHALStatus(gpio_setup());

  DEBUG_OUT("setup complete\n");
  _printPrompt();
}

void loop() {
  debug_tick();
  compass_tick();
  servo_tick();
}

void onFeeler(Feeler feeler, bool active) {
  DEBUG_OUT(
    "feeler: %s %s\n",
    feeler == FEELER_LEFT ? "left" : "right",
    active ? "on" : "off"
  );
}

void compass_onChange(uint16_t heading) {
  //DEBUG_OUT("heading: %d\n", heading);
}

void debug_processLine(const char* line) {
  if (strlen(line) == 0) {
  } else if (strcmp(line, "exit") == 0) {
    _debugMode = DEBUG_MODE_NOT_SET;
  } else if (strcmp(line, "servo") == 0) {
    _debugMode = DEBUG_MODE_SERVO;
  } else {
    bool result = false;
    switch (_debugMode) {
    case DEBUG_MODE_NOT_SET:
      break;
    case DEBUG_MODE_SERVO:
      result = servo_debugProcessLine(line);
      break;
    default:
      printf("unhandled debug mode %d\n", _debugMode);
      result = true;
      break;
    }
    if (!result) {
      printf("invalid debug command: %s\n", line);
    }
  }
  _printPrompt();
}

void _printPrompt() {
  switch (_debugMode) {
  case DEBUG_MODE_NOT_SET:
    break;
  case DEBUG_MODE_SERVO:
    printf("servo");
    break;
  default:
    printf("unknown");
    break;
  }
  printf("> ");
}

