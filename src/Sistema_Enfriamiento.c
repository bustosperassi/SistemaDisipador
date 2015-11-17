#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "defines.h"
#endif
#include <cr_section_macros.h>

extern void Config_Puertos 	(void);
extern void config_timer 	(int, int, int);
extern void Config_ADC		(void);
extern void config_dac 		(void);
extern void Config_Eintx	(int, int);
extern void Interrupciones 	(void);
extern void Config_Uart 	(void);
extern int 	Calc_Moda 		(int[]);
extern void Delay			(int);
extern void Enviar			(int);
extern void Cooler_ON	 	(void);
extern void Cooler_OFF	 	(void);

int fila=0;
int columna=1;
int numero=0;
int tempAux=0;
int tempSen=0;
int tempSet=88;
int tempTemp=88;
int index=0;
int valorADC=0;
int selector=0;
int habTec=0;
int decena=0;
int decSet=0;
int uniSet=0;
int botonPanic=0;
int datoRecibido=0;
int vector[tamanio]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t segmentos[]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xe7};

int main(void) {
	Config_Puertos();
	Interrupciones();
	SysTick_Config(ticks);
	Config_Eintx(eint2, puerto_eint2);
	Config_Uart();
	Config_ADC();
	config_timer(timer1, match1, prescaler1);
	while(1) {
		if ((botonPanic==1) | (tempSen > tempTemp)) Cooler_ON();
		else if (tempSen < (tempTemp-2)) Cooler_OFF();
			}
	return 0;
}

void SysTick_Handler(void){ 	//Encargado de multiplexar los display a una frecuencia de 10 ms
	LPC_GPIO2->FIOCLR0 |= (0xFF);
	LPC_GPIO0->FIOCLR = 1<<10;
	LPC_GPIO0->FIOCLR = 1<<11;
	if (selector==0){				//Variable selector=0 muestra Unidad, sino muestra decena
		LPC_GPIO0->FIOSET = 1<<11;	//Prende puerto P0[11]
		if(habTec) LPC_GPIO2->FIOPIN0 = segmentos[tempSet/10];
		else 		LPC_GPIO2->FIOPIN0 = segmentos[tempSen/10];	//Mandamos a mostrar la decena
		selector++;
	}else{							//Decena
		LPC_GPIO0->FIOSET = 1<<10;	//Prende puerto P0[10]
		if(habTec) LPC_GPIO2->FIOPIN0 = segmentos[tempSet%10];
		else		LPC_GPIO2->FIOPIN0 = segmentos[tempSen%10];	//Mandamos a mostrar la unidad
		selector=0;
	}
}
void TIMER1_IRQHandler (void){
	tempSen =tempAux;
	LPC_TIM1->IR=1;

}
void ADC_IRQHandler (void){
	valorADC = ((LPC_ADC->ADDR0 >>8) & 0xFF);
	valorADC = (valorADC * 1.2)+3;
	if(index<tamanio){
				vector[index]=valorADC;
				index++;}
		else {	index=0;
	tempAux = Calc_Moda(vector);}
	LPC_ADC->ADCR |= (1<<21);
}

void EINT2_IRQHandler (void){
	LPC_GPIOINT->IO0IntClr |= ((1 << 3)|(1 << 2)|(1 << 1)|(1 << 0));
	LPC_GPIO0->FIOCLR |= ((1 << 3)|(1 << 2)|(1 << 1)|(1 << 0));
	if(habTec==0){
		LPC_GPIOINT->IO0IntEnF |= ((1 << 3)|(1 << 2)|(1 << 1)|(1 << 0));
		NVIC_EnableIRQ(EINT3_IRQn);
		habTec=1;
	}else{
		LPC_GPIOINT->IO0IntEnF &= ~((1 << 3)|(1 << 2)|(1 << 1)|(1 << 0));
		NVIC_DisableIRQ(EINT3_IRQn);
		habTec=0;
		tempTemp= tempSet;
	}
	Delay(250);
	LPC_GPIOINT->IO0IntClr |= ((1 << 3)|(1 << 2)|(1 << 1)|(1 << 0));
	LPC_SC->EXTINT|=(1<<2);
}

void EINT3_IRQHandler (void){
	int k=0;
	int i=0;
	for(i=0;i<4;i++){
		if((LPC_GPIOINT->IO0IntStatF&(1<<i))){
			if(((LPC_GPIO0->FIOPIN&(1<<i))>>i)==0)fila=i;
		}
	}
	for(k=0;k<4;k++){
		LPC_GPIO0->FIOSET|= ((1 << 6)|(1 << 7)|(1 << 8)|(1 << 9));
		LPC_GPIO0->FIOCLR|=(1<<(k+6));
		if(((LPC_GPIO0->FIOPIN&(1<<fila))>>fila)==0) columna=k;
	}
	numero= ((3*( fila))+ columna)+1;
	if (numero == 11)numero=0;

	decSet=uniSet;
	uniSet=numero;

	tempSet=(10*decSet)+uniSet;
	Delay(200);
	LPC_GPIO0->FIOCLR |=  ((1 << 6)|(1 << 7)|(1 << 8)|(1 << 9));
	LPC_GPIOINT->IO0IntClr |= ((1 << 3)|(1 << 2)|(1 << 1)|(1 << 0));
}

void UART1_IRQHandler(void){
	datoRecibido=LPC_UART1->RBR;
	switch (datoRecibido){
		case 49: botonPanic=1;
		break;
		case 50: Enviar(tempSen);
		break;
		case 51: Enviar(tempSet);
		break;
		default: botonPanic=0;
	}
}
