.include "defines.s"
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
_start:  
.include "defines.s"
.include "address_map_arm.s"

//r5 = [LEDRs]
//r11 = [CONFIG TIMER]      
//r12 = [keys]                                 
/* Set up stack pointers for IRQ and SVC processor modes */
                
				mov r0, #0b11010010		//changes mode to irq, but also changes the interrupt bit to 0 
				msr cpsr_c, r0
				ldr sp, =0x20000
				mov r0, #0b11010011			//converts back to svc mode //bug 0b10011
				msr cpsr_c, r0
				ldr sp, =0x3FFFFFFC

				
				ldr		r11, TIMER_BASE 
				ldr 	r12, KEY_BASE
                  BL       CONFIG_GIC       // configure the ARM generic
                                            // interrupt controller
                  BL       CONFIG_TIMER     // configure the Interval Timer
                  BL       CONFIG_KEYS      // configure the pushbutton
                                            // KEYs port

/* Enable IRQ interrupts in the ARM processor */
				mov r0, #0b01010011			//converts back to svc mode //bug 0b10011
				msr cpsr_c, r0
				  
				  
                  LDR      R5, =0xFF200000  // LEDR base address
LOOP:                                          
                  LDR      R3, COUNT        // global variable
                  STR      R3, [R5]         // write to the LEDR lights
                  B        LOOP                

/* Configure the Interval Timer to create interrupts at 0.25 second intervals */
CONFIG_TIMER:     ldr 	r4, =0b0111100001000000
                  str	r4, [r11, #0xC]
				  ldr	r4, =0b101111101
				  str	r4, [r11, #0x8]
				  mov	r4, #0b0111
				  str	r4, [r11, #0x4]
                  BX       LR                  

/* Configure the pushbutton KEYS to generate interrupts */
CONFIG_KEYS:                                    
					mov r0, #1111
					str r0, [r12, #0x8]
                  BX       LR  

				  
SERVICE_IRQ:    PUSH     {R0-R7, LR}     
                LDR      R4, =0xFFFEC100 // GIC CPU interface base address
                LDR      R5, [R4, #0x0C] // read the ICCIAR in the CPU interface
				
KEYS_HANDLER:                       
                CMP      R5, #73         // check the interrupt ID
				beq	KEY_ISR
				cmp		R5, #INTERVAL_TIMER_IRQ
				beq	CLOCK_ISR
				
				B		UNEXPECTED

UNEXPECTED:     BNE      UNEXPECTED      // if not recognized, stop here
                
				
KEY_ISR:		ldr		r7, [r12, #0xC]
				ldr		r6, RUN
				mov r0, #0
				cmp r6, r0
				moveq r0, #1
				str r0, [r6]
				mov 	r1, #FF			//reset the key constant global variable
				str r1, [r12]
				b EXIT_IRQ
				  
CLOCK_ISR:		ldr r7, COUNT
				ldr r6, RUN
				add r7, r6
				str r7, [r7]
				mov r0, #0
				str r0, [r11]
				b EXIT_IRQ
				  
EXIT_IRQ:       STR      R5, [R4, #0x10] // write to the End of Interrupt
                                         // Register (ICCEOIR)
                POP      {R0-R7, LR}     
                SUBS     PC, LR, #4      // return from exception
				
			
				  
				  
/* Global variables */
                  .global  COUNT                           
COUNT:            .word    0x0              // used by timer
                  .global  RUN              // used by pushbutton KEYs
RUN:              .word    0x1              // initial value to increment
                                            // COUNT
                  .end    
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
