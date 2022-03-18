/*
 * Copyright (c) 2009-2018 Arm Limited. All rights reserved.
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/******************************************************************************
 * @file     system_cm32m4xxr.c
 * @brief    NMSIS Nuclei N/NX Device Peripheral Access Layer Source File for
 *           Device CM32M4xxR
 * @version  V1.00
 * @date     29. Jun 2020
 ******************************************************************************/

#include "cm32m4xxr.h"
#include "cm32m4xxr_def.h"
#include "cm32m4xxr_conf.h"

#include <stddef.h>
#include <stdio.h>

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define SYSTEM_CORE_CLOCK    (144000000) /* System clock freqency */

/*----------------------------------------------------------------------------
  Define system clock source
 *----------------------------------------------------------------------------*/
#define SYSCLK_USE_HSI			0		/* Use HSI clock as SYSCLK 		*/
#define SYSCLK_USE_HSE			1		/* Use HSE clock as SYSCLK 		*/
#define SYSCLK_USE_HSI_PLL		2		/* Use HSI_PLL clock as SYSCLK 	*/
#define SYSCLK_USE_HSE_PLL		3		/* Use HSE_PLL clock as SYSCLK 	*/

/* Uncomment the line corresponding to the desired System clock (SYSCLK)
   frequency (after reset the HSI is used as SYSCLK source)

   IMPORTANT NOTE:
   ==============
   1. After each device reset the HSI is used as System clock source.

   2. Please make sure that the selected System clock doesn't exceed your
   device's maximum frequency.

   3. If none of the define below is enabled, the HSI is used as System clock
    source.

   4. The System clock configuration functions provided within this file assume
   that:
        - For Low, Medium and High density Value line devices an external 8MHz
          crystal is used to drive the System clock.
        - For Low, Medium and High density devices an external 8MHz crystal is
          used to drive the System clock.
        - For Connectivity line devices an external 25MHz crystal is used to
   drive the System clock. If you are using different crystal you have to adapt
   those functions accordingly.
*/

//#define SYSCLK_SRC	SYSCLK_USE_HSI
//#define SYSCLK_SRC	SYSCLK_USE_HSE
//#define SYSCLK_SRC	SYSCLK_USE_HSI_PLL
#define SYSCLK_SRC	SYSCLK_USE_HSE_PLL


#if SYSCLK_SRC == SYSCLK_USE_HSI

#if SYSTEM_CORE_CLOCK != HSI_VALUE
#error SYSCL_FREQ must be set to HSI_VALUE
#endif

#elif SYSCLK_SRC == SYSCLK_USE_HSE

#ifndef HSE_VALUE
#error HSE_VALUE must be defined!
#endif

#if SYSTEM_CORE_CLOCK != HSE_VALUE
#error SYSCL_FREQ must be set to HSE_VALUE
#endif

#elif SYSCLK_SRC == SYSCLK_USE_HSI_PLL	/* When use HSI_PLL as SYSCLK, the PLLSRC should be HSI/2 and PLLMULFCT should be 2~32 */

/* Make sure that the HSI is suitable for SYSCLK */
#if (SYSTEM_CORE_CLOCK % (HSI_VALUE / 2) == 0) && (SYSTEM_CORE_CLOCK / (HSI_VALUE / 2) >= 2)                                       \
    && (SYSTEM_CORE_CLOCK / (HSI_VALUE / 2) <= 32)

#define PLLSRC_DIV 2
#define PLL_MUL    (SYSTEM_CORE_CLOCK / (HSI_VALUE / 2))

#else
#error Cannot make a PLL multiply factor to SYSTEM_CORE_CLOCK.
#endif

#elif SYSCLK_SRC == SYSCLK_USE_HSE_PLL	/* When use HSE_PLL as SYSCLK, the PLLSRC should be HSE or HSE/2, and PLLMULFCT should be 2~32 */

#ifndef HSE_VALUE
#error HSE_VALUE must be defined!
#endif

/* Make sure that the HSE is suitable for SYSCLK */
#if ((SYSTEM_CORE_CLOCK % (HSE_VALUE / 2)) == 0) && (SYSTEM_CORE_CLOCK / (HSE_VALUE / 2) >= 2)                                     \
    && (SYSTEM_CORE_CLOCK / (HSE_VALUE / 2) <= 32)

#define PLLSRC_DIV 2
#define PLL_MUL    (SYSTEM_CORE_CLOCK / (HSE_VALUE / 2))

/* Make sure that the HSE is suitable for SYSCLK */
#elif (SYSTEM_CORE_CLOCK % HSE_VALUE == 0) && (SYSTEM_CORE_CLOCK / HSE_VALUE >= 2) && (SYSTEM_CORE_CLOCK / HSE_VALUE <= 32)

#define PLLSRC_DIV 1
#define PLL_MUL    (SYSTEM_CORE_CLOCK / HSE_VALUE)

#else
#error Cannot make a PLL multiply factor to SYSTEM_CORE_CLOCK.
#endif

#else
#error wrong value for SYSCLK_SRC
#endif

/**
 * \defgroup  NMSIS_Core_SystemConfig       System Device Configuration
 * \brief Functions for system init, clock setup and interrupt/exception/nmi functions available in system_<device>.c.
 * \details
 * Nuclei provides a template file **system_Device.c** that must be adapted by
 * the silicon vendor to match their actual device. As a <b>minimum requirement</b>,
 * this file must provide:
 *  -  A device-specific system configuration function, \ref SystemInit.
 *  -  A global variable that contains the system frequency, \ref SystemCoreClock.
 *  -  A global eclic configuration initialization, \ref ECLIC_Init.
 *  -  Global \ref _premain_init and \ref _postmain_fini functions called right before calling main function.
 *  -  Vendor customized interrupt, exception and nmi handling code, see \ref NMSIS_Core_IntExcNMI_Handling
 *
 * The file configures the device and, typically, initializes the oscillator (PLL) that is part
 * of the microcontroller device. This file might export other functions or variables that provide
 * a more flexible configuration of the microcontroller system.
 *
 * And this file also provided common interrupt, exception and NMI exception handling framework template,
 * Silicon vendor can customize these template code as they want.
 *
 * \note Please pay special attention to the static variable \c SystemCoreClock. This variable might be
 * used throughout the whole system initialization and runtime to calculate frequency/time related values.
 * Thus one must assure that the variable always reflects the actual system clock speed.
 *
 * \note \ref _init function and \ref _fini function are now deprecated, but still need to provide a empty implementation
 * here, due to newlibc still need it. Please put your pre- and post- main steps in \ref _premain_init and \ref _postmain_fini
 *
 * \attention
 * Be aware that a value stored to \c SystemCoreClock during low level initialization (i.e. \c SystemInit()) might get
 * overwritten by C libray startup code and/or .bss section initialization.
 * Thus its highly recommended to call \ref SystemCoreClockUpdate at the beginning of the user \c main() routine.
 *
 * @{
 */

/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/
/**
 * \brief      Variable to hold the system core clock value
 * \details
 * Holds the system core clock, which is the system clock frequency supplied to the SysTick
 * timer and the processor core clock. This variable can be used by debuggers to query the
 * frequency of the debug timer or to configure the trace clock speed.
 *
 * \attention
 * Compilers must be configured to avoid removing this variable in case the application
 * program is not using it. Debugging systems require the variable to be physically
 * present in memory so that it can be examined to configure the debugger.
 */
uint32_t SystemCoreClock = SYSTEM_CORE_CLOCK;  /* System Clock Frequency (Core Clock) */

const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

#ifdef DATA_IN_ExtSRAM
static void SystemInit_ExtMemCtl(void);
#endif /* DATA_IN_ExtSRAM */

/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/

/**
 * @brief  Configures the System clock frequency, HCLK, PCLK2 and PCLK1
 * prescalers.
 */
static void SystemClockInit(void)
{
    uint32_t rcc_cfgr       = 0;
    bool HSEStatus          = 0;
    uint32_t StartUpCounter = 0;
    uint32_t SystimerCtrl   = 0;

#if SYSCLK_SRC == SYSCLK_USE_HSE || SYSCLK_SRC == SYSCLK_USE_HSE_PLL

    /* Enable HSE */
    RCC->CTRL |= ((uint32_t) RCC_CTRL_HSEEN);

	/* Wait till HSE is ready and if Time out is reached exit */
    do{
        HSEStatus = RCC->CTRL & RCC_CTRL_HSERDF;
        StartUpCounter++;
    } while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    HSEStatus = ((RCC->CTRL & RCC_CTRL_HSERDF) != RESET);
    if (!HSEStatus){
        /* If HSE fails to start-up, the application will have wrong clock
         * configuration(and will use HSI as SYSCLK). User can add here some code to deal with this error.
		 * Warn: Can not use SystemCoreClock here(like "SystemCoreClock = HSI_VALUE"),
		 * because this function is called before reaching pre-main. */
		 
        return;
    }
#endif

    /* Flash wait state
        0: HCLK <= 32M
        1: HCLK <= 64M
        2: HCLK <= 96M
        3: HCLK <= 128M
        4: HCLK <= 144M
     */
    FLASH->AC &= (uint32_t)((uint32_t)~FLASH_AC_LATENCY);
    FLASH->AC |= (uint32_t)((SYSTEM_CORE_CLOCK - 1) / 32000000);

    /* HCLK = SYSCLK */
    RCC->CFG |= (uint32_t)RCC_CFG_AHBPRES_DIV1;

    /* PCLK2 max 72M */
    if (SYSTEM_CORE_CLOCK > 72000000){
        RCC->CFG |= (uint32_t)RCC_CFG_APB2PRES_DIV2;
    }
    else{
        RCC->CFG |= (uint32_t)RCC_CFG_APB2PRES_DIV1;
    }

    /* PCLK1 max 36M */
    if (SYSTEM_CORE_CLOCK > 72000000){
        RCC->CFG |= (uint32_t)RCC_CFG_APB1PRES_DIV4;
    }
    else if (SYSTEM_CORE_CLOCK > 36000000){
        RCC->CFG |= (uint32_t)RCC_CFG_APB1PRES_DIV2;
    }
    else{
        RCC->CFG |= (uint32_t)RCC_CFG_APB1PRES_DIV1;
    }

#if SYSCLK_SRC == SYSCLK_USE_HSE
    /* Select HSE as system clock source */
    RCC->CFG &= (uint32_t)((uint32_t) ~(RCC_CFG_SCLKSW));
    RCC->CFG |= (uint32_t)RCC_CFG_SCLKSW_HSE;

    /* Wait till HSE is used as system clock source */
    while ((RCC->CFG & (uint32_t)RCC_CFG_SCLKSTS) != (uint32_t)0x04){
		
    }
#elif SYSCLK_SRC == SYSCLK_USE_HSI_PLL || SYSCLK_SRC == SYSCLK_USE_HSE_PLL

    /* clear bits */
    RCC->CFG &= (uint32_t)((uint32_t) ~(RCC_CFG_PLLSRC | RCC_CFG_PLLHSEPRES | RCC_CFG_PLLMULFCT));

    /* set PLL source */
    rcc_cfgr = RCC->CFG;
    rcc_cfgr |= (SYSCLK_SRC == SYSCLK_USE_HSI_PLL ? RCC_CFG_PLLSRC_HSI_DIV2 : RCC_CFG_PLLSRC_HSE);

#if SYSCLK_SRC == SYSCLK_USE_HSE_PLL
    rcc_cfgr |= (PLLSRC_DIV == 1 ? RCC_CFG_PLLHSEPRES_HSE : RCC_CFG_PLLHSEPRES_HSE_DIV2);
#endif

    /* set PLL multiply factor */
#if PLL_MUL <= 16
    rcc_cfgr |= (PLL_MUL - 2) << 18;
#else
    rcc_cfgr |= ((PLL_MUL - 17) << 18) | (1 << 27);
#endif

    RCC->CFG = rcc_cfgr;

    /* Enable PLL */
    RCC->CTRL |= RCC_CTRL_PLLEN;

    /* Wait till PLL is ready */
    while ((RCC->CTRL & RCC_CTRL_PLLRDF) == 0){
		
    }

    /* Select PLL as system clock source */
    RCC->CFG &= (uint32_t)((uint32_t) ~(RCC_CFG_SCLKSW));
    RCC->CFG |= (uint32_t)RCC_CFG_SCLKSW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFG & (uint32_t)RCC_CFG_SCLKSTS) != (uint32_t)0x08){
		
    }
#endif

    /* use HCLK as system timer clock */
    SystimerCtrl = SysTimer_GetControlValue();
    SysTimer_SetControlValue(SystimerCtrl | SysTimer_MTIMECTL_CLKSRC_Msk);
}

/**
 * @brief  Update SystemCoreClock variable according to Clock Register Values.
 *         The SystemCoreClock variable contains the core clock (HCLK), it can
 *         be used by the user application to setup the SysTick timer or
 * configure other parameters.
 *
 * @note   Each time the core clock (HCLK) changes, this function must be called
 *         to update SystemCoreClock variable value. Otherwise, any
 * configuration based on this variable will be incorrect.
 *
 * @note   - The system frequency computed by this function is not the real
 *           frequency in the chip. It is calculated based on the predefined
 *           constant and the selected clock source:
 *
 *           - If SYSCLK source is HSI, SystemCoreClock will contain the
 * HSI_VALUE(*)
 *
 *           - If SYSCLK source is HSE, SystemCoreClock will contain the
 * HSE_VALUE(**)
 *
 *           - If SYSCLK source is PLL, SystemCoreClock will contain the
 * HSE_VALUE(**) or HSI_VALUE(*) multiplied by the PLL factors.
 *
 *         (*) HSI_VALUE is a constant defined in cm32m4xxr_conf.h file (default value
 *             8 MHz) but the real value may vary depending on the variations
 *             in voltage and temperature.
 *
 *         (**) HSE_VALUE is a constant defined in cm32m4xxr_conf.h file (default value
 *              8 MHz or 25 MHz, depedning on the product used), user has to
 * ensure that HSE_VALUE is same as the real frequency of the crystal used.
 *              Otherwise, this function may have wrong result.
 *
 *         - The result of this function could be not correct when using
 * fractional value for HSE crystal.
 */
void SystemCoreClockUpdate(void)
{
    uint32_t tmp		= 0;
	uint32_t pllmull	= 0;
	uint32_t pllsource	= 0;

	/* Get SYSCLK source */
    tmp = RCC->CFG & RCC_CFG_SCLKSTS;

    switch (tmp)
    {
    case 0x00: /* HSI used as system clock */
        SystemCoreClock = HSI_VALUE;
        break;
    case 0x04: /* HSE used as system clock */
        SystemCoreClock = HSE_VALUE;
        break;
    case 0x08: /* PLL used as system clock */

        /* Get PLL clock source and multiplication factor */
        pllmull   = RCC->CFG & RCC_CFG_PLLMULFCT;
        pllsource = RCC->CFG & RCC_CFG_PLLSRC;

        if ((pllmull & RCC_CFG_PLLMULFCT_4) == 0){
            pllmull = (pllmull >> 18) + 2; /* PLLMUL[4] = 0 */
        }
        else{
            pllmull = ((pllmull >> 18) - 512 + 16) + 1; /* PLLMUL[4] = 1 */
        }

        if (pllsource == 0x00){
            /* HSI oscillator clock divided by 2 selected as PLL clock entry */
            SystemCoreClock = (HSI_VALUE >> 1) * pllmull;
        }
        else{
            /* HSE selected as PLL clock entry */
            if ((RCC->CFG & RCC_CFG_PLLHSEPRES) != (uint32_t)RESET){
				/* HSE oscillator clock divided by 2 */
                SystemCoreClock = (HSE_VALUE >> 1) * pllmull;
            }
            else{
                SystemCoreClock = HSE_VALUE * pllmull;
            }
        }

        break;

    default:
        SystemCoreClock = HSI_VALUE;
        break;
    }

    /* Compute HCLK clock frequency */
    /* Get HCLK prescaler */
    tmp = AHBPrescTable[((RCC->CFG & RCC_CFG_AHBPRES) >> 4)];
	
    /* HCLK clock frequency */
    SystemCoreClock >>= tmp;
}

/**
 * \brief      Function to Initialize the system.
 * \details
 * Initializes the microcontroller system. Typically, this function configures the
 * oscillator (PLL) that is part of the microcontroller device. For systems
 * with a variable clock speed, it updates the variable \ref SystemCoreClock.
 * SystemInit is called from the file <b>startup<i>_device</i></b>.
 */
void SystemInit (void)
{
    /* Warn: do not use global variables because this function is called before
     * reaching pre-main. RW section maybe overwritten afterwards.
     */

	/* FPU settings
	* ------------------------------------------------------------*/
#if defined(__FPU_PRESENT) && (__FPU_PRESENT > 0)
    /* FPU is enabled during startup */
#endif

    /* Reset the RCC clock configuration to the default reset state(for debug purpose) */
    /* Set HSIEN bit */
    RCC->CTRL |= (uint32_t)0x00000001;

    /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
    RCC->CFG &= (uint32_t)0xF8FFC000;

    /* Reset HSEON, CLKSSEN and PLLEN bits */
    RCC->CTRL &= (uint32_t)0xFEF6FFFF;

    /* Reset HSEBYP bit */
    RCC->CTRL &= (uint32_t)0xFFFBFFFF;

    /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRES/OTGFSPRE bits */
    RCC->CFG &= (uint32_t)0xF700FFFF;

    /* Reset CFG2 register */
    RCC->CFG2 = 0x00000000;

    /* Reset CFG3 register */
    RCC->CFG3 = 0x00000000;

    /* Disable all interrupts and clear pending bits  */
    RCC->CLKINT = 0x009F0000;

    /* Enable ex mode */
    RCC->APB1PCLKEN |= RCC_APB1PCLKEN_PWREN;
    PWR->CTRL3 |= 0x00000001;
    RCC->APB1PCLKEN &= (uint32_t)(~RCC_APB1PCLKEN_PWREN);

    /* Enable ICACHE and Prefetch Buffer */
    FLASH->AC |= (uint32_t)(FLASH_AC_ICAHEN | FLASH_AC_PRFTBFE);

#ifdef DATA_IN_ExtSRAM
    SystemInit_ExtMemCtl();
#endif /* DATA_IN_ExtSRAM */

    /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
    /* Configure the Flash Latency cycles and enable prefetch buffer */
    SystemClockInit();
}

/**
 * \defgroup  NMSIS_Core_IntExcNMI_Handling   Interrupt and Exception and NMI Handling
 * \brief Functions for interrupt, exception and nmi handle available in system_<device>.c.
 * \details
 * Nuclei provide a template for interrupt, exception and NMI handling. Silicon Vendor could adapat according
 * to their requirement. Silicon vendor could implement interface for different exception code and
 * replace current implementation.
 *
 * @{
 */
/** \brief Max exception handler number, don't include the NMI(0xFFF) one */
#define MAX_SYSTEM_EXCEPTION_NUM        12

/** 
 * \brief      Store the exception handlers for each exception ID
 * \note
 * - This SystemExceptionHandlers are used to store all the handlers for all
 * the exception codes Nuclei N/NX core provided.
 * - Exception code 0 - 11, totally 12 exceptions are mapped to SystemExceptionHandlers[0:11]
 * - Exception for NMI is also re-routed to exception handling(exception code 0xFFF) in startup code configuration, the handler itself is mapped to SystemExceptionHandlers[MAX_SYSTEM_EXCEPTION_NUM] 
 */
static unsigned long SystemExceptionHandlers[MAX_SYSTEM_EXCEPTION_NUM + 1];

/**
 * \brief      Exception Handler Function Typedef
 * \note
 * This typedef is only used internal in this system_cm32m4xxr.c file.
 * It is used to do type conversion for registered exception handler before calling it.
 */
typedef	void (*EXC_HANDLER) (unsigned long mcause, unsigned long sp);

/**
 * \brief      System Default Exception Handler
 * \details
 * This function provided a default exception and NMI handling code for all exception ids.
 * By default, It will just print some information for debug, Vendor can customize it according to its requirements.
 */
static void system_default_exception_handler(unsigned long mcause, unsigned long sp)
{
    /* TODO: Uncomment this if you have implement printf function.
     * Or you can implement your own version as you like */
    printf("MDCAUSE: 0x%lx\r\n", __RV_CSR_READ(CSR_MDCAUSE));
    printf("MTVAL : 0x%lx\r\n", __RV_CSR_READ(CSR_MBADADDR));
    Exception_DumpFrame(sp);
    while(1);
}

/** 
 * \brief      Initialize all the default core exception handlers
 * \details
 * The core exception handler for each exception id will be initialized to \ref system_default_exception_handler.
 * \note
 * Called in \ref _init function, used to initialize default exception handlers for all exception IDs
 */
static void Exception_Init(void)
{
    for (int i = 0; i < MAX_SYSTEM_EXCEPTION_NUM + 1; i++) {
        SystemExceptionHandlers[i] = (unsigned long)system_default_exception_handler;
    }
}

/**
 * \brief      Dump Exception Frame
 * \details
 * This function provided feature to dump exception frame stored in stack.
 */
void Exception_DumpFrame(unsigned long sp)
{
    EXC_Frame_Type *exc_frame = (EXC_Frame_Type *)sp;

#ifndef __riscv_32e
    printf("ra: 0x%x, tp: 0x%x, t0: 0x%x, t1: 0x%x, t2: 0x%x, t3: 0x%x, t4: 0x%x, t5: 0x%x, t6: 0x%x\n" \
           "a0: 0x%x, a1: 0x%x, a2: 0x%x, a3: 0x%x, a4: 0x%x, a5: 0x%x, a6: 0x%x, a7: 0x%x\n" \
           "mcause: 0x%x, mepc: 0x%x, msubm: 0x%x\n", exc_frame->ra, exc_frame->tp, exc_frame->t0, \
           exc_frame->t1, exc_frame->t2, exc_frame->t3, exc_frame->t4, exc_frame->t5, exc_frame->t6, \
           exc_frame->a0, exc_frame->a1, exc_frame->a2, exc_frame->a3, exc_frame->a4, exc_frame->a5, \
           exc_frame->a6, exc_frame->a7, exc_frame->mcause, exc_frame->mepc, exc_frame->msubm);
#else
    printf("ra: 0x%x, tp: 0x%x, t0: 0x%x, t1: 0x%x, t2: 0x%x\n" \
           "a0: 0x%x, a1: 0x%x, a2: 0x%x, a3: 0x%x, a4: 0x%x, a5: 0x%x\n" \
           "mcause: 0x%x, mepc: 0x%x, msubm: 0x%x\n", exc_frame->ra, exc_frame->tp, exc_frame->t0, \
           exc_frame->t1, exc_frame->t2, exc_frame->a0, exc_frame->a1, exc_frame->a2, exc_frame->a3, \
           exc_frame->a4, exc_frame->a5, exc_frame->mcause, exc_frame->mepc, exc_frame->msubm);
#endif
}

/**
 * \brief       Register an exception handler for exception code EXCn
 * \details
 * * For EXCn < \ref MAX_SYSTEM_EXCEPTION_NUM, it will be registered into SystemExceptionHandlers[EXCn-1].
 * * For EXCn == NMI_EXCn, it will be registered into SystemExceptionHandlers[MAX_SYSTEM_EXCEPTION_NUM].
 * \param   EXCn    See \ref EXCn_Type
 * \param   exc_handler     The exception handler for this exception code EXCn
 */
void Exception_Register_EXC(uint32_t EXCn, unsigned long exc_handler)
{
    if ((EXCn < MAX_SYSTEM_EXCEPTION_NUM) && (EXCn >= 0)) {
        SystemExceptionHandlers[EXCn] = exc_handler;
    } else if (EXCn == NMI_EXCn) {
        SystemExceptionHandlers[MAX_SYSTEM_EXCEPTION_NUM] = exc_handler;
    }
}

/**
 * \brief       Get current exception handler for exception code EXCn
 * \details
 * * For EXCn < \ref MAX_SYSTEM_EXCEPTION_NUM, it will return SystemExceptionHandlers[EXCn-1].
 * * For EXCn == NMI_EXCn, it will return SystemExceptionHandlers[MAX_SYSTEM_EXCEPTION_NUM].
 * \param   EXCn    See \ref EXCn_Type
 * \return  Current exception handler for exception code EXCn, if not found, return 0.
 */
unsigned long Exception_Get_EXC(uint32_t EXCn)
{
    if ((EXCn < MAX_SYSTEM_EXCEPTION_NUM) && (EXCn >= 0)) {
        return SystemExceptionHandlers[EXCn];
    } else if (EXCn == NMI_EXCn) {
        return SystemExceptionHandlers[MAX_SYSTEM_EXCEPTION_NUM];
    } else {
        return 0;
    }
}

/**
 * \brief      Common NMI and Exception handler entry
 * \details
 * This function provided a command entry for NMI and exception. Silicon Vendor could modify
 * this template implementation according to requirement.
 * \remarks
 * - RISCV provided common entry for all types of exception. This is proposed code template
 *   for exception entry function, Silicon Vendor could modify the implementation.
 * - For the core_exception_handler template, we provided exception register function \ref Exception_Register_EXCn
 *   which can help developer to register your exception handler for specific exception number.
 */
uint32_t core_exception_handler(unsigned long mcause, unsigned long sp)
{
    uint32_t EXCn = (uint32_t)(mcause & 0X00000fff);
    EXC_HANDLER exc_handler;

    if ((EXCn < MAX_SYSTEM_EXCEPTION_NUM) && (EXCn >= 0)) {
        exc_handler = (EXC_HANDLER)SystemExceptionHandlers[EXCn];
    } else if (EXCn == NMI_EXCn) {
        exc_handler = (EXC_HANDLER)SystemExceptionHandlers[MAX_SYSTEM_EXCEPTION_NUM];
    } else {
        exc_handler = (EXC_HANDLER)system_default_exception_handler;
    }

    if (exc_handler != NULL) {
        exc_handler(mcause, sp);
    }
	
	return 0;
}

/**
 * \brief Initialize Global ECLIC Config
 * \details
 * ECLIC needs be initialized after boot up,
 * Vendor could also change the initialization
 * configuration.
 */
void ECLIC_Init(void)
{
    /* Global Configuration about MTH and NLBits.
     * TODO: Please adapt it according to your system requirement. 
     * This function is called in _init function */
	 
    /* Set CSR MTH to zero */
    ECLIC_SetMth(0);
	
    /* Set Nlbits to the CLICINTCTLBITS, all the bits are level bits */
    ECLIC_SetCfgNlbits(__ECLIC_INTCTLBITS);
}

/**
 * \brief  Initialize a specific IRQ and register the handler
 * \details
 * This function set vector mode, trigger mode and polarity, interrupt level and priority,
 * assign handler for specific IRQn.
 * \param [in]  IRQn        NMI interrupt handler address
 * \param [in]  shv         \ref ECLIC_NON_VECTOR_INTERRUPT means non-vector mode, and \ref ECLIC_VECTOR_INTERRUPT is vector mode
 * \param [in]  trig_mode   see \ref ECLIC_TRIGGER_Type
 * \param [in]  lvl         interupt level
 * \param [in]  priority    interrupt priority
 * \param [in]  handler     interrupt handler, if NULL, handler will not be installed
 *
 * \return       -1 means invalid input parameter. 0 means successful.
 * \remarks
 * - This function use to configure specific eclic interrupt and register its interrupt handler and enable its interrupt.
 * - If the vector table is placed in read-only section(FLASHXIP mode), handler could not be installed
 */
int32_t ECLIC_Register_IRQ(IRQn_Type IRQn, uint8_t shv, ECLIC_TRIGGER_Type trig_mode, uint8_t lvl, uint8_t priority, void *handler)
{
    if ((IRQn > SOC_INT_MAX) || (shv > ECLIC_VECTOR_INTERRUPT) \
        || (trig_mode > ECLIC_NEGTIVE_EDGE_TRIGGER )) {
        return -1;
    }

    /* set interrupt vector mode */
    ECLIC_SetShvIRQ(IRQn, shv);
	
    /* set interrupt trigger mode and polarity */
    ECLIC_SetTrigIRQ(IRQn, trig_mode);
	
    /* set interrupt level */
    ECLIC_SetLevelIRQ(IRQn, lvl);
	
    /* set interrupt priority */
    ECLIC_SetPriorityIRQ(IRQn, priority);
	
    if (handler != NULL) {
        /* set interrupt handler entry to vector table */
        ECLIC_SetVector(IRQn, (rv_csr_t)handler);
    }
	
    /* enable interrupt */
    ECLIC_EnableIRQ(IRQn);
	
    return 0;
}
/** @} */ /* End of Doxygen Group NMSIS_Core_ExceptionAndNMI */

/**
 * \brief early init function before main
 * \details
 * This function is executed right before main function.
 * For RISC-V gnu toolchain, _init function might not be called
 * by __libc_init_array function, so we defined a new function
 * to do initialization
 */
void _premain_init(void)
{
    /* TODO: Add your own initialization code here, called before main  */
    /* __ICACHE_PRESENT and __DCACHE_PRESENT are defined in <Device>.h */
#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT == 1
    EnableICache();
#endif
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    EnableDCache();
#endif
	
	SystemCoreClockUpdate(); /* Get SystemCoreClock value */

    //TODO: Add code to initialize necessary gpio and basic uart for debug print
	
    /* Initialize exception default handlers */
    Exception_Init();
    /* ECLIC initialization, mainly MTH and NLBIT settings */
    ECLIC_Init();
}

/**
 * \brief finish function after main
 * \param [in]  status     status code return from main
 * \details
 * This function is executed right after main function.
 * For RISC-V gnu toolchain, _fini function might not be called
 * by __libc_fini_array function, so we defined a new function
 * to do initialization
 */
void _postmain_fini(int status)
{
    /* TODO: Add your own finishing code here, called after main */
}

/**
 * \brief _init function called in __libc_init_array()
 * \details
 * This `__libc_init_array()` function is called during startup code,
 * user need to implement this function, otherwise when link it will
 * error init.c:(.text.__libc_init_array+0x26): undefined reference to `_init'
 * \note
 * Please use \ref _premain_init function now
 */
void _init(void)
{
    /* Don't put any code here, please use _premain_init now */
}

/**
 * \brief _fini function called in __libc_fini_array()
 * \details
 * This `__libc_fini_array()` function is called when exit main.
 * user need to implement this function, otherwise when link it will
 * error fini.c:(.text.__libc_fini_array+0x28): undefined reference to `_fini'
 * \note
 * Please use \ref _postmain_fini function now
 */
void _fini(void)
{
    /* Don't put any code here, please use _postmain_fini now */
}

/** @} */ /* End of Doxygen Group NMSIS_Core_SystemAndClock */
