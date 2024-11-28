#include "mbed.h"

Serial pc(USBTX, USBRX);
AnalogIn x_axis(PC_2);
AnalogIn y_axis(PC_3);
InterruptIn button(PA_15);

int x, y, debug_print;
Ticker joystick;
DigitalOut led(LED1);       

void joystick_Handler()
{
    x = x_axis.read() * 1000;
    y = y_axis.read() * 1000;
		debug_print = 1;
}

void BUTTON_ISR() {
		pc.printf ("\rButton Pressed\n");
		led = !led;   
}

int main()
{
		pc.baud(9600);  
  	joystick.attach(joystick_Handler, 0.5);    
    
    while(1) {
        if (!button) {           
					  button.rise(&BUTTON_ISR);           						
        }
				if(debug_print) {
            pc.printf("\rX=%3d, Y=%3d \n", x,y);
						debug_print = 0;
				}
    }
}


