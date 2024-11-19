#ifndef MY_UART
#define MY_UART
#include "stdint.h"
#ifdef __cplusplus
extern "C"
{
#endif
void get_data(uint8_t msg);
uint8_t compile_cmd(double *cmd_data);
#ifdef __cplusplus
} // extern "C"
#endif
#endif