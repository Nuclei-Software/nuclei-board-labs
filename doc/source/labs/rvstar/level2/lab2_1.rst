.. _lab2_1:

Benchmark Test
==============

**Purpose**

- To know what benchmark for processor is
- To know how to run Dhrystone benchmark test

**Requirements**
  
The following hardware and source codes are required:

* PC host
* Nuclei board (|rv_star|)
* USB Type-C cable
* ``nuclei-sdk/board-labs/rvstar/running_red`` 

**Content**

- A brief introduction of benchmark for processor.
- Based on |nuclei_sdk|, run the Dhrystone benchmark test.   
    
**Principles**

What is benchmark for processor?

- Running an executable program in order to evaluate the performance of processor 
- Benchmark is usually written in a standard high-level program language (such as C/C++), and tests are developed that allow comparison of different hardware architectures
- Benchmark score is the most direct reflection of performance and widely used in competiveness evaluation and product engagement

There are many benchmarks in the field of processor, here we just take Dhrystone (one of most common in the embedded processor field) as an example to run. About the brief introduction of Dhrystone, please refer to this `wiki docs <https://en.wikipedia.org/wiki/Dhrystone>`_.

The code for this lab is located in ``nuclei-sdk/application/baremetal/benchmark/dhrystone``.

In the code, the benchmark score will be displayed in the UART teminal.


**Steps**

1. Connect |rv_star| and your computer with the USB Type-C cable, and open UART terminal.

2. Compile and run the ``nuclei-sdk/application/baremetal/benchmark/dhrystone`` example.

  Using the following commands:

  .. code-block:: console

     cd /nuclei-sdk/application/baremetal/benchmark/dhrystone
     make SOC=gd32vf103 BOARD=gd32vf103v_rvstar upload

3. Check the result in UART terminal.

  .. code-block:: console

      Nuclei SDK Build Time: Feb 13 2020, 20:06:06
      Download Mode: FLASHXIP
      CPU Frequency 109336633 Hz

      Dhrystone Benchmark, Version 2.1 (Language: C)

      Program compiled without 'register' attribute

      Please give the number of runs through the benchmark:
      Execution starts, 1600000 runs through Dhrystone
      Execution ends

      Final values of the variables used in the benchmark:

      Int_Glob:            5
              should be:   5
      Bool_Glob:           1
              should be:   1
      Ch_1_Glob:           A
              should be:   A
      Ch_2_Glob:           B
              should be:   B
      Arr_1_Glob[8]:       7
              should be:   7
      Arr_2_Glob[8][7]:    1600010
              should be:   Number_Of_Runs + 10
      Ptr_Glob->
        Ptr_Comp:          536883352
              should be:   (implementation-dependent)
        Discr:             0
              should be:   0
        Enum_Comp:         2
              should be:   2
        Int_Comp:          17
              should be:   17
        Str_Comp:          DHRYSTONE PROGRAM, SOME STRING
              should be:   DHRYSTONE PROGRAM, SOME STRING
      Next_Ptr_Glob->
        Ptr_Comp:          536883352
              should be:   (implementation-dependent), same as above
        Discr:             0
              should be:   0
        Enum_Comp:         1
              should be:   1
        Int_Comp:          18
              should be:   18
        Str_Comp:          DHRYSTONE PROGRAM, SOME STRING
              should be:   DHRYSTONE PROGRAM, SOME STRING
      Int_1_Loc:           5
              should be:   5
      Int_2_Loc:           13
              should be:   13
      Int_3_Loc:           7
              should be:   7
      Enum_Loc:            1
              should be:   1
      Str_1_Loc:           DHRYSTONE PROGRAM, 1'ST STRING
              should be:   DHRYSTONE PROGRAM, 1'ST STRING
      Str_2_Loc:           DHRYSTONE PROGRAM, 2'ND STRING
              should be:   DHRYSTONE PROGRAM, 2'ND STRING

       (*) User_Cycle for total run through Dhrystone with loops 1600000:
      726400102
             So the DMIPS/MHz can be caculated by:
             1000000/(User_Cycle/Number_Of_Runs)/1757 = 1.253639 DMIPS/MHz

**Exercises**

Try to create you own application to run more other benchmarks in |nuclei_core|.

.. note::

  You can find more tests in ``nuclei-sdk/application``, have a try.
