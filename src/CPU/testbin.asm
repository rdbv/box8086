cpu 8086
org 0x7c00

add ax, 0xface


times 510 - ($-$$) db 0
dw 0xAA55
