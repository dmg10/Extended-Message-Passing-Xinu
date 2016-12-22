#include<xinu.h>

uint32	sendMsgs (
        pid32	pid,
        umsg32*	msgs,
        uint32	msg_count
)
{
    int i,sucess=0;
    intmask mask;
    struct procent *prptr;
    mask = disable();
    if (isbadpid(pid)) {
        restore(mask);
        return SYSERR;
    }

    prptr = &proctab[pid];
    if(msg_count>10){
        return SYSERR;
    }

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

    for(i=0;i<msg_count;i++){
    //if(prptr->tail==prptr->head+1 || prptr->head-prptr->tail==BUFFER_SIZE-1){
       if((prptr->tail-prptr->head>=msg_count)|| (BUFFER_SIZE-prptr->head>=msg_count)){
            prptr->buffer[prptr->head]=msgs[i];
//	    kprintf("Message Sent: %d to consumer with id: %d\n",prptr->buffer[prptr->head],pid);	
            prptr->head++;
            sucess++;
            prptr->prhasmsg=TRUE;
        }
	else{
		return SYSERR;
	}
    }

    signal(prptr->prsem1);


    if (prptr->prstate == PR_RECV) {
        ready(pid);
    } else if (prptr->prstate == PR_RECTIM) {
        unsleep(pid);
        ready(pid);
    }
    restore(mask);
    kprintf("\nTotal messages sent is:%d\n",sucess);
    return sucess;

}