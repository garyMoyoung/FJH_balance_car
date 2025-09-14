#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "timer.H"
#include "inv_mpu.h"
#include "MPU6050.H"
#include "pid.h"
extern float KP,KI,KD;
extern float VKp,VKi;
extern float TKP,TKD;
extern float FKp,FKd;
extern float OKp,OKd;
extern float F_error,F_error_last;
extern short gyro[3];
extern u8 motor_speed_control;
extern float V_target_right;
extern float V_target_left;
extern float angle_turn;
extern float Pitch,Roll,Yaw;
float err;
float last_err=0;
float err_sum=0;
float err_difference=0;
float filt_velocity;
float last_filt_velocity=0;
float velocity_sum=0;
int velocity_err;
int velocity_last_err;
float a=0.32;
float b=0.3;
typedef struct {
	int8_t Trace_error;
	int8_t Trace_last_error;
	int8_t Trace_kp;
	float Trace_ki;
	int8_t Trace_kd;
} Trace_pid;
int vertical_pid_value(float measure,float calcu)
{
	err=measure-calcu;
	err_sum+=err;
	err_difference=err-last_err;
	last_err=err;
	return KP*err+KD*gyro[0];
}
int velocity_PID_value(int speed,int target)
{
	velocity_err = speed-target;
	velocity_sum += velocity_err;
	if(velocity_sum>=1000)
	{
		velocity_sum=0;
	}
	if(velocity_sum<=-1000)
	{
		velocity_sum=0;
	}
	return VKp*velocity_err+VKi*velocity_sum;
}
int turn_pid_value(float expect, float angle, short gyro_z)
{
	static float gyro_z_last=0;
	gyro_z=b*gyro_z+(1-b)*gyro_z_last;
	gyro_z_last=gyro_z;
	return TKP*(angle-expect)+TKD*gyro_z;
}

float PID_Position_Calculate(PIDController *pid, float target, float current) 
{
    // 计算误差
    pid->error = target - current;
    
    // 积分项计算
    pid->integral += pid->error;

    // 积分限幅
    if(pid->integral > pid->maxIntegral) {
        pid->integral = pid->maxIntegral;
    }
    else if(pid->integral < -pid->maxIntegral) {
        pid->integral = -pid->maxIntegral;
    }
    // 位置式PID计算
    pid->output = pid->kp * pid->error +
                 pid->ki * pid->integral +
                 pid->kd * (pid->error - pid->lastError);
    
    // 输出限幅
    if(pid->output >= pid->maxOutput) {
        pid->output = pid->maxOutput;
    }
    else if(pid->output <= pid->minOutput) {
        pid->output = pid->minOutput;
    }
    // 保存误差
    pid->lastError = pid->error;
    
    return pid->output;
}

void PID_init(PIDController *pid, float kp, float ki, float kd, float maxOutput, float minOutput) 
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->maxOutput = maxOutput;
	pid->minOutput = minOutput;
	
	// 初始化其他变量
	pid->error = 0.0f;
	pid->lastError = 0.0f;
	pid->preError = 0.0f;
	pid->integral = 0.0f;
	pid->output = 0.0f;
}
