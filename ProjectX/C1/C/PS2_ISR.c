extern volatile char byte1, byte2, byte3;

/***************************************************************************************
 * Pushbutton - Interrupt Service Routine                                
 *                                                                          
 * This routine checks which KEY has been pressed. If it is KEY1 or KEY2, it writes this 
 * value to the global variable key_pressed. If it is KEY3 then it loads the SW switch 
 * values and stores in the variable pattern
****************************************************************************************/
#include "Game.h"




#define W 0x1D
#define A 0x1C
#define S 0x1B
#define D 0x23
#define SPACE 0x29

#define BREAK 0xF0

bool ps2Break = false;
bool ps2Static = false;

Key leftKey;
Key rightKey;
Key upKey;
Key downKey;
Key spaceKey;

void HEX_PS2(char b1, char b2, char b3) {
    //volatile int * HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
    //volatile int * HEX5_HEX4_ptr = (int *)HEX5_HEX4_BASE;

    /* SEVEN_SEGMENT_DECODE_TABLE gives the on/off settings for all segments in
     * a single 7-seg display in the DE2 Media Computer, for the hex digits 0 -
     * F */
//    unsigned char seven_seg_decode_table[] = {
//        0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
//        0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
//    unsigned char hex_segs[] = {0, 0, 0, 0, 0, 0, 0, 0};
    // unsigned int  shift_buffer, nibble;
    // unsigned char code;
    // int           i;
	//W = 0x1D
	//A = 0x1C
	//S = 0x1B
	//D = 0x23
	//space bar = 29
	//B = 32
	//enter key = 5A
	//Break key = 0xF0
	
	//if both b2 and b3 are the same, key is begin held down,
	//keep moving in specified direction
	
	//if b2 is F0, break code when a key is released,
	//only move in specified direction once
	if (b2 == BREAK)
		ps2Static = true;
	else ps2Static = false;
	
	switch (b3){
		case W:{
			if (ps2Break){
				ps2Break = false;
				upKey.pressed = false;
				upKey.released = true;
			}
			else if (!ps2Static) upKey.pressed = true;
			break;	
		}
		case A:{
			if (ps2Break){
				ps2Break = false;
				leftKey.pressed = false;
				leftKey.released = true;
			}
			else if (!ps2Static) leftKey.pressed = true;
			break;	
		}
		case S:{
			if (ps2Break){
				ps2Break = false;
				downKey.pressed = false;
				downKey.released = true;
			}
			else if (!ps2Static) downKey.pressed = true;
			break;
		}
		case D:{
			if (ps2Break){
				ps2Break = false;
				rightKey.pressed = false;
				rightKey.released = true;
			}
			else if (!ps2Static) rightKey.pressed = true;
			break;	
		}
		case SPACE:{
			if (ps2Break){
				ps2Break = false;
				spaceKey.pressed = false;
				spaceKey.released = true;
			}
			else if (!ps2Static) spaceKey.pressed = true;
			break;	
		}
		case BREAK:{
			ps2Break = true;
			break;	
		}
		default:{
			ps2Break = false;
			break;	
		}
	}

	




	
//	int tempTime = tick;
//	
//	int mins = tempTime / 60;
//	tempTime -= mins * 60;
//	
//	int tens = tempTime / 10;
//	tempTime -= tens * 10;
//	
//	int ones = tempTime;
	
//	shift_buffer = (mins << 8) | (tens << 4) | ones;
//    for (i = 0; i < 6; ++i) {
//        nibble = shift_buffer & 0x0000000F; // character is in rightmost nibble
//        code   = seven_seg_decode_table[nibble];
//        hex_segs[i]  = code;
//        shift_buffer = shift_buffer >> 4;
//    }
//    /* drive the hex displays */
//    *(HEX3_HEX0_ptr) = *(int *)(hex_segs);
//    *(HEX5_HEX4_ptr) = *(int *)(hex_segs + 4);
	
}


void PS2_ISR( void )
{
  	volatile int * PS2_ptr = (int *) 0xFF200100;		// PS/2 port address
	int PS2_data, RAVAIL;

	PS2_data = *(PS2_ptr);									// read the Data register in the PS/2 port
	RAVAIL = (PS2_data & 0xFFFF0000) >> 16;			// extract the RAVAIL field
	if (RAVAIL > 0)
	{
		/* always save the last three bytes received */
		byte1 = byte2;
		byte2 = byte3;
		byte3 = PS2_data & 0xFF;
		HEX_PS2(byte1, byte2, byte3);
		if ( (byte2 == (char) 0xAA) && (byte3 == (char) 0x00) )
			// mouse inserted; initialize sending of data
			*(PS2_ptr) = 0xF4;
	}
	return;
}
