.. _lab2_1:

Nuclei SDK Demos
================

**Purpose**

- To know what the applications included in |nuclei_sdk| are
- To know how to run the demo in |nuclei_sdk|

**Requirements**
  
The following hardware and source codes are required:

* PC host
* Nuclei board (|rv_star|)
* USB Type-C cable
* ``nuclei-sdk/application/`` 

**Content**

- A brief introduction about applications included in |nuclei_sdk|.
- Based on |nuclei_sdk|, run the FreeRTOS demo.   
    
**Principles**

About the brief introduction of applications included in |nuclei_sdk|, please refer to this `online docs <http://doc.nucleisys.com/nuclei_sdk/design/app.html>`_.

There are many demos in ``nuclei-sdk/application/``, here we just take the FreeRTOS demo as an example to run. This demo shows the swapping of tasks and the usage of software timer.

The code for this demo is located in ``nuclei-sdk/application/freertos/demo``.

In the code, the different task contents will be displayed in the UART teminal.

**Steps**

1. Connect |rv_star| and your computer with the USB Type-C cable, and open UART terminal.

2. Compile and run the ``nuclei-sdk/application/freertos/demo`` example.

  Using the following commands:

  .. code-block:: console

     cd /nuclei-sdk/application/freertos/demo
     make SOC=gd32vf103 BOARD=gd32vf103v_rvstar upload

3. Check the output in UART terminal.

  .. code-block:: console

      Nuclei SDK Build Time: Feb 13 2020, 20:06:06
      Download Mode: FLASHXIP
      CPU Frequency 109058823 Hz
      Before StartScheduler
      task_1
      task1_running.....
      task_2
      task2_running.....
      timers Callback
      timers Callback
      task1_running.....
      task2_running.....
      timers Callback
      timers Callback
      task1_running.....
      task2_running.....
      timers Callback
      timers Callback
      task1_running.....
      ...More Message Below(Not Displayed)...


**Exercises**

Try to run more other applications in |nuclei_sdk|.
