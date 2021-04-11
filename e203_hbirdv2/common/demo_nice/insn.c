#include "insn.h"
#include <stdio.h>

 
// normal test case without NICE accelerator.
int normal_case(unsigned int array[ROW_LEN][COL_LEN])
{
  volatile unsigned char i=0, j=0;
  volatile unsigned int col_sum[COL_LEN]={0};
  volatile unsigned int row_sum[ROW_LEN]={0};
  volatile unsigned int tmp=0;
  for (i = 0; i < ROW_LEN; i++)
  {
    tmp = 0;
    for (j = 0; j < COL_LEN; j++)
    {
      col_sum[j] += array[i][j];
      tmp += array[i][j];
    }
    row_sum[i] = tmp;
  }
#ifdef _DEBUG_INFO_
  printf ("the element of array is :\n\t");
  for (i = 0; i < ROW_LEN; i++) printf("%d\t", array[0][i]); printf("\n\t");
  for (i = 0; i < ROW_LEN; i++) printf("%d\t", array[1][i]); printf("\n\t");
  for (i = 0; i < ROW_LEN; i++) printf("%d\t", array[2][i]); printf("\n\n");
  printf ("the sum of each row is :\n\t\t");
  for (i = 0; i < ROW_LEN; i++) printf("%d\t", row_sum[i]); printf("\n");
  printf ("the sum of each col is :\n\t\t");
  for (j = 0; j < COL_LEN; j++) printf("%d\t", col_sum[j]); printf("\n");
#endif
  return 0;
}


// test case using NICE accelerator.
int nice_case(unsigned int array[ROW_LEN][COL_LEN])
{
  volatile unsigned char i, j;
  volatile unsigned int col_sum[COL_LEN]={0};
  volatile unsigned int row_sum[ROW_LEN]={0};
  volatile unsigned int init_buf[3]={0};

  custom_lbuf((int)init_buf);
  for (i = 0; i < ROW_LEN; i++)
  {
    row_sum[i] = custom_rowsum((int)array[i]);
  }
  custom_sbuf((int)col_sum);
#ifdef _DEBUG_INFO_
  printf ("the element of array is :\n\t");
  for (i = 0; i < ROW_LEN; i++) printf("%d\t", array[0][i]); printf("\n\t");
  for (i = 0; i < ROW_LEN; i++) printf("%d\t", array[1][i]); printf("\n\t");
  for (i = 0; i < ROW_LEN; i++) printf("%d\t", array[2][i]); printf("\n\n");
  printf ("the sum of each row is :\n\t\t");
  for (i = 0; i < ROW_LEN; i++) printf("%d\t", row_sum[i]); printf("\n");
  printf ("the sum of each col is :\n\t\t");
  for (j = 0; j < COL_LEN; j++) printf("%d\t", col_sum[j]); printf("\n");
#endif
  return 0;
}

