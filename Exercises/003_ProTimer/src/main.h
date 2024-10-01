#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "lcd.h"

// defined pins
#define BTN1_PIN_INC_TIME         2
#define BTN2_PIN_DEC_TIME         3
#define BTN3_PIN_START_PAUSE      4
#define LCD_PIN_RS                5
#define LCD_PIN_RW                6
#define LCD_PIN_EN                7
#define LCD_PIN_D4                8
#define LCD_PIN_D5                9
#define LCD_PIN_D6                10
#define LCD_PIN_D7                11
#define BUZZER_PIN                12

// defining button pad value (for software debouncing)
/**
 *      B1 B2 B3 V Sig
 *      0  0  1  1 START_PAUSE
 *      0  1  0  2 DEC_TIME
 *      1  0  0  4 INC_TIME
 *      1  1  0  6 ABRT
 *      X  X  X  X DC (For Other Combinations)
 */
#define BTN_PAD_INC_TIME          4
#define BTN_PAD_DEC_TIME          2
#define BTN_PAD_ABRT              6
#define BTN_PAD_START_PAUSE       1

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
} PROTIMER_SIGNAL_t;

// defined states in machine
typedef enum 
{
    IDLE,
    TIME_SET,
    COUNTDOWN,
    PAUSE,
    STAT
} PROTIMER_STATE_t;

// defining event status
typedef enum 
{
    EVENT_HANDLED,
    EVENT_IGNORED,
    EVENT_TRANSITION
} EVENT_STATUS_t;

// defining button states
typedef enum
{
    RELEASED,
    BOUNCING,
    PRESSED
} BUTTON_STATE_t;

// struct for generic/super event
typedef struct
{
    PROTIMER_SIGNAL_t sig; // event signal
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

// main app object
typedef struct
{
    PROTIMER_STATE_t active_state;
    uint16_t counter_time;
    uint16_t elapsed_time;
    uint16_t productive_time;
} protimer_t;

void protimer_init(protimer_t *const mobj);
EVENT_STATUS_t protimer_state_machine(protimer_t *const mobj, const event_t *const e);
void protimer_event_dispatcher (protimer_t *const mobj, const event_t *const e);

#endif