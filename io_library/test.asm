    global _start
    
    section .data
test_string:    db "0123456789", 0
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
    call print_char
    ret

print_uint:
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
    lea rdx, [test_string + rdx]
    mov dl, byte [rdx]
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
    

_start:
    mov rdi, 12345
    call print_uint
    
    mov rdi, rax
    call exit
