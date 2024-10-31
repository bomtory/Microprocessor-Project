#include "mbed.h"

AnalogIn analog_value(A0);
DigitalOut myled(LED1);
#define operating_voltage 5

float measured_voltage;
float output_voltage;

int main(){
	while(1){
		measured_voltage = analog_value.read();
		output_voltage = measured_voltage*operating_voltage;
		
		if (output_voltage > 2) {
			myled = 1;
			printf("output voltage = %.3f \r\n", output_voltage);
		} else {
			myled = 0;
			printf("output voltage = %.3f \r\n", output_voltage);
		}
		wait(0.5);
	}
}
