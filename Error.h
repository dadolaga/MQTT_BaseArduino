#ifndef __ERROR__
#define __ERROR__

#ifdef ARDUINO_UNOR4_WIFI
#include "Arduino_LED_Matrix.h"
#include "animation.h"
#endif

#include <Arduino.h>
#include "api/Common.h"
#include "pins_arduino.h"

class Error {
  public:
    
};

class MatrixError {
  private:
    ArduinoLEDMatrix* matrix;

  public:
    MatrixError() {
#ifdef ARDUINO_UNOR4_WIFI
      matrix = new ArduinoLEDMatrix();
#else
      matrix = nullptr;
#endif
    }

    void searchWifi() {
      if(matrix == nullptr)
        return;

      matrix->clear();
      matrix->loadSequence(LEDMATRIX_ANIMATION_WIFI_SEARCH);
      matrix->begin();
      matrix->play(true);
    }

    void connectToServer() {
      if(matrix == nullptr)
        return;

      matrix->clear();
      matrix->loadSequence(UPLOAD);
      matrix->play(true);
    }

    void complete() {
      if(matrix == nullptr)
        return;

      matrix->clear();
      matrix->loadSequence(LEDMATRIX_ANIMATION_CHECK);
      matrix->play(false);

      delay(1000);

      matrix->clear();
    }
};

class LedError {
  private:
  unsigned long LedOnTime = 150;
  unsigned long FastBlink = 300;
  unsigned long SlowBlink = 700;
  unsigned long StopBlink = 1500;

  unsigned long int Time;
  int Led;

  public:
  LedError(int led = LED_BUILTIN) {
    Led = led;
  }

  void init() {
    pinMode(Led, OUTPUT);
  }

  // led plink for 3 times fast
  void wifiConnectionFail() {
    while(true) {
      for(byte i = 0; i < 3; i++) {
        digitalWrite(Led, HIGH);
        delay(LedOnTime);
        digitalWrite(Led, LOW);
        delay(FastBlink);
      }
  
      delay(StopBlink);
    }
  }

  // led plink for 2 times fast
  void serverNotResponse() {
    while(true) {
      for(byte i = 0; i < 2; i++) {
        digitalWrite(Led, HIGH);
        delay(LedOnTime);
        digitalWrite(Led, LOW);
        delay(FastBlink);
      }
  
      delay(StopBlink);
    }
  }
};

#endif