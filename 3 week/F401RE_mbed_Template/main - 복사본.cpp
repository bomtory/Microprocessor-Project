#include "mbed.h"

PwmOut sound(PC_9);

int main(){
		sound.period(0.000625);
		sound = 0.5;
}
