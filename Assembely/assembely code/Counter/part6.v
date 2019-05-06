.define LEDR_ADDRESS 0x1000
.define SW_ADDRESS 0x3000
.define HEX_ADDRESS 0x2000
.define DELAY_VAL 8888
.define STACK 256

/* # // Read SW switches and display on LEDs
  # mvi r3, #LEDR_ADDRESS  // point to LED port
  # mvi r4, #SW_ADDRESS  // point to SW port
# MAIN:  ld  r0, [r4]     // read SW values
  # st  r0, [r3]     // light up LEDs
  # mvi r7, #MAIN */


			mvi r6, #STACK		//stack pointer
			mvi r1, #1			//need to add/subb
			mv r5, r7			//return address for subroutine	
			mvi r7, #BLANK		//call subroutine blank the HEX display
MAIN:  		mvi r0, #0			//initialize counter
LOOP:  		mvi r3, #HEX_ADDRESS//point to hex port
			sub r6, r1			//save current counter value on stack
			st r0, [r6]
  //loop for extracting each decimal digit. by repaetedly dividing the counter by 10
DISPL:  	sub r6, r1			//save current hex digit pointer on the stack
			st r3, [r6]
			mvi r3, #10
			mv r5, r7
			mvi r7, #DIV
  
  
			mvi r4, #DATA
			add r4, r0
			ld r4, [r4]
			ld r3, [r6]
			add r6, r1
			st r4, [r3]
  
			mv r0, r2
			add r3, r1
			mvi r5, #DISPL
			mv r0, r0
			mvnz r7, r5
  
  
			mvi r5, #DELAY_VAL
			mv r4, r7
Inner:  mvi r1, #SW_ADDRESS
			ld r2, [r3]
			add r2, r1
			mv r3, r7
			sub r2, r1
			mvnz r7, r3
DELAY:  	sub r5, r1
			mvnz r7, r4
		  
			mvi r3, #LOOP
			ld r0, [r6]
			add r6, r1
			add r0, r1
			mvnc r7, r3
		  
			mv r5, r7
			mvi r7, #BLANK
			mvi r7, #MAIN
		  
		  
		  
			mvnz 
  
			ld r4, [r6]
			add r4, r1
			ld r4, [r6]
			add r4, r1
			ld r4, [r6]
			add r4, r1
			ld r4, [r6]
			add r4, r1

			add r4, r1
			add r4, r1
			mv
BLANK:		//Uses r0,r2 so they must be stacked
			add r6, r1
			st	r0, [r6]
			add r6, r1
			st  r2, [r6]

			mvi	r0, #0				// used for clearing
			mvi	r2, #HEX_ADDRESS	// point to HEX displays
			st		r0, [r2]				// clear HEX0
			add	r2, r1
			st		r0, [r2]				// clear HEX1
			add	r2, r1
			st		r0, [r2]				// clear HEX2
			add	r2, r1
			st		r0, [r2]				// clear HEX3
			add	r2, r1
			st		r0, [r2]				// clear HEX4
			add	r2, r1
			st		r0, [r2]				// clear HEX5
			
			ld  r2,[r6]
			sub r6,[r1]
			ld  r0,[r6]
			sub r6,[r1]
			
			add	r5, r1
			add	r5, r1
			mv		r7, r5				// return from subroutine
  
  
DATA:  	.word 0b0000000000111111 %0
		.word 0b0000000000000110 %1
		.word 0b0000000001011011 %2
		.word 0b0000000001001111 %3
		.word 0b0000000001100110 %4
		.word 0b0000000001101101 %5
		.word 0b0000000001111101 %6
		.word 0b0000000000000111 %7
		.word 0b0000000001111111 %8
		.word 0b0000000001100111 %9

