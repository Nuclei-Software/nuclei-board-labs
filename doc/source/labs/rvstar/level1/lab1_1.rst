.. _lab1_1:

How to use Nuclei board
=======================

**Purpose**

- To get familiar with the usage of Nuclei board and on-board peripherals
- To know how to program and debug the Nuclei board and on-board peripherals

**Requirements**
  
The following hardware and source codes are required:

* PC host
* Nuclei board (|rv_star|)
* USB Type-C cable
* ``nuclei-sdk/board-labs/rvstar/running_led`` 

**Content**

- A brief introduction of Nuclei board and on-board peripherals.
- Based on |nuclei_sdk| and |ses| individually, program the GPIO to control some on-board peripherals and send message to PC through UART then printf that in the terminal.

.. note::
    About the detailed usage of |nuclei_sdk|, please refer to `How to develop with NUclei SDK <http://www.riscv-mcu.com/quickstart-quickstart.html>`__; 
    About the detailed usage of |ses|, please refer to `How to develop with Segger Embedded Studio <http://www.riscv-mcu.com/quickstart-quickstart.html>`__  
    
    
**Principles**

About the brief introduction of |rv_star|, please refer to this `online docs <http://www.riscv-mcu.com/quickstart-quickstart.html>`_.

.. _figure_lab1_1_1:

.. figure:: /asserts/medias/lab1_1_fig1.jpg
   :width: 500
   :alt: lab1_1_fig1

   |rv_star|

There are RGB LED, UART(to USB), and User Keys on |rv_star|, 
this lab shows how to program the GPIO to control these on-board peripherals of |rv_star|, and send message to PC through UART.

In |nuclei_sdk|, ``gd32vf103_rvstar.h`` provides API to operate these on-board peripherals.

The code for this lab is located in ``nuclei-sdk/board-labs/rvstar/running_led``.
In the code, the number of on-board User Key(PA0-WKUP) pressed is counted, the status of RGB LED changes according to this number, and this info is displayed in the UART teminal.


**Steps**

1. Connect |rv_star| and your computer with the USB Type-C cable, and open UART terminal.

2. Compile and run the ``nuclei-sdk/board-labs/rvstar/running_led`` example.

* |nuclei_sdk|

    Using the following commands:

    .. code-block:: console

       cd /nuclei-sdk/board-labs/rvstar/running_led
       make SOC=gd32vf103 BOARD=gd32vf103v_rvstar upload

* |ses|

    Using the following actions:

    .. _figure_lab1_1_2:

    .. figure:: /asserts/medias/lab1_1_fig2.jpg
       :width: 900
       :alt: lab1_1_fig2

       Operations in Embedded Studio

3. Press the on-board User Key(PA0-WKUP), then check the result in UART terminal, and watch the changes of on-board RGB LED.

.. _figure_lab1_1_3:

.. figure:: /asserts/medias/lab1_1_fig3.jpg
   :width: 800
   :alt: lab1_1_fig3

   Experimental results

**Exercises**

Try to create you own application to control the peripherals of |rv_star| in other mode.
