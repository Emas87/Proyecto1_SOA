#include <ucontext.h>
#include <signal.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
/* machine context data structure */
typedef struct mctx_st{
   jmp_buf jb;
}mctx_t;

/* save machine context */
#define mctx_save(mctx) \
   setjmp((mctx)->jb)
/* restore machine context */
#define mctx_restore(mctx) \
   longjmp((mctx)->jb, 1)
/* switch machine context */
#define mctx_switch(mctx_old,mctx_new) \
   if (setjmp((mctx_old)->jb) == 0) \
   longjmp((mctx_new)->jb, 1)

void mctx_create_trampoline(int);
void mctx_create_boot(void);

static mctx_t mctx_caller;
static sig_atomic_t mctx_called;
static mctx_t *mctx_creat;
static void (*mctx_creat_func)(void *);
static void *mctx_creat_arg;
static sigset_t mctx_creat_sigs;

void mctx_create(mctx_t *mctx, void (*sf_addr)(void *), void *sf_arg, void *sk_addr, size_t sk_size) {
   struct sigaction sa;
   struct sigaction osa;
   struct sigaltstack ss;
   struct sigaltstack oss;
   sigset_t osigs;
   sigset_t sigs;

	/* Step 1: */
	sigemptyset(&sigs);
	sigaddset(&sigs, SIGUSR1);
	sigprocmask(SIG_BLOCK, &sigs, &osigs);//agregar SIGUSR1 a las senales enmascaradas
   printf("1\n");

	/* Step 2: */
	memset((void *)&sa, 0,
	sizeof(struct sigaction));
	sa.sa_handler = mctx_create_trampoline;
	sa.sa_flags = SA_ONSTACK;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, &osa);
   printf("2\n");

	/* Step 3: */
	ss.ss_sp = sk_addr;
	ss.ss_size = sk_size;
	ss.ss_flags = 0;
	sigaltstack(&ss, &oss);
   printf("3\n");

	/* Step 4: */
	mctx_creat = mctx;
	mctx_creat_func = sf_addr;
	mctx_creat_arg = sf_arg;
	mctx_creat_sigs = osigs;
	mctx_called = false;
	kill(getpid(), SIGUSR1);
	sigfillset(&sigs); //Llena el set con todas las senales existentes
	sigdelset(&sigs, SIGUSR1);
   //Esperando a una senal
	while (!mctx_called)
	   sigsuspend(&sigs); //Enmascara todas las senales que contiene "sig", o sea todas menos SIGUSR1

   printf("4\n");
	/* Step 6: */
	sigaltstack(NULL, &ss);
	ss.ss_flags = SS_DISABLE;
	sigaltstack(&ss, NULL);
	if (!(oss.ss_flags & SS_DISABLE))
	   sigaltstack(&oss, NULL);
	sigaction(SIGUSR1, &osa, NULL);
	sigprocmask(SIG_SETMASK,&osigs, NULL);
   printf("6\n");

	/* Step 7 & Step 8: */
	mctx_switch(&mctx_caller, mctx);
   printf("7-8-14\n");
	
	/* Step 14: */	
	return;
}
void mctx_create_trampoline(int sig){
   /* Step 5: */
   if (mctx_save(mctx_creat) == 0){
      mctx_called = true;
      printf("4.5\n");      
      return;
   }
   printf("5\n");

   /* Step 9: */
   mctx_create_boot();
   printf("9\n");   
}
void mctx_create_boot(void){
	void (*mctx_start_func)(void *);
	void *mctx_start_arg;
	/* Step 10: */
	sigprocmask(SIG_SETMASK,&mctx_creat_sigs, NULL);
   printf("10\n");   
	/* Step 11: */
	mctx_start_func = mctx_creat_func;
	mctx_start_arg = mctx_creat_arg;
   printf("11\n");

	/* Step 12 & Step 13: */
	mctx_switch(mctx_creat, &mctx_caller);
   printf("12-13\n");

	/* The thread ‘‘magically’’ starts... */
	mctx_start_func(mctx_start_arg);


	/* NOTREACHED */
	
	abort();
}
