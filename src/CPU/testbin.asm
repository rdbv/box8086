cpu 8086
org 0x7c00

mov al, 1

lp:
    add bl, al
    jmp lp


times 510 - ($-$$) db 0
dw 0xAA55
