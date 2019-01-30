    global _start
    extern find_word
    extern read_word
    extern print_string
    extern string_length
    extern exit
    
    %include "words.inc"
    
    section .data
error_message:   db "cannot find the key", 0
    
    section .text

_start:
    sub rsp, 256
    mov rdi, rsp
    ;; push rdi
    mov rsi, 256
    call read_word
    ;; now rax holds the pointer to the input key
    
    mov rdi, rax
    mov rsi, last
    call find_word

    test rax, rax
    jz .error
    add rax, 8                  ; now rax points to the key string
    mov rbx, rax
    mov rdi, rbx
    call string_length          ; now rax holds the length of the key string

    lea rdi, [rbx + rax + 1]    ; now rdi points to the value string
    call print_string
    add rsp, 256
    call exit

    .error:
    mov rdi, error_message
    call print_string
    add rsp, 256
    call exit
