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
        int *workload, *item,*percent_halt,*percent_done;
        double *pi;
        mctx_t * mctx_ret;
        mctx_t * mctx_func;   
}arctan_t;

int *workload, *item,*percent_halt,*percent_done;
double *pi;
mctx_t * mctx_return;
mctx_t * mctx_function;   

void arctan (void* arctan_arg) {
	arctan_t *arctan_dat=(arctan_t*)arctan_arg;
	workload = arctan_dat -> workload;
	percent_halt = arctan_dat -> percent_halt;
	pi = arctan_dat -> pi;
	percent_done = arctan_dat -> percent_done;
	item = arctan_dat -> item;
	mctx_return = arctan_dat -> mctx_ret;
	mctx_function = arctan_dat -> mctx_func;

	int n=0;
	int halt_flag=0;
	int iterations=*workload*50;
	double arctan_result=0.0;
	int percent_done_local=0;

	//printf("Workload=%i\n", *workload);
	//printf("Starting pi value calculation\n");
	
	printf("Porcentaje Inicial=%d%% \n",*percent_done);
	//printf("Item entrada=%d\n",*item);

	for (n=0; n<iterations; n++) {
		if ((percent_done_local+*percent_done) <100){
			if (halt_flag==1) {
				*pi+=4.0 * arctan_result;
				*percent_done += percent_done_local;
				*item += n;
		        	//printf("Workload percent done=%d\n",*percent_done);
		        	printf("Resultado Parcial: PI=%.10lf\n",*pi);
		        	mctx_switch(mctx_function,mctx_return);
				halt_flag=0;
			}
			arctan_result=arctan_result + pow((double)-1.0, (double) (n+*item)) / (2.0*(double)(n+*item)+1.0);
			//printf("Iteration %i value %f\n",n+*item,4*arctan_result+*pi);
			percent_done_local = ((double)(n+1)/(double)iterations)*100.0;
			//printf("Workload percent done=%f\n",*percent_done);
			if (percent_done_local == *percent_halt) {
				halt_flag=1;
				//printf("Halt flag set\n");
			}
		}
	}

        *pi+=4.0 * arctan_result;
	*percent_done += percent_done_local;
	//printf("Workload percent done=%d\n",*percent_done);
        printf("Resultado Final: PI=%.10lf\n",*pi);
//	return(pi,percent_done);
	mctx_switch(mctx_function,mctx_return);

}