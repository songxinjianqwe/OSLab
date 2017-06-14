
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               clock.c
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
                           clock_handler
 *======================================================================*/
PUBLIC void clock_handler(int irq)
{
	ticks++;

	PROCESS * p;
	//for each process,if it is sleeping ,then sub a mills_second from `sleeping`
	for(p = proc_table; p < proc_table + NR_TASKS;++p){
		if(p->sleeping != AWAKE){
			p->sleeping--;
		}
	}

	//if current process is awake,then sub a tick
	if(p_proc_ready->sleeping == AWAKE){
		p_proc_ready->ticks--;
	}else{
		//if is sleeping ,then execute the schedule
		schedule();
		return ;
	}


	if (k_reenter != 0) {
		return;
	}

	if (p_proc_ready->ticks > 0) {
		return;
	}

	schedule();

}

/*======================================================================*
                              milli_delay
 *======================================================================*/
PUBLIC void milli_delay(int milli_sec)
{
        int t = get_ticks();

        while(((get_ticks() - t) * 1000 / HZ) < milli_sec) {}
}

