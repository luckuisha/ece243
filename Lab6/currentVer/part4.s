
.section .vectors, "ax"  
B        _start              // reset vector
B        SERVICE_UND         // undefined instruction vector
B        SERVICE_SVC         // software interrupt vector
B        SERVICE_ABT_INST    // aborted prefetch vector
B        SERVICE_ABT_DATA    // aborted data vector
.word    0                   // unused vector
B        SERVICE_IRQ         // IRQ interrupt vector
B        SERVICE_FIQ         // FIQ interrupt vector
	

					
					.text                                       
                    .global _start       
				
_start:                                         
/* Set up stack pointers for IRQ and SVC processor modes */
                mov r0, #0b11010010		//changes mode to irq, but also changes the interrupt bit to 0 
				msr cpsr_c, r0
				ldr sp, =0x20000
				mov r0, #0b11010011			//converts back to svc mode //bug 0b10011
				msr cpsr_c, r0
				ldr sp, =0x3FFFFFFC
				
					
				
				
                    BL      CONFIG_GIC          // configure the ARM generic
                                                // interrupt controller
												
					ldr		r10, =0xFFFEC600
					ldr		r11, =0xFF202000 
					ldr 	r12, =0xFF200050
					ldr     r7, = 6000
					BL      CONFIG_PRIV_TIMER   // configure the private timer
                    BL      CONFIG_TIMER        // configure the Interval Timer
                    BL      CONFIG_KEYS         // configure the pushbutton
                                                // KEYs port
                    
/* Enable IRQ interrupts in the ARM processor */
        mov r0, #0b01010011			//converts back to svc mode //bug 0b10011
		msr cpsr_c, r0
		
		
					
					
                    LDR     R5, =0xFF200000     // LEDR base address
                    LDR     R6, =0xFF200020     // HEX3-0 base address
LOOP:                                           
                    LDR     R4, COUNT           // global variable
                    STR     R4, [R5]            // light up the red lights
                    LDR     R4, HEX_code        // global variable
                    STR     R4, [R6]            // show the time in format
                                                // SS:DD
                    B       LOOP                

/* Configure the MPCore private timer to create interrupts every 1/100 seconds */
CONFIG_PRIV_TIMER:                              
                    ldr r0, =2000000
					str r0, [r10]
					mov r0, #0b111
					str r0, [r10, #0x8]
                    BX      LR                  
/* Configure the Interval Timer to create interrupts at 0.25 second intervals */
CONFIG_TIMER:	  ldr	r4, COUNT_VALUE
				  ldrh	r4, [r4]
                  str	r4, [r11, #0xC]
				  ldr	r4, COUNT_VALUE
				  ldrh	r4, [r4, #2]
				  str	r4, [r11, #0x8]
				  mov	r4, #0b1111
				  str	r4, [r11, #0x4]
                  BX       LR    
                    BX      LR                  
/* Configure the pushbutton KEYS to generate interrupts */
CONFIG_KEYS:                                    
					mov r0, #0b1111
					str r0, [r12, #0x8]
                    BX      LR
	  
				  
				  
SERVICE_IRQ:    PUSH     {R0-R6, LR}     
                LDR      R4, =0xFFFEC100 // GIC CPU interface base address
                LDR      R5, [R4, #0x0C] // read the ICCIAR in the CPU interface
				
KEYS_HANDLER:                       
                CMP      R5, #73         // check the interrupt ID
				beq	KEY_ISR
				
				cmp		R5, #72
				beq	CLOCK_ISR
				
				cmp r5, #29
				beq PRIVATE_ISR
				
				B		UNEXPECTED

UNEXPECTED:     BNE      UNEXPECTED      // if not recognized, stop here
                
				
KEY_ISR:		ldr		r3, [r12, #0xC]
				ldr		r6, RUN
				
				cmp 	r3, #0b0001
				beq		STOP
				
				cmp		r3, #0b0010
				beq		DOUBLE
				
				cmp		r3, #0b0100
				beq		HALF
				
				cmp		r3, #0b1000
				beq		STOP_PRIVATE
				
UNEXPECTED_KEY:	b UNEXPECTED_KEY

DOUBLE:			mov r0, #0b1011
				str r0, [r11, #0x4]
				ldr r1, COUNT_VALUE
				ldr r2, [r1]
				lsl r2, #1
				str r2, [r1]
				ldrh r2, [r1]
				str r2, [r11, #0x8]
				ldrh r2, [r1, #2]
				str r2, [r11, #0xC]
				mov r0, #0b0111
				str r0, [r11]
				mov 	r0, #0xFF
				str r0, [r12, #0xC]
				b EXIT_IRQ

HALF:			mov r0, #0b1011
				str r0, [r11, #4]
				ldr r1, COUNT_VALUE
				ldr r2, [r1]
				lsr r2, #1
				str r2, [r1]
				ldrh r2, [r1]
				str r2, [r11, #0x8]
				ldrh r2, [r1, #2]
				str r2, [r11, #0xC]
				mov r0, #0b0111
				str r0, [r11]
				mov 	r0, #0xFF
				str r0, [r12, #0xC]
				b EXIT_IRQ				


				
STOP:			mov r0, #0
				cmp r6, r0
				moveq r0, #1
				str r0, [r6]
				mov 	r1, #0xFF			//reset the key constant global variable
				str r1, [r12]
				b EXIT_IRQ
	
STOP_PRIVATE:	ldr r1, [r10, #0x8]
				mov r0, #1
				and r1, r0
				cmp r1, r0
				mov r2 , #0
				streq r2, [r10, #0x8]
				strne r0, [r10, #0x8]
				mov 	r0, #0xFF
				str r0, [r12, #0xC]
				b EXIT_IRQ
				
				  
CLOCK_ISR:		ldr r3, COUNT
				ldr r2,[r3]
				ldr r6, RUN
				add r2, r6
				str r2, [r3]
				mov r0, #0
				str r0, [r11]
				b EXIT_IRQ
				
PRIVATE_ISR:	ldr r0, TIME
				ldr r0, [r0]
				cmp r0, r7
				mov r1, #1
				add r0, r1
				moveq r0, #0
				ldr r0, TIME
				str r0, [r0]
				str r1, [r2, #0xC]
				B DISPLAY
				
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
DISPLAY:    push {r0,r1,r2,r3}
			ldr r8, TIME
			ldr r8, [r8]
			MOV R0, R8
            MOV R1, #1000
            BL DIVIDE //We wanna display R1 and R0
            MOV R2, R0 //Store remainder R2
            MOV R0, R1 // Move division result in R0
			BL      SEG7_CODE  //Takes R0 number returns r0 as bit code for hex display
            
            mov r3, r0, LSL #24 //r3 holds all the bit codes
            
            MOV r0, r2 //The remainder was in R2 now divide by 100
            MOV R1, #100
            BL DIVIDE //Takes r0/r1 returns r0 is remainder r1 is quotient

            MOV R2, R0 //Store remainder R2
            MOV R0, R1 // Move division result in R0
			BL      SEG7_CODE  //Takes R0 number returns r0 as bit code for hex display
            
            mov r0, r0, LSL #16 //r3 holds all the bit codes
            ORR     r3, R0

            MOV r0, r2 //The remainder was in R2 now divide by 100
            MOV R1, #10
            BL DIVIDE //Takes r0/r1 returns r0 is remainder r1 is quotient

            MOV R2, R0 //Store remainder R2
            MOV R0, R1 // Move division result in R0
			BL      SEG7_CODE  //Takes R0 number returns r0 as bit code for hex display
            
            mov r0, r0, LSL #8 //r3 holds all the bit codes
            ORR     r3, R0

            mov r0, r2
  			BL      SEG7_CODE  //Rn R0 is the bit code
            ORR     r3, R0
	
	ldr r9, HEX_code
			STR     R3, [r9]
			pop {r0,r1,r2,r3}
			b EXIT_IRQ
			
				  
EXIT_IRQ:       STR      R5, [R4, #0x10] // write to the End of Interrupt
                                         // Register (ICCEOIR)
                POP      {R0-r6, LR}     
                SUBS     PC, LR, #4      // return from exception
					
					
					
					
					
					
/* Global variables */
                    .global COUNT                               
COUNT:              .word   0x0       // used by timer
                    .global RUN       // used by pushbutton KEYs
RUN:                .word   0x1       // initial value to increment COUNT
                    .global TIME                                
TIME:               .word   0x0       // used for real-time clock
                    .global HEX_code                            
HEX_code:           .word   0x0       // used for 7-segment displays
				.global  COUNT_VALUE
COUNT_VALUE:		.word	0b1011111010111100001000000
                                       

					
						  /* Undefined instructions */
SERVICE_UND:                                
                    B   SERVICE_UND         
/* Software interrupts */
SERVICE_SVC:                                
                    B   SERVICE_SVC         
/* Aborted data reads */
SERVICE_ABT_DATA:                           
                    B   SERVICE_ABT_DATA    
/* Aborted instruction fetch */
SERVICE_ABT_INST:                           
                    B   SERVICE_ABT_INST    
SERVICE_FIQ:                                
                    B   SERVICE_FIQ         
                    
                    .end                    