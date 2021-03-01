#include "insn.h"
#include <stdio.h>

////////////////////////////////////////////////////////////
// custom3:
// Supported format: only R type here
// Supported instr:
//  1. custom3 lbuf: load data(in memory) to row_buf
//     lbuf (a1)
//     .insn r opcode, func3, func7, rd, rs1, rs2    
//  2. custom3 sbuf: store data(in row_buf) to memory
//     sbuf (a1)
//     .insn r opcode, func3, func7, rd, rs1, rs2    
//  3. custom3 acc rowsum: load data from memory(@a1), accumulate row datas and write back 
//     rowsum rd, a1, x0
//     .insn r opcode, func3, func7, rd, rs1, rs2    
////////////////////////////////////////////////////////////
 
// custom lbuf 
inline void custom_lbuf(int addr)
{
 int zero = 0;

 asm volatile (
    ".insn r 0x7b, 2, 1, x0, %1, x0"
	:"=r"(zero)
	:"r"(addr)
  );
  
}

// custom sbuf 
inline void custom_sbuf(int addr)
{
 int zero = 0;

 asm volatile (
    ".insn r 0x7b, 2, 2, x0, %1, x0"
	:"=r"(zero)
	:"r"(addr)
  );
  
}

// custom rowsum 
inline int custom_rowsum(int addr)
{
 int rowsum;

 asm volatile (
    ".insn r 0x7b, 6, 6, %0, %1, x0"
	  :"=r"(rowsum)
	  :"r"(addr)
  );

 return rowsum; 
}



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

