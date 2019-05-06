
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
.global  _start 
//.include "interrupt_ID.s"
//.include	"address_map_arm.s"
//				.include "defines.s"
//.include "exceptions.s"

_start:    
//r12 = [KEYs]                      
//r11 = [HEX display]
//
/* Set up stack pointers for IRQ and SVC processor modes */
                 
				
				mov r0, #0b11010010		//changes mode to irq, but also changes the interrupt bit to 0 
				msr cpsr_c, r0
				ldr sp, =0x20000
				mov r0, #0b11010011			//converts back to svc mode //bug 0b10011
				msr cpsr_c, r0
				ldr sp, =0x3FFFFFFC
				
                BL       CONFIG_GIC      // configure the ARM generic
                                         // interrupt controller
/* Configure the KEY pushbuttons port to generate interrupts */
                
				ldr r12, =0xFF200050
				mov r0, #0b1111
				str r0, [r12, #0x8]  
				
				ldr r11, =0xFF200020
				
				

/* Enable IRQ interrupts in the ARM processor */
				mov r0, #0b01010011			//converts back to svc mode //bug 0b10011
				msr cpsr_c, r0
				
IDLE:                                    
                B        IDLE            // main program simply idles

/* Define the exception service routines */
////Exceptions happen in IRQ mode 
////R12 -> KEY_BASE 
////R11 -> HEX3_HEX0_BASE
//// R4 -> GIC CPU interface base address
//// R5 -> ICCIAR in the CPU interface (Interrupt flag in GIC)
SERVICE_IRQ:    PUSH     {R0-R7, LR}

                LDR      R4, =0xFFFEC100 // GIC CPU interface base address
                LDR      R5, [R4, #0x0C] // read the ICCIAR in the CPU interface


				
KEYS_HANDLER:                       
                CMP      R5, #73         // check the interrupt ID

UNEXPECTED:     BNE      UNEXPECTED      // if not recognized, stop here
                BL       KEY_ISR  		
				B 		EXIT_IRQ

				
				
KEY_ISR:		ldr		r7, [r12, #0xC] //Load Key edge
				mov 	r1, #0xff			//reset the key constant global variable
				mov 	r2, #1			//// To see which key is pressed
				
				cmp 	r7, r2
				beq		DISPLAY0
				
				lsr		r7, #1
				cmp		r7, r2
				beq		DISPLAY1
				
				lsr		r7, #1
				cmp		r7, r2
				beq		DISPLAY2
				
				lsr		r7, #1
				cmp		r7, r2
				beq		DISPLAY3

								
DISPLAY0:		
				ldrb	r6, [r11]
				cmp		r6, #0
				moveq	r0, #0b00111111
				movne	r0, #0
				strb	r0, [r11]
				str		r1, [r12, #0xC]
				bx 		lr
				
DISPLAY1:		ldrb	r6, [r11, #1]
				cmp		r6, #0
				moveq	r0, #0b00000110
				movne	r0, #0
				strb r0, [r11, #1]
				str		r1, [r12, #0xC]
				bx 		lr
				
DISPLAY2:		ldrb 	r6, [r11, #2]
				cmp		r6, #0
				moveq	r0, #0b01011011
				movne	r0, #0
				strb	r0, [r11, #2]
				str		r1, [r12, #0xC]
				bx lr
				
DISPLAY3:		ldrb 	r6, [r11, #3]
				cmp		r6, #0
				moveq	r0, #0b01001111
				movne	r0, #0
				strb	r0, [r11, #3]
				str		r1, [r12, #0xC]
				bx lr
	
						

EXIT_IRQ:       STR      R5, [R4, #0x10] // write to the End of Interrupt
                                         // Register (ICCEOIR)
                POP      {R0-R7, LR}     
                SUBS     PC, LR, #4      // return from exception
				
      
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


					
		