#ifndef MANUAL_INTERFACE_H
#define MANUAL_INTERFACE_H 

#include "ControlButton.h"
#include "PositionController.h"

class ManualInterface{
  public:
    ManualInterface(const int upPin, const int upLed, const int downPin, const int downLed, PositionController *controller);
    void setup();
    void listen();
    void update();

  private:
    ControlButton _up;
    ControlButton _down;
    PositionController *_controller;
};
#endif