    global _start

    %define pc r15
    %define w r14
    %define rstack r13

    section .bss
rstack_start:   resq 65536
input_buf:  resb 1024

    section .data
program_stub:    dq 0
xt_interpreter: dq .interpreter
    .interpreter: dq interpreter_loop

    section .text

interpreter_loop:
    
