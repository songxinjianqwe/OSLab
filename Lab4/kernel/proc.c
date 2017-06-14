
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               proc.c
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

/*======================================================================*
                              schedule
 *======================================================================*/
PUBLIC void schedule()
{
	PROCESS* p;
	int	 greatest_ticks = 0;

	while (!greatest_ticks) {
		for (p = proc_table; p < proc_table+NR_TASKS; p++) {
            //if this process's ticks != 0 and is awake
            //then goto this process
			if (p->ticks > greatest_ticks && p->sleeping == AWAKE) {
				greatest_ticks = p->ticks;
				p_proc_ready = p;
			}
		}
        //current process is the process whose ticks are the greatest

        //if we can't find a process whose ticks > 0(fail to schedule)
        //then reset ticks to priority
		if (!greatest_ticks) {
			for (p = proc_table; p < proc_table+NR_TASKS; p++) {
                if(p->sleeping == AWAKE){
                    p->ticks = p->priority;
                }
			}
		}
	}
}

/*======================================================================*
                           sys_get_ticks
 *======================================================================*/
PUBLIC int sys_get_ticks()
{
	return ticks;
}

/*======================================================================*
                           sys_process_sleep
 *======================================================================*/
PUBLIC void sys_process_sleep(int mill_seconds){
    //add a field `sleeping` to control the scheduler
    p_proc_ready->sleeping += mill_seconds * HZ /1000;
    schedule();
}

/*======================================================================*
                           sys_disp_str_with_color
 *======================================================================*/
PUBLIC void sys_disp_str_with_color(char * info,int color){
	disp_color_str(info,color);
}
