#include "mbed.h"
#include "motordriver.h"

InterruptIn button1(PB_7);
InterruptIn button2(PB_7);
InterruptIn button3(PC_4);
Serial pc(USBTX, USBRX);
Motor A(D11, PC_8);

int direction = 1;
float speed = 0.1;

void print_current_state() {
	pc.printf("Current State:\r\n");
	pc.printf("direction: %d\r\n", direction);
	pc.printf("speed: %3.1f\r\n\n", speed);
}
	
void change_direction() {
	direction = !direction;
	pc.printf("Change Rotation Direction to %d\r\n", direction);
	print_current_state();
	wait(0.2);
}

void acc() {
	if(speed < 1) {
		speed += 0.1; 
		pc.printf("Increase Speed to %3.1f\r\n", speed);
	} else {
		pc.printf("Reached Max Speed (No Increase)\r\n");
	}
	print_current_state();
	wait(0.2); 
}


void dec() {
	if (speed>0.2) {
		speed -= 0.1;
		pc.printf("Decrease Speed to %3.1f\r\n", speed);
	} else {
		pc.printf("Reached MIN speed (No Decrease)\r\n");
	}
	print_current_state();
	wait(0.2);
}

int main() {
	print_current_state();
    button1.fall(&change_direction);  
    button2.fall(&acc);
	button3.fall(&dec);
	A.forward(speed);
	while(1) {
		if(direction == 1) {
			A.forward(speed);
		} else {
			A.backward(speed);
		}
	}
}
