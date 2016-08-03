cpu 8086
org 0x7c00

mov al, [nap0]

mov ax, 0x23

nap0: db "SPERMA"

times 510 - ($-$$) db 0
dw 0xAA55
