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
/* Set up stack pointers for IRQ and SVC processor modes */
MOV      R0, #0b11010010    //IRQ mode bits
MSR      CPSR_c, R0        //change to IRQ mode
LDRB     SP, =0x20000    //set SP
MOV      R0, #0b11010011    //SVC mode bits
MSR      CPSR_c, R0
LDR      SP, =0x3FFFFFFC


//Code supplied for CONFIG_GIC
//BL       CONFIG_GIC      // configure the ARM generic
                         // interrupt controller
/* Configure the KEY pushbuttons port to generate interrupts */

LDR      R0, =0xFF200000 //base address for interrupts
MOV      R7, #0b1111
STR      R7, [R0, #0x58]          //set interrupt mask to 0b1111

/* Enable IRQ interrupts in the ARM processor */
BL       CONFIG_GIC
MOV      R0, #0b01010010    //IRQ mode bits
MSR      CPSR_c, R0        //change to IRQ mode/enable interrupt


IDLE:
B        IDLE            // main program simply idles

/* Define the exception service routines */
KEY_ISR:
LDR      R0, =0xFF200000 //base address for interrupts
LDR      R2, [R0, #0x5C]   //read edge capture register
STR      R2, [R0, #0x5C]   //clear the interrupt
LDR      R0, =KEYPAST
LDR      R1, [R0]
EOR      R2, R1, R2
STR      R2, [R0]

LDR      R0, =0xFF200020 //HEX3-0
MOV      R1, #0

CHK_KEY3:
TST      R2, #0b1000      //KEY3 pressed?
BEQ      CHK_KEY2

MOV      R3, #0b01001111
ORR      R1, R1, R3, LSL #24

CHK_KEY2:
TST      R2, #0b0100      //KEY2 pressed?
BEQ      CHK_KEY1
MOV      R3, #0b01011011
ORR      R1, R1, R3, LSL #16

CHK_KEY1:
TST      R2, #0b0010      //KEY1 pressed?
BEQ      CHK_KEY0
MOV      R3, #0b00000110
ORR      R1, R1, R3, LSL #8

CHK_KEY0:
TST      R2, #0b0001      //KEY0 pressed?
BEQ      END_KEY_ISR
MOV      R3, #0b00111111
ORR      R1, R1, R3


END_KEY_ISR:
STR      R1, [R0]
MOV      PC, LR

SERVICE_IRQ:    PUSH     {R0-R7, LR}
LDR      R4, =0xFFFEC100 // GIC CPU interface base address
LDR      R5, [R4, #0x0C] // read the ICCIAR in the CPU
                         // interface

KEYS_HANDLER:
CMP      R5, #73         // check the interrupt ID

UNEXPECTED:     BNE      UNEXPECTED      // if not recognized, stop here
BL       KEY_ISR

EXIT_IRQ:       STR      R5, [R4, #0x10] // write to the End of Interrupt
                         // Register (ICCEOIR)
POP      {R0-R7, LR}
SUBS     PC, LR, #4      // return from exception

KEYPAST:    .byte 0b00000000
            .skip 3

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
					
