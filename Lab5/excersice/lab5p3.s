.global _start    //do display part              
_start:
//R12 <-	[ KEY[0-3] ]
//R11 Edge detector value
//R10 <-	[ HEX0-3[0-6] ]
//R9 is const 0;
//R8 is the counter value (0~99)
//R7 holds the control flags for the clock
//R6 100 to reset at 100
//R5 reset for edge key flags
//R4 address of the timer
//R0 and R1 reserved for functions

			
			ldr r12, =0xFF20005C //r12 holds the address of keys 0-3
            //mov r5, #255
            //strb r5, [r12]
            ldr r4, =0xFFFEC600
            ldr r7, =50000000
            str r7, [r4]
            mov r7, #0b011
			mov R8, #0		//counter value
			mov r9, #0
            mov r6, #100
			mov r5, #15
			LDR r10, =0xFF200020	//r10 holds the address of HEX0 display
			b COUNT

COUNT:		ldrb r11, [r12]		//load the last 4 bits to r11
			cmp r11, r9			//if 0000, continue
			bne STOP
			
            cmp r8, r6
            moveq r8, #0
            
			add R8, #1
            push {r0,r1,r2,r3}
			bl DO_DELAY
            pop {r0,r1,r2,r3}
			
			b DISPLAY

			
			beq COUNT		//if 0 display
			
DO_DELAY:	str r7, [r4, #8]  // Start counter
SUB_LOOP: 	LDR r0, [r4, #0xC]    //Flag when counter set to zero
            cmp r0, r9		//r9 is 0
            beq SUB_LOOP
			mov r0, #1
            str r0, [r4, #0xC]  //Reset the counter set to zero flag.
            mov r0, #2
            str r0, [r4, #8]    //Stop counter
			bx lr
			
STOP:		strb r5, [r12]
			b WAIT
            
WAIT:		ldrb r11, [r12]
			cmp r11, r9
            strneb r5, [r12]
            bne COUNT
            b WAIT
            


/* Subroutine to perform the integer division R0 / R1.
 * Returns: quotient in R1, and remainder in R0
*/
DIVIDE:     MOV    R2, #0
CONT:       CMP    R0, R1
            BLT    DIV_END
            SUB    R0, R1
            ADD    R2, #1
            B      CONT
DIV_END:    MOV    R1, R2     // quotient in R1 (remainder in R0)
            BX lr
		  
SEG7_CODE:  MOV     R1, #BIT_CODES  
            LDRB    R0, [R1, R0]    
            BX      LR


BIT_CODES:  .byte   0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
            .byte   0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
            .skip   2      // pad with 2 bytes to maintain word alignment
			


/* Display R8 on HEX1-0 */
//Stores R12 into the HEX0~3
DISPLAY:    
			MOV R0, R8
            MOV R1, #10
            BL DIVIDE //We wanna display R1 and R0
            MOV R2, R0
            MOV R0, R1
			BL      SEG7_CODE  //Rn R0 is the bit code
            
            mov r3, r0, LSL #8
            
            MOV r0, r2
  			BL      SEG7_CODE  //Rn R0 is the bit code
        
			ORR     r3, R0		
			STR     R3, [R10]
			b COUNT
			