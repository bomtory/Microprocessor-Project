#include "mbed.h"
#include "Adafruit_SSD1306.h"
#define PROGMEM

PwmOut buzzer(PC_9); // Set the output voltage of the buzzer
#define Mi 1000000/659 //Define the frquency of Mi

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

// Manage the player's position
class Player{
	public:
		Player(uint8_t startLine = 0) : line(startLine) {}
	
		void move_up(){
			if (line != 0) line--; //If not on the top line 0, move upward
		}
		void move_down(){
			if (line != 2) line++; //If not on the bottom line 2, move downward
		}
		uint8_t check_line() { // Access variable "line" from outside of class
			return line; //Return the current state
		}
	
	protected:
		uint8_t line;
};
// Manage score and time(display on OLED)
class Board{
	public:
		Board(Adafruit_SSD1306_I2c *display, uint8_t initialScore = 0, uint8_t initialLife = 3, int16_t orig_x = 93, int16_t orig_y = 5) 
			: score(initialScore), time(0), orig_x(orig_x), orig_y(orig_y){
				for(int i = 0; i<2; i++){
					score_clr[i] = '0';
				}
				for(int i = 0; i<3; i++){
					time_clr[i] = '0';
				}
			}
		void score_up(){
			score++; // Increase the score by 1
		}
		void time_up(){
			time++; // Increase the time by 1 
		}
		uint8_t check_score() { // Access variable "score" from outside of class
			return score; // Return the current score
		}
		uint8_t check_time() { // Access variable "time" from outside of class
			return time; // Return the current time
		}
		char* Int2CharArray(int number, char charArray[]){
			int index = 0;
			do {
					charArray[index] = '0' + (int)(number % 10); //Add '0' to convert the unit digit of an integer to ASCLL code
					index++;
					number = (int)(number/10); //Manage the hundreds and tens digits                           
			} while (number > 0);
			
			for (int i = 0; i < index / 2; ++i) {
					char temp = charArray[i];
					charArray[i] = charArray[index - i - 1];
					charArray[index - i - 1] = temp;
			}
			return charArray;
		}

		void drawStr (int16_t x, int16_t y, Adafruit_SSD1306_I2c *display, int Array_len, char* charArray, char* charClear){
			int16_t clr_x = x;
			for(int i = 0; i<Array_len; i++){
				display->drawChar(clr_x, y, charClear[i], BLACK, BLACK, 1);
				clr_x = clr_x+6;
			}
			for(int i = 0; i<Array_len; i++){
				display->drawChar(x, y, charArray[i], WHITE, WHITE, 1);
				charClear[i] = charArray[i];
				x = x+6;
			}
			
		} 

		void drawScore(Adafruit_SSD1306_I2c *display){
			char score_cab[] = {0,0};
			char *score_ca = Int2CharArray(score, score_cab);
			char time_cab[] = {0,0,0};
			char *time_ca = Int2CharArray(time,time_cab);
			// Display the values throughout execution
			drawStr(orig_x, score_y, display, 2, score_ca, score_clr); // How can I display score? You can find it within Board class // Use the drawStr function to display the score throughout execution, length is defined below
			drawStr(orig_x, time_y, display, 3, time_ca, time_clr); // How can I display time? You can find it within Board class // Use the drawStr function to display the time throughout execution, length is defined below
		}
		void initScore(Adafruit_SSD1306_I2c *display){
				int16_t y = orig_y; //Initialize the values
				int str_len = 6;
				char score_str []= {'S', 'c', 'o', 'r', 'e', ':'}; //Print "Score" as the name
				char score_cab[] = {0,0}; //Initialize it
				char *score_ca = Int2CharArray(score, score_cab); 
				char time_str [] = {'T','i', 'm', 'e', ':', ' '}; //Print "Time" as the name
				char time_cab[] = {0,0,0}; //Initialize it
				char *time_ca = Int2CharArray(time,time_cab);
				
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
		}

	protected:
		uint8_t score;
		uint8_t time;
		int16_t orig_x;
		int16_t orig_y;
		int16_t score_y;
		int16_t time_y;
		char score_clr[2];
		char time_clr[3];
};
// Manage the position of enemies, determine movement and danger status
class Enemy{
	public:
		Enemy(uint8_t startLine, uint8_t startX = 13,  bool startactive = false, bool danger = false) : x(startX), line(startLine), active(startactive) {}
		void move(Board& board){
			if(active){
				if(x==0) { // x : enemy position
					board.score_up(); // You need to increase the score on the board by one point // Call the function to increase the score in the class Board using the variable score
					deactivate(); // Deactivate enemy
					x = 13; // 13 means outside position of the game display
					danger = false; // Indicates that the object is "safe" //Indicate that false for danger means it is not dangerous
				}
				else {
					if(x==3){
							danger = true; //Indicates that the object is "Dangerous" //Indicate that ture for danger means there is a threat	
					}
					x--; // Move the enemy "forward"
				}
			}
		}
		bool check_active(){
			return active;
		}
		void activate() {
			active = true;
		}
		void deactivate() {
			active = false;
		}
		uint8_t check_x() {
			return x;
		}
		void danger_false(){
			danger = false;
		}
		bool check_danger() {
			return danger;
		}
	protected:
	
		uint8_t x;
		uint8_t line;
		bool active;
		bool danger;
};



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

int x, y;
uint8_t state;
uint8_t generate_count = 5; // Enemy cycle, shorter is more difficult
int delay;
I2C myI2C(I2C_SDA, I2C_SCL); // Instance to communicate with OLED
Adafruit_SSD1306_I2c myGUI(myI2C, D13, 0x78, 64, 128); // Instance to display OLED
AnalogIn y_axis(PC_3); // Instance to get input of joystick //Left button
DigitalIn button(PB_7); // Instance to get input of button //Middle button
InterruptIn button2(PC_4); // Define button 2 as the button with the pause functionality
Player player = Player(1);
Enemy enemy0 = Enemy(0);
Enemy enemy1 = Enemy(1);
Enemy enemy2 = Enemy(2);
Board board = Board(&myGUI); 
Ticker joystick;
Ticker counter;
Timer t;

void initializeDisplay() { // Don't change it
	myGUI.begin(SSD1306_SWITCHCAPVCC);
	myGUI.clearDisplay();
	myGUI.display();
}

void joystick_Handler(){
	y = y_axis.read() * 1000;

	if (y<600) state = 2; // Which "state" is down? up? neutral? player down //If it is in state2, move upward
	else if (y>787) state = 1; //If it is in state1, move downward
	else {}; //Indicate no action is performed
}
void time_Handler(){
	board.time_up(); // You need to increase "time" in "board" //Call the function to increase time from the board
}
void create_enemy(){
	// Activate the enemy in random lane
	// Enemy moves only when it's active
	uint8_t num = rand() % 3; // num: A variable that randomly takes a value between 0 and 2, 
	                          //     storing information about the lane where the enemy to activate is located.
	switch (num) {
		case 0:
			enemy0.activate(); // Call the activate function in enemy0 class
			break;
		case 1:
			enemy1.activate(); // Call the activate function in enemy1 class
			break;
		case 2:
			enemy2.activate(); // Call the activate function in enemy2 class
			break;
	}
}
// Pause menu phase has to be declared by interrupt with button!

int pause = 0; // initial pause value is 0 (pause_func off)
void pause_func() { 
	pause = !pause; // reverse the pause value
}

int life = 3; //Current life
int max_life = 3; //Maximum life

void drawlife(Adafruit_SSD1306_I2c* diplay, int life, int maxlife) { //Display life on OLED
	myGUI.setTextCursor(93, 38); //Set the position where the "life" appears
	myGUI.printf("Life:"); //Display the name "Life"
	myGUI.setTextCursor(93, 46); //Set the position where the life appears
	myGUI.printf("%d/%d", life, maxlife); //Display current life and maximum life
}

int main() {
	//Initialize OLED
	initializeDisplay();
	//Start menu phase
	myGUI.setTextCursor(11,22); //Define the position to display the game start screen
	myGUI.printf("If you press the button, game starts~!"); // Try to make Game start screen //Print the game start guide message
	myGUI.display();
	while(button.read()==1){
	} // Wait until button pressed

	// Initialize Game
	// Player movement has to be declared by interrupt with joystick!
	joystick.attach(&joystick_Handler, 0.15); // Setting ticker for joystick input // Set the joystick handler detection time(reduce if the movement is unresponsive)
	counter.attach(&time_Handler, 1.0); // Setting ticker for timer increase 1 //Increase time by 1 second using the time handler
	
	myGUI.clearDisplay(); //Drawing outline + initialize score board
	for(int16_t l=0; l<128; l++){
		myGUI.drawPixel(l,0,WHITE);
		myGUI.drawPixel(l,63,WHITE);
		if(((10<l) && (l<=25)) || ((35<l) && (l<=50)) || ((60<l) && (l<=75))) {
			myGUI.drawPixel(l, 21, WHITE);
			myGUI.drawPixel(l, 42, WHITE);
		}
	}
	for(int16_t m=0; m<64; m++){
		myGUI.drawPixel(85, m, WHITE);
		myGUI.drawPixel(127, m, WHITE);
	}
	board.initScore(&myGUI);
	drawlife(&myGUI, life, max_life); //Call the function to display life
	myGUI.display();
	bool running = true;
	t.start();

	// Game phase
	while(running) {

		button2.fall(&pause_func); // if the pause button pushed, pause_func on
		while (pause == 1) { // pause phase
			counter.detach(); // time stop
			myGUI.clearDisplay();
			myGUI.setTextCursor(0,30); //Set the position where the 'pause message' appears
			myGUI.printf("PAUSE-press the button to restart"); // PAUSE message on
			myGUI.display();
			button2.fall(&pause_func); // if the pause button pushed, pause_func off
			if (pause == 0) { // pause off phase 
				myGUI.clearDisplay(); // PAUSE message off
				for (int16_t l = 0; l < 128; l++) {
					myGUI.drawPixel(l, 0, WHITE);
					myGUI.drawPixel(l, 63, WHITE);
					if (((10 < l) && (l <= 25)) || ((35 < l) && (l <= 50)) || ((60 < l) && (l <= 75))) {
						myGUI.drawPixel(l, 21, WHITE);
						myGUI.drawPixel(l, 42, WHITE);
					}
				}
				for (int16_t m = 0; m < 64; m++) {
					myGUI.drawPixel(85, m, WHITE);
					myGUI.drawPixel(127, m, WHITE);
				} //Redraw the background lines
				board.initScore(&myGUI); //Redisplay the names 'Score' and 'Time'
				drawlife(&myGUI, life, max_life); //Redisplay the number of life
				myGUI.display();
				counter.attach(&time_Handler, 1.0); // time restart
				break;
			}
		}

		t.reset();
		
		if(generate_count== 5){ // What is "generate_count" for? // Indicate the 'enemy spawn time"
			create_enemy(); // Spawn enemies when the count reaches 5
			generate_count = 0;
		}
		

		myGUI.drawBitmap(0, 5 + player.check_line()*20, car_bitmap, 14, 14, BLACK ); // Player: Clear the previous location 
		if (state==1){ //Move the joystick downward
			player.move_down(); //Call the function for moving downward inside the Player class through the player objec
		}else if(state==2){ //Move the joystick upward
			player.move_up(); //Call the function for moving upward inside the Player class through the player object
		}
		myGUI.drawBitmap(0, 5 + player.check_line()*20, car_bitmap, 14, 14, WHITE ); // Player: Draw in the current location

		// Enemy: Clear the previous location
		if (enemy0.check_x() > 0) myGUI.drawBitmap(enemy0.check_x()*7 - 7, 7, motorcycle_bitmap_left, 7, 10, BLACK ); //BLACK means 'Clear'
		if (enemy0.check_x() < 12) myGUI.drawBitmap(enemy0.check_x()*7, 7, motorcycle_bitmap_right, 7, 10, BLACK );
		if (enemy1.check_x() > 0) myGUI.drawBitmap(enemy1.check_x()*7 - 7, 27, motorcycle_bitmap_left, 7, 10, BLACK );
		if (enemy1.check_x() < 12) myGUI.drawBitmap(enemy1.check_x()*7, 27, motorcycle_bitmap_right, 7, 10, BLACK );
		if (enemy2.check_x() > 0) myGUI.drawBitmap(enemy2.check_x()*7 - 7, 47, motorcycle_bitmap_left, 7, 10, BLACK );
		if (enemy2.check_x() < 12) myGUI.drawBitmap(enemy2.check_x()*7, 47, motorcycle_bitmap_right, 7, 10, BLACK );
		// Enemy: Movement
		enemy0.move(board);
		enemy1.move(board);
		enemy2.move(board);
		// Enemy: Draw in the current location
		if (enemy0.check_x() > 0 && enemy0.check_x() < 13) myGUI.drawBitmap(enemy0.check_x()*7 - 7, 7, motorcycle_bitmap_left, 7, 10, WHITE ); //WHITE means 'Draw' 
		if (enemy0.check_x() < 12) myGUI.drawBitmap(enemy0.check_x()*7, 7, motorcycle_bitmap_right, 7, 10, WHITE );
		if (enemy1.check_x() > 0 && enemy1.check_x() < 13) myGUI.drawBitmap(enemy1.check_x()*7 - 7, 27, motorcycle_bitmap_left, 7, 10, WHITE );
		if (enemy1.check_x() < 12) myGUI.drawBitmap(enemy1.check_x()*7, 27, motorcycle_bitmap_right, 7, 10, WHITE );
		if (enemy2.check_x() > 0 && enemy2.check_x() < 13) myGUI.drawBitmap(enemy2.check_x()*7 - 7, 47, motorcycle_bitmap_left, 7, 10, WHITE );
		if (enemy2.check_x() < 12) myGUI.drawBitmap(enemy2.check_x()*7, 47, motorcycle_bitmap_right, 7, 10, WHITE );
		
		switch(player.check_line()){
			case 0:
				if(enemy0.check_danger()) {
					life --; //Decrease life
					drawlife(&myGUI, life, max_life); //Display the updated life
					buzzer.period_us(Mi); //Have the frequency of 'Mi' (the frequency of Mi is defined above)
					buzzer = 0.5; //Emit sound for 0.5 of the total period
					wait(0.2); //Wait for 0.2 seconds
					buzzer = 0; //Turn off the sound
					wait(0.2); //Wait for 0.2 seconds
					if(life==0){ //When there are no lives remaining
						running = false; // How can I finish the game phase? //Stop the execution (executed when running is true)
					} 
					enemy0.danger_false(); //Reinitialize the danger status
				}
				break;
			case 1:
				if(enemy1.check_danger()) {
					life --; //Decrease life
					drawlife(&myGUI, life, max_life); //Display the updated life
					buzzer.period_us(Mi); //Have the frequency of 'Mi' (the frequency of Mi is defined above)
					buzzer = 0.5; //Emit sound for 0.5 of the total period
					wait(0.2); //Wait for 0.2 seconds
					buzzer = 0; //Turn off the sound
					wait(0.2); //Wait for 0.2 seconds
					if(life==0){ //When there are no lives remaining
						running = false; // How can I finish the game phase? //Stop the execution (executed when running is true)
					}
					enemy1.danger_false(); //Reinitialize the danger status
				}
				break;
			case 2:
				if(enemy2.check_danger()) {
					life --; //Decrease life
					drawlife(&myGUI, life, max_life);//Display the updated life
					buzzer.period_us(Mi); //Have the frequency of 'Mi' (the frequency of Mi is defined above)
					buzzer = 0.5; //Emit sound for 0.5 of the total period
					wait(0.2); //Wait for 0.2 seconds
					buzzer = 0; //Turn off the sound
					wait(0.2); //Wait for 0.2 seconds
					if(life==0){ //When there are no lives remaining
						running = false; // How can I finish the game phase? //Stop the execution (executed when running is true)
					}
					enemy2.danger_false(); //Reinitialize the danger status
				}
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
	myGUI.setTextCursor(0,5); //Set the position where the 'game over message' appears when the game ends
	myGUI.printf("Game over.."); // You need to display game over screen //Print "Game over..." on the OLED
	myGUI.setTextCursor(0,26); // Game over screen should include {score} //Set the position where the score appears when the game ends
	myGUI.printf("Score : %d", board.check_score()); //Return and display the final score
	myGUI.setTextCursor(0,47); // Game over screen should include {time} //Set the position where the time appears when the game ends
	myGUI.printf("Time : %d", board.check_time()); //Return and display the final score
	myGUI.display();
}
