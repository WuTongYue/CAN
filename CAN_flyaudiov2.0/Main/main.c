#include "can_simple_tx_rx.h"


extern uint8_t menu[];
extern __IO CAN_MSG_Type RxMsgbuf[];
extern uint8_t count;
 CAN_MSG_Type TxMsg;
 CAN_MSG_Type RxMsg;
__IO Bool RxFlg =  FALSE;
__IO Bool TxFlg =  FALSE;


int main(void) 
{ 
    uint8_t status;
    PINSEL_CFG_Type PinCfg;
  
 //  	SystemInit();
	
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

    _DBG_("CAN init5\n\r");
    
    //Initialize CAN
    CAN_Init(LPC_CAN, 1000000);
	
    //Enable Interrupt
    CAN_IRQCmd(LPC_CAN, CANINT_RIE, ENABLE);
 
   
    //Enable CAN Interrupt
    NVIC_EnableIRQ(CAN_IRQn);
     
		// Set AF Mode
    CAN_SetAFMode(LPC_CANAF, CAN_AccBP);
		
		CAN_ModeConfig(LPC_CAN,CAN_OPERATING_MODE,ENABLE);
    
		CAN_InitRXMessage();     
		
		
    while (1)                                         
    {
       status=CAN_SendMsg(LPC_CAN,&TxMsg);
			if(status==SUCCESS )
			{
					 _DBG_("CAN send Message success!");
			}
        if(RxFlg)
        {
         
           RxFlg = FALSE;
           PrintMessage((CAN_MSG_Type*)&RxMsgbuf[count]);
        }
				 
       // CLKPWR_Sleep();                                    // Enter normal sleep mode
    }
}

