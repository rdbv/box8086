cpu 8086
org 0x7c00

repnz cmp ax, 0xe183


times 510 - ($-$$) db 0
dw 0xAA55
