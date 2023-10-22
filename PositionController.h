#ifndef POSITION_CONTROLLER_H
#define POSITION_CONTROLLER_H 

#include <TMC2209.h>

class PositionController{
  public:
    PositionController(const int statusPin, const int stepPin, int const directionPin, int const topPositionSensorPin, int const bottomPositionSensorPin);
    void setup(HardwareSerial &serial, void (*onPositionChange)(), void (*onDisabled)());
    bool isEnabled();
    bool canRaise();
    bool canLower();
    bool isBusy();
    void raise();
    void lower();
    void seek(int percentOpen);
    int percentOpen();
    bool isSeekingUp();
    bool isSeekingDown();
    void calibrate();

  private:
    void _step();
    int _statusPin;
    int _stepPin;
    int _directionPin;
    int _topPositionSensorPin;
    int _bottomPositionSensorPin;
    uint16_t _stepRange;
    TMC2209 _stepperDriver;
    uint16_t _position;
    bool _status;
    bool _isSeekingUp;
    bool _isSeekingDown;
    intptr_t _accessor;
    void (*_onPositionChange)();
    void (*_onDisabled)();
    void _refreshStatus();
};
#endif