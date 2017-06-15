
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                               syscall.asm
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                                                     Forrest Yu, 2005
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

%include "sconst.inc"

_NR_get_ticks       equ 0 ; 要跟 global.c 中 sys_call_table 的定义相对应！
INT_VECTOR_SYS_CALL equ 0x90
_NR_process_sleep      equ 1 ; 要跟 global.c 中 sys_call_table 的定义相对应！
_NR_disp_str_with_color      equ 2 ; 要跟 global.c 中 sys_call_table 的定义相对应！
_NR_sem_p    equ 3 ; 要跟 global.c 中 sys_call_table 的定义相对应！
_NR_sem_v      equ 4 ; 要跟 global.c 中 sys_call_table 的定义相对应！

; 导出符号
global	get_ticks
global  process_sleep
global  disp_str_with_color
global sem_p
global sem_v

bits 32
[section .text]

; ====================================================================
;                              get_ticks
; ====================================================================
get_ticks:
	mov	eax, _NR_get_ticks
	int	INT_VECTOR_SYS_CALL
	ret

; ====================================================================
;                              process_sleep
; ====================================================================
process_sleep:
    mov eax,_NR_process_sleep
    mov ebx, [esp + 4]
    int INT_VECTOR_SYS_CALL
    ret

; ====================================================================
;                              disp_str_with_color
; ====================================================================
disp_str_with_color:
    mov eax,_NR_disp_str_with_color
    mov ebx, [esp + 4]
    mov ecx, [esp + 8]
    int INT_VECTOR_SYS_CALL
    ret
; ====================================================================
;                              disp_str_with_color
; ====================================================================
sem_p:
   mov eax,_NR_sem_p
   mov ebx, [esp + 4]
   int INT_VECTOR_SYS_CALL
   ret

; ====================================================================
;                              disp_str_with_color
; ====================================================================
sem_v:
   mov eax,_NR_sem_v
   mov ebx, [esp + 4]
   int INT_VECTOR_SYS_CALL
   ret

