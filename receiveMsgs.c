
#include<xinu.h>

syscall	receiveMsgs (
        umsg32*	msgs,
        uint32	msg_count
){

//		kprintf("RecMsgs Enter\n");
    intmask mask; /* Saved interrupt mask */
	int i3,sucess_m=0;
    struct procent *prptr; /* Ptr to process’ table entry */
    umsg32 msg; /* Message to return */
    mask = disable();
    currpid=getpid();

    prptr = &proctab[currpid];

    if(prptr->head==prptr->tail){
        prptr->prhasmsg=FALSE;

    }
    if (prptr->prhasmsg == FALSE) {
        if(prptr->prsem1==NOSEM)
            prptr->prsem1 = semcreate(0);     /*Wait for the message to arrive*/

        else
            semreset(prptr->prsem1,0);

    }


    wait(prptr->prsem1);


    if(prptr->tail>=BUFFER_SIZE-1){
        prptr->tail=0;

    }
    for(i3=0;i3<msg_count;i3++) {
        msgs[i3] = prptr->buffer[prptr->tail]; /* Retrieve message */
        prptr->tail++;
//	kprintf("Message Received : %d\n",msgs[i3]);		
	sucess_m++;
    }

    semdelete(prptr->prsem1);
    prptr->prsem1=NOSEM;

    restore(mask);

    return sucess_m++;

}
