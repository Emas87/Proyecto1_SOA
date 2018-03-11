#include "Threads_Progress.h"
#include <stdio.h>

void hello(void* mctx_return_arg){
   mctx_t * mctx_return =  (mctx_t*)mctx_return_arg;
   printf("Hello Wolrd \n");
   mctx_restore(mctx_return);
}

int main(int argc,char *argv[]){
   gtk_init(&argc, &argv);   
   int Porcentaje_temp[5] = {26,56,89,99,'\0'};
   double Resultado_temp[5] = {3.1,3.14,3.141,3.1416,'\0'};
   int tids_tmp[5] = {6,1,3,5,'\0'};
   mctx_t mctx_create_thread[4],mctx_main;


   mctx_t *mctx_p = &mctx_create_thread[2];
   mctx_t *mctx_p2 = &mctx_create_thread[3];
   
   grap_t grap_arg,hello_arg;   
   grap_t *grap_args = &grap_arg;
   grap_t *hello_args = &hello_arg;
   
   grap_args -> Porcentaje = Porcentaje_temp ;
   grap_args -> Resultado = Resultado_temp;
   grap_args -> tids = tids_tmp;
   grap_args -> mctx_ret = & mctx_main;
   grap_args -> mctx_func = & mctx_create_thread[2];

   hello_args -> mctx_ret = & mctx_main;
   hello_args -> mctx_func = & mctx_create_thread[3];

   size_t sk_size = SIGSTKSZ;
   char sk_addr[2][SIGSTKSZ];
   //mctx_create(mctx_p2,hello, (void*)hello_args,(void*)sk_addr[3], sk_size);      
   mctx_create(mctx_p,graphics, (void*)grap_args,(void*)sk_addr[2], sk_size);
   mctx_t *mctx_pmain = &mctx_main;   
   mctx_switch(mctx_pmain,&mctx_create_thread[2]);
   
   /*int turno = 0;
   set_quantum(100,mctx_pmain);
   while(1){
      if(turno==0){
         printf("turno 0\n");
         mctx_switch(mctx_pmain,&mctx_create_thread[2]);
         turno=1;
      } else {
         printf("turno 1\n");
         mctx_switch(mctx_pmain,&mctx_create_thread[3]);
         turno=0;
      }
   }*/
   return 0;
}
