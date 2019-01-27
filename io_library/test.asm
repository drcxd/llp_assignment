    global _start

    section .data
test_string:    db "0123456789", 0
buffer: db "0123456789", 0
input:  db "1234567890987654321hehehey", 0
    
    ;; number: db "+18446744073709551615", 0
    section .text

exit:
    mov rax, 60
    syscall
    ret

string_length:
    xor rax, rax
    .loop:
    cmp byte[rdi + rax], 0
    jz .end
    inc rax
    jmp .loop

    .end:
    ret

print_string:
    push rdi
    call string_length
    mov rdi, 1
    pop rsi
    mov rdx, rax
    mov rax, 1
    syscall
    ret

print_char:
    push di
    lea rdi, [rsp]
    call print_string
    pop di

    ret

print_newline:
    mov rdi, 0xA
    jmp print_char
    ret


print_int:
    ;; first the sign
    cmp rdi, 0
    jge .pos
    push rdi
    mov rdi, 0x2d
    call print_char
    pop rdi
    neg rdi

    .pos:
print_uint:
    test rdi, rdi
    jnz .not_zero
    mov rdi, 0x30
    call print_char
    ret

    .not_zero:   
    mov rax, rdi
    mov rcx, 10
    dec rsp
    mov byte[rsp], 0
    
    .loop:
    test rax, rax
    jz .end
    xor rdx, rdx
    div rcx
    dec rsp
    add rdx, 0x30
    ;; lea rdx, [test_string + rdx]
    ;; mov dl, byte [rdx]
    mov byte [rsp], dl
    jmp .loop

    .end:
    mov rdi, rsp
    call print_string

    mov rdi, rsp
    call string_length
    add rsp, rax
    inc rsp
    ret

read_char:
    dec rsp

    mov rax, 0
    mov rdi, 0
    mov rsi, rsp
    mov rdx, 1
    syscall

    test rax, rax
    jz .end
    mov al, byte [rsp]

    .end:
    inc rsp
    ret

read_word:
    push r12
    push r13
    push rbx                   

    mov r12, rdi                    ; buffer address
    mov r13, rsi                    ; size

    .skip_white:
    call read_char
    cmp rax, 0x20
    jz .skip_white
    cmp rax, 0x9
    jz .skip_white
    cmp rax, 0xa
    jz .skip_white

    ;; now rax holds the first non-white character
    xor rbx, rbx

    .loop:
    cmp rbx, r13    
    je .buffer_full

    cmp rax, 0x20
    jz .word_end
    cmp rax, 0x9
    jz .word_end
    cmp rax, 0xa
    jz .word_end
    cmp rax, 0xd
    jz .word_end
    test rax, rax
    jz .word_end

    lea rdx, [r12 + rbx]
    mov byte [rdx], al
    inc rbx
    call read_char
    jmp .loop

    .buffer_full:
    xor rax, rax
    jmp .end

    .word_end:
    lea rax, [r12 + rbx]
    mov byte [rax], 0
    mov rdx, rbx
    mov rax, r12

    .end:
    pop rbx
    pop r13
    pop r12
    ret

parse_uint:
    xor rax, rax
    xor rsi, rsi
    mov rcx, 10
    xor r8, r8

    .loop:
    mov sil, byte [rdi]
    test rsi, rsi
    jz .end
    cmp rsi, 0x30
    jl .end
    cmp rsi, 0x39
    jg .end
    
    sub rsi, 0x30
    mul rcx
    add rax, rsi
    inc rdi
    inc r8
    jmp .loop
    
    .end:
    mov rdx, r8
    ret

    ;; + 0x2B - 0x2D
parse_int:    
    cmp byte [rdi], 0x2d
    jnz .pos
    inc rdi
    call parse_uint
    neg rax
    inc r8
    ret    

    .pos:
    cmp byte [rdi], 0x2b
    jnz .no_sign
    inc rdi
    call parse_uint
    inc rdx
    ret

    .no_sign:
    call parse_uint
    ret

string_equals:
    xor rdx, rdx

    .loop:
    mov dl, byte[rsi]
    cmp dl, byte[rdi]
    je .char_equal
    jmp .not_equal

    .char_equal:
    cmp byte [rdi], 0
    je .equal
    inc rdi
    inc rsi
    jmp .loop

    .equal:
    mov rax, 1
    ret

    .not_equal:
    mov rax, 0
    ret

    ;; rdi string rsi buffer rdx buffer length
string_copy:

    push rsi
    .loop:
    test rdx, rdx
    jz .buffer_full
    mov al, byte [rdi]
    mov byte [rsi], al
    inc rdi
    inc rsi
    dec rdx
    cmp al, 0
    jz .end
    jmp .loop

    .buffer_full:
    pop rsi
    xor rax, rax
    ret

    .end:
    pop rax
    ret

_start:

    call print_newline
    call exit
