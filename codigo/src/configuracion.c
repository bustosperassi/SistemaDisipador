#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "defines.h"
#endif
#include <cr_section_macros.h>

void Interrupciones (){
	NVIC_EnableIRQ(ADC_IRQn);
	NVIC_EnableIRQ(EINT2_IRQn);

}
void config_timer (num_timer, match, prescaler){
	if(num_timer==0){					//Configura Timer0
		LPC_TIM0->MR0=match;
		LPC_TIM0->PR=prescaler;			//Cargamos el valor del Prescaler Register,
		LPC_TIM0->MCR|=0b11;			//Cuando TC y MR0 son iguales:Genera interrupcion y Resetea TC
		NVIC_EnableIRQ(TIMER0_IRQn);	//Habilitamos NVIC para TIMER
		LPC_TIM0->TCR=0b10;				//Reseteamos el PC y TC
		LPC_TIM0->TCR=0b01;				//Empezamos a contar
	}
	else if (num_timer==1){				//Configura Timer1
		LPC_TIM1->MR0=match1;
		LPC_TIM1->PR=prescaler1;		//Cargamos el valor del Prescaler Register,
		LPC_TIM1->MCR|=0b11;			//Cuando TC y MR0 son iguales:Genera interrupcion y Resetea TC
		NVIC_EnableIRQ(TIMER1_IRQn);	//Habilitamos NVIC para TIMER
		LPC_TIM1->TCR=0b10;				//Reseteamos el PC y TC
		LPC_TIM1->TCR=0b01;				//Empezamos a contar
	}
	else if (num_timer==2){					//Configura Timer2
			LPC_TIM2->MR2=match;
			LPC_TIM2->PR=prescaler;			//Cargamos el valor del Prescaler Register,
			LPC_TIM2->MCR|=0b11;			//Cuando TC y MR0 son iguales:Genera interrupcion y Resetea TC
			NVIC_EnableIRQ(TIMER2_IRQn);	//Habilitamos NVIC para TIMER
			LPC_TIM2->TCR=0b10;				//Reseteamos el PC y TC
			LPC_TIM2->TCR=0b01;				//Empezamos a contar
		}
	else if (num_timer==3){					//Configura Timer3
			LPC_TIM3->MR3=match;
			LPC_TIM3->PR=prescaler;			//Cargamos el valor del Prescaler Register,
			LPC_TIM3->MCR|=0b11;			//Cuando TC y MR0 son iguales:Genera interrupcion y Resetea TC
			NVIC_EnableIRQ(TIMER3_IRQn);	//Habilitamos NVIC para TIMER
			LPC_TIM3->TCR=0b10;				//Reseteamos el PC y TC
			LPC_TIM3->TCR=0b01;				//Empezamos a contar
		}
}

void Config_ADC (){
	LPC_SC->PCONP |=  ADC_POWERON ;
	LPC_PINCON->PINSEL1	|= SELECT_ADC0;
	LPC_PINCON->PINMODE1 |= PUERTO_ANALOGICO;
	LPC_ADC->ADCR |= OPERATIONAL_ADC |DIV_VEL| MODE_BURTS | START_ADC;
	NVIC_EnableIRQ(ADC_IRQn);
}
void Delay (int mseg){	   	//Retardo con Timer0
	mseg=25000 * mseg;
	LPC_TIM0->TCR = 0x02;                /* reset timer */
	LPC_TIM0->PR  = 0x00;                /* set prescaler to zero */
	LPC_TIM0->MR0 = mseg;
	LPC_TIM0->IR  = 0xff;                /* reset all interrrupts */
	LPC_TIM0->MCR = 0x04;                /* stop timer on match */
	LPC_TIM0->TCR = 0x01;                /* start timer */
	while (LPC_TIM0->TCR & 0x01);		/* wait until Delay time has elapsed */

}

void Config_Puertos(){	//Funcion de configuracion de puertos
	LPC_GPIO2->FIODIR=0x1ff; 		// P2[7]-P2[0]=Leds
	LPC_GPIO0->FIODIR1=0xff; 		// P0[11]=Display Unidad P0[10]=Display Decena
	//Filas del teclado P0[3]-P0[2]-P0[26]-P0[25] Salidas
	int j;
	for(j=0;j<100000;j++){}
//	LPC_GPIOINT->IO0IntEnF |= ((1 << 3)|(1 << 2)|(1 << 1)|(1 << 0));
	LPC_GPIO0->FIODIR |=  ((1 << 6)|(1 << 7)|(1 << 8)|(1 << 9));
	LPC_GPIO0->FIOCLR |=  ((1 << 6)|(1 << 7)|(1 << 8)|(1 << 9));
	LPC_GPIO0->FIODIR |= (1<<26);
	LPC_GPIO1->FIODIR |= puerto_cooler;
	LPC_GPIO0->FIOCLR |= (1<<26);
	LPC_PINCON->PINSEL0|=(1<<30); //configuramos los pines P0.0 como TX y P0.1 como RX
	LPC_PINCON->PINSEL1|=(1<<0);
}

void Config_Eintx(num_int,num_puerto){	//Funcion que configura la EINT correspondiente a 'num_int'
	int inter=0;
	LPC_PINCON->PINSEL4 |= (1<<num_puerto); 	//Configuracion del puerto para interrupcion externa
	LPC_SC->EXTMODE		|=(1<<num_int);			//Seleccionamos interrupcion por flanco
	LPC_SC->EXTPOLAR 	|=(0<<num_int);			//Flanco Descendente
	inter=18 + num_int;							//EINT0_IRQn=18 External Interrupt 0 Interrupt
	Delay(100);
	NVIC_EnableIRQ(inter);
}

void Config_Uart (){
	LPC_SC->PCONP|=1<<25;	//Encendemos el periférico
	LPC_UART1->LCR|=11;		// 8 bits
	LPC_UART1->LCR|=(1<<7); //Activamos BaudRate
	/*Configuraremos DLL y DLM para obtener un Baud Rate de 9600 baudios para esto tenemos */
	//							   Pclk
	//Baudrate   =    ----------------------------------
	//						16 X (256*DLM+DLL)
	//
	//no haremos caso al divisor de frecuencia fraccional dado que el error que cometemos no es significativo
	LPC_UART1->DLL=0b10100001; //DLL = 161 -> 9585 baudios -> error de 14 -> 0.1458%
	LPC_UART1->DLM=0;
	/*Colocamos el Bit DLAB nuevamente a cero para que esto nos impide el acceso a DLL y DLM pero nos permite acceder a RBR, THR e IER*/
	LPC_UART1->LCR &= 0b01111111;
	LPC_UART1->IER=1; // Corresponde a la interrupción por Receive Data Available
	NVIC_EnableIRQ(UART1_IRQn);
	LPC_PINCON->PINSEL0 |=1<<30; //TXD1
	LPC_PINCON->PINSEL1 |=1<<0;	 //RXD1
}

int Calc_Moda (int *vector){
	int i,j,k=1,p,frec[20],cnt=1,big;
	for(i=1; i<=tamanio; i++)	{
	   for(j=i+1;j<=tamanio; j++)	{
		   	   if(vector[i]==vector[j])
		   		cnt++;
	}
	frec[k]=cnt;
	k++;
	cnt=1;
	}
	big=frec[1];
	p=1;
	for(i=2; i<=tamanio; i++)	{
		if(big<frec[i]){
		big=frec[i];
		p=i;
		}
	}
	return vector[p];
}
void Enviar (int temp)
{
	while((LPC_UART1->LSR&(1<<5))==0){}
	LPC_UART1->THR=temp;
}

void Cooler_ON (){ LPC_GPIO1->FIOSET |= puerto_cooler;}
void Cooler_OFF(){ LPC_GPIO1->FIOCLR |= puerto_cooler;}
