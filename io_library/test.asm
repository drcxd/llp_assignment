    global _start
    
    section .data
test_string:    db "1234567890", 0
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

_start:
    mov rdi, 0x41
    call print_char
    
    mov rdi, rax
    call exit
