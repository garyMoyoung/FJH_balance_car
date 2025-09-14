#include "stm32f10x.h"
#include "oled_hard_iic_font.h"
#include "oled_hard_iic.h"

#define OLED_ADDRESS 		0x78


void OLED_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;									//????????
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)	//????????
	{
		Timeout --;										//???,?????
		if (Timeout == 0)								//???0?,????
		{
			/*?????????,???????*/
			break;										//????,???
		}
	}
}

/**
  * @brief  OLED??????
  * @param  RegAddress??(0x40,0x00),Data ??????
  * @retval 0x00????0x00???
  */
void OLED_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	I2C_GenerateSTART(I2C1, ENABLE);										//??I2C??????
	OLED_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);					//??EV5
	
	I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter);	//??I2C??????,?????
	OLED_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	//??EV6
	
	I2C_SendData(I2C1, RegAddress);											//??I2C???????
	OLED_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING);			//??EV8
	
	I2C_SendData(I2C1, Data);												//??I2C????
	OLED_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);				//??EV8_2
	
	I2C_GenerateSTOP(I2C1, ENABLE);											//??I2C??????
}

/**
  * @brief  OLED??????
  * @param  Y ???????,???????,??:0~7
  * @param  X ???????,???????,??:0~127
  * @retval ?
  */
void OLED_Hard_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteReg(0x00,0xB0 | Y);					//??Y??
	OLED_WriteReg(0x00,0x10 | ((X & 0xF0) >> 4));	//??X???4?
	OLED_WriteReg(0x00,0x00 | (X & 0x0F));			//??X???4?
}

/**
  * @brief  OLED??
  * @param  ?
  * @retval ?
  */
void OLED_Hard_Clear(void)
{  
    uint8_t i, j;
    for (j = 0; j < 8; j++)
    {
        OLED_Hard_SetCursor(j, 0);
        for(i = 0; i < 128; i++)
        {
            OLED_WriteReg(0x40,0x00);
        }
    }
}

/**
  * @brief  OLED??????
  * @param  Line ???,??:1~4
  * @param  Column ???,??:1~16
  * @param  Char ????????,??:ASCII????
  * @retval ?
  */
void OLED_Hard_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
    uint8_t i;
    OLED_Hard_SetCursor((Line - 1) * 2, (Column - 1) * 8);
    for (i = 0; i < 8; i++)
    {
        OLED_WriteReg(0x40,OLED_hard_F8x16[Char - ' '][i]);
    }
    OLED_Hard_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);
    for (i = 0; i < 8; i++)
    {
        OLED_WriteReg(0x40,OLED_hard_F8x16[Char - ' '][i + 8]);
    }
}

/**
  * @brief  OLED?????
  * @param  Line ?????,??:1~4
  * @param  Column ?????,??:1~16
  * @param  String ???????,??:ASCII????
  * @retval ?
  */
void OLED_Hard_ShowString(uint8_t Line, uint8_t Column, char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        OLED_Hard_ShowChar(Line, Column + i, String[i]);
    }
}

/**
  * @brief  OLED????
  * @retval ?????X?Y??
  */
uint32_t OLED_Hard_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED????(???,??)
  * @param  Line ?????,??:1~4
  * @param  Column ?????,??:1~16
  * @param  Number ??????,??:0~4294967295
  * @param  Length ????????,??:1~10
  * @retval ?
  */
void OLED_Hard_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_Hard_ShowChar(Line, Column + i, Number / OLED_Hard_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
  * @brief  OLED????(???,????)
  * @param  Line ?????,??:1~4
  * @param  Column ?????,??:1~16
  * @param  Number ??????,??:-2147483648~2147483647
  * @param  Length ????????,??:1~10
  * @retval ?
  */
void OLED_Hard_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
    uint8_t i;
    uint32_t Number1;
    if (Number >= 0)
    {
        OLED_Hard_ShowChar(Line, Column, '+');
        Number1 = Number;
    }
    else
    {
        OLED_Hard_ShowChar(Line, Column, '-');
        Number1 = -Number;
    }
    for (i = 0; i < Length; i++)
    {
        OLED_Hard_ShowChar(Line, Column + i + 1, Number1 / OLED_Hard_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
  * @brief  OLED????(????,??)
  * @param  Line ?????,??:1~4
  * @param  Column ?????,??:1~16
  * @param  Number ??????,??:0~0xFFFFFFFF
  * @param  Length ????????,??:1~8
  * @retval ?
  */
void OLED_Hard_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++)
    {
        SingleNumber = Number / OLED_Hard_Pow(16, Length - i - 1) % 16;
        if (SingleNumber < 10)
        {
            OLED_Hard_ShowChar(Line, Column + i, SingleNumber + '0');
        }
        else
        {
            OLED_Hard_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
        }
    }
}

/**
  * @brief  OLED????(???,??)
  * @param  Line ?????,??:1~4
  * @param  Column ?????,??:1~16
  * @param  Number ??????,??:0~1111 1111 1111 1111
  * @param  Length ????????,??:1~16
  * @retval ?
  */
void OLED_Hard_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_Hard_ShowChar(Line, Column + i, Number / OLED_Hard_Pow(2, Length - i - 1) % 2 + '0');
    }
}

///**
//  * @brief  OLED???
//  * @param  ?
//  * @retval ?
//  */
void OLED_HD_Init(void)
{
	/*????*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);		//??I2C1???
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//??GPIOB???
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
  /* I2C1重映射到PB8/PB9 */
  GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
	/*GPIO???*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//?PB10?PB11????????????
	
	/*I2C???*/
	I2C_InitTypeDef I2C_InitStructure;						//???????
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;				//??,???I2C??
	I2C_InitStructure.I2C_ClockSpeed = 100000;				//????,???350KHz 390KHZ???????? ???iic??
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;		//?????,??Tlow/Thigh = 2
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;				//??,????
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	//????,??7?,????????
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;				//????,????????
	I2C_Init(I2C1, &I2C_InitStructure);						//????????I2C_Init,??I2C1
	
	/*I2C??*/
	I2C_Cmd(I2C1, ENABLE);									//??I2C1,????
	
	OLED_WriteReg(0x00,0xAE);	//????
	              
	OLED_WriteReg(0x00,0xD5);	//?????????/?????
	OLED_WriteReg(0x00,0x80);
	              
	OLED_WriteReg(0x00,0xA8);	//???????
	OLED_WriteReg(0x00,0x3F);
	              
	OLED_WriteReg(0x00,0xD3);	//??????
	OLED_WriteReg(0x00,0x00);
	             
	OLED_WriteReg(0x00,0x40);	//???????
	              
	OLED_WriteReg(0x00,0xA1);	//??????,0xA1?? 0xA0????
	              
	OLED_WriteReg(0x00,0xC8);	//??????,0xC8?? 0xC0????
                 
	OLED_WriteReg(0x00,0xDA);	//??COM??????
	OLED_WriteReg(0x00,0x12);
	              
	OLED_WriteReg(0x00,0x81);	//???????
	OLED_WriteReg(0x00,0xCF);
                  
	OLED_WriteReg(0x00,0xD9);	//???????
	OLED_WriteReg(0x00,0xF1);
                  
	OLED_WriteReg(0x00,0xDB);	//??VCOMH??????
	OLED_WriteReg(0x00,0x30);
                  
	OLED_WriteReg(0x00,0xA4);	//????????/??
                  
	OLED_WriteReg(0x00,0xA6);	//????/????
                  
	OLED_WriteReg(0x00,0x8D);	//?????
	OLED_WriteReg(0x00,0x14);
                  
	OLED_WriteReg(0x00,0xAF);	//????
		
	OLED_Hard_Clear();				//OLED??
}
