/* Program that counts consecutive 1's */

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
		beq END
		
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
		
		cmp r7, r8
		movlt r7, r8
		
		add R12, #4
		ldr r1, [R12]
		b LOOP
		
		
		
		  
		  
		  
		  
/*R10 to receive the input data and register
* R8 output number of consecutive 1's.
*/		  
ONEZEROS:           
          LSR     R2, R10, #1      // perform SHIFT, followed by AND
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
