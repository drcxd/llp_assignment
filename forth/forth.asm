    global _start

    %include "util.inc"
    %include "macro.inc"

    %define pc r15
    %define w r14
    %define rstack r13

    %include "words.inc"

    section .bss
    resq 1023
rstack_start:   resq 1
input_buf:  resb 1024

    section .rodata
msg_no_such_word:   db ": no such word", 10, 0

    section .text
next:
    mov w, [pc]
    add pc, 8
    jmp [w]

_start:
    jmp i_init
