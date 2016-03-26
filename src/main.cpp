#include <Arduino.h>

#include <CmdMessenger.h>

#include "finger.h"

#define SAMPLE_SIZE 30

#define MIN_SPEED 160.0f
#define MAX_SPEED 250.0f

#define MIN_SETSPEED 190.0f

#define MIN_POS 50.0f
#define MAX_POS 970.0f

#define CYCLE_MOV 50

typedef enum {
  kIdleState,
  kWaitMoveState,
  kMovingState,
  kErrorState,
} FingerState;

CmdMessenger cmdMessenger = CmdMessenger(MYSERIAL);
int	oldPos[NUM_FINGERS];

enum
{
  kSetFingers,
};

FingerState states[NUM_FINGERS];

void OnSetFingers()
{
  MYSERIAL.println("OnSetFingers");
  int i;
  for(i = 0; i < NUM_FINGERS; ++i)
  {
    double pos = cmdMessenger.readDoubleArg();
    pos = min(1, max(0, pos)) * (MAX_POS-MIN_POS) + MIN_POS;
    double speed = cmdMessenger.readDoubleArg();
    speed = min(1, max(0, speed)) * (MAX_SPEED-MIN_SETSPEED) + MIN_SETSPEED;

    MYSERIAL.print("finger: ");
    MYSERIAL.println(i);
    MYSERIAL.print("pos: ");
    MYSERIAL.println(pos);
    MYSERIAL.print("speed: ");
    MYSERIAL.println(speed);
    finger[i].writeSpeed(speed);
    finger[i].writePos(pos);
    states[i] = kWaitMoveState;
  }
}

float speed = 0;
float samples[NUM_FINGERS][SAMPLE_SIZE];
int errors[NUM_FINGERS];
uint32_t currentSample = 0;

void sampleSpeed() {
  for (int i = 0; i < NUM_FINGERS; ++i) {
    samples[i][currentSample % SAMPLE_SIZE] = finger[i].readPos();
  }
  ++currentSample;
}

float avgSample(float sample[], int sampleSize) {
  float result = 0;

  for (int i = 0; i < sampleSize; ++i) {
    result += sample[i];
  }
  return result / sampleSize;
}

float avgSpeed(float sample[], int sampleSize) {
  float result = 0;

  for (int i = currentSample + 1; i < currentSample + SAMPLE_SIZE - 1; ++i) {
    int previous = i % SAMPLE_SIZE - 1;
    previous = previous < 0 ? SAMPLE_SIZE - previous : previous;
    result += sample[i % SAMPLE_SIZE] - sample[previous];
  }
  return result / sampleSize;
}

void updateState() {
  for (int i = 0; i < NUM_FINGERS; ++i) {
    if (finger[i].reachedPos()) {
      states[i] = kIdleState;
    } else if (states[i] == kWaitMoveState && fabs(avgSpeed(samples[i], SAMPLE_SIZE)) > 0.8) {
      states[i] = kMovingState;
      errors[i] = 0;
    } else if (states[i] == kMovingState && fabs(avgSpeed(samples[i], SAMPLE_SIZE)) < 0.6) {
      states[i] = kErrorState;
    } else if (states[i] == kErrorState) {
      errors[i]++;
      if (errors[i] > 5) {
	finger[i].writePos(finger[i].readPos() - avgSpeed(samples[i], SAMPLE_SIZE) / 2);
      }
    }
  }
}

void setup()
{
  memset(errors, 0, sizeof(errors));
  memset(states, 0, sizeof(states));
  memset(samples, 0, sizeof(samples));
  sampleSpeed();

  MYSERIAL.begin(115200);
  MYSERIAL.println("bionicohand v1.0");

  pinAssignment(LEFT);

  cmdMessenger.attach(kSetFingers, OnSetFingers);
}

void loop()
{
  cmdMessenger.feedinSerialData();
  sampleSpeed();

  if (currentSample >= SAMPLE_SIZE) {
	  updateState();
	  for (int i = 0; i < NUM_FINGERS; ++i) {
	    MYSERIAL.print(states[i]);
	    MYSERIAL.print(" ");
	    MYSERIAL.print(errors[i]);
	    MYSERIAL.print(" ");
	    MYSERIAL.print(avgSample(samples[i], SAMPLE_SIZE));
	    MYSERIAL.print(" ");
	    MYSERIAL.print(avgSpeed(samples[i], SAMPLE_SIZE));
	    MYSERIAL.print(" ");
	  }
	  MYSERIAL.println();
  }
}
