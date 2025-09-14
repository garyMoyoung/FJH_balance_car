#ifndef __OLED_HARD_IIC_H
#define __OLED_HARD_IIC_H
#include "stm32f10x.h"
void OLED_Hard_Clear(void);
void OLED_Hard_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_Hard_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_Hard_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_Hard_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_Hard_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_Hard_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
uint32_t OLED_Hard_Pow(uint32_t X, uint32_t Y);
void OLED_Hard_SetCursor(uint8_t Y, uint8_t X);
void OLED_HD_Init(void);
void OLED_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
void OLED_WriteReg(uint8_t RegAddress, uint8_t Data);
#endif
