
#include "platform_config.h"
#include "compass.h"
#include <utils/debug.h>
#include <utils/time.h>
#include <utils/utils.h>
#include <string.h>

void setup() {
  debug_setup();
  sleep_ms(100);
  DEBUG_OUT("setup\n");

  assertNonOKHALStatus(compass_setup());

  DEBUG_OUT("setup complete\n");
  DEBUG_OUT("> ");
}

void loop() {
  debug_tick();
  compass_tick();
}

void debug_processLine(const char* line) {
  if (strlen(line) == 0) {
  } else {
    DEBUG_OUT("invalid debug command: %s\n", line);
  }
  DEBUG_OUT("> ");
}

void compass_onChange(uint16_t heading) {
  DEBUG_OUT("heading: %d\n", heading);
}
