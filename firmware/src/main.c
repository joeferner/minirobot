
#include "platform_config.h"
#include "compass.h"
#include "servo.h"
#include "colorSensor.h"
#include "exti.h"
#include "ble.h"
#include "battery.h"
#include <utils/debug.h>
#include <utils/time.h>
#include <utils/utils.h>
#include <string.h>

typedef enum {
  DEBUG_MODE_NOT_SET,
  DEBUG_MODE_SERVO,
  DEBUG_MODE_BLE
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
  assertNonOKHALStatus(exti_setup());
  assertNonOKHALStatus(colorSensor_setup());
  assertNonOKHALStatus(ble_setup());
  assertNonOKHALStatus(battery_setup());

  colorSensor_setGain(1);
  ble_updateColorSensorGain(1);
  
  colorSensor_setLedBrightness(50);
  ble_updateColorSensorLedBrightness(50);
  
  DEBUG_OUT("setup complete\n");
  _printPrompt();
}

void loop() {
  debug_tick();
  compass_tick();
  servo_tick();
  colorSensor_tick();
  ble_tick();
  battery_tick();
}

void onBatteryLevelUpdate(uint8_t level) {
  //DEBUG_OUT("battery level: %d\n", level);
  ble_updateBatteryLevel(level);
}

void onFeeler(Feeler feeler, bool active) {
  /*
  DEBUG_OUT(
    "feeler: %s %s\n",
    feeler == FEELER_LEFT ? "left" : "right",
    active ? "on" : "off"
  );
  */
  ble_updateFeeler(feeler, active);
}

void onWheelSensor(WheelSensor wheelSensor, bool active) {
  /*
  DEBUG_OUT(
    "wheelSensor: %s %s\n",
    wheelSensor == WHEEL_SENSOR_LEFT ? "left" : "right",
    active ? "on" : "off"
  );
  */
}

void onLineSensor(LineSensor lineSensor, bool active) {
  /*
  const char* sensorName = "";
  switch (lineSensor) {
  case LINE_SENSOR_LEFT_IN:
    sensorName = "left in";
    break;
  case LINE_SENSOR_LEFT_OUT:
    sensorName = "left out";
    break;
  case LINE_SENSOR_RIGHT_IN:
    sensorName = "right in";
    break;
  case LINE_SENSOR_RIGHT_OUT:
    sensorName = "right out";
    break;
  }
  DEBUG_OUT("lineSensor: %s %s\n", sensorName, active ? "on" : "off");
  */
  ble_updateLineSensor(lineSensor, active);
}

void onBLESetMotor(int8_t left, int8_t right) {
  DEBUG_OUT("ble set motor: %d %d\n", left, right);
  servo_setSpeed(left, right);
}

void onBLEWriteConfiguration(BLE_Configuration* configuration) {
  colorSensor_setGain(configuration->colorSensorGain);
  colorSensor_setLedBrightness(configuration->colorSensorLedBrightness);
}

void onCompassChange(uint16_t heading) {
  //DEBUG_OUT("heading: %d\n", heading);
  ble_updateCompass(heading);
}

void onColorSensorData(ColorSensorData* colorData) {
  //DEBUG_OUT("colorSensor: r,g,b,c=>%d,%d,%d,%d\n", colorData->r, colorData->g, colorData->b, colorData->c);
  ble_updateColorSensorData(colorData);
}

void debug_processLine(const char* line) {
  if (strlen(line) == 0) {
  } else if (strcmp(line, "exit") == 0) {
    _debugMode = DEBUG_MODE_NOT_SET;
  } else if (strcmp(line, "servo") == 0) {
    _debugMode = DEBUG_MODE_SERVO;
  } else if (strcmp(line, "ble") == 0) {
    _debugMode = DEBUG_MODE_BLE;
  } else {
    bool result = false;
    switch (_debugMode) {
    case DEBUG_MODE_NOT_SET:
      break;
    case DEBUG_MODE_SERVO:
      result = servo_debugProcessLine(line);
      break;
    case DEBUG_MODE_BLE:
      result = ble_debugProcessLine(line);
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
  case DEBUG_MODE_BLE:
    printf("ble");
    break;
  default:
    printf("unknown");
    break;
  }
  printf("> ");
}

