#include "main.h"

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

// define protimer object as static
static protimer_t protimer;

// define 3 variables to store button state
static uint8_t button1_incTime;
static uint8_t button2_decTime;
static uint8_t button3_startPause;

// variable for combined buttons state (debouncing)
static uint8_t buttonPadValue;

// user event variable
user_event_t user_event;

// function prototypes
static uint8_t process_button_pad_value(uint8_t buttonPadValue);
void protimer_event_dispatcher (protimer_t *const mobj, const event_t *const e);

void setup() 
{
    // enable serial monitor at baud rate 9600
    Serial.begin(9600);
    
    // initial pseudostate
    protimer_init(&protimer);

    // set pin state for 3 buttons
    pinMode(BTN1_PIN_INC_TIME, INPUT);
    pinMode(BTN2_PIN_DEC_TIME, INPUT);
    pinMode(BTN3_PIN_START_PAUSE, INPUT);
}

void loop() 
{
    // read buttons
    button1_incTime = digitalRead(BTN1_PIN_INC_TIME);
    button2_decTime = digitalRead(BTN2_PIN_DEC_TIME);
    button3_startPause = digitalRead(BTN3_PIN_START_PAUSE);

    // software debouncing
    buttonPadValue = (button1_incTime << 2) | (button2_decTime << 1) | (button3_startPause << 0);
    buttonPadValue = process_button_pad_value(buttonPadValue);

    if (buttonPadValue) // at least one button has been pressed
    {
        // make an event based on pressed buttons
        if (buttonPadValue == BTN_PAD_INC_TIME)
            user_event.super.sig = INC_TIME;
        else if (buttonPadValue == BTN_PAD_DEC_TIME)
            user_event.super.sig = DEC_TIME;
        else if (buttonPadValue == BTN_PAD_ABRT)
            user_event.super.sig = ABRT;
        else if (buttonPadValue == BTN_PAD_START_PAUSE)
            user_event.super.sig = START_PAUSE;

        // sent the event to event dispatcher function
        protimer_event_dispatcher(&protimer, &user_event.super);
    }

}

void protimer_event_dispatcher (protimer_t *const mobj, const event_t *const e)
{
    // active state for passed object (source) (before new event)
    STATE_t source;
    source = mobj->active_state;
    
    // event status for passed object and NEW event
    EVENT_STATUS_t status;
    status = protimer_state_machine(mobj, e);

    if (status == EVENT_TRANSITION)
    {
        // active state for passed object (target) (after new event)
        STATE_t target;
        target = mobj->active_state;

        // new event variable to execute exit action for old state and entry action for new state
        event_t ee;

        // run exit action for source state
        ee.sig = EXIT;
        mobj->active_state = source; // active state was already updated
        protimer_state_machine(mobj, &ee);

        // run entry action for target state
        ee.sig = ENTRY;
        mobj->active_state = target; // active state was already updated
        protimer_state_machine(mobj, &ee);
    }
    else if (status == EVENT_HANDLED)
    {

    }
    else if (status == EVENT_IGNORED)
    {

    }
}

static uint8_t process_button_pad_value(uint8_t buttonPadValue)
{
    return 0;
}