#ifndef __CAN_SIMPLE_TX_RX_H
#define __CAN_SIMPLE_TX_RX_H

#include "lpc_types.h"
#include "lpc17xx_can.h"
#include "lpc17xx_libcfg.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_clkpwr.h"
#include "debug_frmwrk.h"

/************************** PRIVATE DEFINTIONS*************************/
#define MCB_LPC_1768
#define LPC_CAN             LPC_CAN1
#define RX_MSG_ID            0x100
#define RX_EXT_MSG_ID        0x00100000

#define TX_MSG_ID            (0x200)
#define TX_EXT_MSG_ID        0x00200000





void print_menu(uint8_t* menu);
void Error_Loop(void);
void PrintMessage(CAN_MSG_Type* CAN_Msg);
void PrintMessage_simple(CAN_MSG_Type* CAN_Msg);
CAN_ERROR CAN_SetupAFTable(void);
void CAN_InitRXMessage(void);



#endif


