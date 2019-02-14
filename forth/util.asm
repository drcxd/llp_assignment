    global find_word
    global cfa
    
    section .text
    ;; rdi key string
    ;; rsi last word address
find_word:
    push r12
    push r13
    mov r12, rsi
    mov r13, rdi
    
    .loop:
    test r12, r12
    jz .not_found

    lea rsi, [r12 + 8]
    mov rdi, r13
    call string_equals

    cmp rax, 1
    jz .found
    mov r12, [r12]
    jmp .loop

    .found:
    mov rax, r12
    pop r13
    pop r12
    ret

    .not_found:
    xor rax, rax
    pop r13
    pop r12
    ret

    ;; rdi word header start
cfa:
    add rdi, 8
    push rdi
    call string_length
    pop rdi

    lea rdi, [rdi + rax + 2]
    mov rax, rdi
    ret
