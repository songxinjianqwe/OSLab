org 0x7c00 ;the start memory address of boot code 
mov ax,cs 
mov ds,ax  ;ds = cs
mov es,ax  ;es = cs
call showStr
jmp $ ;loop forever 
showStr:
    mov ax,Hello 
    mov bp,ax ;es:bp = address of "Hello" string to be displayed
    mov cx,16 ;cx = the length of string
    mov ax,0x1301 ;AH = 13h ,store the function number ; AL = 01h cursor follows the characters 
    mov bx,0x00c ;page number = 0 (BH)  BL = 0ch(color)
    mov dl,0   ; start column 
    int 10h  ;call 10th system call
    ret
Hello: db "Hello OS!"
times 510-($-$$) db 0  ;padding 
dw 0xaa55 ; the end of boot sector