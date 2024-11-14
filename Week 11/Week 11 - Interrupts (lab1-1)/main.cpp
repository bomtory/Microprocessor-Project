#include "mbed.h"

DigitalIn button(PA_14);
Serial pc(USBTX, USBRX);
DigitalOut myled(LED1);

int main() {
		int i = 0;
    while(1) {
        if(!button) {
					pc.printf("Button Press Detected!\r\n");
					myled = !myled;
				}
				i++;
				pc.printf("%d\r\n", i);
				wait(1);
    }
}
