#include <msp430.h>
#include <math.h>
#include<string.h>
#include "st7565r.h"
#include "hzconf.h"

#pragma pack(1)
#define   WAVE_ZERO           682
#define   KEY_FRE             1
#define   KEY_AMP             2  
#define   KEY_DUTYCYCLE       3
#define   KEY_HALFCYCLE       4

const unsigned int ucArray_Sin[]={1365,1381,1398,1415,1431,1448,1465,1482,1498,1515,1532,1548,1565,1581,1598,1614,1631,1647,1664,1680,1696,1712,1729,1745,1761,1777,1793,1809,1824,1840,1856,1871,1887,1902,1918,1933,1948,1963,1978,1993,2008,2023,2037,2052,2066,2081,2095,2109,2123,2137,2150,2164,2178,2191,2204,2217,2230,2243,2256,2269,2281,2294,2306,2318,2330,2341,2353,2365,2376,2387,2398,2409,2420,2430,2441,2451,2461,2471,2481,2490,2499,2509,2518,2527,2535,2544,2552,2560,2568,2576,2584,2591,2598,2606,2612,2619,2626,2632,2638,2644,2650,2655,2661,2666,2671,2675,2680,2684,2689,2693,2696,2700,2703,2706,2709,2712,2715,2717,2719,2721,2723,2724,2726,2727,2728,2729,2729,2729,2730,2729,2729,2729,2728,2727,2726,2724,2723,2721,2719,2717,2715,2712,2709,2706,2703,2700,2696,2693,2689,2684,2680,2675,2671,2666,2661,2655,2650,2644,2638,2632,2626,2619,2612,2606,2598,2591,2584,2576,2568,2560,2552,2544,2535,2527,2518,2509,2499,2490,2481,2471,2461,2451,2441,2430,2420,2409,2398,2387,2376,2365,2353,2341,2330,2318,2306,2294,2281,2269,2256,2243,2230,2217,2204,2191,2178,2164,2150,2137,2123,2109,2095,2081,2066,2052,2037,2023,2008,1993,1978,1963,1948,1933,1918,1902,1887,1871,1856,1840,1824,1809,1793,1777,1761,1745,1729,1712,1696,1680,1664,1647,1631,1614,1598,1581,1565,1548,1532,1515,1498,1482,1465,1448,1431,1415,1398,1381,1365,1349,1332,1315,1299,1282,1265,1248,1232,1215,1198,1182,1165,1149,1132,1116,1099,1083,1066,1050,1034,1018,1001,985,969,953,937,921,906,890,874,859,843,828,812,797,782,767,752,737,722,707,693,678,664,649,635,621,607,593,580,566,552,539,526,513,500,487,474,461,449,436,424,412,400,389,377,365,354,343,332,321,310,300,289,279,269,259,249,240,231,221,212,203,195,186,178,170,162,154,146,139,132,124,118,111,104,98,92,86,80,75,69,64,59,55,50,46,41,37,34,30,27,24,21,18,15,13,11,9,7,6,4,3,2,1,1,1,0,1,1,1,2,3,4,6,7,9,11,13,15,18,21,24,27,30,34,37,41,46,50,55,59,64,69,75,80,86,92,98,104,111,118,124,132,139,146,154,162,170,178,186,195,203,212,221,231,240,249,259,269,279,289,300,310,321,332,343,354,365,377,389,400,412,424,436,449,461,474,487,500,513,526,539,552,566,580,593,607,621,635,649,664,678,693,707,722,737,752,767,782,797,812,828,843,859,874,890,906,921,937,953,969,985,1001,1018,1034,1050,1066,1083,1099,1116,1132,1149,1165,1182,1198,1215,1232,1248,1265,1282,1299,1315,1332,1349,};
uint sinpx[512];
uint sin_num;
uint wave_zero;
//数据与参数
typedef struct
{
   uchar  freq;
   uchar  half;
   uchar  duty;
   UNION_INT ampl;
   UNION_INTG offset;
} D_PAR;

//设备参数公用体
typedef union
{
   D_PAR dtu_data;
   uchar parameter[7];
} DEVICE_PA;
DEVICE_PA device_pa;


float wavefrequency;      //波形频率
unsigned char wavehalfcycle;       //波形的半周期脉冲数
unsigned char waveduty;
float waveamplitude;       //波形幅值
float waveamplitude_ff;
float  wavedutycycle;             //波形的占空比
float  wave_yoffset;        //波形y轴平移值

unsigned char timing_flag;        //波形时间阶段标志
unsigned char timing_flag_interrupt;//波形时间中断标志

unsigned char key_value;
unsigned char key_function;
unsigned char keyflag; 
unsigned char keysecod;
unsigned char fun_second;
unsigned int disVoltage;
unsigned int used_minute;
unsigned int used_second;
unsigned char index_ad;
unsigned int Voltage_A0results[20];
unsigned char wdt_flag;

void Use_High_Frequency_Clock(void);
void Use_Low_Frequency_Clock(void);
void DAC12_init(void);
uint ADC12_voltage_check(void);
void timerA_init(unsigned int ssA_count);
void timerB_init(unsigned int ssB_count);
void Delay_ms(unsigned int n);
void Delay_50us(unsigned int n);
void Key_set(void);
void display_ascii(unsigned char num,unsigned char x,unsigned char y);
void display_time_vol(unsigned int vol,unsigned int min,unsigned int sec);
void display_wavefrequency(float fre);
void display_waveamplitude(float amp);
void display_wavedutycycle(float dut);
void display_wavehalfcycle(unsigned int hal);
void display_wave_yoffset(float offset);
void display_shuxian(void);
void Waveform_output_function(uchar pulse); 
void write_fre_data(uint address, uchar num, uchar *pointer);
void read_fre_data(uint address, uchar num, uchar *pointer);
float ordination(uint *buffer_p, uint n);
void wdt_second_handle(void);

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
   //timerB_init(32768);
   read_fre_data(0x00,7,device_pa.parameter);
   if((device_pa.dtu_data.freq != 0 && device_pa.dtu_data.freq != 0xff)
      &&(device_pa.dtu_data.half != 0 && device_pa.dtu_data.half != 0xff)
         &&(device_pa.dtu_data.duty != 0 && device_pa.dtu_data.duty != 0xff)
            &&(device_pa.dtu_data.ampl.u_int != 0 && device_pa.dtu_data.ampl.u_int != 65535)
               &&device_pa.dtu_data.offset.u_intg < 401&&device_pa.dtu_data.offset.u_intg > -401)      
   { 
      wavefrequency = device_pa.dtu_data.freq;
      if(wavefrequency > 64) wavefrequency = 0.5;
      waveamplitude = device_pa.dtu_data.ampl.u_int;
      wavedutycycle = device_pa.dtu_data.duty/10.0;
      wavehalfcycle = device_pa.dtu_data.half;
      wave_yoffset = device_pa.dtu_data.offset.u_intg;
   }
   else
   {
      wavefrequency = 16;
      waveamplitude =546;
      wavedutycycle = 0.2;
      wavehalfcycle = 4;
      wave_yoffset = 0;
   }
   
   ClearlcdRAM();
   PrintGB(0,0,(unsigned char*)fu_hz);   
   PrintGB(16,0,(unsigned char*)zhi_hz);
   display_waveamplitude(waveamplitude);
   //WDTCTL = WDT_ARST_250;;
   PrintGB(0,2,(unsigned char*)pin_hz);
   PrintGB(16,2,(unsigned char*)lv_hz);
   display_wavefrequency(wavefrequency);
   
   //WDTCTL = WDT_ARST_250;;
   
   waveamplitude_ff = waveamplitude/1365.0;
   for(uint i=0;i<512;i++)
      sinpx[i] = (uint)(waveamplitude_ff*(ucArray_Sin[i]));
   wave_zero = 2047 - sinpx[0];
   timerA_init((unsigned int)(7812.5/wavefrequency));
   keyflag = 1;
   key_function = 6;
   disVoltage = ADC12_voltage_check();    
   while(1)
   { 
      //WDTCTL = WDT_ARST_250;;
      display_shuxian();
      if(timing_flag_interrupt)
      {
         DAC12_0DAT = (uint)(wave_zero + wave_yoffset + sinpx[sin_num++]);
         if(sin_num >= 512)sin_num = 0;
         timing_flag_interrupt = 0;
         //TACCTL0 = CCIE;
      }
      wdt_second_handle();
      //Waveform_output_function(wavehalfcycle);      
   }     
   
}
void display_shuxian(void)
{                                    
   switch(key_function)
   {                     
      case 1:
      for(uint i=0;i<600;i++)
      {
         Delay_ms(1);
         if(key_function != 1)break;
      }
      PrintASCII(116,2,(unsigned char*)ASCII_SHUXIAN);
      for(uint i=0;i<600;i++)
      {
         Delay_ms(1);
         if(key_function != 1)break;
      }
      PrintASCII(116,2,(unsigned char*)ASCII_KONG);
      break;
      case 2:
      for(uint i=0;i<600;i++)
      {
         Delay_ms(1);
         if(key_function != 2)break;
      }
      PrintASCII(116,0,(unsigned char*)ASCII_SHUXIAN);
      for(uint i=0;i<600;i++)
      {
         Delay_ms(1);
         if(key_function != 2)break;
      }
      PrintASCII(116,0,(unsigned char*)ASCII_KONG);
      PrintASCII(116,2,(unsigned char*)ASCII_KONG);
      break;
      case 3:
      for(uint i=0;i<600;i++)
      {
         Delay_ms(1);
         if(key_function != 3)break;
      }
      PrintASCII(116,2,(unsigned char*)ASCII_SHUXIAN);
      for(uint i=0;i<600;i++)
      {
         Delay_ms(1);
         if(key_function != 3)break;
      }
      PrintASCII(116,2,(unsigned char*)ASCII_KONG);
      break;
      case 4:
      for(uint i=0;i<600;i++)
      {
         Delay_ms(1);
         if(key_function != 4)break;
      }
      PrintASCII(116,0,(unsigned char*)ASCII_SHUXIAN);
      for(uint i=0;i<600;i++)
      {
         Delay_ms(1);
         if(key_function != 4)break;
      }
      PrintASCII(116,0,(unsigned char*)ASCII_KONG);
      PrintASCII(116,2,(unsigned char*)ASCII_KONG);
      break;
      case 5:
      for(uint i=0;i<600;i++)
      {
         Delay_ms(1);
         if(key_function != 5)break;
      }
      PrintASCII(116,2,(unsigned char*)ASCII_SHUXIAN);
      for(uint i=0;i<600;i++)
      {
         Delay_ms(1);
         if(key_function != 5)break;
      }
      PrintASCII(116,2,(unsigned char*)ASCII_KONG);
      break;
      default:
      //PrintASCII(116,2,(unsigned char*)ASCII_KONG);
      //PrintASCII(116,0,(unsigned char*)ASCII_KONG);
      //PrintASCII(127,2,(unsigned char*)ASCII_KONG);
      break;
   }
   
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
void display_time_vol(unsigned int vol,unsigned int min,unsigned int sec)
{
   uint min_h,min_l;
   uint sec_h,sec_l;
   uint vol_h,vol_l;
   
   
   vol_h = vol/10;
   vol_l = vol%10;
   min_h = min/10;
   min_l = min%10;
   sec_h = sec/10;
   sec_l = sec%10;
   
   
   PrintGB(0,0,(unsigned char*)dian_hz);
   PrintASCII(16,0,(unsigned char*)ASCII_KONG);
   PrintASCII(24,0,(unsigned char*)ASCII_KONG);
   PrintGB(32,0,(unsigned char*)liang_hz);
   PrintASCII(48,0,(unsigned char*)ASCII_KONG);
   PrintASCII(56,0,(unsigned char*)ASCII_KONG);
   PrintASCII(64,0,(unsigned char*)ASCII_KONG);
   PrintASCII(72,0,(unsigned char*)ASCII_KONG);
   
   display_ascii(vol_h,78,0);//54
   PrintASCII(86,0,(unsigned char*)ASCII_DIAN);  
   display_ascii(vol_l,94,0);
   PrintASCII(102,0,(unsigned char*)ASCII_V);
   
   PrintGB(0,2,(unsigned char*)shi_hz);
   PrintASCII(16,2,(unsigned char*)ASCII_KONG);
   PrintASCII(24,2,(unsigned char*)ASCII_KONG);
   PrintGB(32,2,(unsigned char*)jian_hz);
   
   PrintASCII(48,2,(unsigned char*)ASCII_KONG);
   PrintASCII(56,2,(unsigned char*)ASCII_KONG);
   PrintASCII(64,2,(unsigned char*)ASCII_KONG);
   PrintASCII(72,2,(unsigned char*)ASCII_KONG);
   PrintASCII(116,2,(unsigned char*)ASCII_KONG);
   PrintASCII(124,2,(unsigned char*)ASCII_KONG);
   //PrintASCII(80,2,(unsigned char*)ASCII_KONG);
   
   
   display_ascii(min_h,78,2);//86
   display_ascii(min_l,86,2);
   PrintASCII(94,2,(unsigned char*)ASCII_MAOHAO);
   display_ascii(sec_h,102,2);
   display_ascii(sec_l,110,2);
   display_ascii(sec_l,108,2);
   PrintASCII(116,2,(unsigned char*)ASCII_KONG);
   PrintASCII(124,2,(unsigned char*)ASCII_KONG);
   
   
}

void display_wavefrequency(float fre)
{
   float tp;
   uint fre_h,fre_l;
   uint free;
   free = (unsigned int)fre;
   tp = (float)fre;
   if(tp == 0.5)
   {
      PrintASCII(32,2,(unsigned char*)ASCII_KONG);
      PrintASCII(40,2,(unsigned char*)ASCII_KONG);
      PrintASCII(48,2,(unsigned char*)ASCII_KONG);
      
      PrintASCII(56,2,(unsigned char*)ASCII_0);
      PrintASCII(64,2,(unsigned char*)ASCII_DIAN);
      PrintASCII(72,2,(unsigned char*)ASCII_5);
      
      
      PrintASCII(80,2,(unsigned char*)ASCII_H);
      PrintASCII(88,2,(unsigned char*)ASCII_z);
      
      PrintASCII(96,2,(unsigned char*)ASCII_KONG);
      PrintASCII(104,2,(unsigned char*)ASCII_KONG);
      PrintASCII(112,2,(unsigned char*)ASCII_KONG);
      PrintASCII(120,2,(unsigned char*)ASCII_KONG);
      PrintASCII(124,2,(unsigned char*)ASCII_KONG);
   }
   else
   {
      fre_h = free/10;
      fre_l = free%10;
      PrintASCII(32,2,(unsigned char*)ASCII_KONG);
      PrintASCII(40,2,(unsigned char*)ASCII_KONG);
      PrintASCII(48,2,(unsigned char*)ASCII_KONG);
      if(fre_h == 0)PrintASCII(56,2,(unsigned char*)ASCII_KONG);
      else display_ascii(fre_h,56,2);
      display_ascii(fre_l,64,2);
      
      PrintASCII(72,2,(unsigned char*)ASCII_H);
      PrintASCII(80,2,(unsigned char*)ASCII_z);
      PrintASCII(88,2,(unsigned char*)ASCII_KONG);
      PrintASCII(96,2,(unsigned char*)ASCII_KONG);
      PrintASCII(104,2,(unsigned char*)ASCII_KONG);
      PrintASCII(112,2,(unsigned char*)ASCII_KONG);
      PrintASCII(120,2,(unsigned char*)ASCII_KONG);
      PrintASCII(124,2,(unsigned char*)ASCII_KONG);
   }
}
void display_waveamplitude(float amp)
{
   uint amp_h,amp_l;
   ulong crut;
   
   crut = (ulong)((((amp+2047.5)*30.0/4095.0)-15.0)*100.0);
   
   if(crut >= 1000)
   {
      PrintASCII(32,0,(unsigned char*)ASCII_KONG);
      PrintASCII(40,0,(unsigned char*)ASCII_1);
      PrintASCII(48,0,(unsigned char*)ASCII_0);
      PrintASCII(56,0,(unsigned char*)ASCII_0);
      
      display_ascii(0,64,0);
      PrintASCII(72,0,(unsigned char*)ASCII_u);
      PrintASCII(80,0,(unsigned char*)ASCII_A);
      
      PrintASCII(96,0,(unsigned char*)ASCII_KONG);
      PrintASCII(104,0,(unsigned char*)ASCII_KONG);
   }
   else
   {
      crut = crut/10;
      amp_h = crut/10;
      amp_l = crut%10;
      
      PrintASCII(32,0,(unsigned char*)ASCII_KONG);
      PrintASCII(40,0,(unsigned char*)ASCII_KONG);
      if(amp_h == 0)
         PrintASCII(48,0,(unsigned char*)ASCII_KONG);
      else
         display_ascii(amp_h,48,0);
      if(amp_l == 0 && amp_h == 0)
         PrintASCII(56,0,(unsigned char*)ASCII_KONG);
      else
         display_ascii(amp_l,56,0);
      
      
      display_ascii(0,64,0);
      PrintASCII(72,0,(unsigned char*)ASCII_u);
      PrintASCII(80,0,(unsigned char*)ASCII_A);
      
      PrintASCII(96,0,(unsigned char*)ASCII_KONG);
      PrintASCII(104,0,(unsigned char*)ASCII_KONG);
   }
}
void display_wavedutycycle(float dut)
{
   uint ss;
   ss = (uint)(dut*10);
   PrintASCII(48,2,(unsigned char*)ASCII_KONG);
   PrintASCII(56,2,(unsigned char*)ASCII_KONG);
   display_ascii(ss,64,2);
   display_ascii(0,72,2);
   PrintASCII(80,2,(unsigned char*)ASCII_BAIFENGHAO);
   PrintASCII(88,2,(unsigned char*)ASCII_KONG);
   PrintASCII(96,2,(unsigned char*)ASCII_KONG);
   PrintASCII(104,2,(unsigned char*)ASCII_KONG);
   PrintASCII(112,2,(unsigned char*)ASCII_KONG);
   
}
void display_wavehalfcycle(unsigned int hal)
{
   PrintASCII(48,0,(unsigned char*)ASCII_KONG);
   PrintASCII(56,0,(unsigned char*)ASCII_KONG);
   
   display_ascii(hal,64,0);
   PrintASCII(72,0,(unsigned char*)ASCII_KONG);
   PrintASCII(80,0,(unsigned char*)ASCII_KONG);
   PrintASCII(88,0,(unsigned char*)ASCII_KONG);
   PrintASCII(96,0,(unsigned char*)ASCII_KONG);
   PrintASCII(104,0,(unsigned char*)ASCII_KONG);
}

void display_wave_yoffset(float offset)
{
   uint amp_h,amp_l;
   ulong crut;
   
   
   //PrintASCII(64,2,(unsigned char*)ASCII_KONG);
   //PrintASCII(80,2,(unsigned char*)ASCII_KONG);
   if(offset<= -13.65)
   {
      crut = (uint)(offset*(-1));
      PrintASCII(64,2,(unsigned char*)ASCII_JIAN);
   }
   else
   {
      crut = (uint)offset;
      PrintASCII(64,2,(unsigned char*)ASCII_KONG);
   }
   crut = (ulong)((((crut+2047.5)*30.0/4095.0)-15.0)*100.0);
   crut = crut/10;
   amp_h = crut/10;
   amp_l = crut%10;
   
   if(amp_h == 0)
      PrintASCII(72,2,(unsigned char*)ASCII_KONG);
   else
      display_ascii(amp_h,72,2);
   if(amp_l == 0 && amp_h == 0)
      PrintASCII(80,2,(unsigned char*)ASCII_KONG);
   else
      display_ascii(amp_l,80,2);
   
   
   display_ascii(0,88,2);
   PrintASCII(96,2,(unsigned char*)ASCII_u);
   PrintASCII(104,2,(unsigned char*)ASCII_A);
   
   //for(uchar i=0;i<124;i+=8)
   //    PrintASCII(i,0,(unsigned char*)ASCII_KONG);
   
   //PrintASCII(96,2,(unsigned char*)ASCII_KONG);
   //PrintASCII(104,2,(unsigned char*)ASCII_KONG);
}

void Key_set(void)
{
   //P1DIR &= ~BIT0;
   //P1SEL |= BIT0;       //选择外围模块功能
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
   // DAC12_0DAT = 2000;                            // Positive ramp
   // DAC12_0DAT &= 0xFFF;                          // Modulo 4096
}

/*------------------------------------------------------*/
/*------------------ADC12初始化检测电压-----------------*/
/*------------------------------------------------------*/
uint ADC12_voltage_check(void)
{
   
   uint Voltage;
   float vo_sum = 0.0;
   
   ADC12CTL0 = ADC12ON + SHT0_12 + REFON + REF2_5V + MSC;
   ADC12CTL1 = ADC12SSEL_2 + SHP + CONSEQ_2 + CSTARTADD_0;
   ADC12MCTL0 = SREF_1 + INCH_3; // 参考电压:V+=Veref+,V-=Veref- ADC通道:A1
   ADC12CTL0 |= ENC + ADC12SC; // 转换使能开始转换
   
   //压力电压数据采集
   for(index_ad = 0;index_ad < 20 ;index_ad++)
   {
      while((ADC12IFG & 0x0001) == 0);
      Voltage_A0results[index_ad] = ADC12MEM0;
      //vo_sum = vo_sum + Voltage_A0results[index_ad];
   }
   ADC12CTL0 &=~ (ENC + ADC12SC);
   vo_sum = ordination(Voltage_A0results,20);
   Voltage = (uint) ((vo_sum*2.5/4095.0) * 37.4);
   
   ADC12CTL0 = 0;
   ADC12CTL1 = 0;
   ADC12MCTL0 = 0;
   ADC12CTL0 = REF2_5V + REFON;  
   return Voltage;
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
   //   tt32ms++;
   //   if(tt32ms>125)
   //   {
   //      tt32ms = 0;  
   //_EINT();
   wdt_flag = 1; 
   //   }
   
}
void wdt_second_handle(void)
{  
   if(wdt_flag)
   {
      used_second += 1;
      if(used_second > 59)
      {
         used_minute += 1;
         used_second = 0;
      }
      if(used_minute > 99) used_minute = 0;
      
//      if(keyflag == 1)keysecod += 1;
//      if(keysecod > 30) 
//      {
//         keyflag = 0;
//         keysecod = 0;
//         ClearlcdRAM();
//         Lcd_Init();
//         display_time_vol(disVoltage,used_minute,used_second);
//      }
      
      if(key_function != 8)fun_second += 1;
      if(fun_second > 6) 
      {
         key_function = 8;
         fun_second = 0;
         TACCTL0 = CCIE;
      }
      wdt_flag = 0;
   }
}
/*------------------------------------------------------*/
/*------------------------按键中断----------------------*/
/*------------------------------------------------------*/
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
   unsigned char temp;
   temp = P1IFG;
   Delay_ms(2);
   //if((P1IN&temp)==(!temp))
   //{
   switch(temp)
   { 
      case 0x40: 
      key_value = 1;
      keyflag = 1;
      
      if(key_function == 1)
      {
         if(wavefrequency<8 || wavefrequency>14)
            wavefrequency *= 2;
         else
            wavefrequency += 2;
         if(wavefrequency > 64)wavefrequency = 64;
         
         PrintGB(0,0,(unsigned char*)fu_hz);   
         PrintGB(16,0,(unsigned char*)zhi_hz);
         display_waveamplitude(waveamplitude);
         
         PrintGB(0,2,(unsigned char*)pin_hz);
         PrintGB(16,2,(unsigned char*)lv_hz);
         display_wavefrequency(wavefrequency);
         
      }
      else if(key_function == 2)                                                                    
      {
         waveamplitude += 27.3;//13.65;
         if(waveamplitude > 1365)waveamplitude = 1365;//819
         
         PrintGB(0,0,(unsigned char*)fu_hz);   
         PrintGB(16,0,(unsigned char*)zhi_hz);
         display_waveamplitude(waveamplitude);
         
         PrintGB(0,2,(unsigned char*)pin_hz);
         PrintGB(16,2,(unsigned char*)lv_hz);
         display_wavefrequency(wavefrequency);
      }
      else if(key_function == 3)
      {
         wavedutycycle *= 2;
         if(wavedutycycle > 0.8) wavedutycycle = 0.8;
         PrintGB(0,0,(unsigned char*)mai_hz);
         PrintGB(16,0,(unsigned char*)chong_hz);
         PrintGB(32,0,(unsigned char*)shu_hz);
         display_wavehalfcycle(wavehalfcycle);
         
         PrintGB(0,2,(unsigned char*)zhan_hz);
         PrintGB(16,2,(unsigned char*)kong_hz);
         PrintGB(32,2,(unsigned char*)bi_hz);
         display_wavedutycycle(wavedutycycle);
      }
      else if(key_function == 4)
      {
         wavehalfcycle *= 2;
         if(wavehalfcycle > 8) wavehalfcycle = 8;
         PrintGB(0,0,(unsigned char*)mai_hz);
         PrintGB(16,0,(unsigned char*)chong_hz);
         PrintGB(32,0,(unsigned char*)shu_hz);
         display_wavehalfcycle(wavehalfcycle);
         
         PrintGB(0,2,(unsigned char*)zhan_hz);
         PrintGB(16,2,(unsigned char*)kong_hz);
         PrintGB(32,2,(unsigned char*)bi_hz);
         display_wavedutycycle(wavedutycycle);
      }
      else if(key_function == 5)
      {
         wave_yoffset += 13.65;
         if(wave_yoffset > 546)wave_yoffset = 546;
         
         PrintGB(0,2,(unsigned char*)zhii_hz);
         PrintGB(16,2,(unsigned char*)liu_hz);
         PrintGB(32,2,(unsigned char*)pian_hz);
         PrintGB(48,2,(unsigned char*)zhiii_hz);
         display_wave_yoffset(wave_yoffset);
      }
      break;
      case 0x5d: 
      key_value = 1;
      keyflag = 1;
      
      if(key_function == 1)
      {
         if(wavefrequency<8 || wavefrequency>14)
            wavefrequency *= 2;
         else
            wavefrequency += 2;
         if(wavefrequency > 64)wavefrequency = 64;
         
         PrintGB(0,0,(unsigned char*)fu_hz);   
         PrintGB(16,0,(unsigned char*)zhi_hz);
         display_waveamplitude(waveamplitude);
         
         PrintGB(0,2,(unsigned char*)pin_hz);
         PrintGB(16,2,(unsigned char*)lv_hz);
         display_wavefrequency(wavefrequency);
         
      }
      else if(key_function == 2)
      {
         waveamplitude += 27.3;
         if(waveamplitude > 1365)waveamplitude = 1365;
         
         PrintGB(0,0,(unsigned char*)fu_hz);   
         PrintGB(16,0,(unsigned char*)zhi_hz);
         display_waveamplitude(waveamplitude);
         
         PrintGB(0,2,(unsigned char*)pin_hz);
         PrintGB(16,2,(unsigned char*)lv_hz);
         display_wavefrequency(wavefrequency);
      }
      else if(key_function == 3)
      {
         wavedutycycle *= 2;
         if(wavedutycycle > 0.8) wavedutycycle = 0.8;
         PrintGB(0,0,(unsigned char*)mai_hz);
         PrintGB(16,0,(unsigned char*)chong_hz);
         PrintGB(32,0,(unsigned char*)shu_hz);
         display_wavehalfcycle(wavehalfcycle);
         
         PrintGB(0,2,(unsigned char*)zhan_hz);
         PrintGB(16,2,(unsigned char*)kong_hz);
         PrintGB(32,2,(unsigned char*)bi_hz);
         display_wavedutycycle(wavedutycycle);//?????????
      }
      else if(key_function == 4)
      {
         wavehalfcycle *= 2;
         if(wavehalfcycle > 8) wavehalfcycle = 8;
         PrintGB(0,0,(unsigned char*)mai_hz);
         PrintGB(16,0,(unsigned char*)chong_hz);
         PrintGB(32,0,(unsigned char*)shu_hz);
         display_wavehalfcycle(wavehalfcycle);
         
         PrintGB(0,2,(unsigned char*)zhan_hz);
         PrintGB(16,2,(unsigned char*)kong_hz);
         PrintGB(32,2,(unsigned char*)bi_hz);
         display_wavedutycycle(wavedutycycle);
      }
      else if(key_function == 5)
      {
         wave_yoffset += 13.65;
         if(wave_yoffset > 546)wave_yoffset = 546;
         
         PrintGB(0,2,(unsigned char*)zhii_hz);
         PrintGB(16,2,(unsigned char*)liu_hz);
         PrintGB(32,2,(unsigned char*)pian_hz);
         PrintGB(48,2,(unsigned char*)zhiii_hz);
         display_wave_yoffset(wave_yoffset);
      }
      break;
      case 0x9d: 
      key_value = 0;
      keyflag = 1;
      
      if(key_function == 1)
      {
         
         if(wavefrequency<=8 || wavefrequency>16)
            wavefrequency /= 2;
         else
            wavefrequency -= 2;
         if(wavefrequency < 0.5)wavefrequency = 0.5;
         
         PrintGB(0,0,(unsigned char*)fu_hz);   
         PrintGB(16,0,(unsigned char*)zhi_hz);
         display_waveamplitude(waveamplitude);
         
         PrintGB(0,2,(unsigned char*)pin_hz);
         PrintGB(16,2,(unsigned char*)lv_hz);
         display_wavefrequency(wavefrequency);
         
      }
      else if(key_function == 2)
      {
         waveamplitude -= 27.3;
         if(waveamplitude < 27.3)waveamplitude = 0;
         
         PrintGB(0,0,(unsigned char*)fu_hz);   
         PrintGB(16,0,(unsigned char*)zhi_hz);
         display_waveamplitude(waveamplitude);
         
         PrintGB(0,2,(unsigned char*)pin_hz);
         PrintGB(16,2,(unsigned char*)lv_hz);
         display_wavefrequency(wavefrequency);
      }
      else if(key_function == 3)
      {
         wavedutycycle /= 2;
         if(wavedutycycle < 0.2) wavedutycycle = 0.2;
         PrintGB(0,0,(unsigned char*)mai_hz);
         PrintGB(16,0,(unsigned char*)chong_hz);
         PrintGB(32,0,(unsigned char*)shu_hz);
         display_wavehalfcycle(wavehalfcycle);
         
         PrintGB(0,2,(unsigned char*)zhan_hz);
         PrintGB(16,2,(unsigned char*)kong_hz);
         PrintGB(32,2,(unsigned char*)bi_hz);
         display_wavedutycycle(wavedutycycle);
      }
      else if(key_function == 4)
      {
         wavehalfcycle /= 2;
         if(wavehalfcycle < 2) wavehalfcycle = 2;
         PrintGB(0,0,(unsigned char*)mai_hz);
         PrintGB(16,0,(unsigned char*)chong_hz);
         PrintGB(32,0,(unsigned char*)shu_hz);
         display_wavehalfcycle(wavehalfcycle);
         
         PrintGB(0,2,(unsigned char*)zhan_hz);
         PrintGB(16,2,(unsigned char*)kong_hz);
         PrintGB(32,2,(unsigned char*)bi_hz);
         display_wavedutycycle(wavedutycycle);
      }
      else if(key_function == 5)
      {
         wave_yoffset -= 13.65;
         if(wave_yoffset < -546)wave_yoffset = -546;
         
         PrintGB(0,2,(unsigned char*)zhii_hz);
         PrintGB(16,2,(unsigned char*)liu_hz);
         PrintGB(32,2,(unsigned char*)pian_hz);
         PrintGB(48,2,(unsigned char*)zhiii_hz);
         display_wave_yoffset(wave_yoffset);
      }
      
      break;        
      case 0x80: 
      key_value = 0;
      keyflag = 1;
      
      if(key_function == 1)
      {
         
         if(wavefrequency<=8 || wavefrequency>16)
            wavefrequency /= 2;
         else
            wavefrequency -= 2;
         if(wavefrequency < 0.5)wavefrequency = 0.5;
         
         PrintGB(0,0,(unsigned char*)fu_hz);   
         PrintGB(16,0,(unsigned char*)zhi_hz);
         display_waveamplitude(waveamplitude);
         
         PrintGB(0,2,(unsigned char*)pin_hz);
         PrintGB(16,2,(unsigned char*)lv_hz);
         display_wavefrequency(wavefrequency);
         
      }
      else if(key_function == 2)
      {
         waveamplitude -= 27.3;
         if(waveamplitude < 27.3)waveamplitude = 0;
         
         PrintGB(0,0,(unsigned char*)fu_hz);   
         PrintGB(16,0,(unsigned char*)zhi_hz);
         display_waveamplitude(waveamplitude);
         
         PrintGB(0,2,(unsigned char*)pin_hz);
         PrintGB(16,2,(unsigned char*)lv_hz);
         display_wavefrequency(wavefrequency);
      }
      else if(key_function == 3)
      {
         wavedutycycle /= 2;
         if(wavedutycycle < 0.2) wavedutycycle = 0.2;
         PrintGB(0,0,(unsigned char*)mai_hz);
         PrintGB(16,0,(unsigned char*)chong_hz);
         PrintGB(32,0,(unsigned char*)shu_hz);
         display_wavehalfcycle(wavehalfcycle);
         
         PrintGB(0,2,(unsigned char*)zhan_hz);
         PrintGB(16,2,(unsigned char*)kong_hz);
         PrintGB(32,2,(unsigned char*)bi_hz);
         display_wavedutycycle(wavedutycycle);
      }
      else if(key_function == 4)
      {
         wavehalfcycle /= 2;
         if(wavehalfcycle < 2) wavehalfcycle = 2;
         PrintGB(0,0,(unsigned char*)mai_hz);
         PrintGB(16,0,(unsigned char*)chong_hz);
         PrintGB(32,0,(unsigned char*)shu_hz);
         display_wavehalfcycle(wavehalfcycle);
         
         PrintGB(0,2,(unsigned char*)zhan_hz);
         PrintGB(16,2,(unsigned char*)kong_hz);
         PrintGB(32,2,(unsigned char*)bi_hz);
         display_wavedutycycle(wavedutycycle);
      }
      else if(key_function == 5)
      {
         wave_yoffset -= 13.65;
         if(wave_yoffset < -546)wave_yoffset = -546;
         
         PrintGB(0,2,(unsigned char*)zhii_hz);
         PrintGB(16,2,(unsigned char*)liu_hz);
         PrintGB(32,2,(unsigned char*)pian_hz);
         PrintGB(48,2,(unsigned char*)zhiii_hz);
         display_wave_yoffset(wave_yoffset);
      }
      break;
      default: break;
   }
   //}
   keysecod = 0;
   fun_second = 0;
   
   device_pa.dtu_data.freq = (uint)wavefrequency;
   device_pa.dtu_data.ampl.u_int = (uint)waveamplitude;
   device_pa.dtu_data.duty = (uint)(wavedutycycle*10);
   device_pa.dtu_data.half = wavehalfcycle;
   device_pa.dtu_data.offset.u_intg = (uint)wave_yoffset;
   timerA_init((unsigned int)(7812.5/wavefrequency));
   waveamplitude_ff = waveamplitude/1365.0;
   for(uint i=0;i<512;i++)
      sinpx[i] = (uint)(waveamplitude_ff*ucArray_Sin[i]);
   write_fre_data(0x00,7,device_pa.parameter);
   TACCTL0 &=~ CCIE;
   DAC12_0DAT = 2047;
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
   Delay_ms(2);
   if((P2IN&temp)==(!temp))
   {
      switch(temp)
      { 
         case 0x01: 
         
         key_function += 1;
         if(key_function<3)
         {
            if(key_function == 1)
            {
               ClearlcdRAM();
               Lcd_Init();
            }
            PrintGB(0,0,(unsigned char*)fu_hz);   
            PrintGB(16,0,(unsigned char*)zhi_hz);
            display_waveamplitude(waveamplitude);
            
            PrintGB(0,2,(unsigned char*)pin_hz);
            PrintGB(16,2,(unsigned char*)lv_hz);
            display_wavefrequency(wavefrequency);
         }
         else 
         {
            key_function =0;
            ClearlcdRAM();
            Lcd_Init();
            display_time_vol(disVoltage,used_minute,used_second);
         }
         
         //         else if(key_function>=3&&key_function<5)
         //         {
         //            
         //            if(key_function == 3)
         //            {
         //               ClearlcdRAM();
         //               Lcd_Init();
         //            }
         //            PrintGB(0,0,(unsigned char*)mai_hz);
         //            PrintGB(16,0,(unsigned char*)chong_hz);
         //            PrintGB(32,0,(unsigned char*)shu_hz);
         //            display_wavehalfcycle(wavehalfcycle);
         //            
         //            PrintGB(0,2,(unsigned char*)zhan_hz);
         //            PrintGB(16,2,(unsigned char*)kong_hz);
         //            PrintGB(32,2,(unsigned char*)bi_hz);
         //            display_wavedutycycle(wavedutycycle);
         //         }
         //         else if(key_function == 5)
         //         {
         //            ClearlcdRAM();
         //            Lcd_Init();
         //            PrintGB(0,2,(unsigned char*)zhii_hz);
         //            PrintGB(16,2,(unsigned char*)liu_hz);
         //            PrintGB(32,2,(unsigned char*)pian_hz);
         //            PrintGB(48,2,(unsigned char*)zhiii_hz);
         //            display_wave_yoffset(wave_yoffset);
         //         }
         //         
         //         
         //         if(key_function > 5)
         //            key_function = 0;
         break;
         default: break;
      }
   }
   //PrintASCII(116,2,(unsigned char*)ASCII_KONG);
   //PrintASCII(128,2,(unsigned char*)ASCII_KONG);
   
   fun_second = 0;
   keysecod = 0;
   keyflag = 1;
   TACCTL0 &=~ CCIE;
   DAC12_0DAT = 2047;
   P2IFG = 0; // 清除中断请求位
}


/*------------------------------------------------------*/
/*----------------------time_a中断函数------------------*/
/*------------------------------------------------------*/
#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A(void)
{
   //_EINT(); 
   timing_flag_interrupt = 1;
   
   
//   DAC12_0DAT = (uint)(wave_zero + wave_yoffset + sinpx[sin_num++]);
//   if(sin_num >= 512)sin_num = 0;
//   timing_flag_interrupt = 0;
}

/*------------------------------------------------------*/
/*----------------------time_b中断函数------------------*/
/*------------------------------------------------------*/
#pragma vector = TIMERB0_VECTOR
__interrupt void Timer_B(void)
{
   
}


void Waveform_output_function(uchar pulse)
{
   unsigned int da_mium;
   unsigned int cl_time;
   static unsigned int pp_count,nn_count;
   if(timing_flag_interrupt)
   {
      
      
      switch( timing_flag )
      {
         case 2://第一个是上升沿
         da_mium = (uint)(WAVE_ZERO + waveamplitude + 136.5 + wave_yoffset);
         DAC12_0DAT = da_mium;
         Delay_50us(2);
         
         DAC12_0DAT = (uint)(WAVE_ZERO + waveamplitude + wave_yoffset);
         cl_time = (unsigned int)((32768/wavefrequency/2)*wavedutycycle);
         
         timing_flag = 3;
         timing_flag_interrupt = 0;
         timerA_init(cl_time);//定时1000ms
         break;
         case 3://第3个是下升沿
         if(waveamplitude > 819)DAC12_0DAT = 1228;
         else
         {
            da_mium = (uint)(WAVE_ZERO - waveamplitude - 136.5 + wave_yoffset);
            DAC12_0DAT = da_mium;
            
         }
         //DAC12_0DAT = da_mium;
         Delay_50us(2);
         DAC12_0DAT = (uint)(WAVE_ZERO + wave_yoffset);
         cl_time = (unsigned int)(((32768/wavefrequency/2)*(1 - wavedutycycle))/(wavehalfcycle - 1));
         if(wavehalfcycle == 2)
         {
            timing_flag = 4;
            nn_count = 0;
         }
         else if(wavehalfcycle == 4)
         {
            timing_flag = 3;
            nn_count += 1;
            if(nn_count >= 3)
            {
               timing_flag = 4;
               nn_count = 0;
            }
         }
         else if(wavehalfcycle == 6)
         {
            timing_flag = 3;
            nn_count += 1;
            if(nn_count >= 5)
            {
               timing_flag = 4;
               nn_count = 0;
            }
         }
         else if(wavehalfcycle == 8)
         {
            timing_flag = 3;
            nn_count += 1;
            if(nn_count >= 7)
            {
               timing_flag = 4;
               nn_count = 0;
            }
         }
         
         timing_flag_interrupt = 0;
         timerA_init(cl_time);
         break;
         case 4://第4个是下升沿
         if(waveamplitude > 819)da_mium = 1228;
         else
         {
            da_mium = (uint)(WAVE_ZERO - waveamplitude - 136.5 + wave_yoffset);
         }
         DAC12_0DAT = da_mium;
         Delay_50us(2);
         DAC12_0DAT = (uint)(WAVE_ZERO - waveamplitude + wave_yoffset);
         cl_time = (unsigned int)((32768/wavefrequency/2)*wavedutycycle);
         
         timing_flag = 1;
         timing_flag_interrupt = 0;
         timerA_init(cl_time);
         break;
         case 1://第1个是上升沿
         da_mium = (uint)(WAVE_ZERO+waveamplitude + 136 + wave_yoffset);
         if(da_mium >= 2866) da_mium = 2866;
         DAC12_0DAT = da_mium;
         Delay_50us(2);
         
         DAC12_0DAT = (uint)(WAVE_ZERO + wave_yoffset);
         
         cl_time = (unsigned int)(((32768/wavefrequency/2)*(1 - wavedutycycle))/(wavehalfcycle - 1));
         if(wavehalfcycle == 2)
         {
            timing_flag = 2;
            pp_count = 0;
         }
         else if(wavehalfcycle == 4)
         {
            timing_flag = 1;
            pp_count += 1;
            if(pp_count >= 3)
            {
               timing_flag = 2;
               pp_count = 0;
            }
         }
         else if(wavehalfcycle == 6)
         {
            timing_flag = 1;
            pp_count += 1;
            if(pp_count >= 5)
            {
               timing_flag = 2;
               pp_count = 0;
            }
         }
         else if(wavehalfcycle == 8)
         {
            timing_flag = 1;
            pp_count += 1;
            if(pp_count >= 7)
            {
               timing_flag = 2;
               pp_count = 0;
            }
         }
         
         timing_flag_interrupt = 0;
         timerA_init(cl_time);
         break;
         default:break;
      }
   }
}

void read_fre_data(uint address, uchar num, uchar *pointer)
{
   unsigned j;
   unsigned char *flash_ptr_A;
   
   flash_ptr_A = (unsigned char *)(0x1080 + address);
   FCTL3 = FWKEY ;
   FCTL1 = FWKEY + ERASE ;
   
   for(j=0;j<num;j++)
   {
      pointer[j] =  *flash_ptr_A++;
   }
   
   FCTL1 = FWKEY ;
   FCTL3 = FWKEY + LOCK ;
   
}

void write_fre_data(uint address, uchar num, uchar *pointer)
{
   unsigned int i;
   unsigned char *flash_ptr_A;
   
   flash_ptr_A = (unsigned char *)(0x1080 + address);
   FCTL3 = FWKEY ;
   FCTL1 = FWKEY + ERASE ;
   *flash_ptr_A = 0;
   // delay_ms(5);
   FCTL3 = FWKEY ;
   FCTL1 = FWKEY + WRT;
   for(i=0;i<num;i++)
   {
      *flash_ptr_A++ = *pointer++;
   }
   
   FCTL1 = FWKEY ;
   FCTL3 = FWKEY + LOCK ;
   
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


/*------------------------------------------------------*/
/*---------------------数据排序函数---------------------*/
/*------------------------------------------------------*/
float ordination(uint *buffer_p, uint n)
{
   float tmpe;
   uint tmpe1;
   uint i, j; 
   
   for (i = 0; i < n; i++)
      for (j = 0; j < n - i - 1; j++)
      {
         if (buffer_p[j] > buffer_p[j + 1])
         {
            tmpe1 = buffer_p[j];
            buffer_p[j] = buffer_p[j + 1];
            buffer_p[j + 1] = tmpe1;
         }
      } 
   tmpe = 0.0;
   
   if (n == 20)
   {
      for (i = 5; i < 15; i++)
      {
         tmpe += buffer_p[i];
      }
      tmpe = tmpe / 10.0;
      
   }
   
   return tmpe;
}
