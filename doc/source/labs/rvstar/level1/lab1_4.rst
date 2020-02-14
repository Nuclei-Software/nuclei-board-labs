.. _lab1_4:

Core feature: Nesting of Interrupts
===================================

**Purpose**

- To get familiar with the interrupt preemption in |nuclei_ncore|
- To know how to use interrupts nesting

**Requirements**

The following hardware and source codes are required:

* PC host
* Nuclei board (|rv_star|)
* USB Type-C cable
* ``nuclei-sdk/board-labs/rvstar/nesting_of_interrupts`` 

**Content**

- Through learning the `Nuclei RISCV ISA Spec <https://nuclei-software.github.io/NMSIS/html/index.html>`_ to get familar with interrupt preemption in |nuclei_ncore|.
- Programming the EXTI peripheral registers and the Timer peripheral registers of SoC, ECLIC registers of Core to create two external interrupts which are triggered by pressing the on-board user key and SoC Timer seperately, and set interrput initiated by user key to a higher level. And here two interrupt service routines change on-board RGB LED to two different statuses.

.. note::
    About the detailed setup of external interrupt, please refer to :ref:`Core feature: Interrupt <lab1_3>`

    
**Principles**

While the processor is handling an interrupt, there may be another new interrupt request of a higher level, and then the core can stop the current interrupt service routine and start to taken the new one and execute its “Interrupt Service Routine”. Hence, the interrupt preemption is formed (that is, the previous interrupt has not returned yet, and the new interrupt is taken), and there could be multi-level of preemptions.

Take the following case as an example:

a. Assuming that the processor is operating main routine normally, suddenly an interrupt is initiated by SoC Timer. The processor will stop the execution of current main program, then turns to handle the interrupt request. 

b. When the processor is executing SoC Timer interrupt service routine, another interrupt is initiated by user key which has a higher level than the SoC Timer interrupt. So The processor will stop processing SoC Timer interrupt service routine and start to handle the interrupt initiated by user key.

c. After that no other higher-level interrupts arrive, the user key interrupt will not be preempted. The processor can successfully complete the interrupt service routine of user key interrupt, and then return to process the previous interrupt service routine of SoC Timer interrupt.

d. Completing the SoC Timer interrupt service routine, the core will return to execute the previous main routine.

.. _figure_lab1_4_1:

.. figure:: /asserts/medias/lab1_4_fig1.jpg
   :width: 800
   :alt: lab1_4_fig1

   Interrupt preemption example

.. note::
	In the Nuclei processor core, the supported methods for interrupt preemption depending on whether the interrupt is a vectored interrupt or a non-vectored interrupt. Please see `(CLIC mode) Vectored and Non-Vectored Processing Mode of Interrupts <https://github.com/riscv-mcu/GD32VF103_DataSheets/blob/master/GD32VF103_User_Manual_EN_V1.0.pdf>`_ for more details.

In |nuclei_sdk|, ``gd32vf103_rvstar.h`` provides API to operate the on-board peripherals, ``gd32vf103_timer.h`` provides API to operate the SoC Timer, ``gd32vf103_exti.h`` provides API to operate the SoC EXTI, ``core_feature_eclic.h`` provides API to config ECLIC feature for Nuclei Core.

The code for this lab is located in ``nuclei-sdk/board-labs/rvstar/nesting_of_interrupts``. It can be divided into 4 parts:

* Part1 : Board config, initialize on-board RGB LED and user key
* Part2 : Timer config, set working mode of *TIMER1* and enable Timer update interrupt in SoC level
* Part3 : EXTI config, set *PA0* to used as EXTI source and enable EXTI interrupt in SoC level
* Part4 : ECLIC config, interrupt config in Processor level for each interrupt sources (interrput enable, level and priority)
* Part5 : EXTI interrupt service routine, change the status of on-board RGB LED (set color to Blue)
* Part6 : SoC Timer interrupt service routine, change the status of on-board RGB LED (set color to Green)
	

The program flow is shown below:

.. _figure_lab1_4_2:

.. figure:: /asserts/medias/lab1_4_fig2.jpg
   :width: 600
   :alt: lab1_4_fig2

   Programming flow of interrupt preemption


**Steps**

1. Connect |rv_star| and your computer with the USB Type-C cable.

2. Compile and run the ``nuclei-sdk/board-labs/rvstar/nesting_of_interrupts`` example.

* |nuclei_sdk|

    Using the following commands:

    .. code-block:: console

       cd /nuclei-sdk/board-labs/rvstar/nesting_of_interrupts
       make SOC=gd32vf103 BOARD=gd32vf103v_rvstar upload

* |ses|

    Using the following actions:

3. Check the status of on-board RGB LED, then press the on-board User Key(PA0-WKUP), and check the status of on-board RGB LED again.

.. _figure_lab1_4_3:

.. figure:: /asserts/medias/lab1_4_fig3.jpg
   :alt: lab1_4_fig3
   :width: 600

   Experimental results

**Exercises**

Try to create you own application which add more interrupt sources (such as internal interrupt source) to implement multi-level nesting of Interrupts.