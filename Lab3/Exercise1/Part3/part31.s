/* Program that finds the largest number in a list of integers	*/

            .text                   // executable code follows
            .global _start                  
_start:                             
            MOV     R4, #RESULT     // R4 points to result location
            LDR     R0, [R4, #4]    // R0 holds the number of elements in the list
            MOV     R1, #NUMBERS    // R1 points to the start of the list
            BL      LARGE           
            STR     R0, [R4]        // R0 holds the subroutine return value
			st		r0, #ff200020

END:        B       END             

/* Subroutine to find the largest integer in a list
 * Parameters: R0 has the number of elements in the lisst
 *             R1 has the address of the start of the list
 * Returns: R0 returns the largest item in the list
 */
LARGE:      
			mov 	r6, #4 //increments memory to traverse the list
			ldr 	r7, [r1]	//R7 holds max val
			subs 	r0, #1 //already did one of the checkings

LOOP:		ldr 	r5, [r1, r6]  //loads into r5 the value we are comparing
			cmp 	r7, r5
			movlt 	r7, r5
			add 	r6, #4
			subs 	r0, #1 //to see how many more times we have to go through this loop
			bne 	LOOP //If  r0 is zero then we dont loop anymore
			mov		r0, r7
			mov 	pc, lr

RESULT:     .word   0           
N:          .word   7           // number of entries in the list
NUMBERS:    .word   4, 5, 3, 6  // the data
            .word   1, 8, 2                 

            .end                            

