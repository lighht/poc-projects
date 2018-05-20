/**
 *                                  ****NOT PORTABLE****
 * Read motor pulse_previous and send info to master. Works only for MEGA328p. ****NOT PORTABLE****
 *
 * The motor controller for velox sends pulse_previous at 100 microseconds per cycle
 * If the motor pulse is unchanged for 3 milliseconds, the motor is considered stopped.
 * Return the byte with info after about 3ms 
 * @author: Dheepan
 */
#include "Arduino.h"

// unsigned long loop_count;
// const unsigned long loop_limit = 300000; //about 300 mseconds
uint16_t loop_count;
const uint16_t loop_limit = 6000; // should take less 20 ms since each while loop takes about than 3us.
                                //  The slowest speed of 1mm/s of the motor has a pulse duration of only 1 us,
                                // but when the motor runs slow, 10ms is apparently not enough. 
                                // If you intend to fix this, check raspberry pi pulse generation logic
                                
int pulse_previous;
int iteration_count;
const uint8_t motor_pin = 8; // which in PINB0
const uint8_t dir_pin = 12; // Which is PINB4
int pulse_read;
void setup()
{
    //For debugging
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // Set dir pin as input
    pinMode(dir_pin, INPUT);
    // Set motor pin as input
    pinMode(motor_pin, INPUT);
    Serial.begin(115200);
    pulse_previous = LOW;    
    loop_count = 0;
}

void loop() 
{
    // Using while loop is apparently faster than the loop function call after every read. 
    // While loop takes 2 clock cycles
    // Further reading at http://www.paleotechnologist.net/?p=1808

    // Each loop takes approximately 25 clock cycles(?) which is about 1.5625us at 16Mhz  
    while(loop_count <= loop_limit){         
    //     // Digital read takes about 7 cycles to read from the pin. So reading directly from the register
    //     // Check http://garretlab.web.fc2.com/en/arduino/inside/arduino/wiring_digital.c/digitalRead.html
        pulse_read = (PINB & 0b00000001); //2 cycles?
        if(pulse_previous == pulse_read){ //20 cycles? 2-20 for if else branch.
            loop_count++; //1 cycle
        }
        else{
            pulse_previous = pulse_read; //1 cycle
            loop_count = 0; //1 cycle
        }              
    }
    Serial.write(PINB & 0b00010001);
    loop_count = 0;
}
