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
DELAYLOOP:	mv r5, r7
			mvi r7, #INCR		//Incriments and prints to hex
			
			mv r5, r7
			mvi r7, #LED		//leds
			
			mv r5, r7
			mvi r7, #SW		//takes sw values put them in r4
			
			mv r5, r7
			mvi r7, #DELAY		//delays
		
SW:
			mvi r3, #SW_ADDRESS
			ld r4, [r3]
			
			add	r5, r1
			add	r5, r1
			mv		r7, r5
		
LED:		
			mvi r3, #LEDR_ADDRESS
			st r0, [r3]
			
			add	r5, r1
			add	r5, r1
			mv		r7, r5
			

DELAY:	//Value of delay (switches) must be stored in r4
			sub r6, r1
			st	r2, [r6]
			sub r6, r1
			st  r3, [r6]
			sub r6, r1
			st	r5, [r6]
			
			mvi r3, #DELAY_VAL
			mvi r2, #1000
			mv r5, r7 //VV
			add r3, r2
			sub r4, r1
			mvnc r7, r5//^^^
			
			mv r5, r7 //VVV
			sub r3, r1
			mvnc r7, r5//^^^
			
			ld r5,[r6]
			add r6, r1
			ld r3,[r6]
			add r6, r1
			ld r2,[r6]
			add r6, r1
			
			mvi r7, #DELAYLOOP
			
			
INCR:		
			add r0, r1
			//mv r5, r7 this is wrong
			mvi r7, #DIV10000


DIV10000:	
			sub r6, r1
			st	r2, [r6]
			sub r6, r1
			st  r3, [r6]
			sub r6, r1
			st	r4, [r6]
			sub r6, r1
			st	r5, [r6]
			
			sub r6, r1
			st r1, [r6] //We store a flag on top of the stack*
			
			mv r2, r0  //Copy R0
			mvi r4, #0
			mv  r5, r7 //R5 return point     V
			add r4, r1 //count our sub
			mvi r3, #10000
			sub r2, r3
			mvnc r7, r5//loop until negative ^
			add r2, r3 //To restore it back to positive ***********
			sub r4, r1 //To reduce the extra count ***********
			mvi r5, #DISPL5
			mv r4, r4
			mvnz r7, r5 //We display if there was any digits r4=not zero
			mvi r7, #DIV1000
			//*On top of stack we have a 1 flag indicating that we had no digit on  left yet if we set it 0 then we have a digit on left. This is to  know if we want to pprint zero or nothing
			//r2 is r0 without it's 5th digit now, r4 is the 5th digit
			
DISPL5:		//We need to hold r0,r1=1,r2,r4,r6,r7
			
			sub r6, r1
			st r2, [r6] //put r2 on stack
			
			mvi	r2, #HEX_ADDRESS	// point to HEX displays
			add	r2, r1
			add	r2, r1
			add	r2, r1
			add	r2, r1//HEX4
			mvi r3, #DATA
			mv  r5, r7 //Check pointV
			add r3, r1 //0->1
			sub r4, r1 //
			mvnz r5, r7//Count r4 based on the condition to enter this function we know r4>1
			st r3, [r2]//Put it on hex
			
			ld r2, [r6]//load back r2 which is r0 without 5th digit
			add r6, r1
			
			st r4, [r6] //(r4=0) Now that we have a 5th digit we set the flag on top of the stack to 0
			mv r7, #DIV1000
						
DIV1000:
			mvi r4, #0
			mv  r5, r7 //R5 return pointV
			add r4, r1 //count our sub
			mvi r3, #1000
			sub r2, r3
			mvnc r7, r5// ^^^^^
			add r2, r3 //To restore it back to positive ***********
			sub r4, r1 //To reduce the extra count ***********
			mvi r7, #DISPL4//***********


DISPL4:		//We need to hold r0,r1=1,r2,r4,r6,r7
			mvi r3, #DISP4DOIT
			mv r4, r4
			mvnz r7, r3 //If it is not zero then display
			//If it is zero
			ld r5, [r6]//When flag is 0 we want to display
			mvi r3, #DIV100
			mv r5, r5
			mvnz r7, r3 //if  flag is not zero then skip
DISP4DOIT:		
			mvi r3, #DATA //++++
			mv  r5, r7 //Check pointV
			add r3, r1
			sub r4, r1
			mvnc r5, r7 //^^^^^
			add r4, r1 //Make it zero
			sub r3, r1 //Reduce the extra count r3, the data is in place
			
			sub r6, r1
			st r2, [r6] //Store r2
			mvi	r2, #HEX_ADDRESS	// point to HEX displays
			add	r2, r1
			add	r2, r1
			add	r2, r1//HEX3
			st r3, [r2]
			
			ld r2, [r6]
			add r6, r1
			
			st r4, [r6] //(r4=0) Now that we have a 5th digit we set the flag on top of the stack to 0
			mv r7, #DIV100

			
			
DIV100:
			mvi r4, #0
			mv  r5, r7 //R5 return pointV
			add r4, r1 //count our sub
			mvi r3, #100
			sub r2, r3
			mvnc r7, r5
			add r2, r3 //To restore it back to positive ***********
			sub r4, r1 //To reduce the extra count ***********
			mvi r7, #DISPL3//***********

DISPL3:		//We need to hold r0,r1=1,r2,r4,r6,r7
			mvi r3, #DISP3DOIT
			mv r4, r4
			mvnz r7, r3 //If it is not zero then display
			//If it is zero:
			ld r5, [r6]//When flag is 0 we want to display (r5 is flag)
			mvi r3, #DIV10
			mv r5, r5
			mvnz r7, r3 //if  flag is not zero then skip to next division
DISP3DOIT:		
			mvi r3, #DATA //++++
			mv  r5, r7 //Check pointV
			add r3, r1
			sub r4, r1
			mvnc r5, r7 //^^^^^
			add r4, r1 //Make it zero
			sub r3, r1 //Reduce the extra count r3, the data is in place
			
			sub r6, r1
			st r2, [r6] //Store r2
			mvi	r2, #HEX_ADDRESS	// point to HEX displays
			add	r2, r1
			add	r2, r1//HEX2
			st r3, [r2] //Store data in r3 in HEX
			
			ld r2, [r6]
			add r6, r1
			
			st r4, [r6] //(r4=0) Now that we have a 5th digit we set the flag on top of the stack to 0
			mv r7, #DIV10

			
DIV10:
			mvi r4, #0
			mv  r5, r7 //R5 return pointV
			add r4, r1 //count our sub
			mvi r3, #10
			sub r2, r3
			mvnc r7, r5
			add r2, r3 //To restore it back to positive ***********
			sub r4, r1 //To reduce the extra count ***********
			mvi r7, #DISPL2//***********

DISPL2:		//We need to hold r0,r1=1,r2,r4,r6,r7
			mvi r3, #DISP2DOIT
			mv r4, r4
			mvnz r7, r3 //If it is not zero then display
			//If it is zero:
			ld r5, [r6]//When flag is 0 we want to display (r5 is flag)
			mvi r3, #DISPL1
			mv r5, r5
			mvnz r7, r3 //if  flag is not zero then skip to displaying 1st digit
DISP2DOIT:		
			mvi r3, #DATA //++++
			mv  r5, r7 //Check pointV
			add r3, r1
			sub r4, r1
			mvnc r5, r7 //^^^^^^
			add r4, r1 //Make it zero
			sub r3, r1 //Reduce the extra count r3, the data is in place
			
			sub r6, r1
			st r2, [r6] //Store r2 on stack
			mvi	r2, #HEX_ADDRESS	// point to HEX displays
			add	r2, r1//HEX1
			st r3, [r2] //Store data in r3 in HEX
			
			ld r2, [r6]
			add r6, r1
			
			st r4, [r6] //(r4=0) Now that we have a 5th digit we set the flag on top of the stack to 0
			mv r7, #DISPL1		

DISPL1:		//Display r2
			mvi r3, #DATA //++++
			mv  r5, r7 //Check pointV
			add r3, r1
			sub r2, r1 //Count r2 instead
			mvnc r5, r7 //^^^^^^
			add r2, r1 //Make it zero
			sub r3, r1 //Reduce the extra count r3, the data is in place
			
			mvi	r2, #HEX_ADDRESS	// point to HEX displays
			st r3, [r2] //Store data in r3 in HEX
			
			add r6, r1 //Skip the stack for our flag
			
			ld r5,[r6]
			add r6, r1
			ld r4,[r6]
			add	r6, r1		
			ld r3,[r6]
			add r6, r1
			ld r2,[r6]
			add r6, r1
			
			add	r5, r1
			add	r5, r1
			mv	r7, r5
			
			
				
			
BLANK:		//Uses r0,r2 so they must be stacked
			sub r6, r1
			st	r0, [r6]
			sub r6, r1
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
			add r6,[r1]
			ld  r0,[r6]
			add r6,[r1]
			
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


