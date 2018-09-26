#include "can_simple_tx_rx.h"


extern uint8_t menu[];
extern __IO CAN_MSG_Type RxMsgbuf[];
extern uint8_t count;
__IO CAN_MSG_Type TxMsg;
__IO CAN_MSG_Type RxMsg;
__IO Bool RxFlg =  FALSE;
__IO Bool TxFlg =  FALSE;


int main(void) 
{ 
    uint8_t data_cnt = 0;
    PINSEL_CFG_Type PinCfg;
  
    debug_frmwrk_init();
    print_menu(menu);

    /* Pin configuration
     * CAN1: select P0.0 as RD1. P0.1 as TD1
     */

    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Pinnum = 0;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 1;
    PINSEL_ConfigPin(&PinCfg);
	
	  PinCfg.Pinnum = 7;
    PinCfg.Portnum = 2;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 8;
    PINSEL_ConfigPin(&PinCfg);

    _DBG_("CAN init4\n\r");
    
    //Initialize CAN
    CAN_Init(LPC_CAN, 500000);
    CAN_Init(LPC_CAN2, 125000);
		
    //Enable Interrupt
    CAN_IRQCmd(LPC_CAN, CANINT_RIE, ENABLE);
    CAN_IRQCmd(LPC_CAN2, CANINT_RIE, ENABLE);
   
    //Enable CAN Interrupt
    NVIC_EnableIRQ(CAN_IRQn);
     
		// Set AF Mode
    CAN_SetAFMode(LPC_CANAF, CAN_Normal);
		
		 // Set up AF Look-up Table
    if(CAN_SetupAFTable() != CAN_OK)
    {
        _DBG_("Setup AF Look-up Table ERROR!!!");
        Error_Loop();
    }
    
    
		CAN_InitRXMessage();     
		
	  TxMsg.id = TX_MSG_ID;
    TxMsg.len = 8;
    TxMsg.format = STD_ID_FORMAT;
    TxMsg.type = DATA_FRAME;
    TxMsg.dataA[0] = TxMsg.dataA[1] = TxMsg.dataA[2] = TxMsg.dataA[3] = data_cnt++;
    TxMsg.dataB[0] = TxMsg.dataB[1] = TxMsg.dataB[2] = TxMsg.dataB[3] = data_cnt++;
    _DBG_("Send a message...");
    PrintMessage((CAN_MSG_Type*)&TxMsg);
    CAN_SendMsg(LPC_CAN2,(CAN_MSG_Type*)&TxMsg);
		

    while (1)                                         
    {
     
        if(RxFlg)
        {
         
           RxFlg = FALSE;
           PrintMessage((CAN_MSG_Type*)&RxMsgbuf[count]);
        }
				 if(TxFlg)
        {
            TxFlg=FALSE;
            TxMsg.dataA[0] = TxMsg.dataA[1] = TxMsg.dataA[2] = TxMsg.dataA[3] = data_cnt++;
            TxMsg.dataB[0] = TxMsg.dataB[1] = TxMsg.dataB[2] = TxMsg.dataB[3] = data_cnt++;
            _DBG_("Send a message...");
            PrintMessage((CAN_MSG_Type*)&TxMsg);
            CAN_SendMsg(LPC_CAN2,(CAN_MSG_Type*)&TxMsg);
        }
     
       // CLKPWR_Sleep();                                    // Enter normal sleep mode
    }
}

