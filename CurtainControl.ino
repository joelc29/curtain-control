#include "secrets.h"
#include "PositionController.h"
#include "ManualInterface.h"
#include "RemoteInterface.h"

#define STATUS_PIN    D2     // Green
#define STEP_PIN      D3     // Purple
#define DIRECTION_PIN D4     // White

#define BOTTOM_SENSOR_PIN   A0
#define TOP_SENSOR_PIN   A1

#define DN_LED   D10
#define DN_NO    D9

#define UP_LED   D8
#define UP_NO    D7

HardwareSerial & serial_stream = Serial0;

PositionController positionController(STATUS_PIN, STEP_PIN, DIRECTION_PIN, TOP_SENSOR_PIN, BOTTOM_SENSOR_PIN);
ManualInterface manualInterface(UP_NO, UP_LED, DN_NO, DN_LED, &positionController);
RemoteInterface remoteInterface(WLAN_SSID, WLAN_PASS, &positionController);

void setup() {
  Serial.begin(9600);

  positionController.setup(serial_stream, &onPositionChange, &onDisabled);
  manualInterface.setup();
  remoteInterface.setup();
}

void loop() {
  manualInterface.listen();
  remoteInterface.listen();
}


void onDisabled()
{
  Serial.println("Stepper Disabled!");
}

void onPositionChange()
{
  manualInterface.update();
}

