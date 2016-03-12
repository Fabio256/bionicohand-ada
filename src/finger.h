#if not defined(FINGER_H)
#define FINGER_H

#include <FingerLib.h>
//
//#define ADULT_BOARD
//#define FOUR_MOTOR_BOARD
//#define CHILD_BOARD
#define ALMOND_BOARD

#if defined(ADULT_BOARD)
#define NUM_FINGERS 5
#elif defined(FOUR_MOTOR_BOARD)
#define NUM_FINGERS 4
#elif defined(CHILD_BOARD)
#define NUM_FINGERS 3
#elif defined(ALMOND_BOARD)
#define NUM_FINGERS 5
#endif

extern Finger finger[NUM_FINGERS];

void pinAssignment(int);

#endif
