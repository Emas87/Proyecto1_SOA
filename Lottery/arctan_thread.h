#include "soa_thread.h"
#include <ucontext.h>
#include <signal.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

/* Procedure to calculate 4arctan(1)=pi */

typedef struct arctan_st{
        int *workload;
        double *percent_halt,*pi,*percent_done;
        mctx_t * mctx_ret;
        mctx_t * mctx_func;   
}arctan_t;

int *workload;
double *percent_halt,*pi,*percent_done;
mctx_t * mctx_return;
mctx_t * mctx_function;   

void arctan (void* arctan_arg) {
 arctan_t *arctan_dat=(arctan_t*)arctan_arg;
 workload = arctan_dat -> workload;
 percent_halt = arctan_dat -> percent_halt;
 pi = arctan_dat -> pi;
 percent_done = arctan_dat -> percent_done;
 mctx_return = arctan_dat -> mctx_ret;
 mctx_function = arctan_dat -> mctx_func;

/*
 int workload=100;
 double percent_halt=50.0;
 int iterations=workload*50;
 double arctan=0.0;
 double pi=0.0;
 double percent_done=0.0;
 int halt_flag=0;

 mctx_t *mctx_return = (mctx_t*)mctx_return_arg;
*/
 
 int n=0;
 int iterations=*workload*50;
 double arctan_result=0.0;
 int halt_flag=0;

 printf("Input workload=%i\n", *workload);
 printf("Starting pi value calculation\n");

 for (n=0; n<iterations; n++) {
	if (halt_flag==1) {
		*pi=4.0 * arctan_result;
        	printf("Workload percent done=%f\n",*percent_done);
        	printf("Partial Pi result=%f\n",*pi);
        	mctx_switch(mctx_function,mctx_return);
		halt_flag=0;
	}
	arctan_result=arctan_result + pow((double)-1.0, (double) n) / (2.0*(double)n+1.0);
	//printf("Iteration %i value %i\n",n,2*n+1);
	*percent_done = ((double)(n+1)/(double)iterations)*100.0;
	//printf("Workload percent done=%f\n",*percent_done);
	if (*percent_done == *percent_halt) {
		halt_flag=1;
		printf("Halt flag set\n");
		}
	}
        *pi=4.0 * arctan_result;
	printf("Workload percent done=%f\n",*percent_done);
        printf("Final Pi result=%f\n",*pi);
//	return(pi,percent_done);
	mctx_switch(mctx_function,mctx_return);
}

/*
int main(int argc,char *argv[]){
   int i=0;
   size_t sk_size = SIGSTKSZ;
   char sk_addr[SIGSTKSZ];
//   char *sf_arg = "a";

   mctx_t mctx_create_thread,mctx_main;
   mctx_t *sf_arg = &mctx_main;
   mctx_t *mctx_p = &mctx_create_thread;

   mctx_create(mctx_p,arctan, sf_arg,(void*)sk_addr, sk_size);
   for (i=0; i<5; i++) {
     mctx_switch(&mctx_main,&mctx_create_thread);
     printf("Back to main, thread created\n");
}
   return 0;
}
*/
