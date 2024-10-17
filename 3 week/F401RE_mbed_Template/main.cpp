#include "mbed.h"
#include "motordriver.h"

Motor A(D11, PC_8); // pwm, dir
Motor B(D12, PD_2); // pwm, dir

int main() {
    while (1) {
        // For speed test.
        for (float s= 0; s < 1.0f ; s += 0.1f) {
			A.forward(s); 
            wait(1);
		}

		A.stop();
        wait(3);

        for (float s= 0; s < 1.0f ; s += 0.1f) {
			A.backward(s);
            wait(1);
       }
    }
}
