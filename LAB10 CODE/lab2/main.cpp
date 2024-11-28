#include "mbed.h"
#include "Adafruit_SSD1306.h"
 
DigitalOut myled(LED1);
I2C myI2C(I2C_SDA,I2C_SCL);
Adafruit_SSD1306_I2c myGUI(myI2C,D13,0x78,64,128);

int main() {
    unsigned int x=0;
    myGUI.printf("%ux%u Hello world!\r\n", myGUI.width(), myGUI.height());
    while(1) {
        myled = 1;
        wait(0.2);
        myled = 0;
        wait(0.2);
        myGUI.printf("%u\r",x);
        myGUI.display();
        x++;
        wait(1.0);
    }
}
