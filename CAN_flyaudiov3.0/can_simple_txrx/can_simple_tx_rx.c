#include "can_simple_tx_rx.h"

uint8_t menu[]=
    "********************************************************************************\n\r"
    "Flyaudio CAN analysis \n\r"
    "\t - MCU: LPC17xx \n\r"
    "\t - Core: ARM CORTEX-M3 \n\r"
    "\t - UART Communicate via: 115200 bps \n\r"
    "use CAN to transmit and receive Message \n\r"
    "********************************************************************************\n\r";

extern __IO CAN_MSG_Type TxMsg;
extern __IO CAN_MSG_Type RxMsg;
extern __IO Bool RxFlg;
extern __IO Bool TxFlg;


/************************** PRIVATE FUNCTION *************************/
/*********************************************************************//**
 * @brief   Print menu
 * @param[in]   menu    Menu String
 * @return  None
 **********************************************************************/
void print_menu(uint8_t* menu)
{
    _DBG_(menu);
}

/*********************************************************************//**
 * @brief       Error Loop
 * @param[in]   None
 * @return      None
 **********************************************************************/
void Error_Loop(void)
{
    /*
     * Insert your code here...
     */
    while(1);
}

/*********************************************************************//**
 * @brief       Print Message via COM
 * param[in]    msg: point to CAN_MSG_Type object that will be printed
 * @return      none
 **********************************************************************/
void PrintMessage(CAN_MSG_Type* CAN_Msg)
{
    uint32_t data;

    _DBG("Message ID:     ");

    _DBH32(CAN_Msg->id);_DBG_("");

    _DBG("Message length: ");

    _DBH32(CAN_Msg->len);_DBG_(" BYTES");

    _DBG("Message type:   ");

    if(CAN_Msg->type==DATA_FRAME)
    {
        _DBG_("DATA FRAME ");
    }
    else
        _DBG_("REMOTE FRAME ");

    _DBG("Message format: ");

    if(CAN_Msg->format==STD_ID_FORMAT)
    {
        _DBG_("STANDARD ID FRAME FORMAT");
    }
    else
        _DBG_("EXTENDED ID FRAME FORMAT");

    _DBG("Message dataA:  ");

    data = (CAN_Msg->dataA[0])|(CAN_Msg->dataA[1]<<8)|(CAN_Msg->dataA[2]<<16)|(CAN_Msg->dataA[3]<<24);

    _DBH32(data);_DBG_("");

    data = (CAN_Msg->dataB[0])|(CAN_Msg->dataB[1]<<8)|(CAN_Msg->dataB[2]<<16)|(CAN_Msg->dataB[3]<<24);

    _DBG("Message dataB:  ");

    _DBH32(data);_DBG_("");

    _DBG_("");
}


/*********************************************************************//**
 * @brief       Setup Acceptance Filter Table
 * @param[in]   none
 * @return      none
 * Note:        not use Group Standard Frame, just use for Explicit
 *              Standard and Extended Frame
 **********************************************************************/
CAN_ERROR CAN_SetupAFTable(void) {
    CAN_ERROR result;
    /* Set up Explicit Standard Frame Format Identifier Section
     * In this simple test, it has 1 entry with ID of 0x200
     */
    result = CAN_LoadExplicitEntry(LPC_CAN, RX_MSG_ID, STD_ID_FORMAT);
    if(result != CAN_OK)
        return result;
    
    /* Set up Explicit Extended Frame Format Identifier Section
     * In this simple test, it has 16 entries ID
     */
    result = CAN_LoadExplicitEntry(LPC_CAN, RX_EXT_MSG_ID, EXT_ID_FORMAT);
    
    return result;
}


/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief        Event Router IRQ Handler
 * @param[in]    None
 * @return       None
 **********************************************************************/
void CAN_IRQHandler(void)
{
    uint8_t IntStatus;
    /* get interrupt status
     * Note that: Interrupt register CANICR will be reset after read.
     * So function "CAN_IntGetStatus" should be call only one time
     */
    IntStatus = CAN_IntGetStatus(LPC_CAN);

    //check receive interrupt
    if((IntStatus >> 0) & 0x01)
    {
        CAN_ReceiveMsg(LPC_CAN, (CAN_MSG_Type *)&RxMsg);
        RxFlg = TRUE;
    }
}
/*********************************************************************//**
 * @brief       External interrupt 0 handler sub-routine
 * @param[in]   None
 * @return      None
 **********************************************************************/
void EINT0_IRQHandler(void)
{
      //clear the EINT0 flag
      EXTI_ClearEXTIFlag(EXTI_EINT0);
      TxFlg = TRUE;
}

#ifdef  DEBUG
/*******************************************************************************
 * @brief        Reports the name of the source file and the source line number
 *                 where the CHECK_PARAM error has occurred.
 * @param[in]    file Pointer to the source file name
 * @param[in]    line assert_param error line source number
 * @return        None
 *******************************************************************************/
void check_failed(uint8_t *file, uint32_t line) {
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
        ;
}
#endif

/**
 * @}
 */
