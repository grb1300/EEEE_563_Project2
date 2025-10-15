/*
 * uart.c
 *
 *  Updated on: Sept 9, 2025
 *      Author: Mitesh Parikh
 */

#include <uart.h>
#include <stdarg.h>
#include <stdio.h>

// UART Ports:
// ===================================================
// PD.5 = USART2_TX (AF7)  |  PD.6 = USART2_RX (AF7)

//void USART2_Init(int baudrate)
//{
//	////////////  CONFIGURE CLOCKS	/////////////////
//	/* Init the low level hardware : GPIO, CLOCK */
//	uint32_t tmpreg;
//	(void) tmpreg;	/// avoid compiler warning
//
//	// enable USART2 CLK
//	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
//	tmpreg = RCC->APB1ENR1 & RCC_APB1ENR1_USART2EN;	// Delay after an RCC peripheral clock enabling
//
//	// enable GPIOA CLK
//	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
//	tmpreg = RCC->AHB2ENR & RCC_AHB2ENR_GPIOAEN;		// Delay after an RCC peripheral clock enabling
//
//	//////////// CONFIGURE GPIO /////////////////
//	/* Configure the IO Speed */
//	//GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED2_Msk | GPIO_OSPEEDR_OSPEED3_Msk);
//	//GPIOA->OSPEEDR |= (0x00000003u) << GPIO_OSPEEDR_OSPEED2_Pos;
//	//GPIOA->OSPEEDR |= (0x00000003u) << GPIO_OSPEEDR_OSPEED3_Pos;
//
//	GPIOA->OSPEEDR |= ((3u<<(2*2)) | (3u<<(3*2)));    // high speed
//
//	/* Configure the IO Output Type to Push-Pull */
//	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT2_Msk | GPIO_OTYPER_OT3_Msk);
//
//	/* Set the Pull-up/down  to none */
//	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD2_Msk | GPIO_PUPDR_PUPD3_Msk);
//
//	/* Configure Alternate functions */
//	GPIOA->AFR[0] &=  ~(GPIO_AFRL_AFSEL2_Msk | GPIO_AFRL_AFSEL3_Msk);
//	GPIOA->AFR[0] |=  GPIO_AF7_USART2 << GPIO_AFRL_AFSEL2_Pos;
//	GPIOA->AFR[0] |=  GPIO_AF7_USART2 << GPIO_AFRL_AFSEL3_Pos;
//
//	/* Configure IO Direction mode to Alternate */
//	GPIOA->MODER &= ~(GPIO_MODER_MODE2_Msk  | GPIO_MODER_MODE3_Msk);
//	GPIOA->MODER |= (0x00000002u) << GPIO_MODER_MODE2_Pos;
//	GPIOA->MODER |= (0x00000002u) << GPIO_MODER_MODE3_Pos;
//
//	//////////// CONFIGURE USART2 /////////////////
//	// Disable UART
//	USART2->CR1 &= ~USART_CR1_UE;
//
//	/* Set the UART Communication parameters */
//	USART2->CR1 &= ~(USART_CR1_M1_Pos | USART_CR1_PCE_Msk | USART_CR1_OVER8_Msk | USART_CR1_TE_Msk | USART_CR1_RE_Msk);
//	//USART2->CR1 |= UART_WORDLENGTH_8B | UART_PARITY_NONE | UART_OVERSAMPLING_16 | USART_CR1_TE | USART_CR1_RE;
//	USART2->CR1 &= ~(USART_CR1_M | USART_CR1_PCE | USART_CR1_OVER8); // 8 data bits, no parity, oversampling by 16
//	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;  // 8N1, oversampling by 16
//
//	/*-------------------------- USART CR2 Configuration -----------------------*/
//	/* Configure the UART Stop Bits: Set STOP[13:12] bits according to huart->Init.StopBits value */
//	USART2->CR2 &= ~USART_CR2_STOP;
//	USART2->CR2 |= UART_STOPBITS_1 ;
//
//	/*-------------------------- USART CR3 Configuration -----------------------*/
//	/* Configure
//	 * - UART HardWare Flow Control: set CTSE and RTSE bits according to HwFlowCtl value
//	 * - One-bit sampling method versus three samples' majority rule according to huart->Init.OneBitSampling */
//	USART2->CR3 &= ~(USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_ONEBIT);
//	USART2->CR3 |= UART_HWCONTROL_NONE | UART_OVERSAMPLING_16;
//
//	/*-------------------------- USART BRR Configuration -----------------------*/
//#define UART_DIV_SAMPLING16(__PCLK__, __BAUD__)  (((__PCLK__) + ((__BAUD__)/2U)) / (__BAUD__))
//	//uint32_t pclk = 16000000;		//PCLK1Freq;
//	//uint32_t usartdiv = (uint16_t)(UART_DIV_SAMPLING16(pclk, baudrate));
//	//uint32_t uartdiv = SystemCoreClock / baudrate;
//	//USART2->BRR = uartdiv;
//	uint32_t pclk1 = 80000000UL;
//	USART2->BRR = (pclk1 + (baudrate/2U)) / baudrate; // rounding
//
//
//	/* In asynchronous mode, the following bits must be kept cleared:
//	 *  - LINEN and CLKEN bits in the USART_CR2 register
//	 *  - SCEN, HDSEL and IREN  bits in the USART_CR3 register.*/
//	USART2->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
//	USART2->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);
//
//	// Enable UART
//	USART2->CR1 |= USART_CR1_UE;
//
//	while ( (USART2->ISR & USART_ISR_TEACK) == 0); // Verify that the USART is ready for reception
//	while ( (USART2->ISR & USART_ISR_REACK) == 0); // Verify that the USART is ready for transmission
//}

void USART2_Init(int baudrate)
{
    // Enable clocks
    RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
    RCC->AHB2ENR  |= RCC_AHB2ENR_GPIOAEN;

    // ----------------------------
    // Configure PA2 (USART2_TX), PA3 (USART2_RX)
    // ----------------------------
    GPIOA->MODER &= ~((3u << (2*2)) | (3u << (3*2)));
    GPIOA->MODER |=  ((2u << (2*2)) | (2u << (3*2)));  		// 10 = alternate function
    GPIOA->AFR[0] &= ~((0xFu << (2*4)) | (0xFu << (3*4)));
    GPIOA->AFR[0] |=  ((7u << (2*4)) | (7u << (3*4))); 		// AF7 = USART2
    GPIOA->OTYPER &= ~((1u<<2) | (1u<<3));             		// push-pull
    GPIOA->OSPEEDR |= ((3u << (2*2)) | (3u << (3*2))); 		// very high speed
    GPIOA->PUPDR &= ~((3u << (2*2)) | (3u << (3*2)));  		// no pull

    // Disable USART before config
    USART2->CR1 &= ~USART_CR1_UE;

    // Word length = 8, no parity, oversampling = 16
    USART2->CR1 &= ~(USART_CR1_M | USART_CR1_PCE | USART_CR1_OVER8);

    // Stop bits = 1
    USART2->CR2 &= ~USART_CR2_STOP;

    // No flow control
    USART2->CR3 &= ~(USART_CR3_RTSE | USART_CR3_CTSE);

    // MP-Sept-25: Set Baud rate --- Update for 16MHz or 80MHz
    //uint32_t pclk1 = 16000000UL; /// if HSI at 16MHz, APB1 prescaler = 1
    uint32_t pclk1 = 80000000UL;  // if PLL at 80 MHz, APB1 prescaler = 1
    USART2->BRR = (pclk1 + (baudrate/2U)) / baudrate;

    // Enable transmitter and receiver
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;

    // Enable USART
    USART2->CR1 |= USART_CR1_UE;

    while (!(USART2->ISR & USART_ISR_TEACK));
    while (!(USART2->ISR & USART_ISR_REACK));
}

uint8_t USART_Read (USART_TypeDef * USARTx)
{
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return ((uint8_t)(USARTx->RDR & 0xFF));
	// Reading USART_DR automatically clears the RXNE flag 
}

//Read from USART - Non-Blocking
uint8_t USART_Read_NB (USART_TypeDef * USARTx)
{
    // SR_RXNE (Read data register not empty) bit is set by hardware
    if (!(USARTx->ISR & USART_ISR_RXNE)) {
        // USART resets the RXNE flag automatically after reading DR
        return 0;
    }
    else {
        return ((uint8_t)(USARTx->RDR & 0xFF));
    // Reading USART_DR automatically clears the RXNE flag
    }
}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes)
{
	int i;
	// A byte to be transmitted is written to the TDR (transmit data register), and the TXE (transmit empty) bit is cleared.
	// The TDR is copied to an output shift register for serialization when that register is empty, and the TXE bit is set.
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE))
			;   							// wait until TXE (TX empty) bit is set
		USARTx->TDR = buffer[i] & 0xFF;		// writing USART_TDR automatically clears the TXE flag
	}
	while (!(USARTx->ISR & USART_ISR_TC))
		;  									// wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}   

void USART_IRQHandler(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t * pRx_counter)
{
	if(USARTx->ISR & USART_ISR_RXNE) {						// Received data
		buffer[*pRx_counter] = USARTx->RDR;         // Reading USART_DR automatically clears the RXNE flag
		(*pRx_counter)++;
		if((*pRx_counter) >= BufferSize )  {
			(*pRx_counter) = 0;
		}
	} else if(USARTx->ISR & USART_ISR_TXE) {
		//USARTx->ISR &= ~USART_ISR_TXE;            // clear interrupt
		//Tx1_Counter++;
	} else if(USARTx->ISR & USART_ISR_ORE) {			// Overrun Error
		while(1);
	} else if(USARTx->ISR & USART_ISR_PE) {				// Parity Error
		while(1);
	} else if(USARTx->ISR & USART_ISR_PE) {				// USART_ISR_FE
		while(1);
	} else if (USARTx->ISR & USART_ISR_NE){ 			// Noise Error Flag
		while(1);
	}
}

//******************************************************************************************
// USART Write functions for testing
//******************************************************************************************
void USART2_WriteChar(char c)
{
    while (!(USART2->ISR & USART_ISR_TXE)); // wait until TX buffer empty - blocking
    USART2->TDR = c;
}

void USART2_WriteString(const char *s)
{
    while (*s)
    {
        USART2_WriteChar(*s++);
    }
}

//******************************************************************************************
// For USART Print -- Could be moved to UART module
//******************************************************************************************
static uint8_t buffer[200];

int printf (const char *format, ...) {
    va_list aptr;
    int ret;

    va_start (aptr, format);
    ret = vsprintf ((char*)buffer, format, aptr);
    va_end (aptr);

    USART_Write (USART2, buffer, ret);

    return ret;
}



