/* retarget.c - redirect printf to USART1 (huart1) */

#include "retarget.h"
#include "stm32f1xx_hal.h"
#include <sys/stat.h>
#include <errno.h>

/* Optionally initialize retargeting (not required for HAL UART) */
void Retarget_Init(void)
{
    /* Nothing to do: USART is initialized in MX_USART1_UART_Init() */
}

#ifdef __GNUC__
/* With GCC, small write syscall for newlib */
int _write(int fd, char *ptr, int len)
{
    HAL_StatusTypeDef status;
    if (fd == 1 || fd == 2) {
        /* Use huart1 to transmit */
        status = HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
        if (status == HAL_OK) return len;
        errno = EIO;
        return -1;
    }
    errno = EBADF;
    return -1;
}
#else
/* For other toolchains, implement fputc */
int fputc(int ch, FILE *f)
{
    uint8_t c = (uint8_t)ch;
    HAL_UART_Transmit(&huart1, &c, 1, HAL_MAX_DELAY);
    return ch;
}
#endif

/* Provide minimal implementations to satisfy newlib references */
#if defined(__ARMCC_VERSION)
/* Keil may require fputc as above */
#endif

/* Stub implementations to avoid linking issues if syscalls referenced */
int _close(int fd) { return -1; }
int _isatty(int fd) { return 1; }
int _lseek(int fd, int ptr, int dir) { return 0; }
int _read(int fd, char *ptr, int len) { errno = EBADF; return -1; }
int _fstat(int fd, struct stat *st) { st->st_mode = S_IFCHR; return 0; }
