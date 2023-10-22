#include "secrets.h"
#include "RemoteInterface.h"
#include <iostream>
using namespace std;

int wiFiStatus = WL_IDLE_STATUS;
WiFiClient _client;
Adafruit_MQTT_Client _mqtt(&_client, IO_SERVER, IO_SERVERPORT, IO_USERNAME, IO_KEY);
Adafruit_MQTT_Subscribe _positionSubscription = Adafruit_MQTT_Subscribe(&_mqtt, POSITION_FEED);

Adafruit_MQTT_Publish _statusPublisher = Adafruit_MQTT_Publish(&_mqtt, STATUS_FEED);

const int wifiConnectionTimeoutMillis = 5000;
uint32_t lastPublishedPosition = 999;

long nextCheck = 0;

RemoteInterface::RemoteInterface(const char *ssid, const char *pass, PositionController *positionController)
{
  _wlan_ssid = ssid;
  _wlan_password = pass;
  _positionController = positionController;
}

void RemoteInterface::setup()
{
  WiFi.begin(_wlan_ssid, _wlan_password);

  int t0 = millis(); 

  while (WiFi.status() != WL_CONNECTED && millis() - t0 < wifiConnectionTimeoutMillis) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("unable to establish network connection");
    return;
  }
  
  Serial.println("WiFi connected");

  _mqtt.subscribe(&_positionSubscription);
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (_mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = _mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(_mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 10 seconds...");
       _mqtt.disconnect();
       delay(10000);  // wait 10 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}


bool containsOnlyDigits(const char* str) {
    while (*str != '\0') {
        if (!isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}

void RemoteInterface::listen()
{
  if(millis() < nextCheck)
  {
    return;
  }

  MQTT_connect();
  
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = _mqtt.readSubscription(100))) {
    if (subscription == &_positionSubscription) {
      char* value = (char*)_positionSubscription.lastread;

      if(containsOnlyDigits)
      {
        _positionController->seek(stoi(value));
      }

      _statusPublisher.publish(_positionController->percentOpen());
    }
  }

  if(!_mqtt.ping()) {
    _mqtt.disconnect();
  }

  nextCheck = millis() + 1000;
}
