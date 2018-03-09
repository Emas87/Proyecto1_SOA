#include "soa_thread.h"
#include "lottery.h"
#include <stdio.h>

void hello(void* mctx_return_arg){
   mctx_t * mctx_return =  (mctx_t*)mctx_return_arg;
   printf("Hello Wolrd \n");
   mctx_restore(mctx_return);
}

int scheduler(char *mode, int thread_num, int tickets[], int quantum, int thread_id[]){
	int winner_thread=0;
	winner_thread = lottery(thread_num, tickets, thread_id);
	printf("Winner Thread: %d\n", winner_thread);
	
	printf("MODO: %s\n", mode);

	if(strcmp(mode,"Expropiativo")==0){
		printf("Quantum: %dms\n", quantum);
	}else if(strcmp(mode,"No Expropiativo")==0){	
		printf("Porcentaje: %d%% \n",quantum);
	} 
	return winner_thread;

}


int main(int argc,char *argv[]){

	int thread_num = 5;
	int thread_id[] = {123,215,349,498,545}; 
	int tickets[] = {20,10,5,30,10};
	char mode[] = "No Expropiativo";
	int quantum = 15;
	int winner_thread = scheduler(mode, thread_num, tickets, quantum, thread_id);
	printf("Winner Thread: %d\n",winner_thread);

	size_t sk_size = SIGSTKSZ;
	char sk_addr[thread_num][SIGSTKSZ];
	mctx_t mctx_create_thread[thread_num], mctx_main;
	mctx_t *sf_arg = &mctx_main;

	int i;
	int indice;
	for(i=0;i<thread_num;i++){
		if(thread_id[i]==winner_thread){
			indice = i;
		}
	}

	for(i=0;i<thread_num;i++){
		mctx_t mctx_create_thread[i];
		mctx_t *mctx_p = &mctx_create_thread[i];

		//mctx_t *mctx_p = &mctx_create_thread;
		mctx_create(mctx_p, hello, (void*)sf_arg, (void*)sk_addr[i], sk_size);
		if(i==indice){
			mctx_switch(&mctx_main, &mctx_create_thread[indice]);
		}
	}


	printf("Indice: %d\n", indice);
	//mctx_switch(&mctx_main, &mctx_create_thread[indice]);
	printf("Fin desde\n");



}
