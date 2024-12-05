#include "mbed.h"
#include "Adafruit_SSD1306.h"
#define PROGMEM



// an I2C sub-class that provides a constructed default
class I2CPreInit : public I2C
{
public:
    I2CPreInit(PinName sda, PinName scl) : I2C(sda, scl)
    {
        frequency(400000);
        start();
    };
};


class Player{
	public:
		Player(uint8_t startLine = 0) : line(startLine) {}
	
		void move_up(){
			if (line > 0) line -= 1;
		}
		void move_down(){
			if (line < 2) line += 1;
		}
		uint8_t check_line() {
			return line;
		}
	
	protected:
		uint8_t line;
};
class Board{
	public:
		Board(Adafruit_SSD1306_I2c *display, uint8_t initialScore = 0, uint8_t initialLife = 3, int16_t orig_x = 93, int16_t orig_y = 10) 
			: score(initialScore), life(initialLife), time(0), orig_x(orig_x), orig_y(orig_y){
				for(int i = 0; i<2; i++){
					score_clr[i] = '0';
				}
				for(int i = 0; i<3; i++){
					time_clr[i] = '0';
				}
				for(int i = 0; i<1; i++){
					life_clr[i] = '0';
				}
			}
		void score_up(){
			// increment score by 1
			score++;
		}
		void life_down(){
			// decrement life by 1
			life--;
		}
		void time_up(){
			// increment time by 1
			time++;
		}
		uint8_t check_score() {
			// return score on method call
			return score;
		}
		uint8_t check_life() {
			// return life on method call
			return life;
		}
		uint8_t check_time() {
			// return time on method call
			return time;
		}
		char* Int2CharArray(int number, char charArray[]){
			int index = 0;
			do {
					charArray[index] = '0' + (int)(number % 10);
					index++;
					number = (int)(number/10);                            
			} while (number > 0);
			
			for (int i = 0; i < index / 2; ++i) {
					char temp = charArray[i];
					charArray[i] = charArray[index - i - 1];
					charArray[index - i - 1] = temp;
			}
			return charArray;
		}

		void drawStr (int16_t x, int16_t y, Adafruit_SSD1306_I2c *display, int Array_len, char* charArray, char* charClear){
			// ?????? string? ???? ?? ??
			for (int i = 0; i < Array_len; i++) {
				display -> setTextCursor(x, y);
				for (int i = 0; i < Array_len; i++) {
					display -> printf("%c", charArray[i]); 	// Print each character
				}
			}
		}

		void drawScore(Adafruit_SSD1306_I2c *display){
			char score_cab[] = {0,0};
			char *score_ca = Int2CharArray(score, score_cab);
			char time_cab[] = {0,0,0};
			char *time_ca = Int2CharArray(time,time_cab);
			char life_cab[] = {0};
			char *life_ca = Int2CharArray(life,life_cab);
			drawStr(orig_x, score_y, display, 2, score_ca, score_clr);
			drawStr(orig_x, time_y, display, 3, time_ca, time_clr);
			drawStr(orig_x, life_y, display, 1, life_ca, life_clr);
		}
		void initScore(Adafruit_SSD1306_I2c *display){
				int16_t y = orig_y;
				int str_len = 6;
				char score_str []= {'S', 'c', 'o', 'r', 'e', ':'};
				char score_cab[] = {0,0};
				char *score_ca = Int2CharArray(score, score_cab);
				char time_str [] = {'T','i', 'm', 'e', ':', ' '};
				char time_cab[] = {0,0,0};
				char *time_ca = Int2CharArray(time,time_cab);
				char life_str [] = {'L', 'i', 'f', 'e', ':', ' '};
				char life_cab[] = {0};
				char *life_ca = Int2CharArray(life,life_cab);
				drawStr(orig_x, y, display, str_len, score_str, score_str);
				y = y + 8;
				score_y = y;
				drawStr(orig_x, score_y, display, 2, score_ca, score_clr);
				y = y + 8;
				drawStr(orig_x, y, display, str_len, time_str, time_str);
				y = y + 8;
				time_y = y;
				drawStr(orig_x, time_y, display, 3, time_ca, time_clr);
				y = y + 8;
				drawStr(orig_x, y, display, str_len, life_str, life_str);
				y = y + 8;
				life_y = y;
				drawStr(orig_x, life_y, display, 1, life_ca, life_clr);
		}

	protected:
		uint8_t score;
		uint8_t life;
		uint8_t time;
		int16_t orig_x;
		int16_t orig_y;
		int16_t score_y;
		int16_t time_y;
		int16_t life_y;
		char score_clr[2];
		char time_clr[3];
		char life_clr[1];
};
class Enemy{
	public:
		Enemy(uint8_t startLine, uint8_t startX = 13,  bool startactive = false, bool danger = false) : x(startX), line(startLine), active(startactive) {}
		void move(Board& board){
			if(active){
				if(x==0) {
					board.score_up();
					deactivate();
					x = 13;
					danger = false;
				}
				else {
					if(x==3){
						danger = true;
					}
					x-=1;
				}
			}
		}
		bool check_active(){
			// return active state
			return active;
		}
		void activate() {
			// set active state to true
			active = true;
		}
		void deactivate() {
			// set active state to false
			active = false;
		}
		uint8_t check_x() {
			// return x
			return x;
		}
		void danger_false(){
			// set danger to false
			danger = false;
		}
		bool check_danger() {
			// return danger
			return danger;
		}
		
	protected:
	
		uint8_t x;
		uint8_t line;
		bool active;
		bool danger;
};


// ------------ don't touch ------------ //
const uint8_t PROGMEM motorcycle_bitmap_left[] = {
	0x30, 0x78, 0xcc, 0x84, 0x84, 0xcc, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t PROGMEM motorcycle_bitmap_right[] = {
	0xcc, 0x84, 0x84, 0xcc, 0x49, 0xfe, 0x78, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00
};

const uint8_t PROGMEM car_bitmap[] = {
	0xcc, 0xfe, 0x02, 0xfe, 0x02, 0x02, 0x02, 0x02, 0x02, 0xfe, 0x02, 0x02, 0x02, 0xfe, 0x00, 0x01, 
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01
};
// ------------ don't touch ------------ //

bool y_lock = false;
int x, y;
uint8_t num;
uint8_t state;
uint8_t generate_count = 20;
int delay;
I2C myI2C(I2C_SDA,I2C_SCL);
Adafruit_SSD1306_I2c myGUI(myI2C,D13,0x78,64,128);
AnalogIn x_axis(PC_2);
AnalogIn y_axis(PC_3);
InterruptIn button(PA_15);
Player player = Player(1);
Enemy enemy0 = Enemy(0);
Enemy enemy1 = Enemy(1);
Enemy enemy2 = Enemy(2);
Board board = Board(&myGUI); 
Ticker joystick;
Ticker counter;
Timer t;


unsigned int score = 80;
unsigned int timev = 10;
unsigned int life = 3;

void joystick_Handler(){
	// ????? ???? ??
    x = x_axis.read_u16(); // read x axis
    y = y_axis.read_u16(); // read y axis
	// handle exception(lock)
	if (y_lock) return;
	if (y > 0x8000) {
		state = 1;
	} else if (y < 0x2000) {
		state = 2;
	} else {
		state = 0;	// Neutral position
	}
}
void time_Handler(){
	board.time_up();	// Increment time
}
void create_enemy(){
	// Activate the enemy in random lane
	// Enemy moves only when it's active
	num = rand() % 3; // num: A variable that randomly takes a value between 0 and 2, 
	                          //     storing information about the lane where the enemy to activate is located.
	switch (num) {
		case 0:
			// Call the activate function in enemy0 class
			enemy0.activate();
			break;
		case 1:
			// Call the activate function in enemy1 class
			enemy1.activate();	
			break;
		case 2:
			// Call the activate function in enemy2 class
			enemy2.activate();
			break;
	}
}

// New method
void play_buzzer() {
	// Buzzer sound for collisions
	DigitalOut buzzer(PA_14);
	buzzer = 1;
	wait_us(500000);	// beep 0.5s
	buzzer = 0;
}

// Pause menu phase has to be declared by interrupt with button!

// New method
void start_game() {
	state = 0;	// Set game state to start
}


int main() {
	//Initialize Game
	myGUI.clearDisplay();
	board.initScore(&myGUI);
	//Start menu phase - ???
	myGUI.printf("Press button to start");
	myGUI.display();
	button.rise(&start_game);	//Start game on button press

	//Game phase
	// Player movement has to be declared by interrupt with joystick!
	joystick.attach(joystick_Handler, 0.005);
	counter.attach(time_Handler, 1);
	t.start();
	while(1) { // Game loop
		if(generate_count== 20){
			create_enemy();
			generate_count = 0;
		}
		
		y_lock = true;
		myGUI.drawBitmap(0, 5 + player.check_line()*20, car_bitmap, 14, 14, BLACK); // Player
		if (state==1){
			player.move_down();
		}else if(state==2){
			player.move_up();
		}
		myGUI.drawBitmap(0, 5 + player.check_line()*20, car_bitmap, 14, 14, WHITE); // Player
		y_lock = false;
		
		if (enemy0.check_x() > 0) myGUI.drawBitmap(enemy0.check_x()*7 - 7, 7, motorcycle_bitmap_left, 7, 10, BLACK);
		if (enemy0.check_x() < 12) myGUI.drawBitmap(enemy0.check_x()*7, 7, motorcycle_bitmap_right, 7, 10, BLACK);
		if (enemy1.check_x() > 0) myGUI.drawBitmap(enemy1.check_x()*7 - 7, 27, motorcycle_bitmap_left, 7, 10, BLACK);
		if (enemy1.check_x() < 12) myGUI.drawBitmap(enemy1.check_x()*7, 27, motorcycle_bitmap_right, 7, 10, BLACK);
		if (enemy2.check_x() > 0) myGUI.drawBitmap(enemy2.check_x()*7 - 7, 47, motorcycle_bitmap_left, 7, 10, BLACK);
		if (enemy2.check_x() < 12) myGUI.drawBitmap(enemy2.check_x()*7, 47, motorcycle_bitmap_right, 7, 10, BLACK);
		enemy0.move(board);
		enemy1.move(board);
		enemy2.move(board);
		// Display current phase
		board.drawScore(&myGUI);
		// Display player, enemies
		myGUI.display();
		
		
		switch(player.check_line()){
			case 0:
				// ???? ?? ???? ? ???? check?? life? ??? ??
				board.life_down();
				enemy0.danger_false();
				play_buzzer();
				break;
			case 1:
				// ???? ?? ???? ? ???? check?? life? ??? ??
				board.life_down();
				enemy1.danger_false();
				play_buzzer();
				break;
			case 2:
				// ???? ?? ???? ? ???? check?? life? ??? ??
				board.life_down();
				enemy2.danger_false();
				play_buzzer();
				break;
		}
		// life? check?? 0??? ???? game over
		if (board.check_life() == 0) {
			break;
		}
		// Display Scoreboard
		generate_count ++;
		delay = 16 - t.read_ms();
		if (delay > 0) wait_ms(delay);
		board.drawScore(&myGUI);
		myGUI.display();
	}

	//Game Over phase
	myGUI.clearDisplay();
	myGUI.printf("\nGame Over!\n");
	myGUI.printf("Your Score: %u\n", board.check_score());
	myGUI.printf("You survived for\n%u seconds!", board.check_time());
	myGUI.display();
}

