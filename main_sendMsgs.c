#include <xinu.h>

pid32 producer_id;
pid32 consumer_id;
sid32 mutex,mutex1;
int msg_count=0;
int n=0;



process producer(void)
{
	int i;char *m1,count=0,k1=100,k2=0,j=0;
	umsg32 msg[5];
	

/*----------------------------Test for Part2 of A----------------------------*/
	for(i=0;i<2;i++){

		msg_count=0;
		for(j=0;j<2;j++){

			msg[j]=k1;
			k1=k1+10;
			msg_count++;
		}
		for(j=msg_count;j<5;j++){

			msg[j]=k2;
			k2++;
			msg_count++;
		}

		m1=sendMsgs(consumer_id,msg,msg_count);

		wait(mutex);

		if(m1!=SYSERR){
			kprintf("Message Sent: %d to consumer with id: %d\n",m1,consumer_id);

		}
		else
			kprintf("SYSERR\n");
			
		signal(mutex);

	}
	return OK;
}


process consumer(void)
{
		int i1,count=0,i2=0,i;
		umsg32 m;
		umsg32 msgs[5];

/*----------------------------Test for Part2 of A----------------------------*/

		for(i1=0;i1<2;i1++){
			m=receiveMsgs(msgs,5);
			wait(mutex);
			if(m!="SYSERR"){
				for(i2=0;i2<m;i2++){
					kprintf("Message Received : %d\n",msgs[i2]);		
				}
			}
		
			signal(mutex);
		
		}
	
	return OK;
}

void initialize1(pid32 pid1){
	struct procent *prptr1;
	prptr1=&proctab[pid1];
	prptr1->head=0;
	prptr1->tail=0;
	prptr1->prsem1=NOSEM;

}

process	main(void)
{

	recvclr();
	mutex=semcreate(1);
	producer_id =create(producer,4096,50,"producer",0);
	consumer_id=create(consumer,4096,50,"consumer",0);
	initialize1(producer_id);
	initialize1(consumer_id);
	resched_cntl(DEFER_START);
	resume(producer_id);
	resume(consumer_id);
	resched_cntl(DEFER_STOP);
	return OK;
	
}