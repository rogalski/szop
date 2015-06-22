/*****************************************************************************/
/*!
 *\MODULE              End Device
 *
 *\COMPONENT           $HeadURL: http://svn/apps/Application_Notes/JN-AN-1085-JenNet-Tutorial/Tags/Release_1v4-Public/Step5_EndDevice/Source/EndDevice.c $
 *
 *\VERSION			   $Revision: 5394 $
 *
 *\REVISION            $Id: EndDevice.c 5394 2010-02-15 14:15:22Z mlook $
 *
 *\DATED               $Date: 2010-02-15 14:15:22 +0000 (Mon, 15 Feb 2010) $
 *
 *\AUTHOR              $Author: mlook $
 *
 *\DESCRIPTION         End Device - implementation.
 */
/*****************************************************************************
 *
 * This software is owned by Jennic and/or its supplier and is protected
 * under applicable copyright laws. All rights are reserved. We grant You,
 * and any third parties, a license to use this software solely and
 * exclusively on Jennic products. You, and any third parties must reproduce
 * the copyright and warranty notice and any other legend of ownership on each
 * copy or partial copy of the software.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". JENNIC MAKES NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
 * ACCURACY OR LACK OF NEGLIGENCE. JENNIC SHALL NOT, IN ANY CIRCUMSTANCES,
 * BE LIABLE FOR ANY DAMAGES, INCLUDING, BUT NOT LIMITED TO, SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER.
 *
 * Copyright Jennic Ltd 2010. All rights reserved
 *
 ****************************************************************************/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h> /* Standard Jennic type definitions */
#include <Jenie.h>	 /* Jenie API definitions and interface */
#include <Printf.h>	 /* Basic Printf to UART0-19200-8-NP-1 {v2} */
#include "App.h"	 /* Application definitions and interface */

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: vJenie_CbConfigureNetwork
 *
 * DESCRIPTION:
 * Entry point for application from boot loader.
 *
 * RETURNS:
 * Nothing
 *
 ****************************************************************************/
PUBLIC void vJenie_CbConfigureNetwork(void) {
	/* Change default network config */
	gJenie_NetworkApplicationID = APPLICATION_ID;
	gJenie_PanID = PAN_ID;
	gJenie_Channel = CHANNEL;
	gJenie_ScanChannels = SCAN_CHANNELS;

	/* Poll every half a second (in 100ms intervals) */
	gJenie_EndDevicePollPeriod = 5;
	/* Sleep for one second between scans (in 1ms intervals) */
	gJenie_EndDeviceScanSleep = 1000;

	/* Open UART for printf use */
	vUART_printInit();
	/* Output function call to UART */
	vPrintf("\nvJenie_CbConfigureNetwork()\n");
}

PUBLIC void vTaskLoop_5s(void) {

	sendInfo();

}

/****************************************************************************
 *
 * NAME: vJenie_CbInit
 *
 * DESCRIPTION:
 * Initialisation of system.
 *
 * RETURNS:
 * Nothing
 *
 ****************************************************************************/
PUBLIC void vJenie_CbInit(bool_t bWarmStart) {
	teJenieStatusCode eStatus; /* Jenie status code */

	/* Warm start - reopen UART for printf use */
	if (bWarmStart)
		vUART_printInit();
	/* Output function call to UART */
	vPrintf("vJenie_CbInit(%d)\n", bWarmStart);

	/* Initialise application, end device {v5} */
	vApp_CbInit(bWarmStart);

	/* Start Jenie */
	eStatus = eJenie_Start(E_JENIE_END_DEVICE);
	/* Output function call to UART */
	vPrintf("eJenie_Start(END_DEVICE) = %d\n", eStatus);
}

/****************************************************************************
 *
 * NAME: vJenie_CbMain
 *
 * DESCRIPTION:
 * Main user routine. This is called by the Basic Operating System (BOS)
 * at regular intervals.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vJenie_CbMain(void) {
	//if ( (! ((u32Licznik+1) %  100))){
	//}
	vTaskLoop_5s();
	vApp_CbMain();
}
/****************************************************************************
 *
 * NAME: vJenie_CbStackMgmtEvent
 *
 * DESCRIPTION:
 * Used to receive stack management events
 *
 * PARAMETERS:      Name            		RW  Usage
 *					*psStackMgmtEvent		R	Pointer to event structure
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vJenie_CbStackMgmtEvent(teEventType eEventType, void *pvEventPrim) {
	/* Pass event on to application */
	vApp_CbStackMgmtEvent(eEventType, pvEventPrim);
}

/****************************************************************************
 *
 * NAME: vJenie_CbStackDataEvent
 *
 * DESCRIPTION:
 * Used to receive stack data events
 *
 * PARAMETERS:      Name            		RW  Usage
 *					*psStackDataEvent		R	Pointer to data structure
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vJenie_CbStackDataEvent(teEventType eEventType, void *pvEventPrim) {
	/* Pass event on to application */
	vApp_CbStackDataEvent(eEventType, pvEventPrim);
}

/****************************************************************************
 *
 * NAME: vJenie_CbHwEvent
 *
 * DESCRIPTION:
 * Adds events to the hardware event queue.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  u32Device       R   Peripheral responsible for interrupt e.g DIO
 *					u32ItemBitmap	R   Source of interrupt e.g. DIO bit map
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vJenie_CbHwEvent(uint32 u32DeviceId, uint32 u32ItemBitmap) {
	/* Pass event on to application */
	vApp_CbHwEvent(u32DeviceId, u32ItemBitmap);
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
