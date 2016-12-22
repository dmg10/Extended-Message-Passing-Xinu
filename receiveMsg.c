
#include<xinu.h>

umsg32 receiveMsg(void){
	//kprintf("Rec Enter\n");
	intmask mask; /* Saved interrupt mask */
	struct procent *prptr; /* Ptr to process’ table entry */
	umsg32 msg; /* Message to return */
	mask = disable();
	currpid=getpid();

	prptr = &proctab[currpid];
//	wait(mutex);
//	kprintf("Tail1: %d\n\n",prptr->tail);
	if(prptr->head==prptr->tail){
		prptr->prhasmsg=FALSE;
//	kprintf("Rec Enter6\n");
	}
	if (prptr->prhasmsg == FALSE) {
		if(prptr->prsem1==NOSEM)
			prptr->prsem1 = semcreate(0);     /*Wait for the message to arrive*/
		
		else
			semreset(prptr->prsem1,0);
//		kprintf("Rec Enter2\n");
	}
//	signal(mutex);
//	kprintf("Tail2: %d\n\n",prptr->tail);

	wait(prptr->prsem1);
//	wait(mutex);

	if(prptr->tail>=BUFFER_SIZE-1){
		prptr->tail=0;
//		kprintf("Rec Enter4\n");
	}
	msg = prptr->buffer[prptr->tail]; /* Retrieve message */
	prptr->tail++;
	kprintf("Message Received : %d\n",msg);
	//kprintf("Received  : %d",msg);
//	kprintf("Rec Enter5\n");
	/*if(prptr->head==prptr->tail){
		prptr->prhasmsg=FALSE;
//	kprintf("Rec Enter6\n");
	}*/
	semdelete(prptr->prsem1);
	prptr->prsem1=NOSEM;

	restore(mask);
//	kprintf("Rec exit\n");
//	signal(mutex);
	return msg;

}
