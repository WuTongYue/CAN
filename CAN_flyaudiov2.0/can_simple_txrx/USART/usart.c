#include "usart.h"

// UART Ring buffer
UART_RING_BUFFER_T rb;
extern CAN_MSG_Type TxMsg;

/********************************************************************//**
 * @brief       UART receive function (ring buffer used)
 * @param[in]   None
 * @return      None
 *********************************************************************/
void UART_IntReceive(void)
{
    uint8_t tmpc,status;
    uint32_t rLen;

    while(1){
        // Call UART read function in UART driver
        rLen = UART_Receive((LPC_UART_TypeDef *)LPC_UART3, &tmpc, 1, NONE_BLOCKING);
        // If data received
        if (rLen){
            /* Check if buffer is more space
             * If no more space, remaining character will be trimmed out
             */
            if (!__BUF_IS_FULL(rb.rx_head,rb.rx_tail)){
                rb.rx[rb.rx_head] = tmpc;
                __BUF_INCR(rb.rx_head);
            }
						if(tmpc=='1')
						{
							status=CAN_SendMsg(LPC_CAN,&TxMsg);
							if(status==SUCCESS)
							{
							  _DBG("send a message");
							}
						}
        }
        // no more data
        else {
            break;
        }
    }
}



/*********************************************************************//**
 * @brief       UART Line Status Error
 * @param[in]   bLSErrType  UART Line Status Error Type
 * @return      None
 **********************************************************************/
void UART_IntErr(uint8_t bLSErrType)
{
    while(1);
}


/*********************************************************************//**
 * @brief       UART3 interrupt handler sub-routine
 * @param[in]   None
 * @return      None
 **********************************************************************/
void UART3_IRQHandler(void)
{
    uint32_t intsrc, tmp, tmp1;

    /* Determine the interrupt source */
    intsrc = UART_GetIntId(DEBUG_UART_PORT);
    tmp = intsrc & UART_IIR_INTID_MASK;

    // Receive Line Status
    if (tmp == UART_IIR_INTID_RLS){
        // Check line status
        tmp1 = UART_GetLineStatus(DEBUG_UART_PORT);
        // Mask out the Receive Ready and Transmit Holding empty status
        tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
                | UART_LSR_BI | UART_LSR_RXFE);
        // If any error exist
        if (tmp1) {
                UART_IntErr(tmp1);
        }
    }

    // Receive Data Available or Character time-out
    if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)){
            UART_IntReceive();
    }

   

}

