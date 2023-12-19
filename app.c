/***************************************************************************//**
 * @file app.c
 * @brief Callbacks implementation and application specific code.
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include "app.h"
#include "led_user.h"

#include "app/framework/include/af.h"
#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"
#endif

#ifdef SL_CATALOG_ZIGBEE_NETWORK_TEST_PRESENT
#include "network_test_config.h"
#endif //SL_CATALOG_ZIGBEE_NETWORK_TEST_PRESENT

#if (LARGE_NETWORK_TESTING == 0)
#ifndef EZSP_HOST
  #include "zigbee_sleep_config.h"
#endif

#include "network-creator.h"
#include "network-creator-security.h"
#include "network-steering.h"
#include "find-and-bind-target.h"
#include "zll-commissioning.h"

#include "sl_pwm_instances.h"
#if defined(SL_CATALOG_LED0_PRESENT)
  #include "sl_led.h"
  #include "sl_simple_led_instances.h"
  #define led_turn_on(led) sl_led_turn_on(led)
  #define led_turn_off(led) sl_led_turn_off(led)
  #define led_toggle(led) sl_led_toggle(led)
  #define COMMISSIONING_STATUS_LED (&sl_led_led0)
  #define ON_OFF_LIGHT_LED         (&sl_led_led0)
#else // !SL_CATALOG_LED0_PRESENT
  #define led_turn_on(led)
  #define led_turn_off(led)
  #define led_toggle(led)
#endif // SL_CATALOG_LED0_PRESENT

//---------------
// Event handlers


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


#define LED_BLINK_PERIOD_MS      2000
#define LIGHT_ENDPOINT           1

static sl_zigbee_event_t commissioning_led_event;
static sl_zigbee_event_t finding_and_binding_event;
static sl_zigbee_event_t com_delay_join;

void report_agreement(void *value,uint16_t len);
void report_Attributes(void *value,uint16_t len);

// Create a NVM area of 24kB (size must equal N * FLASH_PAGE_SIZE, N is integer). Create a cache of 10 entries.
//NVM3_DEFINE_SECTION_STATIC_DATA(nvm3Data1, 24576, 10);
// This macro creates the following:
// 1. An array to hold NVM data named nvm3Data1_nvm
// 2. A section called nvm3Data1_section containing nvm3Data1_nvm. The application linker script must place this section correctly in memory.
// 3. A cache array: nvm3Data1_cache

void report_all_data(void);
void join_net_work(uint8_t join_v);
void set_write_flag(uint8_t flag);


G_USER_CTRL_T tUserCtrl ={0,};
G_USER_CTRL_T *tpUserControl =&tUserCtrl;


#define   User_Dat_ID   1   //存储对象标识
#define   Mark_Head     0x55
#define   Mark_End      0xa4

void emberAfMainTickCallback(void)
{
 // led_toggle(&sl_led_led0);
}

uint8_t nvm_init_ok =0;
void nvm3_user_init(void)
{

//  nvm3_Handle_t handle;
  // Declare a nvm3_Init_t struct of name nvm3Data1 with initialization data. This is passed to nvm3_open() below.
 // NVM3_DEFINE_SECTION_INIT_DATA(nvm3Data1, &nvm3_halFlashHandle);

  G_USER_CTRL_T tUserInit ={0,};
  G_USER_CTRL_T *tpUserInit =&tUserInit;
  Ecode_t status;

  //status = nvm3_open(&handle, &nvm3Data1);  //打开 nvm3存储器
  status = nvm3_initDefault();
  sl_zigbee_app_debug_println("oPen status: 0x%x \n",status);
  if (status != ECODE_NVM3_OK) {
    // Handle error
      sl_zigbee_app_debug_println("oPen nvm3 error \n");
  }


#if 0
    size_t dataLen1;
    //读取对应句柄中的存储数量，如果没有对象写入初始值，
    // Get the number of valid keys already in NVM3
    dataLen1 = nvm3_countObjects(nvm3_defaultHandle);

    sl_zigbee_app_debug_println("object dataLen1 %d\n",dataLen1);
      //想要添加新的对象标识符时可以先查一下总共定义了多少个标识nvm3_countObjects(nvm3_defaultHandle);
         //然后使用下面的nvm3_enumObjects   跑遍所有的标识，将标识名打印出来，这样可以确保新添加的标识符不会与旧的重复
    nvm3_ObjectKey_t keyList[dataLen1];
    size_t numKeys = nvm3_enumObjects(nvm3_defaultHandle, keyList, dataLen1, NVM3_KEY_MIN, NVM3_KEY_MAX);
    for (size_t i = 0; i < numKeys; i++) {
        nvm3_ObjectKey_t key = keyList[i];
        // 打印对象标识
         sl_zigbee_app_debug_println("Object Key: %u\n", key);
    }

    if (dataLen1 <= 72)   //72是SDK中被用到的标识数量，想要新增则以这个数量做判断


    //读回目标标识对象的数据类型和数据长度
       nvm3_getObjectInfo(nvm3_defaultHandle, User_Dat_ID, &objectType, &dataLen1);
#endif
    // Skip if we have initial keys. If not, generate objects and store
    // persistently in NVM3 before proceeding.

  // nvm3_getObjectInfo(nvm3_defaultHandle, User_Dat_ID, &objectType, &dataLen1);
  // sl_zigbee_app_debug_println("objectType: %d dataLen1 %d\n",objectType,dataLen1);
   status = nvm3_readData(nvm3_defaultHandle, User_Dat_ID, tpUserInit, sizeof(G_USER_CTRL_T));
  // sl_zigbee_app_debug_println("INIT  MarkHead: %d MarkEnd %d\n",tpUserInit->MarkHead,tpUserInit->MarkEnd);
   if(status != 0){
      tpUserInit->MarkHead       = Mark_Head;
      tpUserInit->MarkEnd        = Mark_End;
      tpUserInit->Brightness     = 100;            //
      tpUserInit->Color_Temp     = 4600;            //
      tpUserInit->Onoff          = 1;              //
      tpUserInit->Power_On_Times = 0;
      tpUserInit->led_mode       = 0;            //0=色温模式， 1=RGB模式
      tpUserInit->Color_H        = 0;            //
      tpUserInit->Color_S        = 1000;            //
      tpUserInit->Color_V        = 1000;            //

      tpUserInit->Off_buff       = 1000;
      tpUserInit->On_buff        = 1000;

      tpUserInit->scen_mode      = 0;
      tpUserInit->work_mode      = 0x30;

      tpUserInit->not_disturb    = 0;
      tpUserControl->Count_Down  = 0;

      tpUserInit->test_mode      = 0;
      tpUserInit->test_time      = 0;
      tpUserInit->memory.mode    = 0;
      tpUserInit->memory.led_mode   = tpUserInit->led_mode;
      tpUserInit->memory.Color_H    = tpUserInit->Color_H;
      tpUserInit->memory.Color_S    = tpUserInit->Color_S;
      tpUserInit->memory.Color_V    = tpUserInit->Color_V;
      tpUserInit->memory.Brightness = tpUserInit->Brightness;
      tpUserInit->memory.Color_Temp = tpUserInit->Color_Temp;

      memcpy(tpUserControl, tpUserInit, sizeof(G_USER_CTRL_T)); //写入应用
   //   status = nvm3_writeData(nvm3_defaultHandle, User_Dat_ID, &tUserInit, sizeof(G_USER_CTRL_T)); //写入flash
      sl_zigbee_app_debug_println("INIT write status: 0x%x \n",status);

   }else{
     if((tpUserInit->MarkHead != Mark_Head) || (tpUserInit->MarkEnd != Mark_End))
     {
         sl_zigbee_app_debug_println("chage  MarkHead: %d MarkEnd %d\n",tpUserInit->MarkHead,tpUserInit->MarkEnd);
         tpUserInit->MarkHead          = Mark_Head;
         tpUserInit->MarkEnd           = Mark_End;
         tpUserInit->Brightness        = 100;            //
         tpUserInit->Color_Temp        = 4600;            //
         tpUserInit->Onoff             = 1;               //
         tpUserInit->Power_On_Times    = 0;
         tpUserInit->led_mode          = 0;            //0=色温模式， 1=RGB模式
         tpUserInit->Color_H           = 0;            //
         tpUserInit->Color_S           = 1000;         //
         tpUserInit->Color_V           = 1000;         //
         tpUserInit->Off_buff          = 1000;
         tpUserInit->On_buff           = 1000;

         tpUserInit->scen_mode         = 0;
         tpUserInit->work_mode         = 0x30;

         tpUserInit->not_disturb       = 0;

         tpUserInit->test_mode         = 0;
         tpUserInit->test_time         = 0;
         tpUserInit->memory.mode       = 0;
         tpUserInit->memory.led_mode   = tpUserInit->led_mode;
         tpUserInit->memory.Color_H    = tpUserInit->Color_H;
         tpUserInit->memory.Color_S    = tpUserInit->Color_S;
         tpUserInit->memory.Color_V    = tpUserInit->Color_V;
         tpUserInit->memory.Brightness = tpUserInit->Brightness;
         tpUserInit->memory.Color_Temp = tpUserInit->Color_Temp;
         tpUserInit->not_disturb       = 0;
         tpUserControl->Count_Down     = 0;
     }

     if(tpUserInit->memory.mode ==0){ //恢复记忆，则不用管
         tpUserInit->memory.mode =0;
     }else if(tpUserInit->memory.mode ==1){//初始化模式
         tpUserInit->Onoff         = 1;
        tpUserInit->led_mode       = 0;            //0=色温模式， 1=RGB模式
        tpUserInit->work_mode      = 0x30;
        tpUserInit->scen_mode      = 0;
        tpUserInit->Color_H        = 0;            //
        tpUserInit->Color_S        = 1000;
        tpUserInit->Color_V        = 1000;
        tpUserInit->Brightness     = 100;
        tpUserInit->Color_Temp     = 4600;
     }else{                                //自定义模式
         tpUserInit->Onoff         = 1;
         tpUserInit->led_mode   = tpUserInit->memory.led_mode;
         tpUserInit->Color_H    = tpUserInit->memory.Color_H;
         tpUserInit->Color_S    = tpUserInit->memory.Color_S;
         tpUserInit->Color_V    = tpUserInit->memory.Color_V;
         tpUserInit->Brightness = tpUserInit->memory.Brightness;
         tpUserInit->Color_Temp = tpUserInit->memory.Color_Temp;
         tpUserInit->work_mode  = 0x30+tpUserInit->led_mode;
     }

     memcpy(tpUserControl, tpUserInit, sizeof(G_USER_CTRL_T)); //写入应用

   }
        if(++tpUserControl->Power_On_Times >=6){
         //   tpUserInit->Power_On_Times =0;    //短时间连续上电3次，执行重置配网
            join_net_work(1);
        }
        sl_zigbee_app_debug_println("tpUserControl->Power_On_Times: %d\n",tpUserControl->Power_On_Times);
        if(tpUserControl->Color_Temp >6500) tpUserControl->Color_Temp=6500;
        if(tpUserControl->Brightness >100) tpUserControl->Brightness=100;
        if(tpUserControl->Color_H >359)  tpUserControl->Color_H=0;
        if(tpUserControl->Color_S >1000) tpUserControl->Color_S=1000;
        if(tpUserControl->Color_V >1000) tpUserControl->Color_V=1000;
        if(tpUserControl->Power_On_Times > tpUserControl->not_disturb){  //启动断电勿扰，要2次通电才能打开
             if(tpUserControl->not_disturb)
                tpUserControl->Onoff =1;   // 满足连续2次上电， 开灯

             set_scen_cw_led_mode(tpUserControl->scen_mode);
             set_scen_hsv_led_mode(tpUserControl->scen_mode);
             sl_zigbee_app_debug_println("hsv %d  %d  %d  lihgt %d temp %d  ",tpUserControl->Color_H,tpUserControl->Color_S,tpUserControl->Color_V,tpUserControl->Color_Temp,tpUserControl->Color_Temp);

             uint16_t s_buff =0;
             if(!tpUserControl->led_mode){
                  s_buff = tpUserControl->Brightness*10;
             }else{
                  s_buff = tpUserControl->Color_V;
             }
             sl_zigbee_app_debug_println("s_buff =%d led_mode %d  light %d   colorv %d ",s_buff,tpUserControl->led_mode,tpUserControl->Brightness,tpUserControl->Color_V);

             if(tpUserControl->Onoff){
                 execute_led_change_cal(s_buff);
             }
             else{
                 execute_led_change_cal(s_buff);
             }
         }else{
             tpUserControl->Onoff =0;   //灭灯状态
         }

        nvm3_writeData(nvm3_defaultHandle, User_Dat_ID, tpUserControl, sizeof(G_USER_CTRL_T)); //写入flash
        nvm_init_ok =1;

       //同步给协议栈
       emberAfWriteServerAttribute(1,ZCL_ON_OFF_CLUSTER_ID,
                                   ZCL_ON_OFF_ATTRIBUTE_ID,
                                    (uint8_t *)&(tpUserControl->Onoff),
                                    ZCL_BOOLEAN_ATTRIBUTE_TYPE);
        sl_zigbee_app_debug_println("tpUserControl-> Onoff %d  work_mode  %d  memory.mode %d  light: %d   color =%d\n",
                                    tpUserControl->Onoff,tpUserInit->work_mode,tpUserControl->memory.mode,tpUserControl->Brightness,tpUserControl->Color_Temp);
   // 关闭 NVM3 控制器
 //  nvm3_deinitDefault();
 //  nvm3_close(nvm3_defaultHandle);
}

void test_pwm(void)
{
  static uint8_t stepy =0;
   switch(stepy){
    case Num_red:
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm_r, 100);
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm_g, 0);
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm_b, 0);
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm0, 0);
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm1, 0);
      break;
    case Num_green:
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm_g, 100);
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm_r, 0);
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm_b, 0);
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm0, 0);
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm1, 0);
      break;
    case Num_blue:
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm_b, 100);
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm_g, 0);
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm_r, 0);
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm0, 0);
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm1, 0);
      break;
    case Num_warm:
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm0, 100);    //黄色  pc02
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm_g, 0);
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm_b, 0);
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm_r, 0);
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm1, 0);
      break;
    case Num_cool:
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm1, 100);    //白色  pd00
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm_g, 0);
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm_b, 0);
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm0, 0);
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm_r, 0);
      break;
    default:
      break;
  }
  if(++stepy > Num_warm)
    stepy = Num_red;
}


void set_rgbcw_pwm(uint8_t num, uint8_t pwm)
{
  static uint8_t temp_r,temp_g,temp_b,temp_c,temp_w;
  switch(num){
    case Num_red:
      if(temp_r != pwm){
          temp_r = pwm;
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm_r, pwm);
      }
      break;
    case Num_green:
      if(temp_g != pwm){
          temp_g = pwm;
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm_g, pwm);
      }
      break;
    case Num_blue:
      if(temp_b != pwm){
          temp_b = pwm;
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm_b, pwm);
      }
      break;
    case Num_warm:
      if(temp_c != pwm){
          temp_c = pwm;
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm0, pwm);    //黄色  pc02
      }
      break;
    case Num_cool:
      if(temp_w != pwm){
          temp_w = pwm;
          sl_pwm_set_custon_duty_cycle(&sl_pwm_pwm1, pwm);    //白色  pd00
      }
      break;
    default:
      break;
  }
}



uint8_t  r_write_flag =0;
uint8_t  delay_write =0;
void set_write_flag(uint8_t flag)
{
  r_write_flag =flag;
  delay_write =0;
  sl_zigbee_app_debug_println("set write flash\n");

}

void clean_flash(void)
{
  G_USER_CTRL_T tUserInit ={0,};
  G_USER_CTRL_T *tpUserInit =&tUserInit;
       tpUserInit->MarkHead       = Mark_Head;
       tpUserInit->MarkEnd        = Mark_End;
       tpUserInit->Brightness     = 100;            //
       tpUserInit->Color_Temp     = 4600;            //
       tpUserInit->Onoff          = 1;              //
       tpUserInit->Power_On_Times = 0;
       tpUserInit->led_mode       = 0;            //0=色温模式， 1=RGB模式
       tpUserInit->Color_H        = 0;            //
       tpUserInit->Color_S        = 1000;            //
       tpUserInit->Color_V        = 1000;            //

       tpUserInit->Off_buff       = 1000;
       tpUserInit->On_buff        = 1000;

       tpUserInit->scen_mode      = 0;
       tpUserInit->work_mode      = 0x30;

       tpUserInit->not_disturb    = 0;
       tpUserControl->Count_Down  = 0;

       tpUserInit->memory.mode    = 0;
       tpUserInit->memory.led_mode   = tpUserInit->led_mode;
       tpUserInit->memory.Color_H    = tpUserInit->Color_H;
       tpUserInit->memory.Color_S    = tpUserInit->Color_S;
       tpUserInit->memory.Color_V    = tpUserInit->Color_V;
       tpUserInit->memory.Brightness = tpUserInit->Brightness;
       tpUserInit->memory.Color_Temp = tpUserInit->Color_Temp;
       //产测标记不会被清理
       tpUserInit->test_mode      = tpUserControl->test_mode;
       tpUserInit->test_time      = tpUserControl->test_time;
  memcpy(tpUserControl, tpUserInit, sizeof(G_USER_CTRL_T)); //写入应用
  nvm3_writeData(nvm3_defaultHandle, User_Dat_ID, &tUserInit, sizeof(G_USER_CTRL_T)); //写入flash
  sl_zigbee_app_debug_println("clean_flash :  \n");
}


void write_flash(void)
{
  if(++delay_write >=20){
      delay_write =0;     //250MS写入flash    主循环时间不稳定

   //   led_toggle(&sl_led_led1);
   //   sl_zigbee_app_debug_println("start write flash\n");
    if(r_write_flag){
      r_write_flag =0;
      G_USER_CTRL_T tUserInit ={0,};
      G_USER_CTRL_T *tpUserInit =&tUserInit;
      nvm3_readData(nvm3_defaultHandle, User_Dat_ID, tpUserInit, sizeof(G_USER_CTRL_T));
      sl_zigbee_app_debug_println("read00 ->light: %d   color =%d\n",tpUserInit->Brightness,tpUserInit->Color_Temp);

      if((tpUserInit->MarkHead == Mark_Head) && (tpUserInit->MarkEnd == Mark_End)){
          EmberStatus status = nvm3_writeData(nvm3_defaultHandle, User_Dat_ID, tpUserControl, sizeof(G_USER_CTRL_T)); //写入flash
          sl_zigbee_app_debug_println("write status: 0x%x \n",status);
          nvm3_readData(nvm3_defaultHandle, User_Dat_ID, tpUserInit, sizeof(G_USER_CTRL_T));
          sl_zigbee_app_debug_println("read onoff :%d   light: %d   color =%d\n",tpUserInit->Onoff,tpUserInit->Brightness,tpUserInit->Color_Temp);
      }
    }
  }
}


void Restore_to_last_state(void)
{
  G_USER_CTRL_T tUserInit ={0,};
  G_USER_CTRL_T *tpUserInit =&tUserInit;
  nvm3_readData(nvm3_defaultHandle, User_Dat_ID, tpUserInit, sizeof(G_USER_CTRL_T));
//  sl_zigbee_app_debug_println("read00 ->light: %d   color =%d\n",tpUserInit->Brightness,tpUserInit->Color_Temp);
  memcpy(tpUserControl, tpUserInit, sizeof(G_USER_CTRL_T)); //写入应用
}


static app_timer_t app_opened_timer;
EmberStatus r_cancel_network =0;  //定时器调用重复配网标志
EmberStatus get_network_state(void)
{
  return r_cancel_network;
}

void join_net_work(uint8_t join_v)
{
  EmberStatus status =0;
  status  = emberAfNetworkState();
  sl_zigbee_app_debug_println("ember work status: 0x%02X\n",status);
  switch (status) {
      case EMBER_NO_NETWORK:
          // 如果当前没有加入任何 Zigbee 网络，则启动网络加入流程。
          status = emberAfPluginNetworkSteeringStart();
          sl_zigbee_app_debug_println("EMBER_NO_NETWORK state: 0x%02X",status);
          if(join_v){
              r_cancel_network =1;   //定时调用加网
              emberAfZllResetToFactoryNew();      //调用加网程序即会删除相关数据进行初始化
              clean_flash();
          }
          break;
      case EMBER_JOINING_NETWORK:
          // 如果当前正在尝试加入 Zigbee 网络，则不进行任何操作。
          break;
      case EMBER_JOINED_NETWORK:
          // 如果当前已经加入了 Zigbee 网络，则不进行任何操作。
          if(join_v){
              emberAfZllResetToFactoryNew();
              clean_flash();
         //   status = emberLeaveNetwork();  //先离开网络
         //   status = emberAfPluginNetworkSteeringStart();
            sl_zigbee_app_debug_println("EMBER_JOINED_NETWORK state: 0x%02X",status);
            r_cancel_network =1;
          }else{
              r_cancel_network =0;
          }
          break;
      case EMBER_JOINED_NETWORK_NO_PARENT:
          // 如果当前已经加入了 Zigbee 网络，但是没有找到合适的父节点，则重新启动网络加入流程。
          emberAfPluginNetworkSteeringStop();
        //  emberAfPluginNetworkSteeringStart();
          r_cancel_network =1;
          break;
      default:
          // 对于其他的状态值，可以进行相应的错误处理。
          break;
  }

}



static uint8_t  one_sec_flg =0;
static uint8_t half_sec_flg =0;







void rep_join(void)
{
  if(get_Test_State())   //产测状态，功能不执行
    return;
  static bool init_power_flag =0;
  static uint8_t  times_count =0;
  static uint8_t  times_count2 =0;

        if(r_cancel_network){
           if(times_count <178){
               join_net_work(0);       //3分钟内才会允许调用beacon加入网络
           }
           if(++times_count >=186){    // 186S 配网提示时长
               times_count = 0;
               r_cancel_network =0;
           }
       }else{
           times_count =0;
       }
       if(!init_power_flag){
           if(++times_count2 >=3){  //通电3S后清除上电数据。
               times_count2 =0;
               init_power_flag =1;
               tpUserControl->Power_On_Times = 0;
               set_write_flag(1);  //写入flash
           }
       }
}

uint8_t init_ok =0;
uint8_t get_init_state(void)
{
  return init_ok;
}



static uint8_t r_Abnormal_recovery =0;  //异常计时恢复
static uint8_t r_30second =0;
static uint16_t delay_report =0;
static uint8_t  f_save_data =0;
void app_opened_timer_cbbbx(app_timer_t *timer, void *data)
{
  static uint8_t  sec_count =0;
  static uint8_t  led_pwm =0;
  static uint8_t  c_250ms =0;
  static uint8_t  c_100ms =0;
  static uint8_t  half_sec_count =0;
  (void)data;
  (void)timer;
   //EmberStatus status  = emberAfNetworkState();
  // sl_zigbee_app_debug_println("ember work status: 0x%02X\n",status);

 // led_toggle(&sl_led_led0);
 // led_toggle(&sl_led_led1);

  if(++c_100ms >=10){ //100MS
      c_100ms =0;
      count_100ms ++;
      r_Abnormal_recovery =0;
      if(++sec_count >=10){  //1S时间到
         sec_count =0;
         one_sec_flg =1;
         r_30second ++;
         init_ok =1;      //初始化化后过1S
      }

      if(++half_sec_count >=5){
          half_sec_flg =1;
          half_sec_count =0;
      }
  }

  if(get_Test_State())   //产测状态，功能不执行
    return;
  if(delay_report > 1){
    delay_report --;
  }

  if(r_cancel_network == 1){
      if(++c_250ms >=50){
        c_250ms =0;

        if(!f_save_data){
            f_save_data =1;
            tpUserControl->Onoff          = 1;   //必须为开灯状态
          //  c_light = tpUserControl->Brightness;
          //  c_temp  = tpUserControl->Color_Temp;
          //  printf("save write  light =%d   temp =%d \n",tpUserControl->Brightness,tpUserControl->Color_Temp);

        }
        if(led_pwm ==0){
            led_pwm =50;
            tpUserControl->Brightness = 5;
            tpUserControl->Color_Temp = 6500;
        }else{
            tpUserControl->Brightness = 50;
            tpUserControl->Color_Temp = 6500;
            led_pwm = 0;
        }
        set_led_temp_color(500);


#if 0
        if(led_pwm ==0){
            led_pwm =30;
        }else{
            led_pwm = 0;
        }
        set_rgbcw_pwm(Num_cool,led_pwm);
        set_rgbcw_pwm(Num_red,0);
        set_rgbcw_pwm(Num_green,0);
        set_rgbcw_pwm(Num_blue,0);
        set_rgbcw_pwm(Num_warm,0);
        cal_speed(0);
#endif
      }
  }else{
      if(f_save_data){
          f_save_data =0;
          tpUserControl->Brightness = 100;
          tpUserControl->Color_Temp = 4600;
          set_led_temp_color(500);
          printf("save read  light =%d   temp =%d \n",tpUserControl->Brightness,tpUserControl->Color_Temp);
      }
  }
  f_pwm_buffer();
}




//定时上报开关状态用于心跳
void time_report_hearbeat(void)
{
//  static uint8_t rand_tims=30;

  uint32_t tickCount  = halCommonGetInt32uMillisecondTick();
  if(HeartbeatTime){  //有查询协议信息来就不执行定时上报，没有就执行随机定时上报
    if((tickCount - Save_HbTime) >=HeartbeatTime)
    {
        sl_zigbee_app_debug_println(" ===  tickCount %d  Save_HbTime =%d \n",tickCount,Save_HbTime);
        printf("hearbeat %d\n",HeartbeatTime);
        HeartbeatTime =0;
        report_hearbeat();
        r_30second =0;
    }
  }
#if 0
  else{
      if(r_30second >=rand_tims){
           rand_tims = 20+ (rand()%20);
           printf("hearbeat %d\n",rand_tims);
           r_30second =0;
           report_hearbeat();
       }
  }
#endif
}



void app_init(void)
{
  // Enable PWM output
  sl_pwm_start(&sl_pwm_pwm_r);
  sl_pwm_start(&sl_pwm_pwm_g);
  sl_pwm_start(&sl_pwm_pwm_b);

    sl_pwm_start(&sl_pwm_pwm0);
#if(User_PWM >= 2)
    sl_pwm_start(&sl_pwm_pwm1);
#endif
    app_timer_start(&app_opened_timer,
    Time_Count,
    app_opened_timer_cbbbx,
    NULL,
    true);


    nvm3_user_init();
    set_rand_source();
    emberSetRadioPower(EMBER_AF_PLUGIN_NETWORK_STEERING_RADIO_TX_POWER);  //设置最大功率输出
}

//修复定时
void  Repair_timer(void)
{
  if(++r_Abnormal_recovery >=25){  //0.25S 异常恢复定时器
      r_Abnormal_recovery =0;
      app_timer_start(&app_opened_timer,
          Time_Count,
          app_opened_timer_cbbbx,
          NULL,
          true);
      sl_zigbee_app_debug_println("Repair_timer \n");
  }
}






void f_half_sec_run(void)
{

  if(half_sec_flg){ //半秒执行
      half_sec_flg =0;
      F_Production_Test();
     // test_pwm();
  }
}





//1秒执行调用
void f_one_sec_run(void)
{
  if(one_sec_flg){
      one_sec_flg =0;
      rep_join();            //重复入网操作
      f_count_down_off(&tpUserControl->Count_Down);
      Cout_Net_Time();
  //    sl_zigbee_app_debug_println("test 5L 1.0.2 \n");

  }
}


bool report_state =0;

void app_process_action(void)    // user_main
{
  static uint32_t save_tick =0;

  uint32_t tickCount  = halCommonGetInt32uMillisecondTick();

  time_report_hearbeat();   //定时上报 心跳
//  set_rgb_pwm(tpStarColor->R,tpStarColor->G,tpStarColor->B);
  if(tickCount - save_tick >=10){ //10ms执行函数
      save_tick = tickCount;

      if(delay_report == 1){
         delay_report = 0;
         report_all_data();

      }
      write_flash();         //写FLASH
      f_one_sec_run();
      f_half_sec_run();
      if(report_state){
          report_state =0;
          report_all_data();
      }
    //  sl_zigbee_app_debug_println("save_tick times =%d\n",save_tick);
      if(r_cancel_network == 0){    //动态场景循环调用
          if(tpUserControl->Onoff){
              set_scen_hsv_led_mode(tpUserControl->scen_mode);
          }
      }
      Repair_timer();

  }

 // sl_zigbee_app_debug_println("tickCount %d\n",tickCount);

}




//不判断广播和多播的上报
void report_Attributes(void *value,uint16_t len)
{
    if(emberAfNetworkState() != EMBER_JOINED_NETWORK){
        sl_zigbee_app_debug_println("no network return report_hearbeat:%d\r\n");
        return;
    }

   uint16_t  idx =0,i;
   uint8_t  status =0;
   uint8_t  attribute_record3[EMBER_AF_PLUGIN_FRAGMENTATION_BUFFER_SIZE] ={0};



    //报开关状态
    idx =0;
    attribute_record3[idx++] = LOW_BYTE(0x0068);
    attribute_record3[idx++] = HIGH_BYTE(0x0068);//私有协议定义属性
    attribute_record3[idx++] = ZCL_OCTET_STRING_ATTRIBUTE_TYPE;
  //  attribute_record3[idx++] = len;
    sl_zigbee_app_debug_print(" len 0x%02X ",len);
    uint8_t *val_ptr = (uint8_t*)value;

     for (i = 0; i < len; i++) {
        attribute_record3[idx++] = val_ptr[i]; // 解引用指针取得数据
        sl_zigbee_app_debug_print(" 0x%02X ",val_ptr[i]);
     }

     //将属性报告填充进入缓冲区   CLUSTER值   数据内容  数据长度      //Fill a ZCL global report attributes command buffer  ZCL_BASIC_CLUSTER_ID  ZCL_GENERIC_TUNNEL_CLUSTER_ID
     status = emberAfFillCommandGlobalServerToClientReportAttributes_no_response(ZCL_BASIC_CLUSTER_ID,attribute_record3,idx);
    sl_zigbee_app_debug_println("ReportAttributes_no_response: 0x%02X\n",status);
    //写入 sourceEndpoint  和 destinationEndpoint    //Specify endpoints for command sending
    emberAfSetCommandEndpoints(0x01, 0x01);

    //调用发送数据接口将缓存好的数据发出           发生选项  和  单播目标地址
    status = emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, 0x0000);
   // sl_zigbee_app_debug_println("emberAfSendCommandUnicast: 0x%02X\n",status);
    //status2 = emberAfSendCommandUnicastToBindings();
}



void report_agreement(void *value,uint16_t len)
{
    if(emberAfNetworkState() != EMBER_JOINED_NETWORK){
        sl_zigbee_app_debug_println("no network return report_hearbeat:%d\r\n");
        return;
    }
    uint8_t  type=emberAfCurrentCommand()->type;


    //   sl_zigbee_app_debug_println("groupId :%d  option %x  destinationEndpoint %d   type =%x\r\n",emberAfCurrentCommand()->apsFrame->groupId,
     //                              emberAfCurrentCommand()->apsFrame->options,emberAfCurrentCommand()->apsFrame->destinationEndpoint,emberAfCurrentCommand()->type);





    if((type == EMBER_INCOMING_MULTICAST) || (recevi_type == EMBER_INCOMING_MULTICAST)){ //群组消息不回复  report
        return;
    }

   uint16_t  idx =0,i;
   uint8_t  status =0;
   uint8_t  attribute_record3[EMBER_AF_PLUGIN_FRAGMENTATION_BUFFER_SIZE] ={0};



    //报开关状态
    idx =0;
    attribute_record3[idx++] = LOW_BYTE(0x0068);
    attribute_record3[idx++] = HIGH_BYTE(0x0068);//私有协议定义属性
    attribute_record3[idx++] = ZCL_OCTET_STRING_ATTRIBUTE_TYPE;
  //  attribute_record3[idx++] = len;
    sl_zigbee_app_debug_print(" len 0x%02X ",len);
    uint8_t *val_ptr = (uint8_t*)value;

     for (i = 0; i < len; i++) {
        attribute_record3[idx++] = val_ptr[i]; // 解引用指针取得数据
        sl_zigbee_app_debug_print(" 0x%02X ",val_ptr[i]);
     }



     //将属性报告填充进入缓冲区   CLUSTER值   数据内容  数据长度      //Fill a ZCL global report attributes command buffer  ZCL_BASIC_CLUSTER_ID  ZCL_GENERIC_TUNNEL_CLUSTER_ID
     status = emberAfFillCommandGlobalServerToClientReportAttributes_no_response(ZCL_BASIC_CLUSTER_ID,attribute_record3,idx);
    sl_zigbee_app_debug_println("ReportAttributes_no_response: 0x%02X\n",status);
    //写入 sourceEndpoint  和 destinationEndpoint    //Specify endpoints for command sending
    emberAfSetCommandEndpoints(0x01, 0x01);

    //调用发送数据接口将缓存好的数据发出           发生选项  和  单播目标地址
    status = emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, 0x0000);
   // sl_zigbee_app_debug_println("emberAfSendCommandUnicast: 0x%02X\n",status);
    //status2 = emberAfSendCommandUnicastToBindings();
}




static void commissioning_led_event_handler(sl_zigbee_event_t *event)
{

  sl_zigbee_app_debug_println("get work %d\n",emberAfNetworkState());
  if (emberAfNetworkState() == EMBER_JOINED_NETWORK) {
    uint16_t identifyTime;
    emberAfReadServerAttribute(LIGHT_ENDPOINT,
                               ZCL_IDENTIFY_CLUSTER_ID,
                               ZCL_IDENTIFY_TIME_ATTRIBUTE_ID,
                               (uint8_t *)&identifyTime,
                               sizeof(identifyTime));
#if 0
    if (identifyTime > 0) {
      led_toggle(COMMISSIONING_STATUS_LED);
      sl_zigbee_event_set_delay_ms(&commissioning_led_event,
                                   LED_BLINK_PERIOD_MS << 1);
    } else {
      led_turn_on(COMMISSIONING_STATUS_LED);
    }
#endif
  } else {
  //  EmberStatus status = emberAfPluginNetworkSteeringStart();
  //  r_cancel_network =1;
    sl_zigbee_app_debug_println("up Not Join network");
    sl_zigbee_event_set_delay_ms(&com_delay_join,500);  //上电后检查到没有加入任何网络延迟500ms后进入配网状态，500MS串口留给产测信标检查时间。
  }
}




static void finding_and_binding_event_handler(sl_zigbee_event_t *event)
{
  if (emberAfNetworkState() == EMBER_JOINED_NETWORK) {
    sl_zigbee_event_set_inactive(&finding_and_binding_event);

    sl_zigbee_app_debug_println("Find and bind target start: 0x%02X",
                                emberAfPluginFindAndBindTargetStart(LIGHT_ENDPOINT));
  }
}
static void com_delay_join_handler(sl_zigbee_event_t *event)
{
    sl_zigbee_app_debug_println("com_delay_join_handler");
    r_cancel_network =1;
}


//----------------------
// Implemented Callbacks

/** @brief Stack Status
 *
 * This function is called by the application framework from the stack status
 * handler.  This callbacks provides applications an opportunity to be notified
 * of changes to the stack status and take appropriate action. The framework
 * will always process the stack status after the callback returns.
 */
void emberAfStackStatusCallback(EmberStatus status)
{
  // Note, the ZLL state is automatically updated by the stack and the plugin.
  uint8_t rand_c =0;
  sl_zigbee_app_debug_println("emberAfStackStatusCallback =0x%02x\n\n",status);
  if (status == EMBER_NETWORK_DOWN) {
      sl_zigbee_app_debug_println("emberAfStackStatusCallback status = EMBER_NETWORK_DOWN\n\n");
      join_net_work(1);
   // led_turn_off(COMMISSIONING_STATUS_LED);
  } else if (status == EMBER_NETWORK_UP) {
   // led_turn_on(COMMISSIONING_STATUS_LED);
      sl_zigbee_event_set_active(&finding_and_binding_event);
      set_rand_source();
      rand_c = (rand()%500);
      rand_c = (rand()%500);
      delay_report = 5+rand_c; //延迟上报所有状态，保证读到flash中的内容
      sl_zigbee_app_debug_println("emberAfStackStatusCallback status = EMBER_NETWORK_UP delay %d\n\n",delay_report);

  }
}

/** @brief Init
 * Application init function
 */
void emberAfMainInitCallback(void)
{
  sl_zigbee_event_init(&commissioning_led_event, commissioning_led_event_handler);
  sl_zigbee_event_init(&finding_and_binding_event, finding_and_binding_event_handler);

  sl_zigbee_event_init(&com_delay_join, com_delay_join_handler);
  sl_zigbee_event_set_active(&commissioning_led_event);
}

/** @brief Complete network steering.
 *
 * This callback is fired when the Network Steering plugin is complete.
 *
 * @param status On success this will be set to EMBER_SUCCESS to indicate a
 * network was joined successfully. On failure this will be the status code of
 * the last join or scan attempt.
 *
 * @param totalBeacons The total number of 802.15.4 beacons that were heard,
 * including beacons from different devices with the same PAN ID.
 *
 * @param joinAttempts The number of join attempts that were made to get onto
 * an open Zigbee network.
 *
 * @param finalState The finishing state of the network steering process. From
 * this, one is able to tell on which channel mask and with which key the
 * process was complete.
 */
void emberAfPluginNetworkSteeringCompleteCallback(EmberStatus status,
                                                  uint8_t totalBeacons,
                                                  uint8_t joinAttempts,
                                                  uint8_t finalState)
{
  sl_zigbee_app_debug_println("Join network complete: 0x%02X", status);

  if (status != EMBER_SUCCESS) {
    // Initialize our ZLL security now so that we are ready to be a touchlink
    // target at any point.

    #if 0
        //尝试自己组件网络 边缘设备不需要
        status = emberAfZllSetInitialSecurityState();
        if (status != EMBER_SUCCESS) {
          sl_zigbee_app_debug_println("Error: cannot initialize ZLL security: 0x%02X", status);
        }

        status = emberAfPluginNetworkCreatorStart(false); // distributed
        sl_zigbee_app_debug_println("Form network start: 0x%02X", status);
    #endif
  }
}

/** @brief Complete the network creation process.
 *
 * This callback notifies the user that the network creation process has
 * completed successfully.
 *
 * @param network The network that the network creator plugin successfully
 * formed.
 *
 * @param usedSecondaryChannels Whether or not the network creator wants to
 * form a network on the secondary channels.
 */
void emberAfPluginNetworkCreatorCompleteCallback(const EmberNetworkParameters *network,
                                                 bool usedSecondaryChannels)
{
  sl_zigbee_app_debug_println("Form Network Complete: 0x%02X",
                              EMBER_SUCCESS);
}

/** @brief Post Attribute Change
 *
 * This function is called by the application framework after it changes an
 * attribute value. The value passed into this callback is the value to which
 * the attribute was set by the framework.
 */


void report_fuction(uint8_t ATTRIBUTE_ID, uint8_t TYPE, uint16_t CLUSTER,void *value,uint8_t len)
{
    if(emberAfNetworkState() != EMBER_JOINED_NETWORK){
        sl_zigbee_app_debug_println("no network return report_hearbeat:%d\r\n");
        return;
    }
   uint8_t  idx =0,i;
   uint8_t  attribute_record3[20] ={0};

   uint8_t  type=emberAfCurrentCommand()->type;


//   sl_zigbee_app_debug_println("groupId :%d  option %x  destinationEndpoint %d   type =%x\r\n",emberAfCurrentCommand()->apsFrame->groupId,
 //                              emberAfCurrentCommand()->apsFrame->options,emberAfCurrentCommand()->apsFrame->destinationEndpoint,emberAfCurrentCommand()->type);




    if((type == EMBER_INCOMING_MULTICAST) || (recevi_type == EMBER_INCOMING_MULTICAST)){ //群组消息不回复  report
        return;
    }

    //报开关状态
    idx =0;
    attribute_record3[idx++] = LOW_BYTE(ATTRIBUTE_ID);
    attribute_record3[idx++] = HIGH_BYTE(ATTRIBUTE_ID);
    attribute_record3[idx++] = TYPE;
  //  attribute_record3[idx++] = len;

  //  sl_zigbee_app_debug_println("len[i]: 0x%02X\n",len);
    uint8_t *val_ptr = (uint8_t*)value;


     for (i = 0; i < len; i++) {
        attribute_record3[idx++] = val_ptr[i]; // 解引用指针取得数据
     //   sl_zigbee_app_debug_println("val_ptr[i]: 0x%02X\n",val_ptr[i]);
     }




    //将属性报告填充进入缓冲区   CLUSTER值   数据内容  数据长度      //Fill a ZCL global report attributes command buffer

    uint8_t status = emberAfFillCommandGlobalServerToClientReportAttributes_no_response(CLUSTER,attribute_record3,idx);
    sl_zigbee_app_debug_println("ReportAttributes_no_response: 0x%02X\n",status);
    //写入 sourceEndpoint  和 destinationEndpoint    //Specify endpoints for command sending
    emberAfSetCommandEndpoints(0x01, 0x01);

    //调用发送数据接口将缓存好的数据发出           发生选项  和  单播目标地址
    status = emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, 0x0000);
  //  sl_zigbee_app_debug_println("emberAfSendCommandUnicast: 0x%02X  CLUSTER 0x%02X\n",status,CLUSTER);
    //status2 = emberAfSendCommandUnicastToBindings();
}


void report_zigbee_onoff_state(void)
{
  report_fuction(ZCL_ON_OFF_ATTRIBUTE_ID,ZCL_BOOLEAN_ATTRIBUTE_TYPE,
                           ZCL_ON_OFF_CLUSTER_ID,&tpUserControl->Onoff,sizeof(tpUserControl->Onoff));
}


uint8_t r_temp_onoff =0;
uint8_t r_temp_light =0;
uint16_t r_temp_color =0;
void emberAfPostAttributeChangeCallback(uint8_t endpoint,
                                        EmberAfClusterId clusterId,
                                        EmberAfAttributeId attributeId,
                                        uint8_t mask,
                                        uint16_t manufacturerCode,
                                        uint8_t type,
                                        uint8_t size,
                                        uint8_t* value)
{
//  sl_zigbee_app_debug_println("clusterId 0x%02x attributeId 0x%02x:  mask 0x%02X  value =%d\n",clusterId, attributeId, mask,*value);
  if(!nvm_init_ok) return; //数据未准备完成，不执行

  if (clusterId == ZCL_ON_OFF_CLUSTER_ID
      && attributeId == ZCL_ON_OFF_ATTRIBUTE_ID
      && mask == CLUSTER_MASK_SERVER) {
      if (emberAfReadServerAttribute(endpoint,
                                     ZCL_ON_OFF_CLUSTER_ID,
                                     ZCL_ON_OFF_ATTRIBUTE_ID,
                                     (uint8_t *)&r_temp_onoff,
                                     sizeof(r_temp_onoff))
          == EMBER_ZCL_STATUS_SUCCESS)
      {
          //回复状态值
          sl_zigbee_app_debug_println("recv attribute onoff :%d\r\n", r_temp_onoff);
          if(r_temp_onoff !=tpUserControl->Onoff){
              tpUserControl->Onoff = r_temp_onoff ;
              set_led_onoff(tpUserControl->Onoff);

              set_write_flag(1);    //写入flash
          }

          report_fuction(ZCL_ON_OFF_ATTRIBUTE_ID,ZCL_BOOLEAN_ATTRIBUTE_TYPE,
                         ZCL_ON_OFF_CLUSTER_ID,&tpUserControl->Onoff,sizeof(tpUserControl->Onoff));
#if 0
              idx =0;
              attribute_record3[idx++] = LOW_BYTE(ZCL_ON_OFF_ATTRIBUTE_ID);
              attribute_record3[idx++] = HIGH_BYTE(ZCL_ON_OFF_ATTRIBUTE_ID);
              attribute_record3[idx++] = ZCL_BOOLEAN_ATTRIBUTE_TYPE;
              attribute_record3[idx++] = tpUserControl->Onoff;
              //将属性报告填充进入缓冲区   CLUSTER值   数据内容  数据长度      //Fill a ZCL global report attributes command buffer
              status = emberAfFillCommandGlobalServerToClientReportAttributes_no_response(ZCL_ON_OFF_CLUSTER_ID,attribute_record3,idx);

              //写入 sourceEndpoint  和 destinationEndpoint    //Specify endpoints for command sending
              emberAfSetCommandEndpoints(g_endpoint, 0x01);

              //调用发送数据接口将缓存好的数据发出           发生选项  和  单播目标地址
              status = emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, 0x0000);

              //status2 = emberAfSendCommandUnicastToBindings();
              sl_zigbee_app_debug_println("recv report onoff, status:%d\r\n", status);
#endif

      }
  }
#if 1
  //亮度
  if (clusterId == ZCL_LEVEL_CONTROL_CLUSTER_ID
        && attributeId == ZCL_CURRENT_LEVEL_ATTRIBUTE_ID
        && mask == CLUSTER_MASK_SERVER) {
      if (emberAfReadServerAttribute(endpoint,
                                     ZCL_LEVEL_CONTROL_CLUSTER_ID,
                                     ZCL_CURRENT_LEVEL_ATTRIBUTE_ID,
                                     (uint8_t *)&r_temp_light,
                                     sizeof(r_temp_light))
          == EMBER_ZCL_STATUS_SUCCESS)
      {
          //回复状态值
          sl_zigbee_app_debug_println("recv attribute r_temp_light  :%d  sOurce %lx\r\n", r_temp_light,recevi_source);
#if 1
          if(tpUserControl->Onoff){
              if(r_temp_light == 0) return; //非法数据，清除
              if(tpUserControl->led_mode == 0 || (recevi_source == 0x0000)){ //色温模式 //网关发来的亮度一定是色温亮度
                  if(r_temp_light != tpUserControl->Brightness){
                    if(r_temp_light >LIGHT_MAX){
                      r_temp_light =LIGHT_MAX;
                    }
                      uint16_t buff_s = (abs(tpUserControl->Brightness - r_temp_light)) *10;
                      tpUserControl->Brightness =r_temp_light;
                    //  set_led_temp_color(buff_s);
                      execute_led_change_cal(buff_s); //200ms缓冲时间
                   // if(!r_temp_return){
                  }
                  report_fuction(ZCL_CURRENT_LEVEL_ATTRIBUTE_ID,ZCL_INT8U_ATTRIBUTE_TYPE,
                                           ZCL_LEVEL_CONTROL_CLUSTER_ID,&tpUserControl->Brightness,
                                           sizeof(tpUserControl->Brightness));

                //  report_chage_work_mode(0x30); //色温模式
                  if(tpUserControl->work_mode ==0x32){
                      tpUserControl->work_mode =0x30;
                  }
              }else{
                  tpUserControl->Color_V = r_temp_light *10;
                 // set_led_hsv_color();
                  execute_led_change_cal(400); //200ms缓冲时间
               //   report_chage_work_mode(0x31); //色温模式
                  if(tpUserControl->work_mode ==0x32){
                      tpUserControl->work_mode =0x31;
                  }
              }

              set_write_flag(1);   //写入flash
          }

#endif
      }

  }
#endif

#if(User_PWM >= 2)
  //色温
  if (clusterId == ZCL_COLOR_CONTROL_CLUSTER_ID
        && attributeId == ZCL_COLOR_CONTROL_COLOR_TEMPERATURE_ATTRIBUTE_ID
        && mask == CLUSTER_MASK_SERVER) {
      if (emberAfReadServerAttribute(endpoint,
                                     ZCL_COLOR_CONTROL_CLUSTER_ID,
                                     ZCL_COLOR_CONTROL_COLOR_TEMPERATURE_ATTRIBUTE_ID,
                                     (uint8_t *)&r_temp_color,
                                     sizeof(r_temp_color))
          == EMBER_ZCL_STATUS_SUCCESS)
      {

          sl_zigbee_app_debug_println("recv attribute r_temp_color  :%d\r\n", r_temp_color);
          if(r_temp_color != tpUserControl->Color_Temp && tpUserControl->Onoff){
             if(r_temp_color >COLOR_MAX)
                 r_temp_color =COLOR_MAX;
             else if(r_temp_color < COLOR_MIN){
                 r_temp_color =COLOR_MIN;
             }
             uint16_t buff_s = (abs(tpUserControl->Color_Temp - r_temp_color))/4;
             tpUserControl->Color_Temp =r_temp_color;
             set_led_temp_color(buff_s);
            //回复状态值
             set_write_flag(1);    //写入flash
          }
         // report_chage_work_mode(0x30); //色温模式
          if(tpUserControl->work_mode ==0x32){
              tpUserControl->work_mode =0x30;
          }
          report_fuction(ZCL_COLOR_CONTROL_COLOR_TEMPERATURE_ATTRIBUTE_ID,ZCL_INT16U_ATTRIBUTE_TYPE,
                         ZCL_COLOR_CONTROL_CLUSTER_ID,&tpUserControl->Color_Temp,
                                   sizeof(tpUserControl->Color_Temp));
      }
  }

#endif
  uint8_t temp_rec =0;
  uint16_t  temp_hs =0;
  if (clusterId == ZCL_COLOR_CONTROL_CLUSTER_ID
        && attributeId == ZCL_COLOR_CONTROL_CURRENT_HUE_ATTRIBUTE_ID
        && mask == CLUSTER_MASK_SERVER) {
      if (emberAfReadServerAttribute(endpoint,
                                     ZCL_COLOR_CONTROL_CLUSTER_ID,
                                     ZCL_COLOR_CONTROL_CURRENT_HUE_ATTRIBUTE_ID,
                                     (uint8_t *)&temp_rec,
                                     sizeof(temp_rec))
          == EMBER_ZCL_STATUS_SUCCESS)
      {
          temp_hs= temp_rec *2;
          if(tpUserControl->Color_H != temp_hs && tpUserControl->Onoff){
              tpUserControl->Color_H = temp_hs;
              sl_zigbee_app_debug_println("_HUE_ATTRIBUTE_ID temp_rec  :%d\r\n", temp_rec);
               set_led_hsv_color();
            //  report_chage_work_mode(0x31); //色温模式
               if(tpUserControl->work_mode ==0x32){
                   tpUserControl->work_mode =0x31;
               }
              set_write_flag(1);    //写入flash
          }


      }
  }
  if (clusterId == ZCL_COLOR_CONTROL_CLUSTER_ID
        && attributeId == ZCL_COLOR_CONTROL_CURRENT_SATURATION_ATTRIBUTE_ID
        && mask == CLUSTER_MASK_SERVER) {
      if (emberAfReadServerAttribute(endpoint,
                                     ZCL_COLOR_CONTROL_CLUSTER_ID,
                                     ZCL_COLOR_CONTROL_CURRENT_SATURATION_ATTRIBUTE_ID,
                                     (uint8_t *)&temp_rec,
                                     sizeof(temp_rec))
          == EMBER_ZCL_STATUS_SUCCESS)
      {

          temp_hs= temp_rec *4;
          if(tpUserControl->Color_S != temp_hs && tpUserControl->Onoff){
             tpUserControl->Color_S = temp_hs;
             sl_zigbee_app_debug_println("_SATURATION_ATTRIBUTE_ID temp_rec  :%d\r\n", temp_rec);
             set_led_hsv_color();
           //  report_chage_work_mode(0x31); //色温模式
             if(tpUserControl->work_mode ==0x32){
                 tpUserControl->work_mode =0x31;
             }
             set_write_flag(1);    //写入flash
          }
      }
  }


}

/** @brief On/off Cluster Server Post Init
 *
 * Following resolution of the On/Off state at startup for this endpoint, perform any
 * additional initialization needed; e.g., synchronize hardware state.
 *
 * @param endpoint Endpoint that is being initialized
 */
void emberAfPluginOnOffClusterServerPostInitCallback(uint8_t endpoint)
{
  // At startup, trigger a read of the attribute and possibly a toggle of the
  // LED to make sure they are always in sync.


  sl_zigbee_app_debug_println("emberAfPluginOnOffClusterServerPostInitCallback 0x=%02x\n\n",endpoint);
  emberAfPostAttributeChangeCallback(endpoint,
                                     ZCL_ON_OFF_CLUSTER_ID,
                                     ZCL_ON_OFF_ATTRIBUTE_ID,
                                     CLUSTER_MASK_SERVER,
                                     0,
                                     0,
                                     0,
                                     NULL);
}

/** @brief
 *
 * Application framework equivalent of ::emberRadioNeedsCalibratingHandler
 */
void emberAfRadioNeedsCalibratingCallback(void)
{
  #ifndef EZSP_HOST

  sl_zigbee_app_debug_println("emberAfRadioNeedsCalibratingCallback\n\n");
  sl_mac_calibrate_current_channel();
  #endif
}

#if defined(SL_CATALOG_SIMPLE_BUTTON_PRESENT) && (SL_ZIGBEE_APP_FRAMEWORK_USE_BUTTON_TO_STAY_AWAKE == 0)
#include "sl_simple_button.h"
#include "sl_simple_button_instances.h"
#ifdef SL_CATALOG_ZIGBEE_FORCE_SLEEP_AND_WAKEUP_PRESENT
#include "force-sleep-wakeup.h"
#endif //SL_CATALOG_ZIGBEE_FORCE_SLEEP_AND_WAKEUP_PRESENT
/***************************************************************************//**
 * A callback called in interrupt context whenever a button changes its state.
 *
 * @remark Can be implemented by the application if required. This function
 * can contain the functionality to be executed in response to changes of state
 * in each of the buttons, or callbacks to appropriate functionality.
 *
 * @note The button state should not be updated in this function, it is updated
 * by specific button driver prior to arriving here
 *
   @param[out] handle             Pointer to button instance
 ******************************************************************************/

void sl_button_on_change(const sl_button_t *handle)
{
  //if(&sl_button_btn0 == handle)//判断是否中断来自btn0

#if 0
  if(SL_SIMPLE_BUTTON_INSTANCE(0) == handle)//判断是否中断来自btn1
  {
      if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED)//Button 1按键按下
      {
          tpUserControl->Onoff ^=1;
          sl_zigbee_app_debug_println("s1 button btn 1 press=%d\n",tpUserControl->Onoff);
          execute_led_change_cal(1000); //200ms缓冲时间

      }
      else //Button 1按键松开
      {
        //  set_rgb_pwm(50,0,0);

      //   sl_zigbee_app_debug_println("sl_button_1 relax  \n");

          #ifdef SL_CATALOG_ZIGBEE_FORCE_SLEEP_AND_WAKEUP_PRESENT
          sl_zigbee_app_framework_force_wakeup();
          #endif //SL_CATALOG_ZIGBEE_FORCE_SLEEP_AND_WAKEUP_PRESENT
      }
  }else if(SL_SIMPLE_BUTTON_INSTANCE(1) == handle)//判断是否中断来自btn1
    {
        if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED)//Button 1按键按下
        {
          //  tpUserControl->Brightness +=20;
            if(tpUserControl->Brightness >100)
              tpUserControl->Brightness =20;

          //  sl_zigbee_app_debug_println("s1 button btn 2 press =%d\n",tpUserControl->Brightness);
        }
        else //Button 1按键松开
        {


            tpUserControl->led_mode  =1;

            if(++tpUserControl->scen_mode >8){
                tpUserControl->scen_mode =1;
            }
            count_100ms=40;
            set_scen_cw_led_mode(tpUserControl->scen_mode);
       //     tpUserControl->Color_H +=20;
            if(tpUserControl->Color_H  >360)
              tpUserControl->Color_H  =0;
          //  set_rgb_pwm(0,50,0);
        //   sl_zigbee_app_debug_println("sl_button_2 relax =%d  tog =%d \n",tpUserControl->Color_H, TIMER_TopGet(sl_pwm_pwm0.timer));
          sl_zigbee_app_debug_println("cw tog =%d \n", TIMER_TopGet(sl_pwm_pwm0.timer));
         //   set_rgb_pwm(0,0,50);
        }
   }else if(SL_SIMPLE_BUTTON_INSTANCE(2) == handle)//判断是否中断来自btn1
     {
         if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED)//Button 1按键按下
         {
             tpUserControl->led_mode  =0;
             if(tpUserControl->Color_Temp ==2700){
                 tpUserControl->Color_Temp =4000;
             }else if(tpUserControl->Color_Temp ==4000){
                 tpUserControl->Color_Temp =6500;
             }else{
                 tpUserControl->Color_Temp =2700;

             }
         //    sl_zigbee_app_debug_println("s1 button btn 3 press =%d\n", tpUserControl->Color_Temp);
         }
         else //Button 1按键松开
         {
           //  sl_zigbee_app_debug_println("sl_button_3 relax \n");

         }
    }
#endif
}
#endif // SL_CATALOG_SIMPLE_BUTTON_PRESENT && SL_ZIGBEE_APP_FRAMEWORK_USE_BUTTON_TO_STAY_AWAKE == 0

//Internal testing stuff
#if defined(EMBER_TEST)
void emberAfHalButtonIsrCallback(uint8_t button, uint8_t state)
{
  if (state == BUTTON_RELEASED) {

    sl_zigbee_event_set_active(&finding_and_binding_event);
  }
}
#endif // EMBER_TEST

#ifdef SL_CATALOG_ZIGBEE_FORCE_SLEEP_AND_WAKEUP_PRESENT
void sli_zigbee_app_framework_force_sleep_callback(void)
{
  // Do other things like turn off LEDs etc
  sl_led_turn_off(&sl_led_led0);
  sl_zigbee_app_debug_println("sli_zigbee_app_framework_force_sleep_callback\n\n");
}
#endif // SL_CATALOG_ZIGBEE_FORCE_SLEEP_AND_WAKEUP_PRESENT
#endif //#if (LARGE_NETWORK_TESTING == 0)
