#include "main.h"
#include "i2c.h"
#include "VL53L0X.h"
#include "stdio.h"
#include "gpio.h"
#include "usart.h"

void SystemClock_Config(void);
uint16_t distance;

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();

  printf("系统初始化完成！开始初始化VL5310X传感器...\r\n");

  /* VL5310X 初始化 */
  if(VL5310X_Init() != HAL_OK)
  {
    /* 初始化失败：循环闪烁LED + 持续打印错误 */
    printf("VL5310X初始化失败！进入错误循环...\r\n");
    while(1)
    {
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); 
      HAL_Delay(500);
      printf("错误状态：传感器初始化失败！\r\n");
    }
  }

  printf("VL5310X初始化成功！进入主循环读取距离...\r\n");
  
  while (1)
  {
    /* 读取距离值 */
    distance = VL5310X_GetDistance();
    
    /* 打印最终距离值（汇总） */
    printf("【最终结果】Distance: %d mm\r\n", distance);
		
    HAL_Delay(500); // 延长采样间隔，方便看打印（原100ms太快）
  }
}

// 以下代码不变（SystemClock_Config/Error_Handler等）
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}
#endif /* USE_FULL_ASSERT */