cpu 8086
org 0x7c00

mov [0xfade], bx
mov [ss:0x03ac], dx

mov [es:bx], cx
mov [es:bx+si], dx
mov byte [es:si], dl


times 510 - ($-$$) db 0
dw 0xAA55
