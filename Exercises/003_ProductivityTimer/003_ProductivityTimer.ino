// led pin (pwm to control brightness) 
#define LED_PIN   3 // D3~ pin (490Hz)

// defining different light intensities
#define LIGHT_INTENSITY_OFF   0
#define LIGHT_INTENSITY_DIM   25
#define LIGHT_INTENSITY_MED   85
#define LIGHT_INTENSITY_BRI   255

typedef enum 
{
    STATE_OFF = 0,
    STATE_DIM,
    STATE_MED,
    STATE_BRI
} state_t;

typedef enum 
{
    EVENT_OFF = 0,
    EVENT_ON = 1
} event_t;

state_t curr_state = STATE_OFF;
char event;

void light_change_intensity (uint8_t _led_pin, uint8_t _light_intensity);
void light_state_machine(event_t _event);
void run_entry_action(state_t _state);
void light_init();

void setup() 
{
    // enable serial monitor at baud rate 9600
    Serial.begin(9600);
    
    // set led pin as output
    pinMode(LED_PIN, OUTPUT);

    // set default state
    light_init();

    // print welcoming message
    Serial.println("Send 'x' for OFF || 'o' for ON");
}

void loop() 
{
    if (Serial.available() > 0)
    {
        // read character corresponding to event
        event = Serial.read();
        // launch state machine based on received event
        if (event == 'o')
            light_state_machine(EVENT_ON);
        else if (event == 'x')
            light_state_machine(EVENT_OFF);
    }
}

void light_init()
{
    // set initial state
    curr_state = STATE_OFF;
    run_entry_action(STATE_OFF);
}

void run_entry_action(state_t _state)
{
    switch (curr_state)
    {
        case STATE_OFF:
            light_change_intensity(LED_PIN, LIGHT_INTENSITY_OFF);
        break;

        case STATE_DIM:
            light_change_intensity(LED_PIN, LIGHT_INTENSITY_DIM);
        break;

        case STATE_MED:
            light_change_intensity(LED_PIN, LIGHT_INTENSITY_MED);
        break;

        case STATE_BRI:
            light_change_intensity(LED_PIN, LIGHT_INTENSITY_BRI);
        break;
    }
}

void light_change_intensity (uint8_t _led_pin, uint8_t _light_intensity)
{
    analogWrite(_led_pin, _light_intensity);
}

void light_state_machine(event_t _event)
{
    state_t prev_state = curr_state;
    
    switch (curr_state)
    {
        case STATE_OFF:
            if (_event == EVENT_ON)
            {
                curr_state = STATE_DIM;
            }
            else if (_event == EVENT_OFF)
            {
                curr_state = STATE_OFF;
            }
        break;

        case STATE_DIM:
            if (_event == EVENT_ON)
            {
                curr_state = STATE_MED;
            }
            else if (_event == EVENT_OFF)
            {
                curr_state = STATE_OFF;
            }
        break;

        case STATE_MED:
            if (_event == EVENT_ON)
            {
                curr_state = STATE_BRI;
            }
            else if (_event == EVENT_OFF)
            {
                curr_state = STATE_OFF;
            }
        break;

        case STATE_BRI:
            if (_event == EVENT_ON)
            {
                curr_state = STATE_DIM;
            }
            else if (_event == EVENT_OFF)
            {
                curr_state = STATE_OFF;
            }
        break;
    }

    // run entry action upon change in state
    if (curr_state != prev_state)
        run_entry_action(curr_state);
}
