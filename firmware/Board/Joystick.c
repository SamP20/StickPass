#include "Joystick.h"

#include <avr/io.h>

static uint8_t lastState = 0;

static volatile uint8_t joyState = 0;
static uint8_t debounceTimer = 0;
static uint16_t repeatCounter = 0;

void Joystick_MillisecondElapsed(void) {
    if(joyState & JOY_STATE_DEBOUNCING) {
        if(debounceTimer>0) {
            debounceTimer--;
        }else{
            joyState &= ~JOY_STATE_DEBOUNCING;
            debounceTimer = JOY_DEBOUNCE_DELAY;
        }
    }

    if(joyState & JOY_STATE_WAIT_INTERVAL) {
        joyState &= ~JOY_STATE_WAIT_INTERVAL;
        repeatCounter = JOY_STATE_WAIT_INTERVAL;
    }else if(joyState & JOY_STATE_WAIT_DELAY) {
        joyState &= ~JOY_STATE_WAIT_DELAY;
        repeatCounter = JOY_REPEAT_DELAY;
    }

    if(repeatCounter>0) {
        repeatCounter--;
    }else{
        joyState |= JOY_STATE_WAIT_FINISHED;
    }

    if(repeatCounter>0) repeatCounter--;
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
    if(!(joyState & JOY_STATE_DEBOUNCING)) {
        uint8_t state = Joystick_ReadRaw();
        if(state != lastState) {
            joyState |= JOY_STATE_DEBOUNCING;
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
            if(joyState & JOY_STATE_WAIT_FINISHED) {
                joyState |= JOY_STATE_WAIT_INTERVAL;
                joyState &= ~JOY_STATE_WAIT_FINISHED;
                return state;
            }
        }else{
            joyState |= JOY_STATE_WAIT_DELAY;
            joyState &= ~JOY_STATE_WAIT_FINISHED;
            return state;
        }
    }
    return 0;

}
