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
    mov rsi, last
    .loop:
    test rsi, rsi
    jz .not_found
    mov rdi, [rsp]
    push rsi
    add rsi, 8    
    call string_equals
    pop rsi
    test rax, rax
    jnz .found
    mov rsi, [rsi]
    jmp .loop

    .not_found:
    mov qword [rsp], 0
    push 0
    jmp next

    .found:
    mov [rsp], rsi
    jmp next

    native "docol", docol
    rpush pc
    add w, 8
    mov pc, w
    jmp next

    native "dup", dup
    push qword [rsp]
    jmp next

    native "0branch", branch0
    pop rax
    test rax, rax
    jz .skip
    add pc, [pc]
    .skip:
    add pc, 8
    jmp next

    native "branch", branch
    add pc, [pc]
    add pc, 8
    jmp next

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

    native "cfa", cfa
    pop rsi
    add rsi, 8
    .loop:
    mov al, [rsi]
    test al, al
    jz .end
    inc rsi
    jmp .loop

    .end:
    add rsi, 2
    push rsi
    jmp next

    native "execute", execute
    pop rax
    mov w, rax
    jmp [rax]
    

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
