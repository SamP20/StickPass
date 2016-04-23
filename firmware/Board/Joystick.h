#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>

/** Mask for the joystick being pushed in the left direction. */
#define JOY_LEFT 0x01

/** Mask for the joystick being pushed in the right direction. */
#define JOY_RIGHT 0x02

/** Mask for the joystick being pushed in the upward direction. */
#define JOY_UP 0x04

/** Mask for the joystick being pushed in the downward direction. */
#define JOY_DOWN 0x08

/** Mask for the joystick being pushed inward. */
#define JOY_PRESS 0x10

#define JOY_DEBOUNCE_DELAY 10
#define JOY_REPEAT_DELAY 300
#define JOY_REPEAT_INTERVAL 100

#define JOY_STATE_DEBOUNCING 0x01
#define JOY_STATE_WAIT_FINISHED 0x02
#define JOY_STATE_WAIT_DELAY 0x04
#define JOY_STATE_WAIT_INTERVAL 0x08

static inline void Joystick_Init(void) {

}

static inline void Joystick_Disable(void) {

}

void Joystick_MillisecondElapsed(void); // Used for debouncing and scroll acceleration
uint8_t Joystick_ReadRaw(void);
uint8_t Joystick_GetStatus(void);
/** Returns key state once when pressed, then only returns every
    JOY_REPEAT_INTERVAL milliseconds after JOY_REPEAT_DELAY has expired. At
	all other times this function returns 0. */
uint8_t Joystick_GetKeyRepeat(void);

#endif
