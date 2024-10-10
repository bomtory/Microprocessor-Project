#include "mbed.h"
Serial pc(USBTX, USBRX);
int main(){
	float input;
	while(1){
		pc.printf("minimum number = 3 and number of decimal place = 3\n\r");
		pc.scanf("%f", &input);
		pc.printf("floating point output = %3.3f\n\r", input);
	}
}	
		