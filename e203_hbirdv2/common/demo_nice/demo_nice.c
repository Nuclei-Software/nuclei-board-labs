// See LICENSE for license details.
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "hbird_sdk_soc.h"

#include "insn.h"

int main(void)
{
   int i=100;
   int arr[4]={1,2,3,4};
   unsigned int array[ROW_LEN][COL_LEN]=
                {{10,20,30},
                 {20,30,40},
                 {30,40,50} 
                                 };
   unsigned int begin_instret, end_instret, instret_normal, instret_nice;
   unsigned int begin_cycle,   end_cycle,   cycle_normal,   cycle_nice;
   
   
   
   printf("**********************************************\n"); 
   printf("** begin to sum the array using ordinary add sum\n"); 
   begin_instret =  __get_rv_instret();
   begin_cycle   =  __get_rv_cycle();

   normal_case(array);

   end_instret = __get_rv_instret();
   end_cycle   = __get_rv_cycle();
   
   instret_normal = end_instret - begin_instret;
   cycle_normal = end_cycle - begin_cycle;
   printf("\n\n");

   printf("**********************************************\n"); 
   printf("** begin to sum the array using nice add sum\n"); 
   begin_instret =  __get_rv_instret();
   begin_cycle   =  __get_rv_cycle();

   nice_case(array);

   end_instret = __get_rv_instret();
   end_cycle   = __get_rv_cycle();
   
   instret_nice = end_instret - begin_instret;
   cycle_nice   = end_cycle - begin_cycle;


   printf("**********************************************\n"); 
   printf("** performance list \n"); 

   printf("\t normal: \n"); 
   printf("\t      instret: %d, cycle: %d \n", instret_normal, cycle_normal); 
   printf("\t nice  : \n"); 
   printf("\t      instret: %d, cycle: %d \n", instret_nice  , cycle_nice  ); 


   printf("**********************************************\n\n"); 


    printf("*     *  *     *   *****   *        *******   *** \n"); 
    printf("**    *  *     *  *     *  *        *          *  \n"); 
    printf("* *   *  *     *  *        *        *          *  \n");
    printf("*  *  *  *     *  *        *        *****      *  \n");
    printf("*   * *  *     *  *        *        *          *  \n");
    printf("*    **  *     *  *     *  *        *          *  \n");
    printf("*     *   *****    *****   *******  *******   *** \n");

   printf("\n\n**********************************************"); 
    return 0;
}
