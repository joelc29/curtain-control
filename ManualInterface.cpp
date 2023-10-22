#include "ManualInterface.h"
#include "ControlButton.h"

ManualInterface::ManualInterface(const int upPin, const int upLedPin, const int downPin, const int downLedPin, PositionController *controller) :
  _up(upPin, upLedPin),
  _down(downPin, downLedPin)
  {
    _controller = controller;
  };

void ManualInterface::setup()
{
  _up.setup();
  _down.setup();
  update();
}

void ManualInterface::listen()
{
  while(_up.isPressed() && _controller->canRaise())
  {
    _controller->raise();
  }

  while(_down.isPressed() && _controller->canLower())
  {
    _controller->lower();
  }
}

void ManualInterface::update()
{
  if(_controller->isSeekingUp())
  {
    _up.setStatus(ControlButton::Status::IN_PROGRESS);
    _down.setStatus(ControlButton::Status::OFF);
    return;
  }

  if(_controller->isSeekingDown())
  {
    _up.setStatus(ControlButton::Status::OFF);
    _down.setStatus(ControlButton::Status::IN_PROGRESS);
    return;
  }

  _up.setStatus(static_cast<ControlButton::Status>(_controller->canRaise()));
  _down.setStatus(static_cast<ControlButton::Status>(_controller->canLower()));
}
