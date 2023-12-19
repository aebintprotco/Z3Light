#include "led_user.h"
#include "time-util.h"


G_USER_RGBCW_COLOR_T StarColor ={0,};
G_USER_RGBCW_COLOR_T EndColor  ={0,};
G_USER_RGBCW_COLOR_T StepColor ={0,};
G_USER_SPEED_T SpeedColor ={0,};

G_USER_RGBCW_COLOR_T *tpStarColor =&StarColor;
G_USER_RGBCW_COLOR_T *tpEndColor  =&EndColor;
G_USER_RGBCW_COLOR_T *tpStep =&StepColor;
G_USER_SPEED_T *tpSpeed =&SpeedColor;


uint8_t  r_light_level=0;
uint8_t  r_light_to_level=0;


uint8_t  r_led_mode_level=0;  //彩色，色温模式


uint8_t  r_set_speed_buff=5;





//执行色彩变化计算
//需要执行的时候才调用   开关， 改变状态的时候。
void execute_led_change_cal(uint32_t speed_ms)
{
  float  temp_r = 0,temp_g = 0,temp_b = 0;
  uint8_t Brightness =0;
  if(tpUserControl->Onoff){
    if(tpUserControl->led_mode ==1){
        hsv_to_rgb(tpUserControl->Color_H, tpUserControl->Color_S, tpUserControl->Color_V, &temp_r, &temp_g, &temp_b);

      //  sl_zigbee_app_debug_println("HSV=%d =%d =%d  RGB %f %f %f \n",tpUserControl->Color_H,tpUserControl->Color_S,tpUserControl->Color_V,temp_r,temp_g,temp_b);
        tpEndColor->R =  (uint8_t)temp_r;
        tpEndColor->G =  (uint8_t)temp_g;
        tpEndColor->B =  (uint8_t)temp_b;
      //  sl_zigbee_app_debug_println("r=%d  g=%d  b=%d\n",tpEndColor->R,tpEndColor->G,tpEndColor->B);
        limit_rgb_dat(&tpEndColor->R,&tpEndColor->G,&tpEndColor->B);
        sl_zigbee_app_debug_println("r=%d  g=%d  b=%d\n",tpEndColor->R,tpEndColor->G,tpEndColor->B);
        //关掉 CW
        tpEndColor->c = tpEndColor->w =0;
/*
        Brightness = tpUserControl->Color_H/2;
        //同步给协议栈
        emberAfWriteServerAttribute(1,ZCL_COLOR_CONTROL_CLUSTER_ID,
                                   ZCL_COLOR_CONTROL_CURRENT_HUE_ATTRIBUTE_ID,
                                     (uint8_t *)&(Brightness),
                                     sizeof(uint8_t));
        Brightness = tpUserControl->Color_S/4;
        emberAfWriteServerAttribute(1,ZCL_COLOR_CONTROL_CLUSTER_ID,
                                     ZCL_COLOR_CONTROL_CURRENT_SATURATION_ATTRIBUTE_ID,
                                     (uint8_t *)&(Brightness),
                                     sizeof(uint8_t));
*/
    }else if(tpUserControl->led_mode == 0){
        Brightness = tpUserControl->Brightness*LIGHT_LIMIT/LIGHT_MAX;
     //   if(Brightness <5)
     //     Brightness =5;
                              //互补色温计算公式， 当前亮度*当前色温/色温总值
        tpEndColor->w = (tpUserControl->Color_Temp-COLOR_MIN)*Brightness/COLOR_DIFF;
        tpEndColor->c =  Brightness - tpEndColor->w;

        //关掉 RGB
        tpEndColor->R =  tpEndColor->B =  tpEndColor->G =  0;
        sl_zigbee_app_debug_println("pwm w =%d   pwm y =%d   color=%d \n",tpEndColor->w,tpEndColor->c,tpUserControl->Color_Temp);

    }
  }else{
      tpEndColor->R =  tpEndColor->B =  tpEndColor->G = tpEndColor->c = tpEndColor->w =  0;
  }
  cal_speed(speed_ms);  //执行变化后计算变化时间
}



//执行一次开关的数据变化
void set_led_onoff(uint8_t onoff)
{
  tpUserControl->Onoff = onoff;
  if(tpUserControl->Onoff)
    execute_led_change_cal(tpUserControl->Brightness*10); //200ms缓冲时间   设置开关不同的缓冲时间
  else
    execute_led_change_cal(tpUserControl->Brightness*10); //200ms缓冲时间
}

//执行一次色温的数据变化
void set_led_temp_color(uint16_t times)
{
  tpUserControl->led_mode =0;
  execute_led_change_cal(times); //200ms缓冲时间
}

//执行一次HSV的数据变化
void set_led_hsv_color(void)
{
  tpUserControl->led_mode =1;
  execute_led_change_cal(400); //200ms缓冲时间
}


//设置缓冲速度
void set_speed_buf(uint8_t speed)
{
  r_set_speed_buff=speed;
}


//计算出步进和 单位时间数   计算只能在变化的时候调用一次，重复调用会导致重复计算异常
void cal_speed(uint32_t speed_ms)
{
  //缓冲总时间MS / 亮度差值数   获得缓冲时间
  uint32_t total_time =0;
  uint8_t diff =0,i=0;
  total_time = speed_ms/Time_Count;  //定时器调用2MS， 步进单位/2 获得1单位变化的总时间

  if(speed_ms ==0){ //跳变
      tpStarColor->R = tpEndColor->R+2;
      tpStarColor->G = tpEndColor->G+2;
      tpStarColor->B = tpEndColor->B+2;
      tpStarColor->c = tpEndColor->c+2;
      tpStarColor->w = tpEndColor->w+2;
      tpStep->R = tpStep->G = tpStep->B =tpStep->c =tpStep->w =1;
      tpSpeed->w =tpSpeed->c =tpSpeed->R =tpSpeed->B = tpSpeed->G =1;
      return;
  }

  diff = abs(tpEndColor->R-tpStarColor->R); //获得差值
  if(diff){
    if(diff > total_time){    //变化时间比变化差值小， 代表最小单位时间也无法在规定时间内完成变化， 此时需要通过改变步进来匹配
        tpSpeed->R =1;  //单位时间是最小的， 通过调整步进来满足条件
        for(i=0;i<diff;i++){
            if((total_time*i)/diff >=1){
                break;
            }
        }
        tpStep->R =i;
    }else{
        tpStep->R = 1;
        tpSpeed->R = total_time/diff;   //总时间/差值 得出步进值。  默认步进
    }
  }


  diff = abs(tpEndColor->G-tpStarColor->G); //获得差值
  if(diff){
    if(diff > total_time){    //变化时间比变化差值小， 代表最小单位时间也无法在规定时间内完成变化， 此时需要通过改变步进来匹配
        tpSpeed->G =1;  //单位时间是最小的， 通过调整步进来满足条件
        for(i=0;i<diff;i++){
            if((total_time*i)/diff >=1){
                break;
            }
        }
        tpStep->G =i;
    }else{
        tpStep->G = 1;
        tpSpeed->G = total_time/diff;   //总时间/差值 得出步进值。  默认步进
    }
  }


  diff = abs(tpEndColor->B-tpStarColor->B); //获得差值
  if(diff){
    if(diff > total_time){    //变化时间比变化差值小， 代表最小单位时间也无法在规定时间内完成变化， 此时需要通过改变步进来匹配
        tpSpeed->B =1;  //单位时间是最小的， 通过调整步进来满足条件
        for(i=0;i<diff;i++){
            if((total_time*i)/diff >=1){
                break;
            }
        }
        tpStep->B =i;
    }else{
        tpStep->B = 1;
        tpSpeed->B = total_time/diff;   //总时间/差值 得出步进值。  默认步进
    }
  }

  diff = abs(tpEndColor->c-tpStarColor->c); //获得差值
  if(diff){
    if(diff > total_time){    //变化时间比变化差值小， 代表最小单位时间也无法在规定时间内完成变化， 此时需要通过改变步进来匹配
        tpSpeed->c =1;  //单位时间是最小的， 通过调整步进来满足条件
        for(i=0;i<diff;i++){
            if((total_time*i)/diff >=1){
                break;
            }
        }
        tpStep->c =i;
    }else{
        tpStep->c = 1;
        tpSpeed->c = total_time/diff;   //总时间/差值 得出步进值。  默认步进
    }
   //   sl_zigbee_app_debug_println("total_time = %d diff  =%d   tpStep->c =%d   tpSpeed->c=%d \n",total_time,diff,tpStep->c,tpSpeed->c);
  }

  diff = abs(tpEndColor->w-tpStarColor->w); //获得差值
  if(diff){
    if(diff > total_time){    //变化时间比变化差值小， 代表最小单位时间也无法在规定时间内完成变化， 此时需要通过改变步进来匹配
        tpSpeed->w =1;  //单位时间是最小的， 通过调整步进来满足条件
        for(i=0;i<diff;i++){
            if((total_time*i)/diff >=1){
                break;
            }
        }
        tpStep->w =i;
    }else{
        tpStep->w = 1;
        tpSpeed->w = total_time/diff;   //总时间/差值 得出步进值。  默认步进
    }
  //  sl_zigbee_app_debug_println("total_time = %d diff  =%d   tpStep->w =%d   tpSpeed->w=%d \n",total_time,diff,tpStep->w,tpSpeed->w);
  }

}


//pwm buffer  无极缓冲
void f_pwm_buffer(void)
{
  static uint32_t r_count =0;
  static uint32_t g_count =0;
  static uint32_t b_count =0;
  static uint32_t c_count =0;
  static uint32_t w_count =0;



//  if(++count <5) return;    //色温，颜色固定缓冲时间为最大1S
//  count =0;

  if(tpStarColor->R != tpEndColor->R)
  {
    if(++r_count >= tpSpeed->R){
      r_count =0;
      if(tpStarColor->R < tpEndColor->R)
      {
          tpStarColor->R +=tpStep->R;
          if(tpStarColor->R >= tpEndColor->R)
            tpStarColor->R =tpEndColor->R;
      }
      else
      {
          if(tpStarColor->R <= (tpEndColor->R+tpStep->R))
            tpStarColor->R =tpEndColor->R;
          else
            tpStarColor->R -=tpStep->R;
      }
      set_rgbcw_pwm(Num_red,tpStarColor->R);
    }
  }


  if(tpStarColor->G != tpEndColor->G)
  {
      if(++g_count >= tpSpeed->G){
        g_count =0;
        if(tpStarColor->G < tpEndColor->G)
        {
            tpStarColor->G +=tpStep->G;
            if(tpStarColor->G > tpEndColor->G)
              tpStarColor->G =tpEndColor->G;
        }
        else
        {
            if(tpStarColor->G <= (tpEndColor->G+tpStep->G))
              tpStarColor->G =tpEndColor->G;
            else
              tpStarColor->G -=tpStep->G;
        }
        set_rgbcw_pwm(Num_green,tpStarColor->G);
    }
  }


  if(tpStarColor->B != tpEndColor->B)
  {
      if(++b_count >= tpSpeed->B){
        b_count =0;
        if(tpStarColor->B < tpEndColor->B)
        {
            tpStarColor->B +=tpStep->B;
            if(tpStarColor->B > tpEndColor->B)
              tpStarColor->B =tpEndColor->B;
        }
        else
        {
            if(tpStarColor->B <= (tpEndColor->B+tpStep->B))
              tpStarColor->B =tpEndColor->B;
            else
              tpStarColor->B -=tpStep->B;
        }
        set_rgbcw_pwm(Num_blue,tpStarColor->B);
    }
  }



  if(tpStarColor->c != tpEndColor->c)
  {
      if(++c_count >= tpSpeed->c){
        c_count =0;
        if(tpStarColor->c < tpEndColor->c)
        {
            tpStarColor->c +=tpStep->c;
            if(tpStarColor->c > tpEndColor->c)
              tpStarColor->c =tpEndColor->c;
        }else
        {
            if(tpStarColor->c <= (tpEndColor->c+tpStep->c))
              tpStarColor->c =tpEndColor->c;
            else
              tpStarColor->c -=tpStep->c;
        }
        set_rgbcw_pwm(Num_warm,tpStarColor->c);
    //    sl_zigbee_app_debug_println("c = %d  %d \n",tpStarColor->c,tpSpeed->c);
    }
  }



  if(tpStarColor->w != tpEndColor->w)
  {
      if(++w_count >= tpSpeed->w){
        w_count =0;
        if(tpStarColor->w < tpEndColor->w)
        {
            tpStarColor->w +=tpStep->w;
            if(tpStarColor->w > tpEndColor->w)
              tpStarColor->w =tpEndColor->w;
        }else
        {
            if(tpStarColor->w <= (tpEndColor->w+tpStep->w))
              tpStarColor->w =tpEndColor->w;
            else
              tpStarColor->w -=tpStep->w;
        }
        set_rgbcw_pwm(Num_cool,tpStarColor->w);
    //    sl_zigbee_app_debug_println("w = %d =%d \n",tpStarColor->w);

    }
  }

}

//设置1-4 色温的场景模式
uint8_t count_100ms  =0;
void set_scen_cw_led_mode(uint8_t led_mode)
{
  if(led_mode <=4 && (tpUserControl->work_mode ==0x32)){
    switch(led_mode){
      case 1://晚安  C=23%
        tpUserControl->led_mode  =0;
        tpUserControl->Color_Temp =2700;
        tpUserControl->Brightness =23;
        break;
      case 2://阅读  CW =50%
        tpUserControl->led_mode  =0;
        tpUserControl->Color_Temp =4600;
        tpUserControl->Brightness =100;
        break;
      case 3://工作   W =100%
        tpUserControl->led_mode  =0;
        tpUserControl->Color_Temp =6500;
        tpUserControl->Brightness =100;
        break;
      case 4://休息  CW =26%
        tpUserControl->led_mode  =0;
        tpUserControl->Color_Temp =4600;
        tpUserControl->Brightness =52;
        break;
      default:
        break;
    }
    execute_led_change_cal(500);
  }
}
//设置5-8 hsv的场景模式
void set_scen_hsv_led_mode(uint8_t led_mode)
{
  static uint8_t level =0;

  if(tpUserControl->Onoff && led_mode >=5 && (tpUserControl->work_mode ==0x32)){
    if(count_100ms >=40){
        count_100ms =0;
        switch(led_mode){
          case 5://海洋
       //     sl_zigbee_app_debug_println(" haiyang %d ",led_mode);
            tpUserControl->led_mode  =1;
            if(level == 0){//B =75%
                level =1;
                tpUserControl->Color_H  =240;
                tpUserControl->Color_S  =1000;
                tpUserControl->Color_V  =750;
            }else{//B =75%  r =22  g =25
                level =0;
                tpUserControl->Color_H  =237;
                tpUserControl->Color_S  =710;
                tpUserControl->Color_V  =750;
            }
            break;
          case 6://向日葵
         //   sl_zigbee_app_debug_println(" xiangrikui %d ",led_mode);
            tpUserControl->led_mode  =1;
            if(level == 0){//r =100% G =41%  B=5
                level = 1;
                tpUserControl->Color_H  =23;
                tpUserControl->Color_S  =950;
                tpUserControl->Color_V  =1000;
            }else if(level == 1){//r =100% G =25%  B=1.5
                level = 2;
                tpUserControl->Color_H  =23;
                tpUserControl->Color_S  =980;
                tpUserControl->Color_V  =1000;
            }else{//r =100% G =15%  B=1.5
                level = 0;
                tpUserControl->Color_H  =8;
                tpUserControl->Color_S  =980;
                tpUserControl->Color_V  =1000;
            }
            break;
          case 7://草原绿
          //  sl_zigbee_app_debug_println(" caoyuanlv %d ",led_mode);
            tpUserControl->led_mode  =1;
            if(level == 0){//G =70%
                level = 1;
                tpUserControl->Color_H  =120;
                tpUserControl->Color_S  =1000;
                tpUserControl->Color_V  =700;
            }else{//G =1.5%
                level = 0;
                tpUserControl->Color_H  =120;
                tpUserControl->Color_S  =1000;
                tpUserControl->Color_V  =20;
            }
            break;
          case 8://炫彩
        //    sl_zigbee_app_debug_println(" xuancai %d ",led_mode);
            tpUserControl->led_mode  =1;
            if(level == 0){//r =100%
                level = 1;
                tpUserControl->Color_H  =0;
                tpUserControl->Color_S  =1000;
                tpUserControl->Color_V  =1000;
            }else if(level == 1){//G=75
                level = 2;
                tpUserControl->Color_H  =120;
                tpUserControl->Color_S  =1000;
                tpUserControl->Color_V  =1000;
            }else{//B=75
                level = 0;
                tpUserControl->Color_H  =240;
                tpUserControl->Color_S  =1000;
                tpUserControl->Color_V  =1000;
            }
            break;
        }
          execute_led_change_cal(500);
    }
  }else{
      count_100ms =40;
  }
}




//倒计时功能  1S执行调用程序
void f_count_down_off(uint32_t* times)
{
  static uint8_t count =0,rand_c =60;
   if(*times){
       (*times)--;
       if((*times) ==0){
           set_led_onoff((tpUserControl->Onoff^1));  //执行取反
           report_zigbee_onoff_state();
           report_cmd_countdown();
           sl_zigbee_app_debug_println(" times %d off",*times);
       }else{
         if(++count >=rand_c){
             count =0;
             rand_c =60+ (rand()%5);
             sl_zigbee_app_debug_println(" rand_c %d",rand_c);
             report_cmd_countdown();
         }
       }
   }else{
       count  =0;
       rand_c =60;
   }
}




uint32_t s_utc_time =0;
void add_utc_times(void)
{
  s_utc_time++;
}

void set_utc_times(uint32_t utc)
{
  s_utc_time =utc;
}

uint32_t get_utc_times(void)
{
  return s_utc_time;
}

//从时间戳中获取小时和分钟
UtcTimeStruct Get_hour_min_FromUtc(uint32_t utc)
{
  UtcTimeStruct return_T ={0};

  uint32_t daysSince2000 = utc / SECONDS_IN_DAY;
  uint32_t secToday = utc - (daysSince2000 * SECONDS_IN_DAY);
  return_T.hours = (uint8_t)(secToday / SECONDS_IN_HOUR);
  return_T.minutes = (uint8_t)((secToday
                                   - (return_T.hours * SECONDS_IN_HOUR)) / 60);
  return_T.seconds = (uint8_t)(secToday
                                  - ((return_T.hours * SECONDS_IN_HOUR)
                                     + (return_T.minutes * 60)));
  return return_T;
}


//从时间戳计算今天是周几
uint8_t GetWeekdayFromUtc(uint32_t utcTime)
{
  uint8_t week;
  utcTime %= SECONDS_IN_WEEK; //日期除以7天一轮回

  for ( week = 0; week < 7; week++ ) {
    if ( utcTime < SECONDS_IN_DAY ) {
      break;
    }
    utcTime -= SECONDS_IN_DAY;
  }
  // Note:  1970年1月1日是周4
  // 所以转换后 0=周4, 1=周5, 2=周六
  week = (week+3)%7;
  return week;   //返回值0 =周一  6=周日
}



//唤醒持续时间恢复
uint8_t Continue_Time =0;

//有其他切换操作需要打断唤醒状态持续时间。
void break_Awaken_Continue(void)
{
  Continue_Time =0;
}
//唤醒灯效持续时间恢复。
void awaken_continue_time_Fuc(void)
{
  static uint8_t T_Count =0;
  if(Continue_Time){
    if(++T_Count >= Continue_Time){
        Continue_Time =0;
        Restore_to_last_state();  //恢复记忆中的状态
        execute_led_change_cal(500);  //500ms  缓冲恢复
    }
  }else{
      T_Count =0;
  }
}


//执行计划
uint8_t Process_Plan(FucTimeStruct *Plan,UtcTimeStruct T_time,uint8_t mode)
{
  uint8_t i=0;
  uint8_t isActive=0;
  for(i=0;i<4;i++){
      if(Plan->onoff[i]){   //有唤醒计划
         if((Plan->hours[i] == T_time.hours)){  //
             if((Plan->minutes[i] == T_time.minutes)){  //  今日到点执行
                 //执行设置的功能
                 if(Plan->week[i][0]){                  //  判断是否为重复执行计划
                     if((Plan->week[i][0] & T_time.week)){  //今日是重复计划中的执行日
                         isActive =1;
                     }
                 }else{
                     //一次性执行计划
                     isActive =1;
                     Plan->onoff[i] =0;
                 }
                 if(isActive){
                     //执行具体功能
                  //   tpUserControl->led_mode  =Plan->led_mode[i];   //从当前状态开始， 唤醒和入睡  只开和关
                     if(tpUserControl->led_mode){  //hsv 执行
                         tpUserControl->Color_H  =(Plan->mode_data[i][0] <<8) | Plan->mode_data[i][1];
                         tpUserControl->Color_S  =Plan->mode_data[i][2];;
                         tpUserControl->Color_V  =Plan->mode_data[i][3];
                     }else{
                         tpUserControl->Color_Temp =(Plan->mode_data[i][0] <<8) | Plan->mode_data[i][1];
                         tpUserControl->Brightness =Plan->mode_data[i][2];
                     }


                     execute_led_change_cal(Plan->execution_time[i]);
                     if(mode==0){ //唤醒计划
                         Continue_Time = Plan->continue_time[i];  //唤醒计划持续时间
                     }else if(mode ==1){  //入睡计划
                      //   execute_led_change_cal(Plan->execution_time[i]);
                     }else{       //生物节律
                      //   execute_led_change_cal(Plan->execution_time[i]);
                         //直接渐变或者持续渐变在接收协议的时候已经计算好，放入execution_time[i] 中
                     }
                     break;   //4个计划同一时刻只会有一个执行
                 }
             }
         }
      }
  }
 /* if(isActive){ //有执行计划
      isActive = 0x80 | i;
  }else{
      isActive =0;
  }*/
  return isActive;
}


//定时计划功能，  一分钟判断一次， 唤醒计划，入睡计划， 生物节律
void  Timing_Plan_Fuction(void)
{
  UtcTimeStruct T_time ={0};
  uint8_t isActive=0;

  T_time = Get_hour_min_FromUtc(s_utc_time);    //获取当前的  时分秒
  isActive= GetWeekdayFromUtc(s_utc_time);  //计算当前是周几

  T_time.week = 0x02 << isActive; //0=周一  6=周日
  //唤醒计划
  isActive = Process_Plan(&tpUserControl->awaken,T_time,0);  //判断执行

  //入睡计划
  isActive = Process_Plan(&tpUserControl->asleep,T_time,1);

  //生物节律
 // isActive = Process_Plan(&tpUserControl->bio_rhythm,T_time,2);




}



//======================HSV 转RGB算法=====================================//
//参数入参范围h(0~360),s(0~100),v(0~100),这里要注意，要把s,v缩放到0~1之间
//转换结果R(0~100),G(0~100),B(0~100)，这里的范围为百分比， 可根据PWM数之间调整范围，计算结果及为对应的PWM不需要再转换级数
#define HUE         360    //0~360   色度 0=红色  （红绿间的比例）   120=绿色  （绿蓝间的比例）  240 =蓝色   （蓝红间的比例） 360=黑
#define SATURATION  1000   //0~100  饱和度
#define Value       1000   //0~100  亮度


#define R_MAX   250//PWM_RATE * 0.55        //最大百分比 //根据级数百分比做设置，直接计算成对应级数的百分比
#define G_MAX   250//PWM_RATE * 0.82      //最大百分比
#define B_MAX   250//PWM_RATE * 0.75      //最大百分比

void hsv_to_rgb(int h,int s,int v,float *R,float *G,float *B)
{
    float C = 0,X = 0,Y = 0,Z = 0;
    int i=0;
    float H,S,V;
    //屏蔽掉黑色的情况
    if(h==360){
      h=359;
    }

    H=(float)(h);
    S=(float)(s)/SATURATION;
    V=(float)(v)/Value;

    if(S == 0){
        *R =1; *G =1; *B = 1;
    }else{
        H = H/60;
        i = (int)H;// 浮点数转换为整型，小数点后被去掉，用来判断当前的色度区间
        C = H - i; // 留下小数点数据

        X = V * (1 - S);      //计算三色各自应占的百分比
        Y = V * (1 - S*C);
        Z = V * (1 - S*(1-C));
        switch(i){
            case 0 : *R = V; *G = Z; *B = X; break; //60°以下
            case 1 : *R = Y; *G = V; *B = X; break; //
            case 2 : *R = X; *G = V; *B = Z; break;
            case 3 : *R = X; *G = Y; *B = V; break;
            case 4 : *R = Z; *G = X; *B = V; break;
            case 5 : *R = V; *G = X; *B = Y; break;
            default:                         break;
        }
    }
    *R = *R *R_MAX;   //输出各自最大的占空比 下的百分比
    *G = *G *G_MAX;
    *B = *B *B_MAX;
}



#define PWR_LIMIT_MAX             750     //MAX 250 *3 = 750
//限制RGB最高亮度%
void limit_rgb_dat(uint16_t *R , uint16_t *G,  uint16_t *B)
{
  uint16_t colorR,colorG,colorB;
  colorR = (uint16_t) *R;
  colorG = (uint16_t) *G;
  colorB = (uint16_t) *B;
  if((colorR + colorG + colorB) > PWR_LIMIT_MAX)
  {
    *R = (colorR*PWR_LIMIT_MAX)/(colorR+colorG+colorB);
    *G = (colorG*PWR_LIMIT_MAX)/(colorR+colorG+colorB);
    *B = (colorB*PWR_LIMIT_MAX)/(colorR+colorG+colorB);
  }
}





/*
 * 产测部分
 */

uint8_t    Production_Test =0;

uint8_t get_Test_State(void)
{
  return Production_Test;
}

void emberEnter_Test(uint8_t clust,uint8_t comm)
{
  if (emberAfNetworkState() == EMBER_JOINED_NETWORK) {
      printf("retrun emberAfNetworkState");
      return; //已入网 退出   不会进入产测
  }
  if(get_network_state()){  //
    printf("retrun r_cancel_network");
    return;   //处于配网状态，也不会进入产测。
  }

    if(get_init_state()){ //初始化超过时间不再进入产测
      printf("retrun Test_Enter");
      return;   //处于配网状态，也不会进入产测。
    }
  //网络情况下和没有配网情况下， 收到产测信号开始进入产测
  if((clust == 0) &&(comm == 0x55)){
      if(!tpUserControl->test_mode){
          Production_Test =1;   //第一次产测
      }else{
          Production_Test =2;   //重复产测
      }

      sl_zigbee_app_debug_println("Enter TestMode =%d \n",Production_Test);
  }
}



void Test_Mode(void)
{
  static uint8_t tran_step =0;
  static uint8_t pwm_out_num =0;
  static uint8_t step_count =0;

  if(Production_Test ==2){
    if(step_count ==0){
        pwm_out_num = LIGHT_MIN_NUM;
        step_count =1;
    }else if(step_count ==1){
        pwm_out_num = LIGHT_LIMIT;
        step_count =2;
    }else{
        pwm_out_num = LIGHT_MIN_NUM;
        step_count =0;
    }

  //  printf("step_count =%d\n",step_count);
  }else{
#if User_PWM ==1
      if(step_count ==0){
          pwm_out_num = LIGHT_LIMIT;
          step_count =1;
      }else if(step_count ==1){
          pwm_out_num = 0;
          step_count =0;
      }
#else
      pwm_out_num = LIGHT_LIMIT;
      step_count =0;
#endif
  }

  switch(tran_step){
    case 0:
      set_rgbcw_pwm(Num_cool, pwm_out_num);
      set_rgbcw_pwm(Num_warm, 0);
      set_rgbcw_pwm(Num_red, 0);
      set_rgbcw_pwm(Num_green, 0);
      set_rgbcw_pwm(Num_blue, 0);
      break;
    case 1:
      set_rgbcw_pwm(Num_warm, pwm_out_num);
      set_rgbcw_pwm(Num_cool, 0);
      break;
    case 2:
      set_rgbcw_pwm(Num_red, pwm_out_num);
      set_rgbcw_pwm(Num_warm, 0);
      break;
    case 3:
      set_rgbcw_pwm(Num_green, pwm_out_num);
      set_rgbcw_pwm(Num_red, 0);
      break;
    case 4:
      set_rgbcw_pwm(Num_green, 0);
      set_rgbcw_pwm(Num_blue, pwm_out_num);
      break;
    default:break;
  }

  if(step_count ==0){
    if(++tran_step >=User_PWM){
        tran_step =0;
    }
//    printf("tran_step =%d\n",tran_step);
  }
}

void Cout_Net_Time(void)
{
  static uint16_t count =0;
  if(tpUserControl->test_mode ==0){
    if (emberAfNetworkState() == EMBER_JOINED_NETWORK) {
        if(++count >= 900){
            count =0;   //15分钟  不能再进入老化产测
            tpUserControl->test_mode =1;
            set_write_flag(1);
        }
    }
  }
}


void F_Production_Test(void)
{
  static uint8_t test_step =0;
  static uint8_t test_count =0;
  static uint8_t next_step_count =0;

    if(Production_Test){

        switch(test_step){
          case 0:   //循环2分钟的 最大亮度颜色切换
            // 单路 =亮->灭   双路 =白->黄   五路 =红绿蓝白黄

            if(++test_count >=2){
                test_count =0;
                Test_Mode();
                if(Production_Test ==1){
                    if(++next_step_count >=120){
                        next_step_count =0;
                        test_step ++; //    进入下一步测试
                    }
                }
            }
            if(Production_Test ==1){
              if(tpUserControl->test_time){
                  test_count = 120;
                  test_step =2;
              }
            }
            break;
          case 1:
            Test_Mode();
            if(Production_Test ==1){
                if(++next_step_count >=20){
                    next_step_count =0;
                    test_step ++; //    进入下一步测试
                    test_count = 120;
                }
            }
            break;
          case 2:
            if(++test_count >=120){
                test_count =0;
                set_rgbcw_pwm(Num_cool, LIGHT_LIMIT);
                set_rgbcw_pwm(Num_warm, 0);
                set_rgbcw_pwm(Num_red, 0);
                set_rgbcw_pwm(Num_green, 0);
                set_rgbcw_pwm(Num_blue, 0);
#if (User_PWM ==1)
                    if(++tpUserControl->test_time >=40){   //20分钟白光
                        test_step =5; //    进入下一步测试
                        test_count = 120;
                        next_step_count =1;
                    }
#else
                    if(++tpUserControl->test_time >=20){   //20分钟白光
                        test_step ++; //    进入下一步测试
                        test_count = 120;
                    }
#endif
                    set_write_flag(1);  //记忆老化时间
            }
            break;
          case 3:
            if(++test_count >=120){
                test_count =0;
                set_rgbcw_pwm(Num_warm, LIGHT_LIMIT);
                set_rgbcw_pwm(Num_cool, 0);
                set_rgbcw_pwm(Num_red, 0);
                set_rgbcw_pwm(Num_green, 0);
                set_rgbcw_pwm(Num_blue, 0);

                    if(++tpUserControl->test_time >=40){   //20分钟白光
#if  (User_PWM ==2)
                        test_step =5;
#else
                        test_step =4; //    进入下一步测试
                        test_count =120;
#endif
                        next_step_count =1;
                    }
                    set_write_flag(1);  //记忆老化时间
            }
            break;
          case 4:
            if(++test_count >=120){
                test_count =0;
                set_rgbcw_pwm(Num_warm, 0);
                set_rgbcw_pwm(Num_cool, 0);
                set_rgbcw_pwm(Num_red,  LIGHT_LIMIT);
                set_rgbcw_pwm(Num_green,LIGHT_LIMIT);
                set_rgbcw_pwm(Num_blue, LIGHT_LIMIT);

                if(++tpUserControl->test_time >=50){   //10分钟RGB
                    test_step =6;
                    next_step_count =1;
                }
                set_write_flag(1);  //记忆老化时间
            }

            break;
          case 5:
            set_rgbcw_pwm(Num_warm, LIGHT_MIN_NUM);
            set_rgbcw_pwm(Num_cool, LIGHT_MIN_NUM);
            tpUserControl->test_mode =1;
            if(next_step_count){
              set_write_flag(1);  //记忆老化时间  和产测模式。
              next_step_count =0;
            }
            break;
          case 6:
            set_rgbcw_pwm(Num_warm, 0);
            set_rgbcw_pwm(Num_cool, 0);
            set_rgbcw_pwm(Num_red,  LIGHT_MIN_NUM);
            set_rgbcw_pwm(Num_green,LIGHT_MIN_NUM);
            set_rgbcw_pwm(Num_blue, LIGHT_MIN_NUM);
            tpUserControl->test_mode =1;
            if(next_step_count){
              set_write_flag(1);  //记忆老化时间  和产测模式。
              next_step_count =0;
            }
            break;

            break;
          default:break;
        }
    }
}



