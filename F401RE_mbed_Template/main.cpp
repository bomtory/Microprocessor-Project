#include "mbed.h"
Serial pc(USBTX, USBRX);
int main(){
	float input1;
	float input2;
	char operation;
	float result;
	while(1){
		pc.printf("Please enter your first operand:\n\r");
		scanf("%f", &input1);
		printf("%3.2f\n\r", input1);
		pc.printf("Please enter your second operand:\n\r");
		scanf("%f", &input2);
		printf("%3.2f\n\r", input2);
		operation = 'e';
		while(operation == 'e') {
			pc.printf("Choose one of the four basic operations(+,-,*,/):\n\r");
			operation = pc.getc();
			pc.printf("%c\n\r",operation);
			switch(operation) {
				case '+':
					result = input1 + input2;
					break;
				case '-':
					result = input1 - input2;
					break;
				case '*':
					result = input1 * input2;
					break;
				case '/':
					result = input1 / input2;
					break;
				default:
					pc.printf("Wrong operation, try again!!\n\r");
					operation = 'e';
					break;
			}
		}
		pc.printf("%3.2f %c %3.2f = %3.2f\n\r", input1, operation, input2, result);
	}
}	
