.global _start                  
_start:		ldr r12, =0xFF200050 //r12 holds the address of keys 0-3
			mov r0, #0		//counter value
			LDR r10, =0xFF200020	//r10 holds the address of HEX0 display
			b WAITKEY

WAITKEY:	ldrb r11, [r12]		//load the first 4 bits to r11
			cmp r11, #0			//if 000, do nothing
			beq WAITKEY		//if 0 display
			
			cmp r11, #1			//0001 (key0)
			moveq r0, #0
			beq RELEASE
			
			cmp r11, #2			//0010 (key1)
			addeq r0, #1
			beq RELEASE
			
			cmp r11, #4			//0100 (key2)
			subeq r0, #1
			beq RELEASE
			
			cmp r11, #8			//1000 (key3)
			moveq r0, #0
			beq RELEASECLEAR
			
			
RELEASE:	ldrb r11, [r12] 
			cmp r11, #0
			beq DISPLAY
			b RELEASE
			
RELEASECLEAR:	
			ldrb r11, [r12] 
			cmp r11, #0
			bne RELEASECLEAR
			str r0, [r10]
			b PRESS

PRESS: 		ldrb r11, [r12]
			cmp r11, #0
			beq WAITKEY
			b PRESS
		  
		  
SEG7_CODE:  MOV     R1, #BIT_CODES  
            LDRB    R2, [R1, R2]    
            BX      LR


BIT_CODES:  .byte   0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
            .byte   0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
            .skip   2      // pad with 2 bytes to maintain word alignment
			


/* Display R0 on HEX1-0 */
DISPLAY:    mov r2, r0
			BL      SEG7_CODE          
			STR     R2, [R10]
			b WAITKEY
			