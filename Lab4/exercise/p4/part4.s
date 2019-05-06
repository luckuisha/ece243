/* Subroutine to convert the digits from 0 to 9 to be shown on a HEX display.
 * 	Parameters: R0 = the decimal value of the digit to be displayed
 * 	Returns: R0 = bit patterm to be written to the HEX display
 */
/* Program that counts consecutive 1s */

          .text                   // executable code follows
          .global _start                  
_start:                             
          MOV     R12, #TEST_NUM   // load the data word ...
          ldr     R1, [r12]        // into R1
		
          MOV     R0, #0          
		  mov R5, #0			// R5 will hold the result
		  mov R6, #0			// R6 will hold the result
		  mov R7, #0			// R7 will hold the result

		  
LOOP:	
		movs r1, r1
		beq DISPLAY
		
		mov r8, #0
		MOV     R10, R1
		bl ONES
		
		cmp r5, r8
		movlt r5, r8
		
		mov r8, #0
		MOV     R10, R1
		bl ZEROS
		
		cmp r6, r8
		movlt r6, r8
		
		mov r8, #0
		MOV     R10, R1
		bl ONEZEROS
        
        mov r9, r8 
        mov r8, #0
        MOV     R10, R1
        
        bl ONEZEROSNOT
		
        cmp r8, r9
		movlt r8, r9
        
		cmp r7, r8
		movlt r7, r8
		
		add R12, #4
		ldr r1, [R12]
		b LOOP
		
		
SEG7_CODE:  MOV     R1, #BIT_CODES  
            LDRB    R0, [R1, R0]    
            BX      LR       

/*R10 to receive the input data and register
* R8 output number of consecutive 1s.
*/		  
ONEZEROS:           
          LDR     R2, =0xaaaaaaaa      // perform SHIFT, followed by AND
          eOR     R10, R10, R2
		  B       ZEROS

ONEZEROSNOT: 
		  //mvn	  r10, r10	
          LDR     R2, =0xaaaaaaaa      // perform SHIFT, followed by AND
          eOR     R10, R10, R2
		  B       ONES

ONES:     
		  CMP     R10, #0          // loop until the data contains no more 1's
          moveq	  pc, lr             
          LSR     R2, R10, #1      // perform SHIFT, followed by AND
          AND     R10, R10, R2      
          ADD     R8, #1          // count the string length so far
          B       ONES 

ZEROS:    
		  mvn     r10, r10
		  B       ONES
		  
		  
		  
END:      B       END    


         


			

BIT_CODES:  .byte   0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
            .byte   0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
            .skip   2      // pad with 2 bytes to maintain word alignment


/* Display R5 on HEX1-0, R6 on HEX3-2 and R7 on HEX5-4 */
DISPLAY:    MOV     R1, #10
			MOV     R4, #0
			LDR     R8, =0xFF200020 // base address of HEX3-HEX0
            MOV     R0, R5          // display R5 on HEX1-0
            BL      DIVIDE          // ones digit will be in R0; tens
                                    // digit in R1
            MOV     R9, R1          // save the tens digit
            BL      SEG7_CODE       
            MOV     R11, R0          // save bit code for first digit
            MOV     R0, R9          // retrieve the tens digit, get bit
                                    // code
            BL      SEG7_CODE       //For the second digit
            ORR     R4, r11, R0, LSL # 8
            
            //code for R6 (not shown)
            MOV     R0, R6          // display R5 on HEX1-0
            BL      DIVIDE          // ones digit will be in R0; tens
                                    // digit in R1
            MOV     R9, R1          // save the tens digit
            BL      SEG7_CODE       
            MOV     r11, R0          // save bit code for first digit
            MOV     R0, R9          // retrieve the tens digit, get bit
                                    // code
            BL      SEG7_CODE       //For the second digit
			LSL R11, R11, #16
            ORR     R11, r11, R0, LSL # 24
			ORR     R4, r11, R4
			
			STR     R4, [R8]

            //code for R7 (not shown)
			//LDR     R8, =0xFF200030
            MOV     R0, R7          // display R5 on HEX1-0
            BL      DIVIDE          // ones digit will be in R0; tens
                                    // digit in R1
            MOV     R9, R1          // save the tens digit
            BL      SEG7_CODE       
            MOV     r11, R0          // save bit code for first digit
            MOV     R0, R9          // retrieve the tens digit, get bit
                                    // code
            BL      SEG7_CODE       //For the second digit
            ORR     R4, r11, R0, LSL #8
			STR     R4, [R8,#16]
			
			b END
			
	  
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
            MOV    PC, LR

TEST_NUM: 	.word 0x103fe00f 
			.word 0x00000000
			.word 0xaaaaaaaa
			.word 0xbbbbbbbb
			.word 0x11111111
			.word 0xabcdef10
			.word 0x01234567
			.word 0x56789abc
			.word 0xafafafaf
			.word 0x00000000

          .end        
