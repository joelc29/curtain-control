#include "ControlButton.h"

ControlButton::ControlButton(const int normallyOpenPin, const int statusIndicatorPin)
{
  _normallyOpenPin = normallyOpenPin;
  _statusIndicatorPin = statusIndicatorPin;
}

void ControlButton::setup()
{  
  pinMode(_normallyOpenPin, INPUT_PULLUP);
  pinMode(_statusIndicatorPin, OUTPUT);
}

bool ControlButton::isPressed() {
  if(_status != Status::ON)
  {
    return false;
  }

  return !digitalRead(_normallyOpenPin);
}

void ControlButton::setStatus(Status status) {
  if(status == _status)
  {
    if(_status == Status::IN_PROGRESS)
    {
      _blink();
    }

    return;
  }
  
  _status = status;
  _statusMillis = millis();
  
  if(status == Status::ON || status == Status::OFF)
  {
    digitalWrite(_statusIndicatorPin, status);
  }
}

void ControlButton::_blink()
{
  int val = (int)((millis() - _statusMillis) / 300) % 2;
  digitalWrite(_statusIndicatorPin, val);
}