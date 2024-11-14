#include "mbed.h"

InterruptIn button(PA_14);
Serial pc(USBTX, USBRX);
DigitalOut myled(LED1);

void ISR1() {
	pc.printf("Button Press Detected!\r\n");
	myled = !myled;
	wait(0.2);
}

int main() {
	button.fall(&ISR1);
	for(int i=0; ; i++) {
		pc.printf("%d\r\n", i);
		wait(1);
	}
}
