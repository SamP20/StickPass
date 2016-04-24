#include "Joystick.h"

#include <avr/io.h>
#include <stdbool.h>

static uint8_t lastState = 0;

static volatile bool debouncing = 0;
static uint8_t debounceTimer = 0;
static volatile uint8_t repeatState = 0;
static uint16_t repeatCounter = 0;

void Joystick_MillisecondElapsed(void) {
    if(debouncing) {
        if(debounceTimer>0) {
            debounceTimer--;
        }else{
            debouncing = false;
            debounceTimer = JOY_DEBOUNCE_DELAY;
        }
    }

    if(repeatState == JOY_STATE_WAIT_INTERVAL) {
        repeatState = 0;
        repeatCounter = JOY_REPEAT_INTERVAL;
    }else if(repeatState == JOY_STATE_WAIT_DELAY) {
        repeatState = 0;
        repeatCounter = JOY_REPEAT_DELAY;
    }

    if(repeatCounter>0) {
        repeatCounter--;
        if(repeatCounter == 0)
            repeatState = JOY_STATE_WAIT_FINISHED;
    }
}

uint8_t Joystick_ReadRaw(void) {
    uint8_t result = 0;
    if(PINF & _BV(PF5)) result |= JOY_LEFT;
    if(PINF & _BV(PF7)) result |= JOY_RIGHT;
    if(PINF & _BV(PF4)) result |= JOY_UP;
    if(PINF & _BV(PF6)) result |= JOY_DOWN;
    if(PINC & _BV(PC6)) result |= JOY_PRESS;
    return result;
}

uint8_t Joystick_GetStatus(void) {
    if(!debouncing) {
        uint8_t state = Joystick_ReadRaw();
        if(state != lastState) {
            debouncing = true;
            lastState = state;
        }
    }
    return lastState;
}

uint8_t Joystick_GetKeyRepeat(void) {
    uint8_t last = lastState;
    uint8_t state = Joystick_GetStatus();
    if(state) {
        if(last) {
            if(repeatState == JOY_STATE_WAIT_FINISHED) {
                repeatState = JOY_STATE_WAIT_INTERVAL;
                return state;
            }
        }else{
            repeatState = JOY_STATE_WAIT_DELAY;
            return state;
        }
    }
    return 0;

}
