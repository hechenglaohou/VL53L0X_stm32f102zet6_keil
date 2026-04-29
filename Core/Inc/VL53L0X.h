#ifndef __VL5310X_H
#define __VL5310X_H

#include "main.h"
#include "i2c.h"

/* VL5310X I2C 地址（默认 0x52，7位地址，HAL库需左移1位） */
#define VL5310X_ADDR        0xA4  

/* VL5310X 核心寄存器 */
#define SYSRANGE_START      0x00
#define RESULT_RANGE_STATUS 0x14
#define RESULT_INTERRUPT_STATUS_GPIO 0x13
#define SYSTEM_MODE_GPIO1_HV 0x84
#define GPIO_HV_MUX_ACTIVE_HIGH 0x8F
#define SYSTEM_INTERRUPT_CLEAR 0x0B

/* 函数声明 */
HAL_StatusTypeDef VL5310X_Init(void);          // 传感器初始化
uint16_t VL5310X_GetDistance(void);            // 获取测距值(mm)
HAL_StatusTypeDef VL5310X_WriteReg(uint8_t reg, uint8_t data); // 写单个寄存器
HAL_StatusTypeDef VL5310X_ReadReg(uint8_t reg, uint8_t *data);  // 读单个寄存器

#endif