#ifndef _CONFIG_H
#define _CONFIG_H

#define SIN_SAMPLE_NUM  128

#define MAX_FM_10 320
#define MIN_FM_10 0
#define INIT_FM_10 100

#define INIT_AM 40.0 
#define MAX_AM  1000.0

#define MIN_AM 0.0
#define TIMER_CLOCK 7372800
#define TIMER_CLOCK_DEV  ((TIMER_CLOCK*10)/SIN_SAMPLE_NUM)
//#define TIMER_CLOCK_DEV  (TIMER_CLOCK_DEV_1/10)

#define STIM_STA  0X01
#define FM_SET_STA 0X03
#define AM_SET_STA 0X04
#define SHAM_STA 0X05
#define STOP_STA 0x06


#define FM_STEP_10 5  //即意味着频率调节幅度为0.1Hz
#define AM_STEP 20.0
#define WAVE_ZERO  2047

#define STIM_TIME_SECOND  1200 //刺激时间 
#define SHAM_TIME_SECOND  30  //sham刺激时间 30s


#endif
