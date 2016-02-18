cpu 8086
org 0x7c00

repz shl word [es:bx+si], 1
repnz shr byte [0xfad], cl

and word [ss:bx], 0xf3

times 510 - ($-$$) db 0
dw 0xAA55
