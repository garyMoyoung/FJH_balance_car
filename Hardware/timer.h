#ifndef __TIMER_H
#define __TIMER_H
extern int32_t pwm_left;
extern int32_t pwm_right;
void PWM_SetCompare1(uint16_t Compare);
void PWM_SetCompare2(uint16_t Compare);
void PWM_Init(void);
void GPIO_Motor_init(void);
void AIN1_set(void);
void AIN1_reset(void);
void AIN2_set(void);
void AIN2_reset(void);
void BIN1_set(void);
void BIN1_reset(void);
void BIN2_set(void);
void BIN2_reset(void);
void tim3_encode_init(void);
int16_t Read_Encoder3(void);
int16_t Read_Encoder4(void);
void tim4_encode_init(void);
void forward(void);
void behind(void);
void MPU_exti_init(void);
void TIM1_Init(u16 arr,u16 psc);
void Limiting_motor(void);
/* float HCSR04GetLength(void);
u32 GetEchoTimer(void); */
void HC_SR04_start(void);
void HC_SR04_init(void);
void NVIC_Config(void);

#endif

