#include "soa_thread.h"
#include <signal.h>
#include <sys/time.h>

/*struct itimerval {
    struct timeval it_interval; // next value 
    struct timeval it_value;    // current value 
};
struct timeval {
    time_t      tv_sec;         // seconds 
    suseconds_t tv_usec;        // microseconds 
};*/

mctx_t *mctx_main;
void
quantum_handler (int sig)
{
  //signal (sig, catch_alarm);
   printf("termino el quantum\n");
   mctx_restore(mctx_main);
}
void set_quantum(int quantum,mctx_t *mctx_q_arg){
   mctx_main = mctx_q_arg;   
   struct timeval new_time,zero_time;
   zero_time.tv_sec = 0;
   zero_time.tv_usec = 0;
   new_time.tv_usec = quantum;
   struct itimerval *interval_new ;
   struct itimerval interval_new_var;
   interval_new = &interval_new_var;  

   interval_new->it_interval = zero_time;
   interval_new->it_value = new_time;

//Adding new handler for SIGALRM
   struct sigaction sa;
   memset((void *)&sa, 0,sizeof(struct sigaction));
   sa.sa_handler = quantum_handler;
	sa.sa_flags = SA_ONSTACK;
	sigemptyset(&sa.sa_mask);

   if(sigaction(SIGALRM,&sa,NULL) == -1){
      printf("Error no se pudo usar quantum_handler para SIGALRM\n");
   }
   setitimer (ITIMER_REAL,interval_new,NULL );

}
