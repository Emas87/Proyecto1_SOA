#include "lottery.h"
#include "arctan_thread.h"
#include <stdio.h>


/*void hello(void* mctx_return_arg){
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

}*/


int main(int argc,char *argv[]){

	int thread_num = 7;
	int thread_id[] = {0,0,123,215,349,498,545}; 
	int tickets[] = {0,0,25,25,25,25,25};
	char mode[] = "No Expropiativo";
	int quantum = 15;
	int winner_thread=0;
	int i=0;
	int j=0;
	int indice=0;


	size_t sk_size = SIGSTKSZ;
	char sk_addr[thread_num][SIGSTKSZ];
	mctx_t mctx_create_thread[thread_num], mctx_main;
	mctx_t *sf_arg = &mctx_main;


	int workload[]={0,0,16,32,64,128,256};
	double percent_halt[]={0,0,10,15,20,25,30};
	double percent_done[]={0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	double pi[]={0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	int item[]={0.0,0.0,0.0,0.0,0.0,0.0,0.0};

	initrand();

	for(j=2;j<thread_num;j++){
		while(percent_done[j] <100){
		printf("-------------------------------------------------------------\n");
		//winner_thread = scheduler(mode, thread_num, tickets, quantum, thread_id);
		winner_thread = lottery(thread_num, tickets, thread_id);
		printf("Winner Thread: %d\n",winner_thread);

		for(i=2;i<thread_num;i++){
			if(thread_id[i]==winner_thread){
				indice = i;
			}
		}
		printf("Indice: %d\n", indice);

		for(i=2;i<thread_num;i++){

			if(i==indice){

			arctan_t arctan_arg[i];
			arctan_t *arctan_args = &arctan_arg[i]; 

			arctan_args -> workload = &workload[i];
			arctan_args -> percent_halt = &percent_halt[i];
			arctan_args -> percent_done = &percent_done[i];
			arctan_args -> pi = &pi[i];
			arctan_args -> item = &item[i];
			arctan_args -> mctx_ret = & mctx_main;
			arctan_args -> mctx_func = & mctx_create_thread[i];

			mctx_t mctx_create_thread[i];
			mctx_t *mctx_p = &mctx_create_thread[i];

			//mctx_t *mctx_p = &mctx_create_thread;
			mctx_create(mctx_p, arctan, (void*)arctan_args, (void*)sk_addr[i], sk_size);
			mctx_switch(&mctx_main, &mctx_create_thread[i]);

			}
		}
	
		printf ("Porcentaje Final: %f Thread %d\n", percent_done[indice], indice-2);
		//mctx_switch(&mctx_main, &mctx_create_thread[indice]);

		}
	}

	printf("Fin Programa \n");

}
