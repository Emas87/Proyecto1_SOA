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

/* This flag controls termination of the main loop. */
volatile sig_atomic_t keep_going = 1;

/* The signal handler just clears the flag and re-enables itself. */
void
quantum_handler (int sig)
{
  //signal (sig, catch_alarm);
   printf("si ya\n");
}
void set_quantum(int quantum){
   struct timeval new_time,zero_time;
   zero_time.tv_sec = 0;
   zero_time.tv_usec = 0;
   new_time.tv_usec = quantum;
   struct itimerval *interval_new ;
   struct itimerval interval_new_var;
   interval_new = &interval_new_var;  

   interval_new->it_interval = zero_time;
   interval_new->it_value = new_time;

   signal (SIGALRM, quantum_handler);   
   setitimer (ITIMER_REAL,interval_new,NULL );

}
int
main (void)
{
  /* Establish a handler for SIGALRM signals. */
   set_quantum(100);


  return EXIT_SUCCESS;

}

