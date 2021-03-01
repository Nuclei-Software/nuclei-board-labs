#ifndef __INSN_H__
#define __INSN_H__

#define  ROW_LEN    3
#define  COL_LEN    3

 
// custom lbuf 
inline void custom_lbuf(int addr);

// custom sbuf 
inline void custom_sbuf(int addr);


// custom rowsum 
inline int custom_rowsum(int addr);


// normal test case without NICE accelerator.
int normal_case(unsigned int array[ROW_LEN][COL_LEN]);

// teat case using NICE accelerator.
int nice_case(unsigned int array[ROW_LEN][COL_LEN]);

#endif

