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

- Through learning the `Chapter 6 of Nuclei RISCV ISA Spec <https://doc.nucleisys.com/nuclei_spec/isa/interrupt.html#clic-mode-interrupt-preemption>`_ and `Interrupt and Exception Handling File in NMSIS <https://doc.nucleisys.com/nmsis/core/core_template_intexc.html>`_ to get familar with interrupt preemption in |nuclei_ncore|.
- Programming the EXTI peripheral registers and the Timer peripheral registers of SoC, ECLIC registers of Core to create two external interrupts which are triggered by pressing the on-board user key and SoC Timer seperately. Set both interrupts to non-vectored processing mode and set interrupt initiated by user key to a higher level. Here two interrupt service routines change on-board RGB LED to two different statuses.

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
  In the Nuclei processor core, the supported methods for interrupt preemption depending on whether the interrupt is a vectored interrupt or a non-vectored interrupt. Please see `(CLIC mode) Vectored and Non-Vectored Processing Mode of Interrupts <http://doc.nucleisys.com/nuclei_spec/isa/interrupt.html#clic-mode-vectored-and-non-vectored-processing-mode-of-interrupts>`_ for more details. 
  
  In this lab, we just show how to implement nesting of non-vectored interrupt. About how to implement nesting of vectored interrupt, please refer to :ref:`Core feature: Vectored Interrupt <lab1_5>`.

In |nuclei_sdk|, ``gd32vf103_rvstar.h`` provides API to operate the on-board peripherals, ``gd32vf103_timer.h`` provides API to operate the SoC Timer, ``gd32vf103_exti.h`` provides API to operate the SoC EXTI, ``core_feature_eclic.h`` provides API to config ECLIC feature for Nuclei Core.

The code for this lab is located in ``nuclei-sdk/board-labs/rvstar/nesting_of_interrupts``. You can see it in the :ref:`appendix <appendix_lab1_4>`.
It can be divided into 6 parts:

* Part1 : Board config, initialize on-board RGB LED and user key
* Part2 : Timer config, set working mode of *TIMER1* and enable Timer update interrupt in SoC level
* Part3 : EXTI config, set *PA0* to used as EXTI source and enable EXTI interrupt in SoC level
* Part4 : ECLIC config, interrupt config in Processor level for each interrupt sources (interrupt enable, level and priority, non-vectored or vectored)
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

.. _figure_lab1_4_3:

.. figure:: /asserts/medias/lab1_4_fig3.jpg
   :width: 500
   :alt: lab1_4_fig3

   Connect with PC

.. note::
   When connect your board with the PC, just keep the on-board jumpers as default. About on-board jumpers' function, please refer to :ref:`Jumper Section <jumper>`.

2. Compile and run the ``nuclei-sdk/board-labs/rvstar/nesting_of_interrupts`` example. About the develop environment, you can choose |nuclei_sdk| or |ses|.

* |nuclei_sdk|

    Using the following commands:

    .. code-block:: shell

       cd /nuclei-sdk/board-labs/rvstar/nesting_of_interrupts
       make SOC=gd32vf103 BOARD=gd32vf103v_rvstar upload

    .. note::
        a. About the acquisition of source codes, please refer to :ref:`Software Source Codes <sw_code>`.

        b. About the detailed usage of |nuclei_sdk|, please refer to `How to develop with Nuclei SDK <http://doc.nucleisys.com/nuclei_sdk/quickstart.html>`_.       

* |ses|

    Using the following actions:

    .. _figure_lab1_4_4:

    .. figure:: /asserts/medias/lab1_4_fig4.jpg
       :width: 900
       :alt: lab1_4_fig4

       Operations in Embedded Studio

    .. note::
        a. About the acquisition of |ses| solutions, please refer to :ref:`Software Source Codes <sw_code>`.

        b. About the detailed usage of |ses|, please refer to `How to develop with Segger Embedded Studio <https://www.riscv-mcu.com/quickstart-quickstart-index-u-RV_STAR_PROJECT.html>`_.       

3. Check the status of on-board RGB LED, then press the on-board User Key(PA0-WKUP), and check the status of on-board RGB LED again.

.. _figure_lab1_4_5:

.. figure:: /asserts/medias/lab1_4_fig5.jpg
   :alt: lab1_4_fig5
   :width: 600

   Experimental results

**Exercises**

Try to create you own application which add more interrupt sources (such as internal interrupt source) to implement multi-level nesting of Interrupts.

.. _appendix_lab1_4:

**Appendix**

``rvstar/nesting_of_interrupts/main.c``

.. code-block:: c

    /**
        \brief      main function
        \param[in]  none
        \param[out] none
        \retval     none
      */
    int main(void)
    {
        uint8_t timer_intlevel=1;
        uint8_t exti_intlevel =2;
        int32_t returnCode;

        /* Board Config */
        gd_rvstar_led_init(LED3);
        gd_rvstar_led_init(LED1);
        gd_rvstar_led_init(LED2);

        gd_rvstar_key_init(WAKEUP_KEY_GPIO_PORT,KEY_MODE_EXTI);

        /* Timer Config */
        soc_timer_config();

        /* EXIT config */
        user_key_exti_config();

        /* ECLIC config */
        returnCode = ECLIC_Register_IRQ(EXTI0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                        ECLIC_LEVEL_TRIGGER, exti_intlevel, 0, NULL);
        returnCode = ECLIC_Register_IRQ(TIMER1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                        ECLIC_LEVEL_TRIGGER, timer_intlevel, 0, NULL);

        /* Enable interrupts in general */
        __enable_irq();

        /* Timer Start */
        timer_enable(TIMER1);

        /* RGB Control */
        while(1)
        {
          /* set led to RED */
          gd_rvstar_led_off(LED2);
          gd_rvstar_led_off(LED1);
          gd_rvstar_led_on(LED3);
        }

        return 0;
    }


    /**
        \brief      configure the TIMER peripheral
        \param[in]  none
        \param[out] none
        \retval     none
      */
    void soc_timer_config()
    {
        timer_parameter_struct timer_initpara;

        /* ----------------------------------------------------------------------------
        TIMER1 Configuration:
        TIMER1CLK = SystemCoreClock/54000 = 2KHz.
        TIMER1CAR = 20000
        ---------------------------------------------------------------------------- */
        rcu_periph_clock_enable(RCU_TIMER1);

        timer_deinit(TIMER1);

        timer_update_source_config(TIMER1, TIMER_UPDATE_SRC_REGULAR);

        /* initialize TIMER init parameter struct */
        timer_struct_para_init(&timer_initpara);
        /* TIMER1 configuration */
        timer_initpara.prescaler         = 53999;
        timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
        timer_initpara.counterdirection  = TIMER_COUNTER_UP;
        timer_initpara.period            = 20000;
        timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
        timer_init(TIMER1, &timer_initpara);

        timer_interrupt_enable(TIMER1, TIMER_INT_UP);

    }


    /**
        \brief      configure the EXTI peripheral for user key
        \param[in]  none
        \param[out] none
        \retval     none
      */
    void user_key_exti_config()
    {
        /* enable the AF clock */
        rcu_periph_clock_enable(RCU_AF);

        /* connect EXTI line to key GPIO pin */
        gpio_exti_source_select(WAKEUP_KEY_EXTI_PORT_SOURCE, WAKEUP_KEY_EXTI_PIN_SOURCE);

        /* configure key EXTI line */
        exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(EXTI_0);
    }

    /**
        \brief      EXTI line0 interrupt service routine
        \param[in]  none
        \param[out] none
        \retval     none
      */
    void EXTI0_IRQHandler()
    {

        if (SET == exti_interrupt_flag_get(WAKEUP_KEY_PIN)){

            if(RESET == gd_rvstar_key_state_get(KEY_WAKEUP)){

                /* clear EXTI lines interrupt flag */
                exti_interrupt_flag_clear(WAKEUP_KEY_PIN);

                /* set led to BLUE */
                gd_rvstar_led_off(LED3);
                gd_rvstar_led_off(LED1);
                gd_rvstar_led_on(LED2);
                delay_1ms(1000);
            }
        }

    }

    /**
        \brief      TIMER1 interrupt service routine
        \param[in]  none
        \param[out] none
        \retval     none
      */
    void TIMER1_IRQHandler()
    {
        uint16_t cnt;

        if(SET == timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_UP)){
          /* clear update interrupt bit */
          timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);

          for(cnt = 0; cnt < 5; cnt++)
          {
              /* set led to GREEN */
              gd_rvstar_led_off(LED3);
              gd_rvstar_led_off(LED2);
              gd_rvstar_led_on(LED1);
              delay_1ms(1000);
          }

        }
    }