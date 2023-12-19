/*
 * app.h
 *
 *  Created on: 2023年6月27日
 *      Author: YWJ
 */

#ifndef APP_H_
#define APP_H_


#include <stdint.h>
#include <stdlib.h>
#include <string.h>


//定时器时间  单位MS
#define  Time_Count    10

#include "sl_simple_rgb_pwm_led.h"

//---------------
// Event handlers
#include "nvm3.h"
#include "nvm3_hal_flash.h"

#include "sl_pwm_instances.h"
#include "app_timer.h"

#include "attribute-table.h"

#include "zap-command.h"
#include "em_device.h"
#include "af.h"
#include "af-main.h"
#include "zap-id.h"
#include "hal.h"
typedef enum {
  Num_red,
  Num_green,
  Num_blue,
  Num_cool,
  Num_warm,
} Num_rgbcw_pwm;


void app_init(void);
void app_process_action(void);

void Restore_to_last_state(void);

void set_write_flag(uint8_t flag);
void report_agreement(void *value,uint16_t len);
void report_Attributes(void *value,uint16_t len);

extern uint16_t HeartbeatTime;
extern uint32_t Save_HbTime;
void set_rand_source(void);
void report_hearbeat(void);

uint8_t get_Test_State(void);
void F_Production_Test(void);
uint8_t get_init_state(void);
EmberStatus get_network_state(void);
void Cout_Net_Time(void);

#endif /* APP_H_ */
