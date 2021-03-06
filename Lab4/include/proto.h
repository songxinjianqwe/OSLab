
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            proto.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifndef SLEEPINGBARBER_PROTO_H
#define SLEEPINGBARBER_PROTO_H

#include "semaphore.h"
/* klib.asm */
PUBLIC void	out_byte(u16 port, u8 value);
PUBLIC u8	in_byte(u16 port);
PUBLIC void	disp_str(char * info);
PUBLIC void	disp_color_str(char * info, int color);

/* protect.c */
PUBLIC void	init_prot();
PUBLIC u32	seg2phys(u16 seg);

/* klib.c */
PUBLIC void	delay(int time);

/* kernel.asm */
void restart();

/* main.c */
void Normal_Proc();
void Barber();
void Customer_C();
void Customer_D();
void Customer_E();
PUBLIC void clearScreen();

/* i8259.c */
PUBLIC void put_irq_handler(int irq, irq_handler handler);
PUBLIC void spurious_irq(int irq);

/* clock.c */
PUBLIC void clock_handler(int irq);


/* 以下是系统调用相关 */

/* proc.c */
PUBLIC  int      sys_get_ticks();        /* sys_call */
PUBLIC  void     sys_process_sleep(int mill_seconds);
PUBLIC  void     sys_disp_str_with_color(char * info,int color);
PUBLIC  void     sys_sem_p(Semaphore * semaphore);
PUBLIC  void     sys_sem_v(Semaphore * semaphore);


/* syscall.asm */
PUBLIC  void     sys_call();             /* int_handler */
PUBLIC  int      get_ticks();
PUBLIC  void     process_sleep(int mill_seconds);
PUBLIC  void     disp_str_with_color(char * info, int color);
PUBLIC  void     sem_p(Semaphore * semaphore);
PUBLIC  void     sem_v(Semaphore * semaphore);


#endif
