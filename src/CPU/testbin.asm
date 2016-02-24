cpu 8086
org 0x7c00



mov ax, 23

times 510 - ($-$$) db 0
dw 0xAA55
