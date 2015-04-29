#include <msp430.h>
#include <math.h>
#include<string.h>
#include "st7565r.h"
#include "hzconf.h"
#include "config.h"
#pragma pack(1)
//const unsigned int ucArray_Sin[SIN_SAMPLE_NUM]={1365,1381,1398,1415,1431,1448,1465,1482,1498,1515,1532,1548,1565,1581,1598,1614,1631,1647,1664,1680,1696,1712,1729,1745,1761,1777,1793,1809,1824,1840,1856,1871,1887,1902,1918,1933,1948,1963,1978,1993,2008,2023,2037,2052,2066,2081,2095,2109,2123,2137,2150,2164,2178,2191,2204,2217,2230,2243,2256,2269,2281,2294,2306,2318,2330,2341,2353,2365,2376,2387,2398,2409,2420,2430,2441,2451,2461,2471,2481,2490,2499,2509,2518,2527,2535,2544,2552,2560,2568,2576,2584,2591,2598,2606,2612,2619,2626,2632,2638,2644,2650,2655,2661,2666,2671,2675,2680,2684,2689,2693,2696,2700,2703,2706,2709,2712,2715,2717,2719,2721,2723,2724,2726,2727,2728,2729,2729,2729,2730,2729,2729,2729,2728,2727,2726,2724,2723,2721,2719,2717,2715,2712,2709,2706,2703,2700,2696,2693,2689,2684,2680,2675,2671,2666,2661,2655,2650,2644,2638,2632,2626,2619,2612,2606,2598,2591,2584,2576,2568,2560,2552,2544,2535,2527,2518,2509,2499,2490,2481,2471,2461,2451,2441,2430,2420,2409,2398,2387,2376,2365,2353,2341,2330,2318,2306,2294,2281,2269,2256,2243,2230,2217,2204,2191,2178,2164,2150,2137,2123,2109,2095,2081,2066,2052,2037,2023,2008,1993,1978,1963,1948,1933,1918,1902,1887,1871,1856,1840,1824,1809,1793,1777,1761,1745,1729,1712,1696,1680,1664,1647,1631,1614,1598,1581,1565,1548,1532,1515,1498,1482,1465,1448,1431,1415,1398,1381,1365,1349,1332,1315,1299,1282,1265,1248,1232,1215,1198,1182,1165,1149,1132,1116,1099,1083,1066,1050,1034,1018,1001,985,969,953,937,921,906,890,874,859,843,828,812,797,782,767,752,737,722,707,693,678,664,649,635,621,607,593,580,566,552,539,526,513,500,487,474,461,449,436,424,412,400,389,377,365,354,343,332,321,310,300,289,279,269,259,249,240,231,221,212,203,195,186,178,170,162,154,146,139,132,124,118,111,104,98,92,86,80,75,69,64,59,55,50,46,41,37,34,30,27,24,21,18,15,13,11,9,7,6,4,3,2,1,1,1,0,1,1,1,2,3,4,6,7,9,11,13,15,18,21,24,27,30,34,37,41,46,50,55,59,64,69,75,80,86,92,98,104,111,118,124,132,139,146,154,162,170,178,186,195,203,212,221,231,240,249,259,269,279,289,300,310,321,332,343,354,365,377,389,400,412,424,436,449,461,474,487,500,513,526,539,552,566,580,593,607,621,635,649,664,678,693,707,722,737,752,767,782,797,812,828,843,859,874,890,906,921,937,953,969,985,1001,1018,1034,1050,1066,1083,1099,1116,1132,1149,1165,1182,1198,1215,1232,1248,1265,1282,1299,1315,1332,1349,};
const unsigned int ucArray_Sin[SIN_SAMPLE_NUM]={1024,1074,1124,1174,1223,1272,1321,1368,1415,1461,1506,1550,1592,1633,1673,1711,1748,1782,1815,1846,1875,1902,1927,1949,1970,1988,2003,2017,2028,2036,2043,2046,2048,2046,2043,2036,2028,2017,2003,1988,1970,1949,1927,1902,1875,1846,1815,1782,1748,1711,1673,1633,1592,1550,1506,1461,1415,1368,1321,1272,1223,1174,1124,1074,1024,974,924,874,825,776,727,680,633,587,542,498,456,415,375,337,300,266,233,202,173,146,121,99,78,60,45,31,20,12,5,2,0,2,5,12,20,31,45,60,78,99,121,146,173,202,233,266,300,337,375,415,456,498,542,587,633,680,727,776,825,874,924,974,};
uint sinpx[SIN_SAMPLE_NUM];
uint sin_num;

float wavefrequency_10;      //10倍波形频率
float waveamplitude;       //波形幅值 
unsigned int tdcs_adc;
unsigned int fsm_state;

unsigned int  g_stim_delay;
unsigned int g_sham_delay;

unsigned char timing_flag_interrupt;//波形时间中断标志


void Use_High_Frequency_Clock(void);
void Use_Low_Frequency_Clock(void);
void DAC12_init(void);

void timerA_init(unsigned int ssA_count);
void timerB_init(unsigned int ssB_count);
void Delay_ms(unsigned int n);
void Delay_50us(unsigned int n);
void Key_set(void);
void display_ascii(unsigned char num,unsigned char x,unsigned char y);

void display_wavefrequency(float fre);
void display_waveamplitude(float amp);

void Sin_Waveform_set_function(void);
void display_fm_activity(void);
void display_am_activity(void);
void display_stim_activity(void);
void display_sham_activity(void);
void display_stop_activity(void);

 void main(void)
{
   WDTCTL = WDT_ADLY_1000;                
   IE1 |= WDTIE;     
   P6SEL |= BIT3;P6DIR |= BIT4;P6OUT &= ~BIT4;
   Use_High_Frequency_Clock(); 
   _EINT(); 
   
   Key_set();               
   Lcd_Init();
   DAC12_init();
 
   wavefrequency_10 = INIT_FM_10;  
   waveamplitude =INIT_AM;  
   fsm_state=FM_SET_STA;

   ClearlcdRAM();
   display_fm_activity();  //显示频率界面

   Sin_Waveform_set_function();
  
   while(1)
   { 
     if(fsm_state==STIM_STA) 
     { 
        if(timing_flag_interrupt) //正弦波输出
        {
          if(wavefrequency_10>0) 
            DAC12_0DAT = (uint)(WAVE_ZERO-sinpx[0] + sinpx[sin_num++]);
          else
          {
            DAC12_0DAT=tdcs_adc;
            sin_num++;
          }
            if(sin_num >= SIN_SAMPLE_NUM) sin_num = 0;
            timing_flag_interrupt = 0;
        }
        if( g_stim_delay > STIM_TIME_SECOND )  
         {
             TACCTL0 &= ~CCIE;
             display_stop_activity();
             fsm_state=STOP_STA;
         }    
     }  
     
     
     if(fsm_state==SHAM_STA) 
     {
        if(timing_flag_interrupt) //正弦波输出
        {
           if(wavefrequency_10>0) 
             DAC12_0DAT = (uint)(WAVE_ZERO-sinpx[0] + sinpx[sin_num++]);
           else
          {
            DAC12_0DAT=tdcs_adc;
            sin_num++;
          }
            if(sin_num >= SIN_SAMPLE_NUM) sin_num = 0;
            timing_flag_interrupt = 0;
        }
        if( g_sham_delay > SHAM_TIME_SECOND )  
         {
             TACCTL0 &= ~CCIE;
             display_stop_activity();
             fsm_state=STOP_STA;
         }   
     } 
     
     
     
   }
}




void Sin_Waveform_set_function(void)
{
  float waveamplitude_ff = waveamplitude/MAX_AM;
  for(uint i=0;i<SIN_SAMPLE_NUM;i++)
     sinpx[i] = (uint)(waveamplitude_ff*(ucArray_Sin[i]));
  tdcs_adc=WAVE_ZERO+(unsigned int)(waveamplitude_ff *2047);  //如果是直流电刺激，dac的值为2047+ 权重值 
  if(wavefrequency_10>0)      //频率不为0
       timerA_init((unsigned int)(288000/wavefrequency_10));
  else  timerA_init((unsigned int)(288000/1));   //直流情况下，随意设定定时器中断
  TACCTL0 &=~ CCIE;  //设定时默认不输出
}


void display_fm_activity(void)
{
   ClearlcdRAM();
   PrintGB(0,0,(unsigned char*)fu_hz);   
   PrintGB(16,0,(unsigned char*)zhi_hz);
   display_waveamplitude(waveamplitude);
 
   PrintGB(0,2,(unsigned char*)pin_hz);
   PrintGB(16,2,(unsigned char*)lv_hz);
   display_wavefrequency(wavefrequency_10);
   
   PrintASCII(116,2,(unsigned char*)ASCII_SHUXIAN);
   PrintASCII(116,0,(unsigned char*)ASCII_KONG);
}


void display_am_activity(void)
{ 
   ClearlcdRAM();
   PrintGB(0,0,(unsigned char*)fu_hz);   
   PrintGB(16,0,(unsigned char*)zhi_hz);
   display_waveamplitude(waveamplitude);
 
   PrintGB(0,2,(unsigned char*)pin_hz);
   PrintGB(16,2,(unsigned char*)lv_hz);
   display_wavefrequency(wavefrequency_10);
   
   PrintASCII(116,0,(unsigned char*)ASCII_SHUXIAN);
   PrintASCII(116,2,(unsigned char*)ASCII_KONG);
}


void display_sham_activity(void)
{
   ClearlcdRAM();
   PrintASCII(32,2,(unsigned char*)ASCII_S);
   PrintASCII(32+8,2,(unsigned char*)ASCII_h);
   PrintASCII(32+16,2,(unsigned char*)ASCII_a);
   PrintASCII(32+24,2,(unsigned char*)ASCII_m);
}


void display_stim_activity(void)
{
   ClearlcdRAM();
   PrintASCII(32,2,(unsigned char*)ASCII_S);
   PrintASCII(32+8,2,(unsigned char*)ASCII_t);
   PrintASCII(32+16,2,(unsigned char*)ASCII_i);
   PrintASCII(32+24,2,(unsigned char*)ASCII_m);
}

void display_stop_activity(void)
{
   ClearlcdRAM();
   PrintASCII(32,2,(unsigned char*)ASCII_S);
   PrintASCII(32+8,2,(unsigned char*)ASCII_t);
   PrintASCII(32+16,2,(unsigned char*)ASCII_o);
   PrintASCII(32+24,2,(unsigned char*)ASCII_p);
}

void display_ascii(unsigned char num,unsigned char x,unsigned char y)
{
   switch(num)
   { 
      case 0x01: PrintASCII(x,y,(unsigned char*)ASCII_1);break;
      case 0x02: PrintASCII(x,y,(unsigned char*)ASCII_2);break;
      case 0x03: PrintASCII(x,y,(unsigned char*)ASCII_3);break;
      case 0x04: PrintASCII(x,y,(unsigned char*)ASCII_4);break;
      case 0x05: PrintASCII(x,y,(unsigned char*)ASCII_5);break;
      case 0x06: PrintASCII(x,y,(unsigned char*)ASCII_6);break;
      case 0x07: PrintASCII(x,y,(unsigned char*)ASCII_7);break;
      case 0x08: PrintASCII(x,y,(unsigned char*)ASCII_8);break;
      case 0x09: PrintASCII(x,y,(unsigned char*)ASCII_9);break;
      case 0x00: PrintASCII(x,y,(unsigned char*)ASCII_0);break;
      default: break;
   }
}

void display_wavefrequency(float fre)
{
   // float tp;
    uint fre_h,fre_l;
    uint free;
  //  uint temp;
    
    free=(unsigned int)(fre/10);
     /*显示整数部分*/
    fre_h = free/10;
    fre_l = free%10;
 
    PrintASCII(48,2,(unsigned char*)ASCII_KONG);
    if(fre_h == 0) PrintASCII(56,2,(unsigned char*)ASCII_KONG);
    else display_ascii(fre_h,56,2);
    display_ascii(fre_l,64,2);
    PrintASCII(72,2,(unsigned char*)ASCII_DIAN);
    display_ascii((unsigned int)fre%10,80,2);
    
   // else  display_ascii(0,80,2);//
    
    PrintASCII(88,2,(unsigned char*)ASCII_H);
    PrintASCII(96,2,(unsigned char*)ASCII_z);
  
    PrintASCII(104,2,(unsigned char*)ASCII_KONG);
    PrintASCII(112,2,(unsigned char*)ASCII_KONG);

}

void display_waveamplitude(float amp)
{
   uint amp_h,amp_l;
   ulong crut; 
   crut=(uint)amp;
   PrintASCII(48,2,(unsigned char*)ASCII_KONG);
   if(crut >= 1000)
   {
      PrintASCII(56,0,(unsigned char*)ASCII_1);
      PrintASCII(64,0,(unsigned char*)ASCII_0);
      PrintASCII(72,0,(unsigned char*)ASCII_0); 
   }
   else
   {
      crut = crut/10;
      amp_h = crut/10;
      amp_l = crut%10;
      
      PrintASCII(56,0,(unsigned char*)ASCII_KONG);
      if(amp_h == 0)
         PrintASCII(66,0,(unsigned char*)ASCII_KONG);
      else
         display_ascii(amp_h,66,0);
      if(amp_l == 0 && amp_h == 0)
         PrintASCII(72,0,(unsigned char*)ASCII_KONG);
      else
         display_ascii(amp_l,72,0);
   }  
      display_ascii(0,80,0);
      PrintASCII(88,0,(unsigned char*)ASCII_u);
      PrintASCII(96,0,(unsigned char*)ASCII_A);
      PrintASCII(104,2,(unsigned char*)ASCII_KONG);
      PrintASCII(112,2,(unsigned char*)ASCII_KONG);
}


void Key_set(void)
{
   
   P1IES |= BIT6 + BIT7;         //下降沿触发
   P1IE  |= BIT6 + BIT7;         //允许中断
   P1IFG = 0;
   
   P2IES |= BIT0;         //下降沿触发
   P2IE  |= BIT0;         //允许中断
   P2IFG = 0;
}
/*------------------------------------------------------*/
/*---------------------高频时钟初始化-------------------*/
/*------------------------------------------------------*/
void Use_High_Frequency_Clock(void)
{
   unsigned int iq0;
   unsigned char cpp;
   
   BCSCTL1 &= ~XT2OFF;
   do
   {
      IFG1 &= ~OFIFG;
      for (iq0 = 0xFF; iq0 > 0; iq0--)
         ;
      cpp += 1;
      if(cpp > 200)
         break;
      
   } while ((IFG1 & OFIFG) != 0);
   BCSCTL2 = SELM_2 + SELS;
   
}

/*------------------------------------------------------*/
/*---------------------低频时钟初始化-------------------*/
/*------------------------------------------------------*/
void Use_Low_Frequency_Clock(void)  
{
   BCSCTL1 |= XT2OFF; //XT2高速晶振关
   BCSCTL1 &= ~XTS; //LFXT1 低速模式
   BCSCTL2 |= SELM_3; //选择MCLK时钟源 为LFXT1
}

/*------------------------------------------------------*/ 
/*------------------DAC12初始化输出电压-----------------*/
/*------------------------------------------------------*/
void DAC12_init(void)
{
   ADC12CTL0 = REF2_5V + REFON;                  // Internal 2.5V ref on
   DAC12_0CTL = DAC12IR + DAC12AMP_7 +DAC12ENC;  // Internal ref gain 1
}

/*------------------------------------------------------*/
/*----------------------timerA初始化-------------------*/
/*------------------------------------------------------*/
void timerA_init(unsigned int ssA_count)
{
   //Timer_A 选用ACLK 32768Hz，不分频，增计数模式
   TACTL = TASSEL_2  + MC_1 + TACLR;//+ ID_3
   TACCTL0 = CCIE;
   TACCR0 = ssA_count;
}


/*------------------------------------------------------*/
/*----------------------timerB初始化-------------------*/
/*------------------------------------------------------*/
void timerB_init(unsigned int ssB_count) 
{
   TBCTL = TASSEL0 + MC_1 + TBCLR;          // SMCLK, continuous mode
   TBCCTL0 = CCIE;                           // CCR0 interrupt enabled
   TBCCR0 = ssB_count;
}


/*------------------------------------------------------*/
/*----------------------看门狗秒中断--------------------*/
/*------------------------------------------------------*/
#pragma vector = WDT_VECTOR
__interrupt void interrupt_wdt(void)
{
  // wdt_flag = 1; 
   if(fsm_state==SHAM_STA)   g_sham_delay++;
   if(fsm_state==STIM_STA)  g_stim_delay++;
}


/*------------------------------------------------------*/
/*------------------------按键中断----------------------*/
/*------------------------------------------------------*/
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
   unsigned char temp;
   temp = P1IFG;
   //Delay_ms(2);
   switch(temp)
   { 
      case 0x40:  //+ 键被按下
      case 0x5d:
        if(fsm_state==FM_SET_STA)
        {
          wavefrequency_10 += FM_STEP_10;
          if(wavefrequency_10 > MAX_FM_10)  wavefrequency_10 = MAX_FM_10;
        }
        if(fsm_state==AM_SET_STA)
        {
           waveamplitude += AM_STEP;
           if(waveamplitude > MAX_AM)  waveamplitude = MAX_AM;
        }  
      break;
      case 0x9d: 
      case 0x80: 
        if(fsm_state==FM_SET_STA)  //- 键被按下
        {
          wavefrequency_10 -= FM_STEP_10;
          if(wavefrequency_10 < MIN_FM_10)  wavefrequency_10 = MIN_FM_10;
        }
        if(fsm_state==AM_SET_STA)
        {
           waveamplitude -= AM_STEP;
           if(waveamplitude < MIN_AM)  waveamplitude = MIN_AM;
        }  
      break;
      default: break;
   }
 if( (fsm_state==FM_SET_STA) || (fsm_state==AM_SET_STA) )
   {
     PrintGB(0,0,(unsigned char*)fu_hz);   
     PrintGB(16,0,(unsigned char*)zhi_hz);
     display_waveamplitude(waveamplitude);
     
     PrintGB(0,2,(unsigned char*)pin_hz);
     PrintGB(16,2,(unsigned char*)lv_hz);
     display_wavefrequency(wavefrequency_10);

     Sin_Waveform_set_function();
     TACCTL0 &=~ CCIE;
     DAC12_0DAT = WAVE_ZERO;
   }
   P1IFG = 0; // 清除中断请求位
}


/*------------------------------------------------------*/
/*------------------------按键中断----------------------*/
/*------------------------------------------------------*/
#pragma vector=PORT2_VECTOR       
__interrupt void Port_2(void)
{
   
   unsigned char temp;
   temp = P2IFG;
   Delay_ms(1);
   if((P2IN&temp)==(!temp))
   {
     switch(fsm_state)
     {
        case FM_SET_STA:
           TACCTL0 &=~ CCIE;
           fsm_state=AM_SET_STA;
           display_am_activity();
           break;
        case AM_SET_STA:
           fsm_state=SHAM_STA;
           display_sham_activity();
           g_sham_delay=0; //刺激时间归位为0
           TACCTL0 = CCIE; //启动刺激
           break;
        case SHAM_STA:
           fsm_state=STIM_STA;
           g_stim_delay=0; //sham刺激归位为0
           TACCTL0 = CCIE;
           display_stim_activity();
           break;
        case STIM_STA:
           TACCTL0 &=~ CCIE;
           fsm_state=FM_SET_STA;
           display_fm_activity();
           break;
     case STOP_STA:
           TACCTL0 &=~ CCIE;
           fsm_state=FM_SET_STA;
           display_fm_activity();
           break;
       default: break;
     }
 
   }
   DAC12_0DAT = WAVE_ZERO;
   P2IFG = 0; // 清除中断请求位
}


/*------------------------------------------------------*/
/*----------------------time_a中断函数------------------*/
/*------------------------------------------------------*/
#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A(void)
{
   timing_flag_interrupt = 1;
}

/*------------------------------------------------------*/
/*----------------------time_b中断函数------------------*/
/*------------------------------------------------------*/
#pragma vector = TIMERB0_VECTOR
__interrupt void Timer_B(void)
{
   
}


void Delay_ms(unsigned int n)
{  
   int j;
   
   for(;n>0;n--){
      //WDTCTL = WDT_ARST_250;;
      for(j=0;j<150;j++){  
         _NOP();
         _NOP();
         _NOP();
         _NOP();
         _NOP();
         _NOP();
         _NOP();
         _NOP();
         _NOP();
         _NOP();
         _NOP();
         _NOP();
         _NOP();
      }
   } 
}
void Delay_50us(unsigned int n)
{  
   int j;
   
   for(;n>0;n--){
      //WDTCTL = WDT_ARST_250;;
      for(j=0;j<7;j++){  
         _NOP();
         _NOP();
         _NOP();
         _NOP();
         _NOP();
         _NOP();
         _NOP();
         _NOP();
         _NOP();
         _NOP();
         _NOP();
         _NOP();
         
      }
   } 
}

