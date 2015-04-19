#ifndef ST7565R_H_
#define ST7565R_H_

/*ioø⁄…Ë÷√   CSB--P1.0,RESEB--P1.1,A0--P1.2,SCLK--P3.0,SDA--P3,1 */
	//P1DIR |=BIT0+BIT1+BIT2;
	//P3DIR |=BIT0+BIT1;

#define CSB_LOW  (P1OUT&=~BIT0)
#define CSB_HIGH (P1OUT|=BIT0)

#define RESEB_LOW  (P1OUT&=~BIT1)
#define RESEB_HIGH (P1OUT|=BIT1)

#define A0_LOW  (P1OUT&=~BIT2)
#define A0_HIGH (P1OUT|=BIT2)

#define SCL_LOW  (P1OUT&=~BIT3)
#define SCL_HIGH (P1OUT|=BIT3)

#define SDA_LOW  (P1OUT&=~BIT4)
#define SDA_HIGH (P1OUT|=BIT4)


void ClearlcdRAM(void);
void disp_border(void);
void disp_bmp(void);
void Lcd_Byte_Wr(unsigned char byte,unsigned char cmd);
unsigned char Lcd_Init(void);
void disp_dat(unsigned char uc_dat1,unsigned char uc_dat2,unsigned int Is_Twice);
void PrintGB(unsigned char x,unsigned char y,unsigned char *pstr);
void PrintASCII(unsigned char x,unsigned char y,unsigned char *pstr);
#endif /* ST7565R_H_ */
