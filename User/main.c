#include "stm32f10x.h"                  // Device header"
#include "OLED.h"
#include "Delay.h"
#include "inv_mpu.h"
#include "timer.H"
#include "inv_mpu.h"
#include "MPU6050.H"
#include "pid.h"
#include "usart_car.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "yb_protocol.h"
#include "oled_hard_iic.h"

#define  HCSR04_TRIG                        GPIO_Pin_11
#define  HCSR04_ECHO                        GPIO_Pin_15
#define  HCSR04_PORT_Trig                   GPIOA
#define  HCSR04_PORT_Echo                   GPIOA
#define AIN1 GPIO_Pin_12
#define AIN2 GPIO_Pin_13
#define BIN1 GPIO_Pin_14
#define BIN2 GPIO_Pin_15
#define vel_bias 		15
float Pitch,Roll,Yaw;
//float KP=0,KI=0,KD=0;
float KP=450*0.6,KD=1.3*0.6;
//float VKp=156,VKi=0.8;
float TKP=100,TKD=1;
//float TKP=0,TKD=0;
float VKp=0.25f,VKi=0.0015f;
//float VKp=-0.65,VKi=-0.9/200;
float FKp=-1.5,FKd=0.015;
float OKp=0,OKd=0.4;
float F_error,F_error_last;
float measure,calcu;
float mid=-7.5f;
float V_target_right = 0;
float V_target_left = 0;
float V_target = 0;
float V_bias = 0;

PIDController coordinate_pid;
uint8_t coordinate_pid_flag = 0;
//pid相关参数
//oled清屏标志位
u16 key_sign;
int16_t circle_count1,circle_count2;
int16_t speed;
int32_t pwm_left,pwm_right;
u8 ain1,ain2,bin1,bin2;
//编码器 电机
uint16_t k230_x, k230_y;
uint16_t k230_w, k230_h;
//视觉上位机
unsigned long sensor_timestamp;
short gyro[3], accel[3], sensors;
unsigned char more;
unsigned long *timestamp;
long quat[4];
unsigned int time_count=0;
//没啥用
extern uint8_t Rx2Buffer[];
/* 接收数据下标 */
extern uint8_t Rx2Index;
/* 接收状态机 */
extern uint8_t Rx2Flag;
/* 新命令接收标志 */
extern uint8_t New_CMD_flag;
/* 新命令数据长度 */
extern uint8_t New_CMD_length;
//协议相关k230
uint8_t usart1_rx_state = 0;
uint8_t usart1_rx_buffer[5];
uint8_t usart1_rx_index = 0;
uint16_t accelerator = 0, bias_data = 0;
uint8_t car_state = 0;
uint8_t car_status = 0;
//与遥控板通信
void motor_run(int16_t pwm_left,int16_t pwm_right);
void oled_show_all(void);
int main(void)
{	
	delay_init();
	OLED_Init();
	OLED_Clear();
	MPU6050_DMP_Init();
	GPIO_Motor_init();
	PWM_Init();
	forward();
	tim3_encode_init();
	tim4_encode_init();
	MPU_exti_init();
	TIM1_Init(2000,72-1);
	Serial_Init();
	K230_Init();
	PID_init(&coordinate_pid, 0.1f, 0.0f, 0.0f, 15.0f, -15.0f);
	while(1)
	{
		// OLED_ShowNum(1,1,Pitch,3);
		// OLED_ShowNum(2,1,Roll,3);
		// OLED_ShowNum(3,1,Yaw,3);
		// OLED_ShowNum(4,1,gyro[2],3);
		// OLED_ShowSignedNum(1,6,Read_Encoder3(),4);
		// OLED_ShowSignedNum(2,6,-Read_Encoder4(),4);
		// OLED_ShowNum(4,5,time_count,5);
		// char msg[50];
//        sprintf(msg, "acc:%d, bias:%d, state:%d\n", accelerator, bias_data, car_state);
//        Serial_SendString(msg);
//		sprintf(msg, "x:%d,y:%d,w:%d,h:%d\n", k230_x,k230_y,k230_w,k230_h);
//		Serial2_SendString(msg);
	}
}

void TIM1_UP_IRQHandler(void)//超声波计时
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
			TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //清除TIMx更新中断标志 
			time_count++;
			oled_show_all();
			
			switch(car_state)
			{
				case 1:
					coordinate_pid_flag = 0;
					V_target = (float)accelerator / 4096 * 50; // 映射到0~100
					if(bias_data >= 3500)
					{
						V_target_right = V_target - vel_bias;
						V_target_left = V_target + vel_bias;
					}
					else if(bias_data <= 1000)
					{
						V_target_right = V_target + vel_bias;
						V_target_left = V_target - vel_bias;
					}
					else
					{
						V_target_right = V_target;
						V_target_left = V_target;
					}
					break;
				case 2:
					coordinate_pid_flag = 0;
					V_target_right = V_target;
					V_target_left = V_target;
					break;
				case 3:
					if(car_status == 0)
					{
						coordinate_pid_flag = 1;
					}
					else if(car_status == 1)//
					{
						coordinate_pid_flag = 0;//取消pid
						V_target_right = -10;
						V_target_left = -10;
						if(k230_w * k230_h >= 70000)//停止跟距
						{
							V_target_left = 0;
							V_target_right = 0;
						}
						else if(k230_w * k230_h <= 15000)//小球被移动位置
						{
							car_state = 0;//重新再来
						}
					}
					if(k230_x >= 235 && k230_x <= 295)//小球到达中心位置
					{
						car_status = 1;//切换为跟距模式
					}
					else
					{
						car_status = 0;
					}
					break;
				case 4:
					coordinate_pid_flag = 0;
					V_target = -(float)accelerator / 4096 * 50; // 映射到0~100
					if(bias_data >= 3500)
					{
						V_target_right = V_target + vel_bias;
						V_target_left = V_target - vel_bias;
					}
					else if(bias_data <= 1000)
					{
						V_target_right = V_target - vel_bias;
						V_target_left = V_target + vel_bias;
					}
					else
					{
						V_target_right = V_target;
						V_target_left = V_target;
					}
					break;
				default:
					coordinate_pid_flag = 0;
					// V_target_right = V_target;
					// V_target_left = V_target;
					break;
			}
		}
	
}

void TIM3_IRQHandler()//没用
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
		if((TIM3->CR1>>4)&(0x01)==0)
		{
			circle_count1++;
			
		}			
		else if((TIM3->CR1>>4)&(0x01)==1)
		{
			circle_count1--;  
		}
			
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}
void TIM4_IRQHandler()//没用
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
	{
		if((TIM4->CR1>>4)&(0x01)==0)
		{
			circle_count2++;
			
		}			
		else if((TIM4->CR1>>4)&(0x01)==1)
		{
			circle_count2--;  
			
		}
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}
}

void EXTI15_10_IRQHandler()//pid输出写入由陀螺仪的中断引脚产生的中断来触发
{
	MPU6050_DMP_Get_Data(&Roll,&Pitch,&Yaw,(short*)&gyro);//Pitch Roll更换
	speed=(Read_Encoder3()-Read_Encoder4());
	if(coordinate_pid_flag == 1)
	{
		V_bias = PID_Position_Calculate(&coordinate_pid, 265, k230_x);
	}
	else 
		V_bias = 0;
	pwm_left=vertical_pid_value(Pitch,mid + velocity_PID_value(speed,
		V_target_left+V_bias));
	pwm_right=vertical_pid_value(Pitch,mid + velocity_PID_value(speed,
		V_target_right-V_bias));
	Limiting_motor();//使用串级pid
	motor_run(pwm_left,pwm_right);
//	if (New_CMD_flag)
//	{
//		Serial2_SendString((char *)Rx2Buffer);
//    	Pto_Data_Parse(Rx2Buffer, New_CMD_length);
//		Pto_Clear_CMD_Flag();
//	}

	EXTI_ClearITPendingBit(EXTI_Line12);
}

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
        uint8_t com_data = USART_ReceiveData(USART1);
        switch (usart1_rx_state)
        {
            case 0: // 等待第一个帧头
                if (com_data == 0xfe)
                    usart1_rx_state = 1;
                break;
            case 1: // 等待第二个帧头
                if (com_data == 0xef)
                {
                    usart1_rx_state = 2;
                    usart1_rx_index = 0;
                }
                else
                    usart1_rx_state = 0;
                break;
            case 2: // 接收数据区
                usart1_rx_buffer[usart1_rx_index++] = com_data;
                if (usart1_rx_index >= 5)
                    usart1_rx_state = 3;
                break;
            case 3: // 等待第一个帧尾
                if (com_data == 0x21)
                    usart1_rx_state = 4;
                else
                    usart1_rx_state = 0;
                break;
            case 4: // 等待第二个帧尾
                if (com_data == 0x22)
                {
                    // 数据解析
                    accelerator = (usart1_rx_buffer[0] << 8) | usart1_rx_buffer[1];
                    bias_data = (usart1_rx_buffer[2] << 8) | usart1_rx_buffer[3];
                    car_state = usart1_rx_buffer[4];
                    // 可在此处处理 usart1_data1, usart1_data2, usart1_data3
                }
                usart1_rx_state = 0;
                break;
            default:
                usart1_rx_state = 0;
                break;
        }
		return;
    }
}

void USART2_IRQHandler()//接受k230发来的数据
{
	u8 com_data;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
	{
		USART_ClearFlag(USART2,USART_IT_RXNE);
		com_data  = USART_ReceiveData(USART2);
		switch(Rx2Flag)
		{
			case 0:
				if (com_data == 0xFE)
                    Rx2Flag = 1;
				break;
			case 1:
				if (com_data == 0xEF)
                {
                    Rx2Flag = 2;
                    Rx2Index = 0;
                }
                else
                    Rx2Flag = 0;
				break;
			case 2: // 接收数据区
                Rx2Buffer[Rx2Index++] = com_data;
                if (Rx2Index >= 8)
                    Rx2Flag = 3;
                break;
			case 3: // 等待第一个帧尾
                if (com_data == 0x21)
                    Rx2Flag = 4;
                else
                    Rx2Flag = 0;
                break;
			case 4: // 等待第二个帧尾
                if (com_data == 0x22)
                {
                    // 数据解析
                    k230_x = (Rx2Buffer[0] << 8) | Rx2Buffer[1];
                    k230_y = (Rx2Buffer[2] << 8) | Rx2Buffer[3];
                    k230_w = (Rx2Buffer[4] << 8) | Rx2Buffer[5];
                    k230_h = (Rx2Buffer[6] << 8) | Rx2Buffer[7];
                }
                Rx2Flag = 0;
                break;
			default:
                Rx2Flag = 0;
                break;
		}

		// Pto_Data_Receive(com_data);
		// Pto_Data_Parse(Rx2Buffer, Rx2Index);
	}
}

void motor_run(int16_t pwm_left,int16_t pwm_right)
{
		if(pwm_left>0)
		{
			GPIO_SetBits(GPIOB,AIN1);
			GPIO_ResetBits(GPIOB,AIN2);
			PWM_SetCompare1(pwm_left);
		//	PWM_SetCompare1(0);					
		}
		else if(pwm_left<0)
		{
			AIN2_set();
			AIN1_reset();
			PWM_SetCompare1(-pwm_left);
		//	PWM_SetCompare1(0);
		}
		if(pwm_right>0)
		{
			GPIO_SetBits(GPIOB,BIN2);
			GPIO_ResetBits(GPIOB,BIN1);
			PWM_SetCompare2(pwm_right);
		//	PWM_SetCompare2(0);
		}
		else if(pwm_right<0)
		{
			BIN2_reset();
			BIN1_set();
			PWM_SetCompare2(-pwm_right);
		//	PWM_SetCompare2(0);
		}
}

void oled_show_all(void)
{
	OLED_ShowNum(4,1,time_count,4);
	OLED_ShowNum(4,6,coordinate_pid_flag,1);
	OLED_ShowNum(4,8,car_status,1);
	OLED_ShowString(1, 1, "Acc:");//time_count
	OLED_ShowNum(1, 5, accelerator, 4);
	OLED_ShowString(2, 1, "Bias:");
	OLED_ShowNum(2, 6, bias_data, 4);
	OLED_ShowString(3, 1, "State:");
	OLED_ShowNum(3, 7, car_state, 2);
//	OLED_ShowNum(1,1,Pitch,3);
//	OLED_ShowNum(2,1,Roll,3);
//	OLED_ShowNum(3,1,Yaw,3);
	OLED_ShowString(1, 10, "X:");
	OLED_ShowNum(1, 12, k230_x, 3);
	OLED_ShowString(2, 10, "Y:");
	OLED_ShowNum(2, 12, k230_y, 3);
	OLED_ShowString(3, 10, "W:");
	OLED_ShowNum(3, 12, k230_w, 3);
	OLED_ShowString(4, 10, "H:");
	OLED_ShowNum(4, 12, k230_h, 3);
}