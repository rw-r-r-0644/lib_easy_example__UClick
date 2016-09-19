#include "lib_easy.h"

//Clicker game; inpired by Maschell
int Menu_Main(void) {
	int highscore = 0;
	ucls(); //Clear screen
	uprintf("Highscore: %d\n",highscore);		//Print highscore
	uprintf("\nPress PLUS to Start\n");		//Print start message and leave one line before (\n=new line)
	uprintf("\n\n\n\n\n\n\n\n\n\n\n\n\n");		//Leave some lines empty
	uprintf("Press the Home Button to exit\n");	//Display home button exit message (at the bottom of the screen)
	while(1) { 
		updatePressedButtons(); //Update buttons state
		if(isPressed(VPAD_BUTTON_PLUS)) { //If PLUS is pressed, start the game
			//---COUTDOWN---
			int64_t coutdown_start_time = uGetTime();				//This is the time when the coutdown was started (used to calculate seconds passed)
			int coutdown_seconds_passed;						//This will contain the seconds that have passed since the coutdown start
			int seconds_remaining_before_start;					//This will contain the seconds that remain before start (3..2..1)
			int seconds_remaining_before_start_old=0;				//These var will be needed to check if the coutdown time changed from a loop to another: as cls and printf 
												//cause a filckering, we update screen only when needed, as less as possible

												//NOTE: Normally you could also fix filckering by using double buffering, but I still didn't find an easy
												//enough for beginnners way to do so
			while(1) {
				coutdown_seconds_passed=uGetTime()-coutdown_start_time;		//The time that has passed since the coutdown start equals the current time minus the coutdown start time
				seconds_remaining_before_start=3-coutdown_seconds_passed;	//The remaining time equals 3 sec (our coutdown) minus the time that has passed since the coutdown start

				if (seconds_remaining_before_start_old!=seconds_remaining_before_start) { //If coutdown value changed update screen
				  ucls();							//Clear screen
				  uprintf("Highscore: %d\n",highscore);				//Print highscore
				  uprintf("\nReady?\n");					//Print ready message and leave one line before (\n=new line)
				  uprintf("%d\n", seconds_remaining_before_start);		//Print time remaining from start
				  uprintf("\n\n\n\n\n\n\n\n\n\n\n\n");				//Leave some lines empty
				  uprintf("Press the Home Button to exit\n");			//Display home button exit message (at the bottom of the screen)
				  seconds_remaining_before_start_old=seconds_remaining_before_start; //As we just updated screen, set last updated coutdown to current value
				}
				if(seconds_remaining_before_start<=0) break;			//If the coutdown has finished (0 seconds remaining), exit coutdown loop and start the game
				updatePressedButtons(); //Update buttons state
				if(isPressed(VPAD_BUTTON_HOME)) break;				//If home is pressed, exit the coutdown
			}
			//---GAME---
			int64_t game_start_time = uGetTime();					//This is the time when the game was started (used to calculate seconds passed)
			int game_seconds_passed;						//This will contain the seconds that have passed since the game started
			int seconds_remaining_before_end;					//This will contain the seconds that remain before the game ends
			int score=0;								//This will contain the score
			int score_old=0;							//These var will be needed to check if the score/time changed from a loop to another: as cls and printf 
			int seconds_remaining_before_end_old=0;					//cause a filckering, we update screen only when needed as less as possible
			int press_valid=1;							//This will make sure you don't just keep pressing; if it's 1 the press is counted, if it's 0 no
			while(1) {
				updatePressedButtons(); //Update buttons state
				if(isPressed(VPAD_BUTTON_A) && press_valid==1) {		//If A is pressed and the press is valid (press_valid==1)..
					score++;						//Increase score by one
					press_valid=0;						//Set the press as invalid until the button is released
				}
				if(press_valid==0&&(isPressed(VPAD_BUTTON_A)==0)) press_valid=1;//If the button was pressed but now it's released count the next press as valid
				if (score>highscore) highscore=score;				//If the current score is greater than the highscore set highscore to current score
				game_seconds_passed=uGetTime()-game_start_time; 		//The time that has passed since the game start equals the current time minus the game start time
				seconds_remaining_before_end=10-game_seconds_passed;		//The remaining time equals 10 sec (game duration) minus the time that has passed since the game start
				if (score_old!=score || seconds_remaining_before_end_old!=seconds_remaining_before_end) { //If some value changed update screen
					ucls();								//Clear screen
					uprintf("Highscore: %d\n",highscore);				//Print highscore
					uprintf("\nTime left: %d sek\n", seconds_remaining_before_end);	//Print remaining time
					uprintf("A Pressed: %d times\n", score);			//Print score (time of press)
					uprintf("\n\n\n\n\n\n\n\n\n\n\n");				//Leave some lines empty
					uprintf("Press the Home Button to exit\n");			//Display home button exit message (at the bottom of the screen)
					seconds_remaining_before_end_old=seconds_remaining_before_end;  //As we just updated screen, set last updated timer value to current timer value
					score_old=score;						//As we just updated screen, set last updated score value to current score value
				}
				if(seconds_remaining_before_end<=0) break;			//If the game has finished (10 seconds passed), exit game loop
				if(isPressed(VPAD_BUTTON_HOME)) break;				//If home is pressed, exit the game
			}
			//--Print menu text as the game just ended--
			ucls(); 								//Clear screen
			uprintf("Highscore: %d\n",highscore);					//Print highscore
			uprintf("\nScore: %d\n", score);					//Print score and leave one line before (\n=new line)
			if (highscore==score) uprintf("New Highscore!\n");
			else uprintf("\n");
			uprintf("\nPress PLUS to Start\n");					//Print start message and leave one line before (\n=new line)
			uprintf("\n\n\n\n\n\n\n\n\n\n");					//Leave some lines empty
			uprintf("Press the Home Button to exit\n");				//Display home button exit message (at the bottom of the screen)
		}
		if(isPressed(VPAD_BUTTON_HOME)) break; //Check if home is pressed
	}
	uprintf("Exiting...");
	return 0;
}

