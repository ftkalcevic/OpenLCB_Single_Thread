#include "main.h"
#include "Can.h"
#include "stm32f4xx_hal_can.h"


void Can::init()    // initialization
{
	// Not much to do.  Hardware init is done in main.c by STM32CubeMX generated initialisation code.
	HAL_CAN_Start(&hcan2);
}

uint8_t Can::avail()                // read rxbuffer available
{
	return HAL_CAN_GetRxFifoFillLevel(&hcan2,CAN_RX_FIFO1);
}

uint8_t Can::read()                 // read a buffer
{
	CAN_RxHeaderTypeDef Header;
	uint8_t Data[8];
	
	// TODO extract message back into members
	
	if ( HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO1, &Header, Data) == HAL_OK )
		return 1;
	else
		return 0;
}

uint8_t Can::txReady()              // write txbuffer available
{
	return HAL_CAN_GetTxMailboxesFreeLevel(&hcan2);
}

uint8_t Can::write(long timeout)    // write, 0= immediately or fail; 0< if timeout occurs fail
{
	for (;;)
	{
		if (txReady())
			return write();
		if (timeout == 0)
			return 0;
		delay(1);
		timeout--;
	}
}

uint8_t Can::write()                // write(0), ie write immediately
{
	CAN_TxHeaderTypeDef Header;
	uint32_t TxMailbox;

	Header.DLC = this->length;		// write DLC (Data Length Code)
	Header.ExtId = this->id;
	Header.IDE = CAN_ID_EXT;
	Header.RTR = CAN_RTR_DATA;
	Header.TransmitGlobalTime = DISABLE;

	if (HAL_CAN_AddTxMessage(&hcan2, &Header, this->data, &TxMailbox) == HAL_OK)
		return 1;
	else
		return 0;
}

void Can::setL(uint16_t l)
{
	this->length = l;
}

