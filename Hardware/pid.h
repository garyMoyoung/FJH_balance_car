#ifndef _PID_H_
#define _PID_H_
#define OUT1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)
#define OUT2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)
#define OUT3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)
#define OUT4  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
#define OUT5  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define OUT6  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)
#define OUT7  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
int vertical_pid_value(float measure,float calcu);
int velocity_PID_value(int speed,int target);
int turn_pid_value(float expect, float angle, short gyro_z);
typedef struct {
    float kp;        // 比例系数
    float ki;        // 积分系数 
    float kd;        // 微分系数
    
    float error;     // 当前误差
    float lastError; // 上次误差
    float preError;  // 前次误差
    
    float integral;  // 积分�????
    float output;    // PID输出�????
    
    float maxOutput; // 输出限幅上限
    float minOutput; // 输出限幅下限
    float maxIntegral; // 积分限幅
} PIDController;
float PID_Position_Calculate(PIDController *pid, float target, float current);
void PID_init(PIDController *pid, float kp, float ki, float kd, float maxOutput, float minOutput);
#endif
