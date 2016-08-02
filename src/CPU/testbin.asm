cpu 8086
org 0x7c00

int 0x32

times 510 - ($-$$) db 0
dw 0xAA55
