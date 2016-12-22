#include <xinu.h>

pid32 producer_id;
pid32 consumer_id,consumer_id1;
pid32 pids[2];
sid32 mutex,mutex1;
int msg_count=0;
int n=0;



process producer(void)
{
	int i;char *m1,count=0,k1=100,k2=0,j=0;
	umsg32 msg[5];
	
	pids[0]=consumer_id;
	pids[1]=consumer_id1;
	sendnMsg(2,pids,15);

	return OK;
}


process consumer(void)
{
		int i1,count=0,i2=0,i;
		umsg32 m;
		umsg32 msgs[5];

		receiveMsg();
	
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
	consumer_id1=create(consumer,4096,50,"consumer",0);
	initialize1(producer_id);
	initialize1(consumer_id);
	initialize1(consumer_id1);
	resched_cntl(DEFER_START);
	resume(producer_id);
	resume(consumer_id);
	resume(consumer_id1);
	resched_cntl(DEFER_STOP);
	return OK;
	
}