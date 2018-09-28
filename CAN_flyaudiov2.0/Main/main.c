#include "can_simple_tx_rx.h"


extern uint8_t menu[];
extern CAN_MSG_Type RxMsgbuf[];
extern uint8_t count;
 CAN_MSG_Type TxMsg;
 CAN_MSG_Type RxMsg;
__IO Bool RxFlg =  FALSE;
__IO Bool TxFlg =  FALSE;


int main(void) 
{ 
	  uint8_t i,status1;
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

    _DBG_("CAN init\n\r");
    
    //Initialize CAN
    CAN_Init(LPC_CAN, 500000);
	
    //Enable Interrupt
    CAN_IRQCmd(LPC_CAN, CANINT_RIE, ENABLE);
	  CAN_IRQCmd(LPC_CAN, CANINT_TIE1, ENABLE);
  
    //Enable CAN Interrupt
    NVIC_EnableIRQ(CAN_IRQn);
     
		// Set AF Mode
    CAN_SetAFMode(LPC_CANAF, CAN_AccBP);
		
    
		CAN_InitRXMessage();     
		
		
    while (1)                                         
    {
		
			for(i=0;i<13;i++)
			{
					 PrintMessage_simple((CAN_MSG_Type*)&RxMsgbuf[i]);
			}
			

//        if(RxFlg)
//        {
//         
//           RxFlg = FALSE;
//           PrintMessage_simple((CAN_MSG_Type*)&RxMsgbuf[count]);
//        }
				
//     	 if(_DG=='1')
//			 {
//					status1=CAN_SendMsg(LPC_CAN,&TxMsg);
//				  if(status1==SUCCESS)
//					{
//						 _DBG_("send message success\n\r");
//					}
//				  
//			 }

				 
       // CLKPWR_Sleep();                                    // Enter normal sleep mode
    }
}

