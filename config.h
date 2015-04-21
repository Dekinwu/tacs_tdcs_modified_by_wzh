#ifndef _CONFIG_H
#define _CONFIG_H

#define SIN_SAMPLE_NUM  128

#define INIT_FM 10
#define INIT_AM 40 
#define MAX_AM  1000
#define MAX_FM 32
#define MIN_FM 0
#define MIN_AM 0
#define TIMER_CLOCK 7372800
#define TIMER_CLOCK_DEV  (TIMER_CLOCK/SIN_SAMPLE_NUM)

#define STIM_STA  0X01
#define FM_SET_STA 0X03
#define AM_SET_STA 0X04
#define SHAM_STA 0X05

#define FM_STEP 0.2
#define AM_STEP 20

#define WAVE_ZERO  2047
 
#endif
