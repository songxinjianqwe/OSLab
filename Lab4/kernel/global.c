
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            global.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define GLOBAL_VARIABLES_HERE

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"


PUBLIC	PROCESS			proc_table[NR_TASKS];

PUBLIC	char			task_stack[STACK_SIZE_TOTAL];

PUBLIC	TASK	task_table[NR_TASKS] = {{Normal_Proc, STACK_SIZE_NORMAL_PROC, "Normal_Proc"},
					{Barber, STACK_SIZE_Barber, "Barber"},
					{Customer_C, STACK_SIZE_CUSTOMER_C, "Customer_C"},
                    {Customer_D, STACK_SIZE_CUSTOMER_D, "Customer_D"},
                    {Customer_E, STACK_SIZE_CUSTOMER_E, "Customer_E"}
                    };

PUBLIC	irq_handler		irq_table[NR_IRQ];

PUBLIC	system_call		sys_call_table[NR_SYS_CALL] = {sys_get_ticks,sys_process_sleep,sys_disp_str_with_color};

