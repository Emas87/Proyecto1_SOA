#include "soa_thread.h"
#include <stdio.h>

void hello(void* mctx_return_arg){
   mctx_t * mctx_return =  (mctx_t*)mctx_return_arg;
   printf("Hello Wolrd \n");
   mctx_restore(mctx_return);
}
int main(int argc,char *argv[]){
   size_t sk_size = SIGSTKSZ;
   char sk_addr[SIGSTKSZ];
   mctx_t mctx_create_thread,mctx_main;
   mctx_t *sf_arg = &mctx_main;   
   mctx_t *mctx_p = &mctx_create_thread;
   mctx_create(mctx_p,hello, (void*)sf_arg,(void*)sk_addr, sk_size);
	mctx_switch(&mctx_main,&mctx_create_thread);
   printf("Fin desde\n");
}
