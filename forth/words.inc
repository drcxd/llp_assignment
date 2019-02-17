    section .rodata
interpreter_stub:   dq xt_interpreter

    section .data
stack_base: dq 0
    native "init", init
    mov rstack, rstack_start
    mov pc, interpreter_stub
    cmp qword[stack_base], 0
    je .first
    mov rsp, [stack_base]
    jmp next
    .first:
    mov [stack_base], rsp
    jmp next

    colon "interpreter", interpreter
    .start:
    
