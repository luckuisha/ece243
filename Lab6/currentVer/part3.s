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

//r5 = [LEDRs]
//r11 = [CONFIG TIMER]      
//r12 = [keys]                                 
/* Set up stack pointers for IRQ and SVC processor modes */
                
				ldr sp, =0xFFFFFFEC
				mov r0, 0b01010010		//changes mode to irq, but also changes the interrupt bit to 0
				msr cpsr, r0
				ldr sp, =0xFFFFAAAA
				//mov r0, 0b10011			//converts back to svc mode
				msr cpsr, spsr
				//msr cpsr, r0

				
				ldr		r11, TIMER_BASE 
				ldr 	r12, KEY_BASE
                  BL       CONFIG_GIC       // configure the ARM generic
                                            // interrupt controller
                  BL       CONFIG_TIMER     // configure the Interval Timer
                  BL       CONFIG_KEYS      // configure the pushbutton
                                            // KEYs port

/* Enable IRQ interrupts in the ARM processor */
                  //done above
				  
				  
                  LDR      R5, =0xFF200000  // LEDR base address
LOOP:                                          
                  LDR      R3, COUNT        // global variable
                  STR      R3, [R5]         // write to the LEDR lights
                  B        LOOP                

/* Configure the Interval Timer to create interrupts at 0.25 second intervals */
CONFIG_TIMER:     ldrh	r4, [COUNT_VALUE]
                  str	r4, [r11, #0xC]
				  ldrh	r4, [COUNT_VALUE, #1]
				  str	r4, [r11, #0x8]
				  mov	r4, 0b0111
				  str	r4, [r11, #0x4]
                  BX       LR                  

/* Configure the pushbutton KEYS to generate interrupts */
CONFIG_KEYS:                                    
					mov r0, #0111
					str r0, [r12, #0x8]
                  BX       LR  

				  
SERVICE_IRQ:    PUSH     {R0-R7, LR}     
                LDR      R4, =0xFFFEC100 // GIC CPU interface base address
                LDR      R5, [R4, #0x0C] // read the ICCIAR in the CPU interface
				
KEYS_HANDLER:                       
                CMP      R5, #73         // check the interrupt ID
				beq	KEY_ISR
				cmp		R5, INTERVAL_TIMER_IRQ
				beq	CLOCK_ISR
				
				B		UNEXPECTED

UNEXPECTED:     BNE      UNEXPECTED      // if not recognized, stop here
                
				
KEY_ISR:		ldr		r7, [r12, #0xC]
				ldr		r6, RUN
				
				cmp 	r7, KEY0
				beq		STOP
				
				cmp		r7, KEY1
				beq		DOUBLE
				
				cmp		r7, KEY2
				beq		HALF
				
UNEXPECTED_KEY:	b UNEXPECTED_KEY

DOUBLE:			mov r0, 0b1011
				str r0, [r11]
				ldr r2, [COUNT_VALUE]
				lsl r2, #1
				str r2, [COUNT_VALUE]
				ldrb r2, [COUNT_VALUE]
				str r2, [r11, #0x8]
				ldrb r2, [COUNT_VALUE, #1]
				str r2, [r11, #0xC]
				mov r0, 0b0111
				str r0, [r11]
				mov 	r0, #FF
				str r0, [r12, #0xC]
				b EXIT_IRQ

HALF:			mov r0, 0b1011
				str r0, [r11]
				ldr r2, [COUNT_VALUE]
				lsr r2, #1
				str r2, [COUNT_VALUE]
				ldrb r2, [COUNT_VALUE]
				str r2, [r11, #0x8]
				ldrb r2, [COUNT_VALUE, #1]
				str r2, [r11, #0xC]
				mov r0, 0b0111
				str r0, [r11]
				mov 	r0, #FF
				str r0, [r12, #0xC]
				b EXIT_IRQ				


				
STOP:			mov r0, #0
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
				.global  COUNT_VALUE
COUNT_VALUE		.word	0b1011111010111100001000000
                  .end                                        
