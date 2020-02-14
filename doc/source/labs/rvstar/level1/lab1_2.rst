.. _lab1_2:

SoC Peripheral: Timer
=====================

**Purpose**

- To get familar with the peripheral of |gd_mcu|: Timer
- To know how to program peripheral registers to control the MCU Timer

**Requirements**

The following hardware and source codes are required:

* PC host
* Nuclei board (|rv_star|)
* USB Type-C cable
* ``nuclei-sdk/board-labs/rvstar/timer_pwm`` 

**Content**

- Through learning the `GD32VF103 MCU User Mannual <https://github.com/riscv-mcu/GD32VF103_DataSheets/blob/master/GD32VF103_User_Manual_EN_V1.0.pdf>`_ to get familar with the usage of Timer.
- Programming the Timer peripheral registers to generate PWM signals, and output these through pins which connected to RGB LED. Assigning the duty cycle of generated PWM signals to set the color of on-board RGB LED.
    
    
**Principles**

|gd_mcu| offers up to one 16-bit advanced timer (*TIMER0*), four 16-bit general timers(*TIMERx=1,2,3,4*), and two 16-bit basic timer (*TIMER5 & TIMER6*).

The advanced timer (*TIMER0*) can be seen as a three-phase PWM multiplexed on 6 channels. It has complementary PWM outputs with programmable dead-time generation. It can also be used as a complete general timer. The 4 independent channels can be used for input capture, output compare, PWM generation (edge-aligned or center-aligned counting modes), single pulse mode output. If configured as a general 16-bit timer, it can be synchronized with external signals or to interconnect with other general timers together which have the same architecture and features.

The general timer, known as *TIMERx=1,2,3,4* can be used for a variety of purposes including general time, input signal pulse width measurement or output waveform generation such as a single pulse generation or PWM output, up to 4 independent channels for input capture/output compare. The general timer also supports an encoder interface with two inputs using quadrature decoder.

The basic timer, known as *TIMER5* and *TIMER6* are mainly used for DAC trigger generation. They can also be used as a simple 16-bit time base.

From the schematic of |rv_star|, we can see that *PA1*, *PA2*, *PA3* are used to control RGB LED, and these pins can be used as *TIMER1_CH1*, *TIMER1_CH2*, *TIMER1_CH3*, so here we use *TIMER1*.

In |nuclei_sdk|, ``gd32vf103_gpio.h`` provides API to config the SoC GPIO, ``gd32vf103_timer.h`` provides API to operate the SoC Timer.

The code for this lab is located in ``nuclei-sdk/board-labs/rvstar/timer_pwm``. It can be divided into 3 parts:

* Part1 : GPIO config, set *PA1*, *PA2*, *PA3* to used as peripheral alternate function(*TIMER1 CH1 CH2 CH3*)
* Part2 : Timer config, program *TIMER1* to generate PWM signals
* Part3 : RGB config, assign the duty cycle of generated PWM signals to set the color of on-board RGB LED

**Steps**

1. Connect |rv_star| and your computer with the USB Type-C cable.

2. Compile and run the ``nuclei-sdk/board-labs/rvstar/timer_pwm`` example.

* |nuclei_sdk|

    Using the following commands:

    .. code-block:: console

       cd /nuclei-sdk/board-labs/rvstar/timer_pwm
       make SOC=gd32vf103 BOARD=gd32vf103v_rvstar upload

* |ses|

    Using the following actions:

3. Check the color change of on-board RGB LED.

.. _figure_lab1_2_1:

.. figure:: /asserts/medias/lab1_2_fig1.jpg
   :width: 500
   :alt: lab1_2_fig1

   Experimental results


**Exercises**

Try to create you own application which using *TIMER4* to generate PWM signals to control on-board RGB LED.



