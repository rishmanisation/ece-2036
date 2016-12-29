// skeleton code for ECE 2036 thermostat lab
// code must be added by students
#include "mbed.h"
#include "TMP36.h"
#include "uLCD_4DGL.h"
#include "PinDetect.h"
#include "Speaker.h"


// use class to setup temperature sensor pins
TMP36 myTMP36(p15);  //Analog in


// use class to setup the  Color LCD
uLCD_4DGL uLCD(p28, p27, p29); // create a global uLCD object


// use class to setup pushbuttons pins
PinDetect pb1(p23);
PinDetect pb2(p24);
PinDetect pb3(p25);


// use class to setup speaker pin
Speaker mySpeaker(p21); //PWM out


// use class to setup Mbed's four on-board LEDs
DigitalOut myLED1(LED1);
DigitalOut myLED2(LED2);
DigitalOut myLED3(LED3);
DigitalOut myLED4(LED4);


//also setting any unused analog input pins to digital outputs reduces A/D noise a bit
//see http://mbed.org/users/chris/notebook/Getting-best-ADC-performance/
DigitalOut P16(p16);
DigitalOut P17(p17);
DigitalOut P18(p18);
DigitalOut P19(p19);
DigitalOut P20(p20);


// Global variables used in callbacks and main program
// C variables in interrupt routines should use volatile keyword
int volatile heat_setting=78; // heat to temp
int volatile cool_setting=68; // cool to temp
bool volatile mode=false; // heat or cool mode

           


// Callback routine is interrupt activated by a debounced pb1 hit
void pb1_hit_callback (void)
{
 
        heat_setting = heat_setting + 1;
  
   
}
// Callback routine is interrupt activated by a debounced pb2 hit


void pb2_hit_callback (void)
{
   
        heat_setting = heat_setting - 1;
   
}


// Callback routine is interrupt activated by a debounced pb3 hit
void pb3_hit_callback (void)
{
// ADD CODE HERE
}


int main()
{
    float Current_temp=0.0;     
    float tempC, tempF;


    // Use internal pullups for the three pushbuttons
    pb1.mode(PullUp);
    pb2.mode(PullUp);
    pb3.mode(PullUp);


    // Delay for initial pullup to take effect
    wait(.01);


    // Setup Interrupt callback functions for a pb hit
    pb1.attach_deasserted(&pb1_hit_callback);
    pb2.attach_deasserted(&pb2_hit_callback);
    pb3.attach_deasserted(&pb3_hit_callback);


    // Start sampling pb inputs using interrupts
    pb1.setSampleFrequency();
    pb2.setSampleFrequency();
    pb3.setSampleFrequency();
    // pushbuttons now setup and running
    
    
    //conversion to degrees C - from sensor output voltage per TMP36 data sheet
    
    while(1) 
    {
            uLCD.cls();
            tempC = myTMP36.read();
            //convert to degrees F
            tempF = (9.0*tempC)/5.0 + 32.0;
            Current_temp = tempF;
            //print current temp
            uLCD.locate(0,0);  
            uLCD.printf("Temp=%2.0F F \n\r",Current_temp);
            enum Statetype { Heat_off = 0, Heat_on };
            Statetype state;
            if (Current_temp < 74)
                state = Heat_off;
            else
                state = Heat_on;
            while(1) {
                switch (state) {
                    
                    case Heat_off:
                        myLED3 = 0;
                        if (Current_temp < 74)
                        {
                        state = Heat_on;
                        uLCD.locate(10,0);
                        uLCD.printf("HEAT ON");
                        uLCD.printf("\n\nHEATING TO %i F",heat_setting);
                        }
                           
                        break;
                    
                    
                    case Heat_on:
                        myLED3 = 1;
                        if (Current_temp >= 74)
                        {
                        state = Heat_off;
                        uLCD.locate(10,0);
                        uLCD.printf("HEAT OFF");
                        uLCD.printf("\n\nHOLDING ABOVE %i F",cool_setting);
                        }
                        break;
                    
                }
                wait(0.33);
                // heartbeat LED - common debug tool
                // blinks as long as code is running and not locked up
                myLED1=!myLED1;
                break;
            }
            wait(0.33);
            
        }
    }
