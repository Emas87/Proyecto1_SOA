#include "soa_thread.h"
#include <stdio.h>
#include <setjmp.h>

void hello(){
   printf("Hello Wolrd\n");
}
int main(int argc,char *argv[]){
   size_t sk_size = SIGSTKSZ;
   char sk_addr[SIGSTKSZ];
   char *sf_arg = "a";
   mctx_t mctx_hello;
   mctx_t *mctx_p = &mctx_hello;
   mctx_create(mctx_p,hello, sf_arg,(void*)sk_addr, sk_size);

}
