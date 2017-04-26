section .data
 
;1bh, "[0;31;40m Your message", 1bh, "[0;37;40m".
numberBegin:	db	1Bh, "["	; colour change sequence
atr:		db	"0;"  ;attribute of display mode
fg:		db	"00;" ; font color 
bg:		db	"47m"; background color
string:                      times 60 db 0
stringLength:        equ	$ - numberBegin ; control message length
color: dw "30"  ;store the base number of font color

section .text
global print
global printNumber

clearBuffer:
   pusha 
   mov ecx,59
   mov esi,0
mainClear:
    mov byte [string+esi],0
    inc esi
    loop  mainClear
    popa
    ret 
    
print:
    call clearBuffer
    mov ebx,[esp+4] ; parameter1 ,color
    mov esi,[esp+8] ; parameter2 ,string address
    
    mov ax,[color] ;bx store the base color 
    add ah,bl ;add the increment 
    mov [fg],ax ;store color
    mov eax,0 
    mov ebx,0
    
;transfer parameter string to data area
storeStr:    
    cmp byte [esi+eax],0
    je write
    mov byte bl,[esi+eax]
    mov byte [string+eax],bl
    inc eax
    jmp storeStr
    
write:    
    mov eax, 4
    mov ebx, 1
    mov ecx,numberBegin
    mov edx, stringLength
    int 80h      
    ret

;-------------------------------------------------------------------------------


printNumber:
    call clearBuffer
    mov ebx,[esp+4] ; parameter1 ,color
    mov esi,[esp+8] ; parameter2 ,string address
    
    mov ax,[color] ;bx store the base color 
    add ah,bl ;add the increment 
    mov [fg],ax ;store color
    
    
    mov eax,esi ;store the low 32 bits of dividend
    mov esi,0  ;counter ,count how many single numbers 
    mov edi,0  ;counter , count the index of temporary stored numbers
    mov edx,0 ;store the high 32 bits of dividend
    mov ebx,10 
    
getNumber:
    div ebx 
    push edx ; push remainder to the stack
    inc esi
    cmp eax,0 ;finish and call storeNumber
    je storeNumber
    mov edx,0
    jmp getNumber  ;repeat divide 
    
;write every single number to memory    
storeNumber:
   pop ecx ; get every number in right order
   add ecx,0x30 ; transfer to ascii number 
   mov byte [string+edi],cl ;store in memory 
   inc edi
   dec esi  ;see if number stack is clear
   cmp esi,0
   je write ;output colored numbers
   jmp storeNumber 
   
