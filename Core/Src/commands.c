#include "commands.h"

#define RECOVERY_TIME_MS 10

struct sp_config_t sp_config = {1, 1, "s"};

bool mode_speed = false, enable = false, direction = false;
unsigned int duty_cycle = 0, mul_pwm = 1;
float speed_rpm = 0;

unsigned char check_command(char* message)
{
    char cmd = INV;

    if((!strncmp((char*) message, "STW", 3)))
		cmd = STW;
    else if((!strncmp((char*) message, "FSW", 3)))
		cmd = FSW;
    else if((!strncmp((char*) message, "KP", 2)))
        cmd = KP;
    else if((!strncmp((char*) message, "UN", 2)))
        cmd = UN;
    else if((!strncmp((char*) message, "EN", 2)))
        cmd = EN;
    else if((!strncmp((char*) message, "CS", 2)))
        cmd = CS;
    else if((!strncmp((char*) message, "VR", 2)))
        cmd = VR;
    else if((!strncmp((char*) message, "HW", 2)))
		cmd = HW;
    else if((!strncmp((char*) message, "SW", 2)))
		cmd = SW;
    else if((!strncmp((char*) message, "/", 1)))
		cmd = INC;
    else if((!strncmp((char*) message, "\\", 1)))
		cmd = DEC;
    else
    	cmd = INV;

    return cmd;
}


void (*exec_command[])(char* message) = {
		proc_inv_cmd,
		proc_cs_cmd,
		proc_en_cmd,
		proc_un_cmd,
		proc_vr_cmd,
		proc_inc_cmd,
		proc_dec_cmd,
		proc_hw_cmd,
		proc_fsw_cmd,
		proc_sw_cmd,
		proc_stw_cmd,
		proc_kp_cmd
};

void proc_inv_cmd(char* message)
{
	send_UART("Invalid instruction. Type '?' for Help.");
}

void proc_cs_cmd(char* message)
{
	/*
	enable = false;
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);

	int val;

	if(sscanf((char*) message, "CS %d", &val) == 1)
	{
		if(val == 0 || val == 1)
		{
			if(val)
			{
				mode_speed = true;
				send_UART("Speed control mode selected.");
			}
			else
			{
				mode_speed = false;
				send_UART("PWM control mode selected.");
			}
		}
		else
			send_UART("Invalid Control System instruction argument values.");
	}
	else
		send_UART("Invalid Control System instruction syntax.");
	*/
}


void proc_en_cmd(char* message)
{
	/*
	int val;

	if(sscanf((char*) message, "EN %d", &val) == 1)
	{
		if(val == 0 || val == 1)
		{
			if(val)
			{
				send_UART("System enabled with success.");
				enable = true;

				if(mode_speed)
				{
					set_units("rads");
					sp_config.sp_limit = 0;
					MX_TIM3_Init1(sp_config);
					HAL_TIM_Base_Start_IT(&htim3);
					HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
					HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
				}
				else
				{
					HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
				}
			}
			else
			{
				// DISABLE ALL
				HAL_TIM_Base_Stop_IT(&htim3);
				HAL_TIM_Base_Stop_IT(&htim4);
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				reset_pulses();
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 0);
				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
				send_UART("System disabled with success.");
				enable = false;
			}
		}
		else
			send_UART("Invalid Enable instruction argument values.");
	}
	else
		send_UART("Invalid Enable instruction syntax.");
	*/
}


void proc_un_cmd(char* message)
{
	/*
	mul_pwm = (TIM2->ARR+1)/100;
	char sign;
	int val;
	int args_read;

	if((args_read = sscanf((char*) message, "UN %c%d", &sign, &val)) == 2)
	{
		if(val >= 0 && val <= 100 && (sign == '+' || sign == '-'))
		{
			if(sign == '+')
			{
				if(!direction)
				{
					for(int i = duty_cycle; i > 0; i--)
					{
						duty_cycle = (i - 1);
						TIM2->CCR4 = duty_cycle * mul_pwm;

						HAL_Delay(RECOVERY_TIME_MS);
					}
				}
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 0);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);

				if(duty_cycle > val)
				{
					for(int i = duty_cycle; i > val; i--)
					{
						duty_cycle = (i - 1);
						TIM2->CCR4 = duty_cycle * mul_pwm;
						HAL_Delay(RECOVERY_TIME_MS);
					}
				}
				else if(duty_cycle < val)
				{
					for(int i = duty_cycle; i < val; i++)
					{
						duty_cycle = (i + 1);
						TIM2->CCR4 = duty_cycle * mul_pwm;
						HAL_Delay(RECOVERY_TIME_MS);
					}
				}

				direction = true;
			}
			else
			{
				if(direction)
				{
					for(int i = duty_cycle; i > 0; i--)
					{
						duty_cycle = (i - 1);
						TIM2->CCR4 = duty_cycle * mul_pwm;
						HAL_Delay(RECOVERY_TIME_MS);
					}
				}
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 1);
				if(duty_cycle > val)
				{
					for(int i = duty_cycle; i > val; i--)
					{
						duty_cycle = (i - 1);
						TIM2->CCR4 = duty_cycle * mul_pwm;
						HAL_Delay(RECOVERY_TIME_MS);
					}
				}
				else if(duty_cycle < val)
				{
					for(int i = duty_cycle; i < val; i++)
					{
						duty_cycle = (i + 1);
						TIM2->CCR4 = duty_cycle * mul_pwm;
						HAL_Delay(RECOVERY_TIME_MS);
					}
				}

				direction = false;
			}

			send_UART("PWM average voltage changed with success.");
		}
		else
			send_UART("Invalid Normalized Voltage instruction argument values.");
	}
	else if(sscanf((char*) message, "UN %d", &val) == 1)
	{
		if(!val)
		{
			for(int i = duty_cycle; i > 0; i--)
			{
				duty_cycle = (i - 1);
				TIM2->CCR4 = duty_cycle * mul_pwm;
				HAL_Delay(RECOVERY_TIME_MS);
			}

			send_UART("PWM average voltage changed with success.");
		}
		else
			send_UART("Invalid Normalized Voltage instruction argument values.");
	}
	else
		send_UART("Invalid Normalized Voltage instruction syntax.");

	*/
}


void proc_vr_cmd(char* message)
{
	/*
	char sign;
	int val;
	int args_read;

	if((args_read = sscanf((char*) message, "VR %c%d", &sign, &val)) == 2)
	{
		if(val >= 0 && val <= 160 && (sign == '+' || sign == '-'))
		{
			speed_rpm = val;

			if(sign == '+')
				direction = true;
			else
				direction = false;

			send_UART("Speed changed with success.");
		}
		else
			send_UART("Invalid Reference Speed instruction argument values.");
	}
	else if(sscanf((char*) message, "VR %d", &val) == 1)
	{
		if(!val)
		{
			speed_rpm = val;

			send_UART("Speed changed with success.");
		}
		else
			send_UART("Invalid Reference Speed instruction argument values.");
	}
	else
		send_UART("Invalid Reference Speed instruction syntax.");
	*/
}

void proc_inc_cmd(char* message)
{
	/*
	mul_pwm = (TIM2->ARR+1)/100;

	if(mode_speed)
	{
		if(speed_rpm < 156)
			speed_rpm += 5;
		else
			speed_rpm = 160;

		sprintf(message, "Speed updated to %.2f rpm.", speed_rpm);
	}
	else
	{
		if(duty_cycle == 0)
		{
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 0);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
			direction = true;
		}

		if(direction)
		{
			for(int i = 0; i < 5 && duty_cycle < 100; ++i)
			{
				TIM2->CCR4 = ++duty_cycle * mul_pwm;
				HAL_Delay(RECOVERY_TIME_MS);
			}
		}
		else
		{
			for(int i = 0; i < 5 && duty_cycle > 0; ++i)
			{
				TIM2->CCR4 = --duty_cycle * mul_pwm;
				HAL_Delay(RECOVERY_TIME_MS);
			}
		}

		sprintf((char*) message, "Duty cycle updated to %d%%.", duty_cycle);
	}

	send_UART(message);
	*/
}

void proc_dec_cmd(char* message)
{
	/*
	mul_pwm = (TIM2->ARR+1)/100;
	if(mode_speed)
	{
		if(speed_rpm > 4)
			speed_rpm -= 5;
		else
			speed_rpm = 0;

		sprintf((char*) message, "Speed updated to %.2f rpm.", speed_rpm);
	}
	else
	{
		if(duty_cycle == 0)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 1);
			direction = false;
		}

		if(!direction)
		{
			for(int i = 0; i < 5 && duty_cycle < 100; ++i)
			{
				TIM2->CCR4 = ++duty_cycle * mul_pwm;
				HAL_Delay(RECOVERY_TIME_MS);
			}
		}
		else
		{
			for(int i = 0; i < 5 && duty_cycle > 0; ++i)
			{
				TIM2->CCR4 = --duty_cycle * mul_pwm;
				HAL_Delay(RECOVERY_TIME_MS);
			}
		}

		sprintf((char*) message, "Duty cycle updated to %d%%.", duty_cycle);
	}

	send_UART(message);
	*/
}

void proc_hw_cmd(char* message)
{
	/*
	unsigned int unit;
	char timeunit[2];

	if(sscanf((char*)message, "HW %s %d", timeunit, &unit) == 2)
		{
			if(strcmp(timeunit,"ms") == 0 || strcmp(timeunit,"s") == 0 || strcmp(timeunit,"us") == 0)
			{


				strcpy(sp_config.timeunit,timeunit);
				sp_config.unit = unit;
				send_UART("Sampling timeunit and units changed with success.");
			}
			else
				send_UART("Invalid Sample Period instruction argument values.");
		}
		else
			send_UART("Invalid Sample Period instruction syntax.");
	*/
}

void proc_fsw_cmd(char* message)
{
	/*
	char units[5] = {0};

	if(sscanf((char*)message, "FSW %s", units) == 1)
		{
			if(strcmp(units,"hz") == 0 || strcmp(units,"rps") == 0 || strcmp(units,"rads") == 0 || strcmp(units,"rpm") == 0)
			{
				set_units(units);
				send_UART("Sampling units changed with success.");
			}
			else
				send_UART("Invalid FSW instruction argument values.");
		}
		else
			send_UART("Invalid FSW instruction syntax.");
	*/
}

void proc_sw_cmd(char* message)
{
	/*
	unsigned int k_values;

	if(message[2] == '\r')
	{
		sp_config.sp_limit = 0;
		MX_TIM3_Init1(sp_config);
		HAL_TIM_Base_Start_IT(&htim3);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	}
	else if(sscanf((char*)message, "SW %d", &k_values) == 1)
	{
		sp_config.sp_limit = k_values;
		MX_TIM3_Init1(sp_config);
		HAL_TIM_Base_Start_IT(&htim3);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	}
	else
		send_UART("Invalid Sample instruction syntax.");
	*/
}

void proc_stw_cmd(char* message)
{
	/*
	if(message[3] == '\r')
	{
		HAL_TIM_Base_Stop_IT(&htim3);
		HAL_TIM_Base_Stop_IT(&htim4);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
		reset_pulses();
		send_UART("Sampling Stopped.");
	}
	else
		send_UART("Invalid Stop Sampling instruction syntax.");
	*/
}

void proc_kp_cmd(char* message)
{
	/*
	unsigned int kp = 0;
	if(sscanf((char*)message, "KP %d", &kp) == 1)
	{
		if(kp <= 200 && kp >= 0)
		{
			set_kp(kp);
		}
		else
			send_UART("Invalid Gain.");
	}
	else
		send_UART("Invalid KP instruction syntax.");
	*/
}

float get_speed(void)
{
	return speed_rpm * 0.10472;
}

bool get_mode_speed()
{
	return mode_speed;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
