/*
 * led_user.h
 *
 *  Created on: 2023年8月15日
 *      Author: YWJ
 */

#ifndef LED_USER_H_
#define LED_USER_H_

#include "app.h"


typedef struct {
  uint16_t year;
  uint8_t month;
  uint8_t week;
  uint8_t day;
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
} UtcTimeStruct;


typedef struct {
  uint8_t week[4][1];  //用1个byte 的bit位0-6 来指示周一到周日
  uint8_t hours[4]; //最大支持4个计划 0-3
  uint8_t minutes[4];
  uint8_t onoff[4];
  uint8_t  led_mode[4];  //执行动作0 为色温或者 1为 HSV
  uint8_t  mode_data[4][4];  // 4byte 分配， 色温则 0,1 = temp 2=light   彩色 则0.1 =h 2=s 3=v
  uint8_t  continue_time[4];  //唤醒计划持续时间 恢复上次状态  //生物节律下 【0】位 代表 全程渐变或者直接渐变
  uint32_t execution_time[4]; //执行动作的时间  转换成ms
} FucTimeStruct;


typedef struct {
  uint8_t  mode;         //1是初始状态， 0是上次状态， 2是自定义
  uint8_t  led_mode;         //模式   //0=色温模式， 1=RGB模式
  uint8_t  Brightness;   //亮度
  uint16_t Color_Temp;  //色温
  uint16_t Color_H;     //色相
  uint16_t Color_S;     //饱和度
  uint16_t Color_V;     //色明度

} G_MEMORY_T; //上电自定义记忆模式


typedef struct {
  uint8_t  MarkHead;
  uint8_t  MarkEnd;
  uint8_t  test_mode;   //产测模式，
  uint8_t  test_time;   //老化计时
  uint8_t  Power_On_Times;//上电次数
  uint8_t  Onoff;        //开关
  uint8_t  Brightness;   //亮度
  uint8_t  resever;      //保留
  uint8_t  led_mode;         //模式   //0=色温模式， 1=RGB模式
  uint8_t  work_mode;         //模式
  uint8_t  scen_mode;         //模式
  uint8_t  not_disturb; //断电勿扰
  uint16_t Color_Temp;  //色温
  uint16_t Color_H;     //色相
  uint16_t Color_S;     //饱和度
  uint16_t Color_V;     //色明度
  uint16_t On_buff;        // 开灯渐变
  uint16_t Off_buff;       // 关灯渐变
  uint32_t Count_Down;     // 倒计时
  G_MEMORY_T memory;
  FucTimeStruct awaken;    //唤醒计划
  FucTimeStruct asleep;    //睡眠计划
  FucTimeStruct bio_rhythm;//生物节律
} G_USER_CTRL_T;
extern G_USER_CTRL_T *tpUserControl;

// 定义命令字索引
#define CMD_HEARTBEAT               0x99
#define CMD_DEVICE_PING             0x01
#define CMD_DEVICE_STATUS_QUERY     0x02
#define CMD_GATEWAY_COMMAND         0x04
#define CMD_DEVICE_REPORT_STATUS    0x06



#define DP_1_SWTICH               1
#define DP_2_Work_Mode            2
#define DP_3_Bright               3
#define DP_4_TEMP                 4
#define DP_5_COLOUR               5
#define DP_6_SCENE                6
#define DP_7_COUNTDOWN            7
#define DP_9_CONTROL              9
#define DP_30_RHYTHM              30
#define DP_31_ASLEEP              31
#define DP_32_WAKEUP              32
#define DP_33_POWER_MEMORY        33
#define DP_34_DO_NOT_DISTURB      34
#define DP_35_SWTICH_GRADIENT     35


typedef enum {
  PROP_UNKNOWN,
    PROP_BOOL,    //1个字节
    PROP_INT,     //4个字节    //小端传输
    PROP_FLOAT,   //4个字节    //小端传输
    PROP_ENUM,    //
    PROP_FAULT,
    PROP_TEXT,
    PROP_RAW,
    PROP_STRUCT,
    PROP_ARRAY,
} DP_PROP_TP_E;


#define   User_PWM      5   //PWM数量
#define  COLOR_MAX     6500
#define  COLOR_MIN     2700
#define  COLOR_DIFF    (COLOR_MAX - COLOR_MIN)
#define  LIGHT_MAX     100
#define  LIGHT_LIMIT   250      //250级最大
#define  LIGHT_MIN_NUM 2      //最低输出
#define  Pwm_Cal  ((COLOR_DIFF*LIGHT_MAX)/LIGHT_LIMIT)   //色温取总值 * 亮度总值 /百分比    //100是百分比   改成N，越小最大数越小

#define  Pwm_Rate     250   //250级的PWM变化



void set_rgbcw_pwm(uint8_t num, uint8_t pwm);
void f_pwm_buffer(void);
void hsv_to_rgb(int h,int s,int v,float *R,float *G,float *B);

void limit_rgb_dat(uint16_t *R , uint16_t *G,  uint16_t *B);
void report_cmd_countdown(void);
void execute_led_change_cal(uint32_t speed_ms);
void set_led_onoff(uint8_t onoff);
void set_led_temp_color(uint16_t times);
void set_led_hsv_color(void);
void report_chage_work_mode(uint8_t work_mode);


void set_scen_hsv_led_mode(uint8_t led_mode);
void set_scen_cw_led_mode(uint8_t led_mode);
void report_light_temp(void);
void report_zigbee_onoff_state(void);
void  Timing_Plan_Fuction(void);
void f_count_down_off(uint32_t* times);
typedef struct {
  uint16_t R;
  uint16_t G;
  uint16_t B;
  uint16_t c;
  uint16_t w;
} G_USER_RGBCW_COLOR_T;


typedef struct {
  uint32_t R;
  uint32_t G;
  uint32_t B;
  uint32_t c;
  uint32_t w;
} G_USER_SPEED_T;

extern G_USER_RGBCW_COLOR_T *tpStarColor;
extern G_USER_RGBCW_COLOR_T *tpEndColor;
extern G_USER_RGBCW_COLOR_T *tpStep;
extern G_USER_SPEED_T *tpSpeed;


extern uint8_t  count_100ms;
//extern uint8_t  r_set_speed_buff;
void cal_speed(uint32_t speed_ms);
void set_speed_buf(uint8_t speed);
void set_scen_cw_mode(uint8_t mode);
void set_scen_hsv_mode(uint8_t mode);
extern uint8_t  r_mode_level;  //彩色，色温模式







#endif /* LED_USER_H_ */
