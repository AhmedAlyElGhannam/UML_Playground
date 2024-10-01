#include "main.h"

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
static void disp_init();

void setup() 
{
    // enable serial monitor at baud rate 9600
    Serial.begin(9600);

    // initialize lcd display
    disp_init();

    // set pin state for 3 buttons
    pinMode(BTN1_PIN_INC_TIME, INPUT);
    pinMode(BTN2_PIN_DEC_TIME, INPUT);
    pinMode(BTN3_PIN_START_PAUSE, INPUT);

    // set buzzer pin
    //pinMode(BUZZER_PIN, OUTPUT);

    // initial pseudostate
    protimer_init(&protimer);
}

void loop() 
{
    // store current time
    static uint32_t current_time = millis();

    // initialize tick event
    static tick_event_t tick_event;

    // read buttons
    button1_incTime = digitalRead(BTN1_PIN_INC_TIME);
    button2_decTime = digitalRead(BTN2_PIN_DEC_TIME);
    button3_startPause = digitalRead(BTN3_PIN_START_PAUSE);

    // software debouncing
    buttonPadValue = (button1_incTime << 2) | (button2_decTime << 1) | (button3_startPause << 0);
    buttonPadValue = process_button_pad_value(buttonPadValue);

    if (buttonPadValue != 0) // at least one button has been pressed
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

    // dispatch time tick event for every 100ms
    if ((millis() - current_time) >= 100)
    {
        // 100ms have passed
        current_time = millis();

        // create an event
        tick_event.super.sig = TIME_TICK;
        
        // one sub-second has passed
        ++(tick_event.ss);

        // if 10 sub-seconds have passed:
        if (tick_event.ss > 10)
            tick_event.ss = 1; // make it 1 subsecond again

        // call event dispatcher for 1 second tick_time event
        protimer_event_dispatcher(&protimer, &tick_event.super);
    }

}

void protimer_event_dispatcher (protimer_t *const mobj, const event_t *const e)
{
    // active state for passed object (source) (before new event)
    PROTIMER_STATE_t source;
    source = mobj->active_state;
    
    // event status for passed object and NEW event
    EVENT_STATUS_t status;
    status = protimer_state_machine(mobj, e);

    if (status == EVENT_TRANSITION)
    {
        // active state for passed object (target) (after new event)
        PROTIMER_STATE_t target;
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
}

static uint8_t process_button_pad_value(uint8_t buttonPadValue)
{
    // button is initially in RELEASED state
    static BUTTON_STATE_t btn_state = RELEASED;

    // variable to keep track of bouncing time
    static uint32_t curr_time = millis();

    switch (btn_state)
    {
        case RELEASED:
            if (buttonPadValue != 0) // any button is pressed
            {
                btn_state = BOUNCING;
                curr_time = millis();
            }
        break;

        case PRESSED:
            if (buttonPadValue == 0) // no button is pressed
            {
                btn_state = BOUNCING;
                curr_time = millis();
            }
        break;

        case BOUNCING:
            if ((millis() - curr_time) >= 50) // 50ms have passed
            {
                if (buttonPadValue != 0) // if a button is pressed
                {
                    btn_state = PRESSED;
                    return buttonPadValue;
                }
                else // no button is pressed
                    btn_state = RELEASED;
            }
        break;
    }

    return 0;
}

static void disp_init()
{
    lcd_begin(16, 2);
    lcd_clear();
    lcd_move_cursor_L_to_R();
    lcd_set_cursor(0, 0);
    lcd_no_auto_scroll();
    lcd_cursor_off();
}