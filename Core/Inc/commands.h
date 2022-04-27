#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct sp_config_t
{
	unsigned int unit;
	char timeunit[2];
};

#include "usart.h"
#include "gpio.h"
#include "tim.h"

#define LM_EN 350

#define RV 0 // '-'
#define FW 1 // '+'

#define U_SAT_MAX 6
#define U_SAT_MIN -6

#define POS_UNIT 0.00654498 // 2*pi / 960

#define a 0.4
#define M 26

enum command { INV = 0, CS, EN, UN, VR, INC, DEC, HW, FSW, SW, STW, KP, KD, KI };

unsigned char check_command(char*);
void (*exec_command[14])(char*);

void proc_inv_cmd(char*);
void proc_cs_cmd(char*);
void proc_en_cmd(char*);
void proc_un_cmd(char*);
void proc_vr_cmd(char*);
void proc_inc_cmd(char*);
void proc_dec_cmd(char*);
void proc_hw_cmd(char*);
void proc_fsw_cmd(char*);
void proc_sw_cmd(char*);
void proc_stw_cmd(char*);
void proc_kp_cmd(char*);
void proc_kd_cmd(char*);
void proc_ki_cmd(char*);

void ISR_PID(void);
float get_speed(void);
bool get_mode_speed();



#endif /* __COMMANDS_H__ */
