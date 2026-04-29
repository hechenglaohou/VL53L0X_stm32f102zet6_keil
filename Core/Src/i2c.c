/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "i2c.h"
/* USER CODE BEGIN 0 */
#include "VL53L0X.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
}


/**
 * @brief  VL5310X 写单个寄存器
 * @param  reg: 寄存器地址
 * @param  data: 写入数据
 * @retval HAL状态
 */
HAL_StatusTypeDef VL5310X_WriteReg(uint8_t reg, uint8_t data)
{
  return HAL_I2C_Mem_Write(&hi2c1, VL5310X_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
}

/**
 * @brief  VL5310X 读单个寄存器
 * @param  reg: 寄存器地址
 * @param  data: 读取数据缓冲区
 * @retval HAL状态
 */
HAL_StatusTypeDef VL5310X_ReadReg(uint8_t reg, uint8_t *data)
{
  return HAL_I2C_Mem_Read(&hi2c1, VL5310X_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, 100);
}

/**
 * @brief  VL5310X 多字节写（可选扩展）
 * @param  reg: 起始寄存器地址
 * @param  pData: 数据缓冲区
 * @param  len: 数据长度
 * @retval HAL状态
 */
HAL_StatusTypeDef VL5310X_WriteMultiReg(uint8_t reg, uint8_t *pData, uint16_t len)
{
  return HAL_I2C_Mem_Write(&hi2c1, VL5310X_ADDR, reg, I2C_MEMADD_SIZE_8BIT, pData, len, 100);
}

/**
 * @brief  VL5310X 多字节读（可选扩展）
 * @param  reg: 起始寄存器地址
 * @param  pData: 数据缓冲区
 * @param  len: 数据长度
 * @retval HAL状态
 */
HAL_StatusTypeDef VL5310X_ReadMultiReg(uint8_t reg, uint8_t *pData, uint16_t len)
{
  return HAL_I2C_Mem_Read(&hi2c1, VL5310X_ADDR, reg, I2C_MEMADD_SIZE_8BIT, pData, len, 100);
}
/* USER CODE END 0 */
