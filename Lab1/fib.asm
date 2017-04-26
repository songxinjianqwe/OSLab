section .data

f0: db 1
f1: db 1
length: dd 0
indexs: times 40 db 0
raw_nums: times 40 db 0
newLine: dw 10  

;1bh, "[0;31;40m Your message", 1bh, "[0;37;40m".
numberBegin:	db	1Bh, "["	; colour change sequence
atr:		db	"0;"  ;attribute of display mode
fg:		db	"00;" ; font color 
bg:		db	"47m"; background color
numbers:	times 30 db	0 ; numbers to print
controlLength:     equ	$ - numberBegin -30 ; control message length
color: dw "30"  ;store the base number of font color

section .bss
var: resd 1


section .text
global main
main:
    mov ebp, esp; for correct debugging
begin:    
    call input
    
    call fib
    
    jmp begin
    ret
    
;require that the last character can not be space,must be a number 
;require that every input number must be separated by one space
input:
    pusha
    
    mov eax,3 ; read a raw string from ccnsole
    mov ebx,0
    mov ecx,raw_nums
    mov edx,40
    int 80h
    
    mov eax,raw_nums
    mov ecx,eax ;ecx store the address of raw numbers
    mov eax,indexs 
    mov edx,eax ;edx store the address of parsed numbers
    
    mov esi,0 ;esi store the offset of raw numbers 
    mov edi,0 ;edi store the offset of indexs
    mov eax,0 ; accumulator,store the whole number
    mov ebx,0 ; temporary var,store each byte from raw numbers
  
parseInput:
   
    mov bl,byte[ecx+esi] 
    inc esi 
    cmp bl,0x0a ;compare bl and end of input
    je storeNum ; if read complete,then store the last number and exit
    cmp bl,0x20 ;compare bl and space 
    jne processNum;if bl != space, then parse and accumulate the input number
    jmp storeNum  ; if bl == space,then finish read a number and store  al value in memory
    
    
processNum:
    push ecx
    sub bl,0x30
    mov cl,10
    mul cl ; al = al * 10
    add al,bl ; al += bl
    pop ecx
    jmp parseInput
    
; store number and exit | exit directly
storeNum:
    
    mov byte [edx+edi],al 
    inc edi
    mov al,0  ;clear accumulator
    cmp bl,0x0a  ;if current single number  is zero, then exit 
    je exitInput
    jmp parseInput
    
exitInput:
    mov dword [length],edi ;store the length of chars has been read  
    popa
    ret

;Fibonacci function
;maximum n is about 46 
fib:
    pusha
    
    mov eax,0 
    mov ebx,0
    mov edx,0
    mov ecx,0  ;eax,ebx,ecx are used for temp vars
    mov dl,-1 ;dl store  current loop times of fib
    mov esi,0 ; esi store the offset of indexs

loopFib:
    inc dl
    cmp dl,2
    jng preprocess
    jmp normal

; check if input is 0 or 1   
preprocess:    
    cmp byte[indexs+edx] ,2
    jl checkZeroAndOne ;if 0 or 1 ,then output 1
    jnl resetAB ; if >= 2 ,then set eax  = f0 and ebx = f1
    
 ;if n>=2 ,  then go to normal
 normal:
    add eax,ebx  ; eax = eax + ebx     eax = a + b
    mov ecx,eax ; ecx = eax         ecx = c = a + b
    mov eax,ebx ; eax = ebx        eax = b
    mov ebx,ecx ; ebx = ecx        ebx =  c
    cmp dl,byte[indexs+esi] 
    je output
    jmp loopFib
    
checkZeroAndOne:
    mov ecx,1
    jmp output
    
resetAB:    
    mov al,[f0]    ; eax = a
    mov bl,[f1]    ; ebx = b
    mov dl,2
    jmp normal
                          
output: ; output the number stored in ecx
    inc esi
    jmp displayResult
    
returnToOutput:    
    cmp esi,dword[length]
    je exitFib
    jmp loopFib
   
exitFib:
    popa
    ret   

displayResult:
    pusha
    mov esi,0  ;counter ,count how many single numbers 
    mov edi,0  ;counter , count the index of temporary stored numbers
    mov edx,0 ;store the high 32 bits of dividend
    mov eax,ecx ;store the low 32 bits of dividend
    mov ebx,10 
    
getNum:
    div ebx 
    push edx ; push remainder to the stack
    inc esi
    cmp eax,0 ;finish and call writeNumbers
    je writeNumbers
    mov edx,0
    jmp getNum  ;repeat divide 
    

;write every single number to console    
writeNumbers:
   pop ecx ; get every number in right order
   add ecx,0x30 ; transfer to ascii number 
   mov [numbers+edi],ecx ;store in memory 
   inc edi ; counter of numbers
   dec esi  ;see if number stack is clear
   cmp esi,0
   je writeColored ;output colored numbers
   jmp writeNumbers 

      
;write the whole number   
writeColored:    
  
    pusha
    mov ax,[color] ;change the font color
    add ah,1
    cmp ah,"7" ;if color is used up,then reset to the original color
    je resetColor
    
writeNormal:
    mov [color],ax ;set the font color
    mov [fg],ax 
    mov eax, 4
    mov ebx, 1
    mov ecx, numberBegin
    mov edx, controlLength
    add edx,edi ; get the control length and numbers length
    int 80h 
    popa
    mov edi,0
    jmp startNewLine

resetColor:  ;return to the original color
    mov ah,"0"
    jmp writeNormal
;write a new line

startNewLine:
    mov eax, 4
    mov ebx, 1
    mov ecx, newLine
    mov edx, 1
    int 80h      
    popa
    jmp returnToOutput
      