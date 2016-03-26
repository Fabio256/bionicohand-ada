#include <Arduino.h>

#include <CmdMessenger.h>

#include "finger.h"

#define MIN_SPEED 190.0f
#define MAX_SPEED 250.0f

#define MIN_POS 50.0f
#define MAX_POS 970.0f

CmdMessenger cmdMessenger = CmdMessenger(MYSERIAL);

enum
{
  kSetFingers,
};

void OnSetFingers()
{
  int i;
  for(i = 0; i < NUM_FINGERS; ++i)
  {
    double pos = cmdMessenger.readDoubleArg();
    pos = min(1, max(0, pos)) * (MAX_POS-MIN_POS) + MIN_POS;
    double speed = cmdMessenger.readDoubleArg();
    speed = min(1, max(0, speed)) * (MAX_SPEED-MIN_SPEED) + MIN_SPEED;

    finger[i].writeSpeed(speed);
    finger[i].writePos(pos);
  }
}

void setup()
{
  MYSERIAL.begin(115200);
  MYSERIAL.println("bionicohand v1.0");

  pinAssignment(LEFT);

  cmdMessenger.printLfCr();

  cmdMessenger.attach(kSetFingers, OnSetFingers);
}

void loop()
{
  cmdMessenger.feedinSerialData();
}
