#include<xinu.h>

syscall sendMsg(
   pid32 pid,
   umsg32 msg
)
{

    intmask mask;
    struct procent *prptr;
    mask = disable();
    if (isbadpid(pid)) {
        restore(mask);
        return SYSERR;
    }

    prptr = &proctab[pid];
    if ((prptr->prstate == PR_FREE)) {
        restore(mask);
        return SYSERR;
    }
    if(prptr->tail==prptr->head+1 || prptr->head-prptr->tail==BUFFER_SIZE-1){
        restore(mask);
        return SYSERR;
    }
    
    if(prptr->prsem1==NOSEM){
        prptr->prsem1=semcreate(0);
    }

    if(prptr->head>=BUFFER_SIZE-1){
        prptr->head=0;
    }

    prptr->buffer[prptr->head]=msg;
    kprintf("Message Sent: %d to consumer with id: %d\n",prptr->buffer[prptr->head],pid);
    prptr->head++;
//	sucess++;
    prptr->prhasmsg=TRUE;

    signal(prptr->prsem1);
    

    if (prptr->prstate == PR_RECV) {
        ready(pid);
    } else if (prptr->prstate == PR_RECTIM) {
        unsleep(pid);
        ready(pid);
    }
    restore(mask);
//	kprintf("Total number of sent messages is: %d\n",sucess);

    return OK;

}