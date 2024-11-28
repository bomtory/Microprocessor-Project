#include "mbed.h"

Timer timer;
Timeout timeout;
Ticker ticker;

Serial pc(USBTX, USBRX);

AnalogIn light_sensor(A0);
DigitalOut led(LED1);
DigitalOut rgb_led(PC_6);

void print_question();
void time_is_out();
void success();
void fail();
void blink_rgb_led();
void blink_led();

void print_question() {
	printf("If you are ready to answer, press any button. \r\n");
	pc.getc();
	printf("7 + 2 = ? \r\n");
	timeout.attach(&time_is_out, 2.0); //set a timeout which calls 'time is out' function after 2 seconds
}

void time_is_out() {
	printf("TIME OUT: Try again. \r\n\n");
	wait(1);
	print_question();
}

void success() {
	printf("Coreect! Good job. \r\n");
	ticker.attach(&blink_rgb_led, 0.1);
	wait(3);
	ticker.detach();
}

void fail() {
	printf("Wrong! Try again. \r\n\n");
	ticker.attach(&blink_rgb_led, 0.5); // set a ticker which calls
	wait(3);
	ticker.detach(); // exit the ticker you set above
}

void blink_rgb_led() {
	rgb_led =! rgb_led;
}

void blink_led() {
	led =! led;
}

int main() {
	while(1) {
		print_question();
		char your_answer = pc.getc();
		timeout.detach(); // cancel timeout because the answer has been input
		printf("Your answer is %c \r\n", your_answer);
		if (your_answer == '9') {
			success();
			break;
		}
		else {
			fail();
		}
	}
}
