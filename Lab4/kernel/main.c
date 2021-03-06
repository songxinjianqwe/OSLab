
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"
#include "semaphore.h"

PRIVATE void setCursor();
PRIVATE void initProcess();
void barber();
void customer(int customer_id,int color);
void cut_hair();
void get_haircut(int color);
/*======================================================================*
                            kernel_main
 *======================================================================*/
PUBLIC int kernel_main()
{
	disp_str("-----\"kernel_main\" begins-----\n");

	TASK*		p_task		= task_table;
	PROCESS*	p_proc		= proc_table;
	char*		p_task_stack	= task_stack + STACK_SIZE_TOTAL;
	u16		selector_ldt	= SELECTOR_LDT_FIRST;
	int i;
	for (i = 0; i < NR_TASKS; i++) {
		strcpy(p_proc->p_name, p_task->name);	// name of the process
		p_proc->pid = i;			// pid

		p_proc->ldt_sel = selector_ldt;

		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3],
		       sizeof(DESCRIPTOR));
		p_proc->ldts[0].attr1 = DA_C | PRIVILEGE_TASK << 5;
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3],
		       sizeof(DESCRIPTOR));
		p_proc->ldts[1].attr1 = DA_DRW | PRIVILEGE_TASK << 5;
		p_proc->regs.cs	= ((8 * 0) & SA_RPL_MASK & SA_TI_MASK)
			| SA_TIL | RPL_TASK;
		p_proc->regs.ds	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
			| SA_TIL | RPL_TASK;
		p_proc->regs.es	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
			| SA_TIL | RPL_TASK;
		p_proc->regs.fs	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
			| SA_TIL | RPL_TASK;
		p_proc->regs.ss	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
			| SA_TIL | RPL_TASK;
		p_proc->regs.gs	= (SELECTOR_KERNEL_GS & SA_RPL_MASK)
			| RPL_TASK;

		p_proc->regs.eip = (u32)p_task->initial_eip;
		p_proc->regs.esp = (u32)p_task_stack;
		p_proc->regs.eflags = 0x1202; /* IF=1, IOPL=1 */

		p_task_stack -= p_task->stacksize;
		p_proc++;
		p_task++;
		selector_ldt += 1 << 3;
	}

	k_reenter = 0;
	ticks = 0;

	p_proc_ready	= proc_table;

        /* 初始化 8253 PIT */
        out_byte(TIMER_MODE, RATE_GENERATOR);
        out_byte(TIMER0, (u8) (TIMER_FREQ/HZ) );
        out_byte(TIMER0, (u8) ((TIMER_FREQ/HZ) >> 8));

        put_irq_handler(CLOCK_IRQ, clock_handler); /* 设定时钟中断处理程序 */
        enable_irq(CLOCK_IRQ);                     /* 让8259A可以接收时钟中断 */

    initProcess();
	clearScreen();
	restart();

	while(1){}
}

PUBLIC void clearScreen(){
	disp_pos=0;
	for(int i=0; i < 80 * 25;i++){
		disp_str(" ");
	}
	disp_pos=0;
	setCursor();
}

PRIVATE void setCursor() {
	out_byte(CRTC_ADDR_REG, CURSOR_H);
	out_byte(CRTC_DATA_REG, ((disp_pos/2) >> 8) & 0xFF);
	out_byte(CRTC_ADDR_REG, CURSOR_L);
	out_byte(CRTC_DATA_REG, (disp_pos/2) & 0xFF);
}

/**************************************************************************************************************/
int waiting = 0;
Semaphore customers;
Semaphore barbers;
Semaphore mutex;

Queue customers_queue;
Queue barbers_queue;
Queue mutex_queue;

PRIVATE void initProcess(){
    for(PROCESS * p = proc_table;p < proc_table + NR_TASKS;++p){
        p->sleeping = 0;
        p->ticks = 1;
        p->priority = 1;
    }
	proc_table[1].ticks = proc_table[1].priority = 2;

    initSemaphore(&customers,0,&customers_queue);
    initSemaphore(&barbers,0,&barbers_queue);
	initSemaphore(&mutex,1,&mutex_queue);
    waiting = 0;
}



void barber(){
	while(1){
		//print current situation
		if(customers.value < 1){
			disp_str_with_color("No Customers,Barber is sleeping\n",BARBER_CHAR_COLOR);
		}
		sem_p(&customers);
		sem_p(&mutex);
		waiting--;
		sem_v(&barbers);
        sem_v(&mutex);
        cut_hair();
    }
}

void cut_hair(){
    disp_str_with_color("Barber is cutting hair",BARBER_CHAR_COLOR);
    disp_str_with_color("\n",BARBER_CHAR_COLOR);
    process_sleep(2000);
}


void customer(int customer_id,int color){
	while(1){
		process_sleep(customer_id * 800);
		sem_p(&mutex);
		if(waiting < CHAIRS){
			waiting++;
			disp_str_with_color(p_proc_ready->p_name,color);
			disp_str_with_color(" is waiting",color);
            disp_str_with_color("\n",color);
            sem_v(&customers);
			sem_v(&mutex);
            sem_p(&barbers);
            get_haircut(color);
        }else{
			disp_str_with_color(p_proc_ready->p_name,color);
			disp_str_with_color(" leave and go",color);//magic
            disp_str_with_color("\n",color);
            sem_v(&mutex);
		}
		p_proc_ready->sleeping = MAX_SLEEP_TIME;
	}
}

void get_haircut(int color){
    disp_str_with_color(p_proc_ready->p_name,color);
    disp_str_with_color(" got service\n",color);
    process_sleep(2000);
}

/*======================================================================*
                               Normal_Proc
 *======================================================================*/
void Normal_Proc()
{
	int i = 0;
	while (1) {
	}
}

/*======================================================================*
                               Barber
 *======================================================================*/
void Barber()
{
	barber();
}

/*======================================================================*
                               Customer_C
 *======================================================================*/
void Customer_C()
{
	customer(1,CUSTOMER_1_CHAR_COLOR);
}

/*======================================================================*
                               Customer_D
 *======================================================================*/
void Customer_D()
{
	customer(2,CUSTOMER_2_CHAR_COLOR);
}

/*======================================================================*
                               Customer_E
 *======================================================================*/
void Customer_E()
{
	customer(3,CUSTOMER_3_CHAR_COLOR);
}


