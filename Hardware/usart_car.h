#ifndef __SERIAL_H
#define __SERIAL_H
#include <stdio.h>
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef uint16_t u16;
extern u16 txpakage[];
extern u16 rxpakage[];
void Serial_Init(void);
void K230_Init(void);
void Serial2_SendString(char *String);
void Serial_SendByte(uint16_t Byte);
void Serial_SendArray(uint16_t *Array, uint16_t Length);
void Serial_SendString(char *String);
uint32_t Serial_Pow(uint32_t X, uint32_t Y);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
int fputc(int ch, FILE *f);
void Serial_Printf(char *format, ...);
uint8_t Serial_GetRxFlag(void);
u16 Serial_GetRxData(void);
void DATApakage(void);
#endif

