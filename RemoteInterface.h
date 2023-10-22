#ifndef REMOTE_INTERFACE_H
#define REMOTE_INTERFACE_H

#include "WiFi.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "PositionController.h"

class RemoteInterface
{
  public:
    RemoteInterface(const char *ssid, const char *pass, PositionController *positionController);
    void setup();
    void listen();

  private:
    PositionController * _positionController;
    const char *_wlan_ssid;
    const char *_wlan_password;
};
#endif