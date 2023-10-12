/*
 * GccApplication3.c
 *
 * Created: 2/20/2023 5:23:23 PM
 * Author : Kai
 
 This is the program for the micro controller in controlling a 16 keypad input to play/pause
 a speaker with a LCD display. The speaker will play a song, specifically twinkle twinkle little star. 
 As the song plays, it can be paused and resumed using 'D' to pause, and '#' to resume playing.
 The LCD will display what song is playing at the current moment, with Now Playing: Twinkle Twinkle.
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "avr.h"
#include "lcd.h"



int setUp = 0;

typedef enum {
	A = 11,
    As = 10, 
	B = 10, 
	C = 19, 
	Cs = 18, 
	D = 17, 
	Ds = 16, 
	E = 15, 
	F = 14, 
	Fs = 14, 
	G  = 13,
	Gs = 12,
	X = 0

} Note;

typedef enum {
	W = 200, H = 100, Q = 50, Ei = 25, Y = 100000
	} Duration; 



typedef struct {
	Note note;
	Duration duration;
} PlayingNote;

//prints Date on top row, time on bottom row.


 PlayingNote twinkle[] = {{C,W}, {X,Y}, {C,H}, {X,Y}, {G,H}, {X,Y}, {G,H}, {X,Y}, {A,H}, {X,Y}, {A,H}, {X,Y}, {G,W},{X,Y},{X,Y}, {F,H}, {X,Y}, {F,H}, {X,Y}, {E,H}, {X,Y}, {E,H}, {X,Y}, {D,H}, {X,Y}, {D,H}, {X,Y}, {C,W},{X,Y}, {X,Y},{G,H}, {X,Y}, {G,H}, {X,Y}, {F,H}, {X,Y}, {F,H}, {X,Y}, {E,H}, {X,Y}, {E,H}, {X,Y}, {D,W},{X,Y},{X,Y},{G,H}, {X,Y}, {G,H}, {X,Y}, {F,H}, {X,Y}, {F,H}, {X,Y}, {E,H}, {X,Y}, {E,H}, {X,Y}, {D,W}};

void play_note(const PlayingNote* note) {
	
	int i;
	for (i = 0; i < note->duration; i++) {
		SET_BIT(PORTB, 3);
		avr_wait(note->note);
		CLR_BIT(PORTB, 3);
		avr_wait(note->note);
	}
}


int get_key()
{
	int i, j;
	for(i = 0; i < 4; ++i){
		for(j = 0; j < 4; ++j){
			if(is_pressed(i,j)){
				return i*4+j+1;
			}
		}
	}
	return 0;
	
}



int is_pressed(int r, int c)
{
	
	DDRC = 0x00;  // set lower 4 bits as input
	PORTC = 0x00;

	
	SET_BIT(DDRC, r);
	CLR_BIT(PORTC, r);
	
	SET_BIT(PORTC, c + 4);
	
	avr_wait(1);
	if(GET_BIT(PINC, c+4) == 0)
	{
		
		return 1;
	}

	return 0;
}


void checkSetUp()
{
	//if we press '#' we resume the clock
	if(get_key() == 15)
	{
		setUp = 0;
	}
	//if we press 'D' we stop the clock
	if(get_key() == 16)
	{
		setUp = 1;
	}
}

void play_song(const PlayingNote song[], int length) {
	int i;
	

	for (i = 0; i < length; i++) {
		//first check setUp
		checkSetUp();
			
		//if it is not paused
		
		if (setUp == 1)	//if it is paused
		{
			while(setUp == 1)
			{
				checkSetUp();
			}
		}
		if(setUp == 0)
		{
			//play the song
			play_note(&song[i]);
		}
		
	}
	
	
}

void print()
{
		char nowP[17];
		// Print Time on bottom row.
		lcd_pos(0,0);
		sprintf(nowP, "Now Playing: ");
		lcd_puts2(nowP);
		char title[17];
		// Print Time on bottom row.
		lcd_pos(1,0);
		sprintf(title, "Twinkle Twinkle ");
		lcd_puts2(title);
	
}
int main(void)
{
	
	lcd_init();
		
	SET_BIT(DDRB, 3);
	print();
	while(1)
	{	

		play_song(twinkle, 58);
		
	}
	

}

