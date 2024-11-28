#include "mbed.h"
#include "Adafruit_SSD1306.h"

I2C myI2C(I2C_SDA,I2C_SCL);
Adafruit_SSD1306_I2c myGUI(myI2C,D13,0x78,64,128); //I2C class, RST pinname, Display address, height, width

InterruptIn x_button(PB_7);
InterruptIn y_button(PC_4);

int x = 0;
int y = 0;

void X_BUTTON_ISR(){
	if(x == 128){
		x = 0;
	}
	else{
		x++;
	}
	wait(0.2);
}

void Y_BUTTON_ISR(){
	if(y == 64){
		y = 0;
	}
	else{
		y++;
	}
	wait(0.2);
}

int main(){
	int r = 2;
	myGUI.begin();	
	x_button.fall(&X_BUTTON_ISR);
	y_button.fall(&Y_BUTTON_ISR);
	while(1){
		myGUI.clearDisplay(); // erase current display
		myGUI.drawCircle(x, y, r, WHITE); // draw circle according to x, y coordinate
		myGUI.setTextCursor(0, 0);
		myGUI.setTextSize(1);
		myGUI.setTextColor(WHITE);
		myGUI.printf("x: %d, y: %d", x, y); // display string (current x, y coordinate)
		myGUI.display(); // display updated buffer (send data to OLED via I2C interface)
		wait(0.01);
	}
}
