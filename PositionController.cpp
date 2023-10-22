#include "PositionController.h"

const uint16_t HALF_STEP_DURATION_MICROSECONDS = 100;
const uint8_t RUN_CURRENT_PERCENT = 33;
const int raiseDirection = LOW;

PositionController::PositionController(const int statusPin, const int stepPin, const int directionPin, int const topPositionSensorPin, int const bottomPositionSensorPin)
{
  _statusPin = statusPin;
  _stepPin = stepPin;
  _directionPin = directionPin;
  _topPositionSensorPin = topPositionSensorPin;
  _bottomPositionSensorPin = bottomPositionSensorPin;
}

void PositionController::setup(HardwareSerial &serial_stream, void (*onPositionChange)(), void (*onDisabled)())
{
  _onPositionChange = onPositionChange;
  _onDisabled = onDisabled;

  pinMode(_statusPin, INPUT_PULLUP);
  pinMode(_stepPin, OUTPUT);
  pinMode(_directionPin, OUTPUT);
  pinMode(_topPositionSensorPin, INPUT_PULLUP);
  pinMode(_bottomPositionSensorPin, INPUT_PULLUP);

  _stepperDriver.setup(serial_stream);
  _stepperDriver.setRunCurrent(RUN_CURRENT_PERCENT);
  _stepperDriver.enableCoolStep();
  _stepperDriver.enable();

  calibrate();
}

void PositionController::_refreshStatus()
{
  int currentStatus = digitalRead(_statusPin);

  if(_status && !currentStatus)
  {
    _onDisabled();
  }

  _status = currentStatus;
}

bool PositionController::canRaise(){
  return _position < _stepRange && _status;
}

bool PositionController::canLower(){
  return  _position > 0 && _status;
}

bool PositionController::isBusy()
{
  return _isSeekingUp || _isSeekingDown;
}

void PositionController::raise()
{
  if(canRaise())
  {
    digitalWrite(_directionPin, raiseDirection);
    _step();
    _position = _position + 1;
    _onPositionChange();
    Serial.println(_position);
  }
}

void PositionController::lower()
{
  if(canLower())
  {
    digitalWrite(_directionPin, !raiseDirection);
    _step();
    _position = _position - 1;
    _onPositionChange();
    Serial.println(_position);
  }
}

void PositionController::calibrate()
{
  uint16_t range = 0;
  
  digitalWrite(_directionPin, !raiseDirection);
  while(digitalRead(_bottomPositionSensorPin))
  {
    _isSeekingDown = true;
    _step();
    _onPositionChange();
    _isSeekingUp = true;
    _isSeekingDown = false;
  }

  Serial.println("bottom position found");

  digitalWrite(_directionPin, raiseDirection);
  while(digitalRead(_topPositionSensorPin))
  {
    range = range + 1;
    _step();
    _onPositionChange();
    _isSeekingUp = false;
  }

  Serial.println("top position found");
  Serial.print("step range is: ");
  Serial.println(range);

  _stepRange = range;
  _position = _stepRange;
  _onPositionChange();
}


void PositionController::seek(int percentOpen)
{
  if( percentOpen > 100 || percentOpen < 0 )
  {
    return;
  }

  int position = map(percentOpen, 0, 100, 0, _stepRange);
  Serial.print("seeking ");
  Serial.println(position);

  if(position == _position)
  {
    return;
  }

  while(_position < position && canRaise())
  {
    _isSeekingUp = true;
    raise();
    _isSeekingUp = false;
  }

  while(_position > position && canLower())
  {
    _isSeekingDown = true;
    lower();
    _isSeekingDown = false;
  }

  _onPositionChange();
}

int PositionController::percentOpen()
{
  return map(_position, 0, _stepRange, 0, 100);
}

bool PositionController::isSeekingUp()
{
  return _isSeekingUp;
}

bool PositionController::isSeekingDown()
{
  return _isSeekingDown;
}

void PositionController::_step()
{
  _refreshStatus();

  digitalWrite(_stepPin, LOW);
  delayMicroseconds(HALF_STEP_DURATION_MICROSECONDS);
  digitalWrite(_stepPin, HIGH);
  delayMicroseconds(HALF_STEP_DURATION_MICROSECONDS);
}