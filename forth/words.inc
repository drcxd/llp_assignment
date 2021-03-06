    native "drop", drop
    add rsp, 8
    jmp next

    native "swap", swap
    pop rax
    pop rcx
    push rax
    push rcx
    jmp next

    native "dup", dup
    push qword [rsp]
    jmp next

    native "rot", rot
    pop rax
    pop rcx
    pop rdx
    push rcx
    push rax
    push rdx
    jmp next    

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
    cmp rax, rcx
    sete al
    movzx rax, al
    push rax
    jmp next

    ;; native "<", less
    ;; pop rax
    ;; pop rcx
    ;; cmp rcx, rax
    ;; setb al
    ;; movzx rax, al
    ;; push rax
    ;; jmp next

    native "not", not
    pop rax
    test rax, rax
    setz al
    movzx rax, al
    push rax
    jmp next

    native "count", count
    pop rdi
    call string_length
    push rax
    jmp next

    native ".", dot
    pop rdi
    call print_int
    call print_newline
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

    native "docol", docol
    rpush pc
    add w, 8
    mov pc, w
    jmp next

    native "exit", exit
    rpop pc
    jmp next

    native ">r", to_r
    pop rax
    rpush rax
    jmp next

    native "r>", from_r
    rpop rax
    push rax
    jmp next

    native "r@", r_fetch
    push qword [rstack]    
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

    native "emit", emit
    pop rdi
    call print_char
    ;; call print_newline
    jmp next

    native "word", word
    pop rdi
    mov rsi, 1024
    call read_word
    push rdx
    jmp next

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

    native "bye", bye
    mov rax, 60
    xor rdi, rdi
    syscall

    native "syscall", syscall
    pop r9
    pop r8
    pop r10
    pop rdx
    pop rsi
    pop rdi
    pop rax
    syscall
    push rax
    jmp next

    native "branch", branch
    add pc, [pc]
    add pc, 8
    jmp next

    native "0branch", branch0
    pop rax
    test rax, rax
    jnz .skip
    add pc, [pc]
    .skip:
    add pc, 8
    jmp next

    native "lit", lit
    push qword [pc]
    add pc, 8
    jmp next
    
    native "inbuf", inbuf
    push qword input_buf
    jmp next

    native "mem", mem
    push qword user_mem
    jmp next

    native "last_word", last_word
    push qword last_word
    jmp next

    native "state", state
    push qword state
    jmp next        

    native "here", here
    push qword here
    jmp next

    native "execute", execute
    pop rax
    mov w, rax
    jmp [rax]

    native "@", fetch
    pop rax
    push qword [rax]
    jmp next
    
    native "!", write
    pop rax
    pop rcx
    mov [rcx], rax
    jmp next    

    native "@c", fetch_char
    pop rax
    movzx rax, byte [rax]
    push rax
    jmp next
    
    native ",", comma
    mov rax, [here]
    pop qword [rax]
    add qword [here], 8
    jmp next

    native "c,", char_comma
    mov rax, [here]
    pop rdx
    mov byte [rax], dl
    add qword [here], 1
    jmp next

    native "create", create
    mov rax, [last_word]
    mov rsi, [here]
    mov [rsi], rax
    mov [last_word], rsi
    add rsi, 8

    pop rdi
    push rsi
    mov rdx, 1024
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

    ;; native "and", and
    ;; pop rax
    ;; pop rcx
    ;; test rax, rax
    ;; jz .zero
    ;; test rcx, rcx
    ;; jz .zero
    ;; mov rax, 1
    ;; push rax
    ;; jmp next
    ;; .zero:
    ;; xor rax, rax
    ;; push rax
    ;; jmp next    

    ;; native "key", key
    ;; call read_char
    ;; push rax
    ;; jmp next    

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

    ;; (a b -- s)
    ;; s = 0 if a % b ==0 
    ;; s = 1 if a % b != 0
    colon "divisible", divisible
    dq xt_mod, xt_not, xt_exit

    ;; (num -- r)
    ;; r = 1 if num is prime number
    ;; else r = 0
    colon "is_prime", is_prime
    dq xt_dup, xt_lit, 2, xt_div
    dq xt_to_r,
    .loop:
    dq xt_dup, xt_from_r, xt_dup, xt_lit, 1, xt_minus, xt_dup
    dq xt_lit, 0, xt_equals, xt_not
    branch0 .is_prime

    dq xt_to_r, xt_mod, xt_lit, 0, xt_equals, xt_not
    branch0 .not_prime
    branch .loop

    .is_prime:
    dq xt_drop, xt_drop, xt_drop, xt_drop, xt_lit, 1, xt_exit

    .not_prime:
    dq xt_from_r, xt_drop, xt_drop, xt_lit, 0, xt_exit

    ;; (n -- )
    ;; out put the first n fibonacci numbers 1 1 2 3 5 ...
    colon "fib", fib
    dq xt_dup, xt_lit, 1, xt_equals, xt_not
    branch0 .one    

    dq xt_dup, xt_lit, 2, xt_equals, xt_not
    branch0 .two

    dq xt_lit, 1, xt_dot, xt_lit, 1, xt_dot
    dq xt_lit, 2, xt_minus, xt_to_r, xt_lit, 1, xt_lit, 1
    .loop:
    dq xt_dup, xt_to_r, xt_plus, xt_dup, xt_dot
    dq xt_from_r, xt_from_r
    dq xt_lit, 1, xt_minus, xt_dup, xt_lit, 0, xt_equals, xt_not
    branch0 .finish
    dq xt_to_r, xt_swap
    branch .loop

    .finish:
    dq xt_drop, xt_drop, xt_drop, xt_exit

    .one:
    dq xt_drop, xt_lit, 1, xt_dot, xt_exit

    .two:
    dq xt_drop, xt_lit, 1, xt_dot, xt_lit, 1, xt_dot, xt_exit

    colon "hello", hello
    dq xt_lit, 1, xt_lit, 1, xt_lit, hello_world, xt_lit, 13
    dq xt_lit, 0, xt_lit, 0, xt_lit, 0
    dq xt_syscall, xt_exit
    

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
