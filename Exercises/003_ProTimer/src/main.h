#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "lcd.h"

// defined signals in machine
typedef enum
{
    INC_TIME,
    DEC_TIME,
    TIME_TICK,
    START_PAUSE,
    ABRT,
    ENTRY,
    EXIT
} SIGNAL_t;

// defined states in machine
typedef enum 
{
    IDLE,
    TIME_SET,
    COUNTDOWN,
    PAUSE,
    STAT
} STATE_t;

// struct for generic/super event
typedef struct
{
    uint8_t sig; // event signal
} event_t;

// user generated event
typedef struct 
{
    event_t super; // generic event signal (structure embedding/inheritence)
} user_event_t;

// tick event
typedef struct
{
    event_t super; // generic event signal (structure embedding/inheritence)
    uint8_t ss; // subsecond
} tick_event_t;



#endif