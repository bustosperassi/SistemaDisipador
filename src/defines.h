#ifndef DEFINES_H_
#define DEFINES_H_

//==========Definiciones===================
#define eint0  	(uint8_t) 		0
#define eint1  (uint8_t) 		1
#define eint2  (uint8_t) 		2
#define eint3  (uint8_t) 		3
#define puerto_eint0  (uint8_t) 20
#define puerto_eint1  (uint8_t) 22
#define puerto_eint2  (uint8_t) 24
#define puerto_eint3  (uint8_t) 26
#define timer0 		(uint8_t) 	0
#define timer1 		(uint8_t) 	1
#define timer2 		(uint8_t) 	2
#define timer3 		(uint8_t) 	3
#define tamanio		(uint8_t)	20
#define prescaler1	(uint8_t)	0
#define match1		(uint32_t)  25000000
#define ticks		(uint32_t)	100000
#define ADC_POWERON 			(1 << 12)
#define SELECT_ADC0 			(0x1<<14)
#define START_ADC 				(1<<24)
#define MODE_BURTS				(1<<16)
#define PUERTO_ANALOGICO		(1<<15)
#define DIV_VEL					(0XFF<<8)
#define OPERATIONAL_ADC 		(1 << 21)
#define SEL_AD0 				(1 <<0)
#define puerto_cooler			(1<<30)

#endif /* DEFINES_H_ */

