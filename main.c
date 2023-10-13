/*
 * GccApplication3.c
 *
 * Created: 3/14/2023 5:23:23 PM
 * Author : Kai
 
 This is the program for the micro controller in controlling a 16 keypad input to play/pause
 a speaker with a LCD display. The speaker can play three recorded songs, specifically twinkle twinkle little star,
 old Macdonald's had a farm, marry had a little lamb. As the song plays, it can be paused and resumed using 'D' to pause, and '#' to resume playing.
 The LCD will display what song is playing at the current moment, with Now Playing: title of song. The user can use the keypad to switch
 between songs mid playing. The user can also press a button to enter record mode, which the user can use pin pad 1-7 to play the seven
 music notes. Afterwards, the user can press '8' to listen back to their recorded song.
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "avr.h"
#include "lcd.h"



int pause = 0;
int setUp = 0;
int findKey = 0;
int g_count = 0;
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
	W = 200, H = 100, Q = 50, Ei = 25, Y = 100000, Z = 50000, P = 350
	} Duration; 



typedef struct {
	Note note;
	Duration duration;
} PlayingNote;

//prints Date on top row, time on bottom r
 
 PlayingNote record[50];
 PlayingNote custom[] = {{A,H}, {B,H}, {C,H},{D,H}, {E,H},{F,H},{G,H}, {X,Y}};
 PlayingNote twinkle[] = {{C,H}, {X,Y}, {C,H}, {X,Y}, {G,H}, {X,Y}, {G,H}, {X,Y}, {A,H}, {X,Y}, {A,H}, {X,Y}, {G,W},{X,Y},{X,Y}, {F,H}, {X,Y}, {F,H}, {X,Y}, {E,H}, {X,Y}, {E,H}, {X,Y}, {D,H}, {X,Y}, {D,H}, {X,Y}, {C,W},{X,Y}, {X,Y},{G,H}, {X,Y}, {G,H}, {X,Y}, {F,H}, {X,Y}, {F,H}, {X,Y}, {E,H}, {X,Y}, {E,H}, {X,Y}, {D,W},{X,Y},{X,Y},{G,H}, {X,Y}, {G,H}, {X,Y}, {F,H}, {X,Y}, {F,H}, {X,Y}, {E,H}, {X,Y}, {E,H}, {X,Y}, {D,W}};
 PlayingNote mac[] = {{G,H}, {X,Y}, {G,H}, {X,Y}, {G,H}, {X,Y}, {D,H}, {X,Y}, {E,H}, {X,Y}, {E,H}, {X,Y}, {D,W}, {X,Y}, {B,H}, {X,Y}, {B,H}, {X,Y}, {A,H}, {X,Y}, {A,H}, {X,Y}, {G,W}, {X,Y}, {D,H}, {X,Y}, {G,H}, {X,Y}, {G,H}, {X,Y}, {G,H}, {X,Y}, {D,H}, {X,Y}, {E,H}, {X,Y}, {E,H}, {X,Y}, {D,W}, {X,Y}, {B,H}, {X,Y}, {B,H}, {X,Y}, {A,H}, {X,Y}, {A,H}, {X,Y}, {G,W} };
 PlayingNote merry[] = {{E, H},{X,Y}, {D,H}, {X,Y}, {C,H}, {X,Y}, {D,H} , {X,Y}, {E, H}, {X, Y}, {E,H},{X, Y}, {E,H}, {X,Y}, {X,Y}, {D,H}, {X,Z}, {D,H}, {X,Z}, {D,H}, {X,Y}, {X,Y}, {E,H}, {X,Y}, {G,H}, {X,Y}, {G, H}, {X,Y}, {X,Y}, {E,H}, {X,Y}, {D,H}, {X,Y}, {C,H}, {X,Y}, {D,H}, {X,Y}, {E,H}, {X,Y}, {E,H}, {X,Y}, {E,H}, {X,Y}, {E,H}, {X,Y}, {D,H}, {X,Y}, {D,H}, {X,Y}, {E,H}, {X,Y}, {D,H}, {X,Y}, {C,H}, {X,Y} };
void play_note(const PlayingNote* note) {
	
	//int i;
	for (int i = 0; i < note->duration; i++) {
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


void checkPause()
{
	//if we press '#' we resume the clock
	if(get_key() == 15)
	{
		pause = 0;
	}
	//if we press 'D' we stop the clock
	if(get_key() == 16)
	{
		pause = 1;
	}
}

void play_song(const PlayingNote song[], int length) {
	int i;
	

	for (i = 0; i < length; i++) {
		//first check setUp
		checkPause();
		findKey = get_key();
		if(findKey == 14)
		{
			break;
		}
		//if it is not paused
		
		if (pause == 1)	//if it is paused
		{
			while(pause == 1)
			{
				checkPause();
			}
		}
		if(pause == 0)
		{
			//play the song
			play_note(&song[i]);
			
		}
	}
	
	
}


void checkSetup()
{
	if(get_key() == 10)
		setUp = 1;
	if(get_key() == 11)
		setUp = 0;
}

void print(int num)
{
		
		char nowP[17];
		// Print Time on bottom row.
		lcd_pos(0,0);
		sprintf(nowP, "Now Playing: ");
		lcd_puts2(nowP);
		
		if(num == 1)
		{
			char title[17];
			// Print Time on bottom row.
			lcd_pos(1,0);
			sprintf(title, "Twinkle Twinkle ");
			lcd_puts2(title);	
		}
		else if (num == 2)
		{
			char title[17];
			// Print Time on bottom row.
			lcd_pos(1,0);
			sprintf(title, "Old Macdonald");
			lcd_puts2(title);
		}
		else if (num == 3)
		{
			char title[17];
			// Print Time on bottom row.
			lcd_pos(1,0);
			sprintf(title, "Merry Lamb");
			lcd_puts2(title);
		}
		else if (num == 4)
		{
			char title[17];
			// Print Time on bottom row.
			lcd_pos(1,0);
			sprintf(title, "Custom Notes");
			lcd_puts2(title);
		}
		else if (num == 5)
		{
			char title[17];
			// Print Time on bottom row.
			lcd_pos(1,0);
			sprintf(title, "Recorded");
			lcd_puts2(title);
		}
	
}
void playKey()
{
	if(get_key() == 1)
	{
		play_note(&custom[0]);
		record[g_count] = custom[0];
		record[g_count + 1] = custom[7];
		g_count += 2;
	}
	if(get_key() == 2)
	{
		play_note(&custom[1]);
		record[g_count] = custom[1];
		record[g_count + 1] = custom[7];
		g_count += 2;
	}
	if(get_key() == 3)
	{
		play_note(&custom[2]);
		record[g_count] = custom[2];
		record[g_count + 1] = custom[7];
		g_count += 2;
	}
	if(get_key() == 5)
	{
		play_note(&custom[3]);
		record[g_count] = custom[3];
		record[g_count + 1] = custom[7];
		g_count += 2;
	}
	if(get_key() == 6)
	{
		play_note(&custom[4]);
		record[g_count] = custom[4];
		record[g_count + 1] = custom[7];
		g_count += 2;
	}
	if(get_key() == 7)
	{
		play_note(&custom[5]);
		record[g_count] = custom[5];
		record[g_count + 1] = custom[7];
		g_count += 2;
	}
	if(get_key() == 9)
	{
		play_note(&custom[6]);
		record[g_count] = custom[6];
		record[g_count + 1] = custom[7];
		g_count += 2;
	}
}

int main(void)
{
	
	lcd_init();
		
	SET_BIT(DDRB, 3);
	while(1)
	{	
			if(get_key() == 4)
			{
				lcd_clr();
				print(1);
				play_song(twinkle, sizeof(twinkle));
			}
			if(get_key() == 8)
			{
				lcd_clr();
				print(2);
				play_song(mac, sizeof(mac));
				
			}
			if(get_key() == 12)
			{
				print(3);
				play_song(merry, sizeof(merry));
			}
			if(get_key() == 13)
			{
				lcd_clr();
				print(4);
				while (1)
				{
					playKey();
					if(setUp == 1)
					{
							print(5);
							play_song(record,  g_count);	
					}
					checkSetup();
				}
			}
			
		
		
	}
	

}