#include "can_simple_tx_rx.h"


extern uint8_t menu[];
__IO CAN_MSG_Type TxMsg;
__IO CAN_MSG_Type RxMsg;
__IO Bool RxFlg =  FALSE;
__IO Bool TxFlg =  FALSE;


int main(void) 
{ 
   
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

    // Set AF Mode
    CAN_SetAFMode(LPC_CANAF, CAN_AccBP);
    
    //Enable CAN Interrupt
    NVIC_EnableIRQ(CAN_IRQn);
 
    
    while (1)                                         
    {
    
        if(RxFlg)
        {
        
           RxFlg = FALSE;
           PrintMessage((CAN_MSG_Type*)&RxMsg);
        }
     
     //   CLKPWR_Sleep();                                    // Enter normal sleep mode
    }
}

