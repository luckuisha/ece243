/* Program that converts a binary number to decimal */
           .text               // executable code follows
           .global _start
_start:
			MOV    SP, #0
            SUB    SP, #3 //set up stack pointer to it's correct position
            
            MOV    R4, #N		//number to be converted	
            MOV    R5, #Digits  // R5 points to the decimal digits storage location
            LDR    R4, [R4]     // R4 holds N
            MOV    R0, R4       // parameter for DIVIDE goes in R0
            BL     DISMANTLE
            STRB   R12, [R5, #3] // 4th digit is now in R12
            STRB   R11, [R5, #2] // 3rd digit is now in R11
            STRB   R10, [R5, #1] // 2nd digit is now in R10
            STRB   R9,  [R5]     // 1st digit is now in R9
END:        B      END

/* Subroutine to dismantle 4 digit decimal R0. (R4 original not to be touched I guess)
 * Returns: quotient in R1, and remainder in R0
*/
DISMANTLE:	MOV     R12, #0
			MOV     R11, #0
			MOV     R10, #0
            MOV     R9 , #0
            
			SUB    SP, #1 //we are using functions in function so we need to store lr
			STR	   LR, [SP]
            
            MOV    R1, #1000		
            
            CMP    R0, R1
            BL     DIVIDE //R1 is quotient and R0 is the remainder
            CMP    R1, #1000
            MOVNE  R12, R1
            
            MOV    R1, #100
			
            CMP    R0, R1
            BL     DIVIDE //R1 is quotient and R0 is the remainder
            CMP    R1, #100
            MOVNE  R11, R1
            
            MOV    R1, #10
			
            CMP    R0, R1
            BL     DIVIDE //R1 is quotient and R0 is the remainder
            CMP    R1, #10
            MOVNE  R10, R1
            
            MOV    R9, R0
            
            LDR    LR, [SP] //load from stack
            ADD    SP, #1
            MOV	   PC, LR
          
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

N:          .word  8954         // the decimal number to be converted
Digits:     .space 4          // storage space for the decimal digits (4 bytes)

            .end
