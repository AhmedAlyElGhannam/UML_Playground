#include "main.h"

// define protimer object as static
static protimer_t protimer;

void setup() 
{
    // enable serial monitor at baud rate 9600
    Serial.begin(9600);
    
    // initial pseudostate
    protimer_init(&protimer);
}

void loop() 
{
    
}

