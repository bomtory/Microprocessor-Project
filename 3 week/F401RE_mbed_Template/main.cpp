#include "mbed.h"

DigitalOut myled(LED1);

int main() {
		unsigned int pin = 0x18;
	
	  volatile unsigned int *portA = (unsigned int *) 0x00000000; //GPIOA_BSRR
	
    while(1) {
        *portA = pin; // LED is ON
        wait(0.5); // 500 ms
        
			  *portA = pin << 32; // LED is OFF
        wait(0.5); 
    }
}
