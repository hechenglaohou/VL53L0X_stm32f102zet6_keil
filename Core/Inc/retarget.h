/* retarget.h - redirect printf to USART (huart1) */
#ifndef __RETARGET_H__
#define __RETARGET_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdio.h>
#include "usart.h"

/* Call this to initialize retargeting if needed (empty for HAL UART) */
void Retarget_Init(void);

#ifdef __cplusplus
 }
#endif

#endif /* __RETARGET_H__ */
