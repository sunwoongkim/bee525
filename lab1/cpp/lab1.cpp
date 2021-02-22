#include <iostream>
#include <unistd.h>              			// for the usleep() function
#include "GPIO.h"											// Download from Canvas

using namespace exploringRPi;
using namespace std;

int main(){
   GPIO outGPIO(17);    							// pin 11 setting
   outGPIO.setDirection(OUTPUT);    	// basic output example
   for (int i=0; i<10; i++){        	// flash the LED 10 times
      outGPIO.setValue(HIGH);       	// turn the LED on
      usleep(500000);               	// sleep for 0.5 seconds
      outGPIO.setValue(LOW);        	// turn the LED off
      usleep(500000);               	// sleep for 0.5 seconds
   }   
   return 0;
}

