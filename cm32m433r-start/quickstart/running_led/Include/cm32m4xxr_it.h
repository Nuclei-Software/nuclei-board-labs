/*******************************************************************************
*
* COPYRIGHT(c) 2020, China Mobile IOT
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*	1. Redistributions of source code must retain the above copyright notice,
*	   this list of conditions and the following disclaimer.
*	2. Redistributions in binary form must reproduce the above copyright notice,
*	   this list of conditions and the following disclaimer in the documentation
*	   and/or other materials provided with the distribution.
*	3. Neither the name of China Mobile IOT nor the names of its contributors
*	   may be used to endorse or promote products derived from this software
*	   without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*******************************************************************************/

/**
 * @file cm32m4xxr_it.h
 * @author CMIOT Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2020, CMIOT. All rights reserved.
 */
#ifndef __CM32M4xxR_IT_H__
#define __CM32M4xxR_IT_H__

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************/
/*                 N308 Processor Exceptions Handlers                         */
/*    Please uncommont the function declaration if used corresponding         */
/*                     peripheral interrupt.                                  */
/******************************************************************************/
void InsUnalign_EXCHandler(void); //TODO:..Need to change core_exception_handler() function's implementation if the exception and NMI functions have no argument.
//void InsAccFault_EXCHandler(void);
//void IlleIns_EXCHandler(void);
//void Break_EXCHandler(void);
//void LdAddrUnalign_EXCHandler(void);
//void LdFault_EXCHandler(void);
//void StAddrUnalign_EXCHandler(void);
//void StAccessFault_EXCHandler(void);
//void UmodeEcall_EXCHandler(void);
//void MmodeEcall_EXCHandler(void);

//void NMI_EXCHandler(void);

/******************************************************************************/
/*                  N308 Processor Interrupt Handlers                         */
/******************************************************************************/
void eclic_msip_handler(void);
void eclic_mtip_handler(void);

/******************************************************************************/
/*              CM32M4xxR Peripheral Interrupt Handlers                       */
/*    Please uncommont the function declaration if used corresponding         */
/*                     peripheral interrupt.                                  */
/******************************************************************************/
//void WWDG_IRQHandler(void);
//void PVD_IRQHandler(void);
//void TAMPER_IRQHandler(void);
//void RTC_IRQHandler(void);
//void FLASH_IRQHandler(void);
//void RCC_IRQHandler(void);
//void EXTI0_IRQHandler(void);
//void EXTI1_IRQHandler(void);
//void EXTI2_IRQHandler(void);
//void EXTI3_IRQHandler(void);
//void EXTI4_IRQHandler(void);
//void DMA1_Channel1_IRQHandler(void);
//void DMA1_Channel2_IRQHandler(void);
//void DMA1_Channel3_IRQHandler(void);
//void DMA1_Channel4_IRQHandler(void);
//void DMA1_Channel5_IRQHandler(void);
//void DMA1_Channel6_IRQHandler(void);
//void DMA1_Channel7_IRQHandler(void);
//void ADC1_2_IRQHandler(void);
//void USB_HP_CAN1_TX_IRQHandler(void);
//void USB_LP_CAN1_RX0_IRQHandler(void);
//void CAN1_RX1_IRQHandler(void);
//void CAN1_SCE_IRQHandler(void);
//void EXTI9_5_IRQHandler(void);
//void TIM1_BRK_IRQHandler(void);
//void TIM1_UP_IRQHandler(void);
//void TIM1_TRG_COM_IRQHandler(void);
//void TIM1_CC_IRQHandler(void);
//void TIM2_IRQHandler(void);
//void TIM3_IRQHandler(void);
//void TIM4_IRQHandler(void);
//void I2C1_EV_IRQHandler(void);
//void I2C1_ER_IRQHandler(void);
//void I2C2_EV_IRQHandler(void);
//void I2C2_ER_IRQHandler(void);
//void SPI1_IRQHandler(void);
//void SPI2_I2S2_IRQHandler(void);
//void USART1_IRQHandler(void);
//void USART2_IRQHandler(void);
//void USART3_IRQHandler(void);
//void EXTI15_10_IRQHandler(void);
//void RTCAlarm_IRQHandler(void);
//void TIM8_BRK_IRQHandler(void);
//void TIM8_UP_IRQHandler(void);
//void TIM8_TRG_COM_IRQHandler(void);
//void TIM8_CC_IRQHandler(void);
//void ADC3_4_IRQHandler(void);
//void TIM5_IRQHandler(void);
//void SPI3_I2S3_IRQHandler(void);
//void UART4_IRQHandler(void);
//void UART5_IRQHandler(void);
//void TIM6_IRQHandler(void);
//void TIM7_IRQHandler(void);
//void DMA2_Channel1_IRQHandler(void);
//void DMA2_Channel2_IRQHandler(void);
//void DMA2_Channel3_IRQHandler(void);
//void DMA2_Channel4_IRQHandler(void);
//void DMA2_Channel5_IRQHandler(void);
//void CAN2_TX_IRQHandler(void);
//void CAN2_RX0_IRQHandler(void);
//void CAN2_RX1_IRQHandler(void);
//void CAN2_SCE_IRQHandler(void);
//void QSPI_IRQHandler(void);
//void DMA2_Channel6_IRQHandler(void);
//void DMA2_Channel7_IRQHandler(void);
//void I2C3_EV_IRQHandler(void);
//void I2C3_ER_IRQHandler(void);
//void I2C4_EV_IRQHandler(void);
//void I2C4_ER_IRQHandler(void);
//void UART6_IRQHandler(void);
//void UART7_IRQHandler(void);
//void DMA1_Channel8_IRQHandler(void);
//void DMA2_Channel8_IRQHandler(void);
//void SAC_IRQHandler(void);
//void MMU_IRQHandler(void);
//void TSC_IRQHandler(void);
//void COMP_1_2_3_IRQHandler(void);
//void COMP_4_5_6_IRQHandler(void);
//void COMP7_IRQHandler(void);
//void R_SRAM_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __CM32M4xxR_IT_H__ */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
