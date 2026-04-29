#include "main.h"
#include "i2c.h"
#include "usart.h"  
#include "stdio.h"
// 传感器I2C地址（注意：0x52是7位地址左移1位后的结果，原始7位地址是0x29）
#define VL53L0X_ADDR  0x52

// XSHUT引脚定义（与gpio.c中PA5复用，需注意：LED也用PA5，建议后续硬件分开）
#define XSHUT_PORT    GPIOB       
#define XSHUT_PIN     GPIO_PIN_5  

// 带【复位I2C】的读写函数，永不死机！
uint8_t VL_Read(uint8_t reg, uint8_t *pData)
{
  // 先复位 I2C 防止死机
  if(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
  {
    HAL_I2C_DeInit(&hi2c1);
    HAL_Delay(10);
    MX_I2C1_Init();
  }

  HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(&hi2c1, 0x52, reg, 1, pData, 1, 200);
  if(ret == HAL_OK) return 0;
  else return 1;
}


// VL53L0X.c 中修改VL_Read/VL_Write


uint8_t VL_Write(uint8_t reg, uint8_t data)
{
  if(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
  {
    HAL_I2C_DeInit(&hi2c1);
    HAL_Delay(10);
    MX_I2C1_Init();
  }

  HAL_StatusTypeDef ret = HAL_I2C_Mem_Write(&hi2c1, 0x52, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 500);
  if(ret != HAL_OK)
  {
    printf("I2C写失败: reg=0x%02X, data=0x%02X, ret=%d\r\n", reg, data, ret);
    return 1;
  }
  return 0;
}



HAL_StatusTypeDef VL5310X_Init(void)
{
  uint8_t temp;

  printf("\r\n========= VL5310X 官方标准初始化 =========\r\n");
  HAL_Delay(800);

  // 读ID确认
  VL_Read(0xC0, &temp);
  printf("[传感器ID] 0xC0 = 0x%02X\r\n", temp);

  // ==============================
  // 【官方完整初始化】
  // ==============================
  VL_Write(0x88, 0x00);
  HAL_Delay(10);

  VL_Write(0x80, 0x01);
  VL_Write(0xFF, 0x01);
  VL_Write(0x00, 0x00);
  VL_Write(0xFF, 0x00);
  VL_Write(0x80, 0x00);

  // 配置必须的寄存器
  VL_Write(0x0B, 0x00);
  VL_Write(0x0A, 0x00);

  VL_Write(0x03, 0x01);  // 单次测距
  VL_Write(0x60, 0x05);
  VL_Write(0x50, 0x28);
  VL_Write(0x52, 0x10);
  VL_Write(0x61, 0x00);
  VL_Write(0x62, 0x00);
  VL_Write(0x44, 0x01);
  VL_Write(0x45, 0x00);
  VL_Write(0x46, 0x00);
  VL_Write(0x64, 0x00);
  VL_Write(0x65, 0x00);
  VL_Write(0x29, 0x05);
  VL_Write(0x2A, 0x05);
  VL_Write(0x31, 0x04);

  VL_Write(0x32, 0x01);  // 开启测距

  HAL_Delay(300);
  printf("========= 初始化完成，开始测距！=========\r\n");

  return HAL_OK;
}

// VL53L0X.c 中重写VL5310X_GetDistance
uint16_t VL5310X_GetDistance(void)
{
  uint8_t status;
  uint8_t h, l;
  uint16_t distance = 0;

  printf("\r\n========= 读取距离开始 =========\r\n");

  // ==========================
  // 【1】触发一次测量（必须！）
  // ==========================
  VL_Write(0x00, 0x01);
  printf("[触发] 开始测量...\r\n");
  HAL_Delay(50);  // 等待测量完成

  // ==========================
  // 【2】等待数据就绪（必须！）
  // ==========================
  VL_Read(0x13, &status);
  printf("[状态] 0x13 = 0x%02X\r\n", status);

  // ==========================
  // 【3】读取真实距离
  // ==========================
  VL_Read(0x1E, &h);
  VL_Read(0x1F, &l);
  distance = (h << 8) | l;

  printf("[读数] 0x1E=0x%02X, 0x1F=0x%02X → 距离=%d mm\r\n", h, l, distance);

  return distance;
}

