#include "mbed.h"

DigitalOut trig(D10);
DigitalIn echo(D7);

Timer timer;
float elapsed_time;
float distance;
#define speed_of_light 342

int main(){
	timer.start();
	while(1){
		trig = 1;
		wait(0.00005);
		trig = 0;
		while(!echo.read());
		timer.reset();
		while(echo.read());
		elapsed_time = timer.read_us();
		distance = speed_of_light * elapsed_time * 0.0001 / 2;
		printf("Distance: %6.1f cm \r\n", distance);
		wait(1);
	}
}
