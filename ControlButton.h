#ifndef CONTROL_BUTTON_H
#define CONTROL_BUTTON_H

#include <Arduino.h>
#include <iostream>
using namespace std;

class ControlButton {
  public:
    ControlButton(const int normallyOpenPin, const int statusIndicatorPin);
    void setup();
    bool isPressed();
    void update();

    enum Status {
      OFF = 0,
      ON = 1,
      IN_PROGRESS = 2
    };

    void setStatus(Status status);

  private:
    int _normallyOpenPin;
    int _statusIndicatorPin;
    Status _status;
    long _statusMillis;
    void _blink();
};
#endif