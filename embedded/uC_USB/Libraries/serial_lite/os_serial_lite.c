/*
 * os_serial_lite.c
 *
 *  Created on: 27 Nov 2012
 *      Author: es
 */

#include <os_serial_lite.h>


int os_serial_pincfg(uint8_t dev) {

	PINSEL_CFG_Type PINSEL_InitStruct;

	if(dev == 0) {

		PINSEL_InitStruct.Funcnum=PINSEL_FUNC_1;/*alternative function*/
		PINSEL_InitStruct.OpenDrain=PINSEL_PINMODE_NORMAL;
		PINSEL_InitStruct.Pinmode=PINSEL_PINMODE_TRISTATE;
		PINSEL_InitStruct.Pinnum=PINSEL_PIN_2; /*UART0 TXD*/
		PINSEL_InitStruct.Portnum=PINSEL_PORT_0;

		PINSEL_ConfigPin(&PINSEL_InitStruct);

		PINSEL_InitStruct.Pinnum=PINSEL_PIN_3; /*UART0 RXD*/

		PINSEL_ConfigPin(&PINSEL_InitStruct);

		return 0;

	} else if (dev == 1) {

		PINSEL_InitStruct.Funcnum=PINSEL_FUNC_1;/*alternative function*/
		PINSEL_InitStruct.OpenDrain=PINSEL_PINMODE_NORMAL;
		PINSEL_InitStruct.Pinmode=PINSEL_PINMODE_TRISTATE;
		PINSEL_InitStruct.Pinnum=PINSEL_PIN_15; /*UART1 TXD*/
		PINSEL_InitStruct.Portnum=PINSEL_PORT_0;

		PINSEL_ConfigPin(&PINSEL_InitStruct);

		PINSEL_InitStruct.Pinnum=PINSEL_PIN_16; /*UART1 RXD*/

		PINSEL_ConfigPin(&PINSEL_InitStruct);


		return 0;
	} else {

		return -1;
	}

}

int os_serial_lite_tx_enable(uint8_t dev) {

	int ret = 0;

	switch(dev) {

	case 0:
		LPC_UART0->TER |= (1<<7);
		ret = 0;
		break;
	case 1:
		LPC_UART1->TER |= (1<<7);
		ret = 0;
		break;

	default:
		ret = -1;
		break;
	};

	return ret;
}

int os_serial_lite_init(uint8_t dev, UART_CFG_Type *cfg) {

	UART_CFG_Type uart_cfg;
	int ret = 0;;

	if((cfg != NULL) && (dev != 0)) {
		/*user configuration*/
		(void)memcpy(&uart_cfg,cfg,sizeof(UART_CFG_Type));
	} else {
		/*default configuration*/
		uart_cfg.Baud_rate = 115200;
		uart_cfg.Parity = UART_PARITY_NONE;
		uart_cfg.Stopbits = UART_STOPBIT_1;
		uart_cfg.Databits = UART_DATABIT_8;

	}

	switch(dev) {

	case 0:
		os_serial_pincfg(0);
		UART_Init(LPC_UART0, &uart_cfg);
		os_serial_lite_tx_enable(0);
#ifdef _INTERRUPT_DRIVEN
		CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN,
				                   (CSP_DEV_NBR   )CSP_INT_SRC_NBR_UART_00,
				                   (CPU_FNCT_PTR  )UART_Interrupt,
				                   (void         *)&com_context);
		CSP_IntEn(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_UART_00);/*enable uart interrupt*/
#endif /*_INTERRUPT_DRIVEN*/
		ret = 0;
		break;

	case 1:
		os_serial_pincfg(1);
		UART_Init((LPC_UART_TypeDef *)LPC_UART1, &uart_cfg);
		os_serial_lite_tx_enable(1);
#ifdef _INTERRUPT_DRIVEN
		CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN,
						                   (CSP_DEV_NBR   )CSP_INT_SRC_NBR_UART_01,
						                   (CPU_FNCT_PTR  )UART_Interrupt,
						                   (void         *)&com_context);
		CSP_IntEn(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_UART_01);/*enable uart interrupt*/
#endif /*_INTERRUPT_DRIVEN*/
		ret = 0;
		break;

	default:
		ret = -1;
		break;

	};

	return ret;

}

int os_serial_sync_send(uint8_t dev, char *buf, uint16_t len) {

	/*hand over pointers to communication structure*/
	com_context.tx_i = 0;
	com_context.tx_ptr = buf;
	com_context.tx_len = len;

	/*start transmission*/
	if(dev == 0) {
		if(com_context.tx_len > LPC1343_BUFMAX)
			com_context.tx_len = LPC1343_BUFMAX;

			while(com_context.tx_i < com_context.tx_len) {
				while(UART_CheckBusy(LPC_UART0));
				UART_SendByte(LPC_UART0,(uint8_t)com_context.tx_ptr[com_context.tx_i]);
				com_context.tx_i++;
			}

		} else if(dev == 1) {

			while(com_context.tx_i < com_context.tx_len) {
				while(UART_CheckBusy((LPC_UART_TypeDef *)LPC_UART1));
				UART_SendByte((LPC_UART_TypeDef *)LPC_UART1,(uint8_t)com_context.tx_ptr[com_context.tx_i]);
				com_context.tx_i++;
			}

		} else {
			return -2;
		}

		return 0;

}

int os_serial_async_send(uint8_t dev, char *buf, uint16_t len) {

#ifdef _INTERRUPT_DRIVEN
	/*hand over pointers to communication structure*/
	com_context.tx_i = 0;
	com_context.tx_ptr = buf;
	com_context.tx_len = len;

	/*start transmission*/
	if(dev == 0) {
		if(com_context.tx_len > LPC1343_BUFMAX)
			com_context.tx_len = LPC1343_BUFMAX;

		while(UART_CheckBusy(LPC_UART0));
		UART_SendByte(LPC_UART0,(uint8_t)com_context.tx_ptr[com_context.tx_i]);
		com_context.tx_i++;
		UART_IntConfig(LPC_UART0,UART_INTCFG_THRE,ENABLE); /*enable transmit buffer empty interrupt*/

	} else if(dev == 1) {

		while(UART_CheckBusy((LPC_UART_TypeDef *)LPC_UART1));
		UART_SendByte((LPC_UART_TypeDef *)LPC_UART1,(uint8_t)com_context.tx_ptr[com_context.tx_i]);
		com_context.tx_i++;
		UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1,UART_INTCFG_THRE,ENABLE);/*enable transmit buffer empty interrupt*/

	} else {
		return -2;
	}

#endif /*_INTERRUPT_DRIVEN*/

	return 0;
}


static void  UART_Interrupt(void *p_arg) {

	uart_context_t *cont = (uart_context_t *)p_arg;
	uint32_t reg0,reg1;

	/* which uart triggered the interrupt?
	 *
	 * bit0 in IIR register is 0 if an interrupt occured on the device
	 * */
	reg0 = UART_GetIntId(LPC_UART0);
	reg1 = UART_GetIntId((LPC_UART_TypeDef *)LPC_UART1);

	if(!(reg0 & 0x01)) {
		//reg = UART_GetIntId(LPC_UART0);//read from IIR acknowledges interrupt
		//check if this really is a THRE interrupt
		if(reg0 & ((1<<1) & ~(1<<2)) ) {
			//write next byte of uart
			if(cont->tx_i < cont->tx_len) {
				UART_SendByte(LPC_UART0,(uint8_t)cont->tx_ptr[cont->tx_i]);
				cont->tx_i++;
			} else {
				/*stop transmission*/
				UART_IntConfig(LPC_UART0,UART_INTCFG_THRE,DISABLE);
			}

		} /*end THRE check*/

	} else if(!(reg1 & 0x01)) {
		//reg = UART_GetIntId((LPC_UART_TypeDef *)LPC_UART1);
		//check if this really is a THRE interrupt
		if(reg1 & ((1<<1) & ~(1<<2)) ) {
			//write next byte of uart
			if(cont->tx_i < cont->tx_len) {
				UART_SendByte((LPC_UART_TypeDef *)LPC_UART1,(uint8_t)cont->tx_ptr[cont->tx_i]);
				cont->tx_i++;
			} else {
				/*stop transmission*/
				UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1,UART_INTCFG_THRE,DISABLE);
			}
		} /*end THRE check*/
	}


    CPU_IntSrcEn(CPU_INT_SYSTICK);                              /* Enable the SYSTICK interrupt. Resume OS operation.   */
}
