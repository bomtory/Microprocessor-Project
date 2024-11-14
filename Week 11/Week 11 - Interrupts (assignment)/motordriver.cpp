#include "motordriver.h"
#include "mbed.h"

Motor::Motor(PinName pwm, PinName dir): _pwm(pwm), _dir(dir) {
	_pwm.period(0.001);
	_pwm = 0;
	_dir = 0;
	sign = 0;
}

void Motor::forward(float speed) {
	float temp = 0;
	
	if (sign == -1) {
		_pwm = 0;
		wait(0.2);
	}
	
	_dir = 1;
	temp = abs(speed);
	_pwm = temp;
	sign = 1;
}

void Motor::backward (float speed) {
	float temp = 0;
	
	if (sign == 1) {
		_pwm = 0;
		wait(0.2);
	}
	
	_dir = 0;
	temp = abs(speed);
	_pwm = temp;
	sign = -1;
}

void Motor::stop(void) {
	_pwm = 0;
}
