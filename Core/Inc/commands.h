#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "usart.h"
#include "gpio.h"
#include "tim.h"

#define LM_EN 350

#define RV 0 // '-'
#define FW 1 // '+'

#define a 0.4
#define M 26

extern bool direction;

struct sp_config_t
{
	unsigned int unit, sp_limit;
	char timeunit[2];
};

enum command { INV = 0, CS, EN, UN, VR, INC, DEC, HW, FSW, SW, STW, KP };

unsigned char check_command(char*);
void (*exec_command[12])(char*);

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

float get_speed(void);
bool get_mode_speed();

#endif /* __COMMANDS_H__ */
