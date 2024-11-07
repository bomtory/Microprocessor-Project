#include "mbed.h"
#include "BMP180.h"

#define BMP180_ADDRESS 0x77<<1
#define Do5 1000000/523
#define Re5 1000000/587
#define Mi5 1000000/659

// Implement #1
#define threshold 0.5
#define threshold_alti 2.0
#define threshold_temperature 1.0f

SPI acc(PB_15, PB_14, PB_13);
DigitalOut cs(PB_12);
Serial pc(USBTX, USBRX);         // set up USB interface to host terminal
char buffer[6];                  //raw data array type char
int16_t data[3];                 // 16-bit twos-complement integer data
float x, y, z;                   
PwmOut Buzzer(PC_9);
uint32_t delt_t, count, tempcount;
float temperature, pressure, temppress, altitude;
float avg_xyz[3];
float avg_temp_alti[2];
float delta_x, delta_y, delta_z, delta_temp, delta_alti;
Timer t;
DigitalOut myled(LED1);
BMP180 bmp180;
int situation = 0;
	
int main() {
	pc.baud(9600);
	t.start();
  cs=1;                        //initially ADXL345 is not activated
  acc.format(8,3);             // 8 bit data, Mode 3
  acc.frequency(2000000);      // 2MHz clock rate
  cs=0;                        //select the device
  acc.write(0x31);             // data format register
	acc.write(0x01);
  cs=1;                        //end of transmission
  cs=0;                        //start a new transmission
  acc.write(0x2D);             // power ctrl register
  acc.write(0x08);             // measure mode
  cs=1;                        //end of transmission
	for (int i=0;i<3;i++) avg_xyz[i]=0;
	for (int i=0;i<2;i++) avg_temp_alti[i]=0;
	
	uint8_t c = bmp180.readByte(BMP180_ADDRESS, BMP180_WHO_AM_I);
	if(c == 0x55) {
		pc.printf("BMP-180 is 0x%x\n\r", c);
		pc.printf("BMP-180 should be 0x55\n\r");
		pc.printf("BMP-180 online...\n\r");
		
		bmp180.BMP180Calibration();
		pc.printf("BMP-180 caliration complete...\n\r");
	}
	else {
		pc.printf("BMP-180 is 0x%x\n\r", c);
		pc.printf("BMP-180 should be 0x55\n\r");
		while(1); // idle here forever
	}
	for (int i=0;i<10;i++){
		wait(0.2);
		pc.printf("Initializing... %i/10\n\r",i+1);
		cs=0;                      //start a transmission
		acc.write(0x80|0x40|0x32);   // RW bit high, MB bit high, plus address
		for (int i = 0;i<=5;i++) {
			buffer[i]=acc.write(0x00);       // read back 6 data bytes
		}
		cs=1;                      //end of transmission
		data[0] = buffer[1]<<8 | buffer[0];  // combine MSB and LSB
		data[1] = buffer[3]<<8 | buffer[2];
		data[2] = buffer[5]<<8 | buffer[4];
		x=0.0078*data[0]; y=0.0078*data[1]; z=0.0078*data[2];
		avg_xyz[0] += x;
		avg_xyz[1] += y;
		avg_xyz[2] += z;
		count = t.read_ms();
		tempcount = 0;
		temppress = 0;
		delt_t = 0;
		while(delt_t < 500){
			temperature = (float)bmp180.BMP180GetTemperature()/10.0f;  
			temppress += (float)bmp180.BMP180GetPressure();   
			tempcount++;
			delt_t = t.read_ms() - count;
		}
		pressure = temppress/tempcount;
		altitude = 44330.0f*( 1.0f - pow((pressure/101325.0f), (1.0f/5.255f))); // Calculate altitude in meters
		avg_temp_alti[0] += temperature;
		avg_temp_alti[1] += altitude;
	}
    // Implement #2, get average x,y,z, temparature, altitude
	avg_xyz[0] /= 10;
	avg_xyz[1] /= 10;
	avg_xyz[2] /= 10;
	avg_temp_alti[0] /= 10;
	avg_temp_alti[1] /= 10;

	pc.printf("initialize complete\n\r");
	pc.printf("average x: %1.2fg, y: %1.2fg, z: %1.2fg, temperature: %.1f C, altitude: %.1f m\n\r", avg_xyz[0],avg_xyz[1],avg_xyz[2],avg_temp_alti[0],avg_temp_alti[1]);
	
  while (1) {
		while (situation == 0){
			wait(0.2);
			cs=0;                      //start a transmission
			acc.write(0x80|0x40|0x32);   // RW bit high, MB bit high, plus address
			for (int i = 0;i<=5;i++) {
				buffer[i]=acc.write(0x00);       // read back 6 data bytes
			}
			cs=1;                      //end of transmission
			data[0] = buffer[1]<<8 | buffer[0];  // combine MSB and LSB
			data[1] = buffer[3]<<8 | buffer[2];
			data[2] = buffer[5]<<8 | buffer[4];
			x=0.0078*data[0]; y=0.0078*data[1]; z=0.0078*data[2]; 
			count = t.read_ms();
			tempcount = 0;
			temppress = 0;
			delt_t = 0;
			while(delt_t < 500){
					temperature = (float)bmp180.BMP180GetTemperature()/10.0f;  
					temppress += (float)bmp180.BMP180GetPressure();            
					tempcount++;
					delt_t = t.read_ms() - count;
			}
			pressure = temppress/tempcount;
			altitude = 44330.0f*( 1.0f - pow((pressure/101325.0f), (1.0f/5.255f))); // Calculate altitude in meters
			

            // Implement #3, Get delta x,y,z,temperature, altitude
			delta_x = fabs(x - avg_xyz[0]);
			delta_y = fabs(y - avg_xyz[1]);
			delta_z = fabs(z - avg_xyz[2]);
			delta_temp = fabs(temperature - avg_temp_alti[0]);
			delta_alti = fabs(altitude - avg_temp_alti[1]);
			pc.printf("delta x: %1.2fg, y: %1.2fg, z: %1.2fg, temperature: %.1f C, altitude: %.1f m temperature: %.1f C\n\r", delta_x,delta_y,delta_z,delta_temp,delta_alti,temperature);

            // Implement #4, Get Situation -> Theft Situation
			if (delta_x>threshold | delta_y>threshold | delta_z>threshold | delta_alti > threshold_alti){
				situation = 1;
				
			}

            // Implment #5, Get Situation -> Temperature Situaion
			else if (delta_temp > threshold_temperature){
				situation = 2;
			}
		}

		if (situation == 1){ //theft situation
		  for (int i = 0; i<4 ; i++){
				myled = 1;
				Buzzer = 0.7;
				Buzzer.period_us(Do5);
				wait_ms(500);
				myled = 0;
				Buzzer = 0;
				wait_ms(500);
			}
			situation = 0;
		}
		if (situation == 2){ //temperature situation
			for (int i = 0; i<4 ; i++){
				Buzzer = 0.7;
				Buzzer.period_us(Mi5);
				wait_ms(500);
				Buzzer = 0;
				wait_ms(500);
			}
			situation = 0;
		}
  }
}
