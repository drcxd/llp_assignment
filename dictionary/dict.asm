    extern string_equals
    global find_word
    
    section .text

    ;; rdi key string
    ;; rsi last word address
find_word:
    push r12
    mov r12, rsi
    .loop:
    test r12, r12
    jz .not_found

    lea rsi, [r12 + 8]          ; rsi point to the key string now
    call string_equals          ; rax holds the string length

    cmp rax, 1
    jz .found
    mov r12, [r12]
    jmp .loop

    .found:
    mov rax, r12
    pop r12
    ret

    .not_found:
    xor rax, rax
    pop r12
    ret

