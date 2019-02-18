    native "drop", drop
    add rsp, 8
    jmp next

    native "inbuf", inbuf
    push qword input_buf
    jmp next

    native "word", word
    pop rdi
    mov rsi, 1024
    call read_word
    push rdx
    jmp next

    native "find", find
    


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
    dq xt_inbuf, xt_word
    branch0 .end_of_input

    dq xt_inbuf, xt_find

    dq xt_dup
    branch0 .number
    dq xt_cfa
    dq xt_execute
    branch .start
    
    .number:
    dq xt_drop
    dq xt_drop
    dq xt_inbuf
    dq xt_number
    branch0 .no_word
    branch .start

    .no_word:
    dq xt_drop, xt_inbuf, xt_prints
    dq xt_lit, msg_no_such_word, xt_prints
    branch .start

    .end_of_input:
    dq xt_bye
    dq xt_exit
