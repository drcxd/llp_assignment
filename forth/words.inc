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
    mov rsi, [last_word]
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
    jnz .skip
    add pc, [pc]
    .skip:
    add pc, 8
    jmp next

    native "branch", branch
    add pc, [pc]
    add pc, 8
    jmp next

    native ".S", show_stack
    mov rcx, rsp
    .loop:
    cmp rcx, [stack_base]
    jz next
    mov rdi, [rcx]
    push rcx
    call print_int
    call print_newline
    pop rcx
    add rcx, 8
    jmp .loop

    native "+", plus
    pop rax
    add [rsp], rax
    jmp next

    native "-", minus
    pop rax
    sub [rsp], rax
    jmp next

    native "*", mul
    pop rcx
    pop rax
    imul rcx
    push rax
    jmp next

    native "/", div
    pop rcx
    pop rax
    cqo                         ; signed extend of rax
    idiv rcx
    push rax
    jmp next

    native "%", mod
    pop rcx
    pop rax
    cqo
    idiv rcx
    push rdx
    jmp next

    native "=", equals
    pop rax
    pop rcx
    test rax, rcx
    sete al
    movzx rax, al
    push rax
    jmp next

    native "<", less
    pop rax
    pop rcx
    cmp rcx, rax
    setb al
    movzx rax, al
    push rax
    jmp next

    native "not", not
    pop rax
    test rax, rax
    setz al
    movzx rax, al
    push rax

    native "and", and
    pop rax
    pop rcx
    test rax, rax
    jz .zero
    test rcx, rcx
    jz .zero
    mov rax, 1
    push rax
    jmp next
    .zero:
    xor rax, rax
    push rax
    jmp next

    native "rot", rot
    pop rax
    pop rcx
    pop rdx
    push rcx
    push rax
    push rdx
    jmp next

    native "swap", swap
    pop rax
    pop rcx
    push rax
    push rcx
    jmp next

    native ".", dot
    pop rdi
    call print_int
    call print_newline
    jmp next

    native "key", key
    call read_char
    push rax
    jmp next

    native "emit", emit
    pop rdi
    call print_char
    ;; call print_newline
    jmp next

    native "mem", mem
    push qword user_mem
    jmp next

    native "!", write
    pop rax
    pop rcx
    mov [rcx], rax
    jmp next

    native "@", fetch
    pop rax
    push qword [rax]
    jmp next

    native "c@", fetch_char
    pop rax
    movzx rax, byte [rax]
    push rax
    jmp next

    native "here", here
    push qword here
    jmp next

    native "create", create
    mov rax, [last_word]
    mov rsi, [here]
    mov [rsi], rax
    add rsi, 8

    pop rdi
    push rsi
    call string_copy
    pop rsi
    push rsi
    mov rdi, rsi
    call string_length
    pop rsi
    add rsi, rax

    inc rsi
    pop rax
    mov [rsi], al
    inc rsi

    mov [here], rsi
    jmp next

    native "state", state
    push qword state
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

    native "number", number
    pop rdi
    call parse_int
    push rax
    push rdx
    jmp next

    native "prints", prints
    pop rdi
    call print_string
    jmp next

    native "lit", lit
    push qword [pc]
    add pc, 8
    jmp next

    native "bye", bye
    mov rax, 60
    xor rdi, rdi
    syscall

    native "exit", exit
    rpop pc
    jmp next

    native ",", comma
    mov rax, [here]
    pop qword [rax]
    add qword [here], 8
    jmp next

    colon ":", colon
    .restart:
    dq xt_inbuf, xt_word
    branch0 .restart
    dq xt_lit, 0, xt_inbuf, xt_create
    dq xt_state, xt_lit, 1, xt_write
    dq xt_lit, i_docol, xt_comma
    dq xt_exit

    colon ";", semicolon, 1
    dq xt_state, xt_lit, 0, xt_write
    dq xt_lit, xt_exit, xt_comma
    dq xt_exit
    

    colon "interpreter", interpreter
    .start:
    dq xt_inbuf, xt_word
    branch0 .end_of_input

    dq xt_inbuf, xt_find

    dq xt_dup
    branch0 .number
    dq xt_cfa

    dq xt_state, xt_fetch
    branch0 .interpreting

    dq xt_dup
    dq xt_lit, 1, xt_minus, xt_fetch_char, xt_not
    branch0 .interpreting

    dq xt_comma
    branch .start

    .interpreting:
    dq xt_execute
    branch .start

    .number:
    dq xt_drop
    dq xt_drop
    dq xt_inbuf
    dq xt_number
    branch0 .no_word

    dq xt_state, xt_fetch
    branch0 .start

    dq xt_here, xt_lit, 8, xt_minus
    dq xt_lit, xt_branch0
    dq xt_equals, xt_not
    branch0 .is_branch

    dq xt_here, xt_lit, 8, xt_minus
    dq xt_lit, xt_branch
    dq xt_equals, xt_not
    branch0 .is_branch

    dq xt_lit, xt_lit, xt_comma

    .is_branch:
    dq xt_comma
    branch .start

    .no_word:
    dq xt_drop, xt_inbuf, xt_prints
    dq xt_lit, msg_no_such_word, xt_prints
    branch .start

    .end_of_input:
    dq xt_bye
    dq xt_exit
