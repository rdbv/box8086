cpu 8086

jmp main

print_string:
    _loop:
        mov al, [si]
        cmp al, 0x00
        je _done
        mov ah, 0xe
        int 0x10
        jmp _loop
    _done:
        ret



main:
    mov si, hello
    call print_string
    cli
    hlt


hello: db "Hello!", 0x00
nop


