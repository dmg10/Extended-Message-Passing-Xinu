#include<xinu.h>

uint32	sendnMsg (
	uint32	pid_count,
	pid32*	pids,
	umsg32	msg
					
)
{
    int i10,sucess=0;
    if(pid_count>3){
	return SYSERR;
    }
    for(i10=0;i10<pid_count;i10++){
//	kprintf("Pid Count %d\n",pid_count);
	    int i;
	    intmask mask;
	    struct procent *prptr;
	pid32 pid;
	pid=pids[i10];
//	kprintf("Pid  %d\n",pid);
	    mask = disable();
	    if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	    }

	    prptr = &proctab[pid];
	    

	    if ((prptr->prstate == PR_FREE || prptr->prhasmsg)) {
		restore(mask);
		return SYSERR;
	    }

	    prptr->buffer[prptr->head]=msg;
	prptr->head++;
	sucess++;
	kprintf("Message sent : %d to receiver %d\n",msg,pid);
	    prptr->prhasmsg=TRUE;   
	   

	    if (prptr->prstate == PR_RECV) {
		ready(pid);
	    } else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	    }
	    restore(mask);


     }
	    kprintf("Total messages sent is:%d\n",sucess);
	    return sucess;

}