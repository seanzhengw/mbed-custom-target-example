//
// Copyright (c) 2017, Sean Zheng
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#ifndef USE_USER_DEFINED_HAL_ETH_MSPINIT

#include "stm32f4xx_hal.h"

// ETH GPIO Configuration
// RMII_REF_CLK ----------------------> PA1
// RMII_MDIO -------------------------> PA2
// RMII_MDC --------------------------> PC1
// RMII_MII_CRS_DV -------------------> PA7
// RMII_MII_RXD0 ---------------------> PC4
// RMII_MII_RXD1 ---------------------> PC5
// RMII_MII_TX_EN --------------------> PG11
// RMII_MII_TXD0 ---------------------> PG13
// RMII_MII_TXD1 ---------------------> PG14

void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
    if (heth->Instance == ETH)
    {
        // Disable DCache
        SCB_DisableDCache();
        // Enable GPIO clocks
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();
        // Prepare GPIO InitStructure for GPIO_AF11_ETH
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStructure.Pull = GPIO_NOPULL;
        GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
        // Configure PA1, PA2, PA7
        GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
        // Configure PB13
        GPIO_InitStructure.Pin = GPIO_PIN_13;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
        // Configure PC1, PC4, PC5
        GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
        // Configure PG11, PG13, PG14
        GPIO_InitStructure.Pin =  GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
        // Enable Eth Interrupt
        HAL_NVIC_SetPriority(ETH_IRQn, 0x7, 0);
        HAL_NVIC_EnableIRQ(ETH_IRQn);
        // Enable ETH clock
        __HAL_RCC_ETH_CLK_ENABLE();
    }
}

void HAL_ETH_MspDeInit(ETH_HandleTypeDef *heth)
{
    if (heth->Instance == ETH)
    {
        // Disable ETH clock
        __HAL_RCC_ETH_CLK_DISABLE();
        // DeInit PA1, PA2, PA7
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7);
        // DeInit PB13
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13);
        // DeInit PC1, PC4, PC5
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5);
        // DeInit PG11, PG13, PG14
        HAL_GPIO_DeInit(GPIOG, GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14);
        // Disable Eth Interrupt
        NVIC_DisableIRQ(ETH_IRQn);
    }
}

#endif
