/* Program that counts consecutive 1's */

          .text                   // executable code follows
          .global _start                  
_start:                             
          MOV     R5, #TEST_NUM   // load the data word ...
          ldr     R1, [R5]        // into R1
		
          MOV     R0, #0          
		  mov r4, #0			// R4 will hold the result
		  
LOOP:	
		movs r1, r1
		beq END
		mov r0, #0
		bl ONES
		
		cmp r4, r0
		movlt r4, r0
		add R5, #4
		ldr r1, [R5]
		b LOOP
		
		
		
		  
		  
		  
		  
/*R1 to receive the input data and register
* R0 output number of consecutive 1's.
*/		  
ONES:     CMP     R1, #0          // loop until the data contains no more 1's
          moveq	  pc, lr             
          LSR     R2, R1, #1      // perform SHIFT, followed by AND
          AND     R1, R1, R2      
          ADD     R0, #1          // count the string length so far
          B       ONES            

		  
		  
END:      B       END    


         

TEST_NUM: 	.word 0x103fe00f 
			.word 0xffffffff
			.word 0xaaaaaaaa
			.word 0xbbbbbbbb
			.word 0x11111111
			.word 0xabcdef10
			.word 0x01234567
			.word 0x56789abc
			.word 0xafafafaf
			.word 0x00000000

          .end                            
