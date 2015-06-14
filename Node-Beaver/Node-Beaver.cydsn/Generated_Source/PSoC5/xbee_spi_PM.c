/*******************************************************************************
* File Name: xbee_spi_PM.c
* Version 2.50
*
* Description:
*  This file contains the setup, control and status commands to support
*  component operations in low power mode.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "xbee_spi_PVT.h"

static xbee_spi_BACKUP_STRUCT xbee_spi_backup =
{
    xbee_spi_DISABLED,
    xbee_spi_BITCTR_INIT,
};


/*******************************************************************************
* Function Name: xbee_spi_SaveConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void xbee_spi_SaveConfig(void) 
{

}


/*******************************************************************************
* Function Name: xbee_spi_RestoreConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void xbee_spi_RestoreConfig(void) 
{

}


/*******************************************************************************
* Function Name: xbee_spi_Sleep
********************************************************************************
*
* Summary:
*  Prepare SPIM Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  xbee_spi_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void xbee_spi_Sleep(void) 
{
    /* Save components enable state */
    xbee_spi_backup.enableState = ((uint8) xbee_spi_IS_ENABLED);

    xbee_spi_Stop();
}


/*******************************************************************************
* Function Name: xbee_spi_Wakeup
********************************************************************************
*
* Summary:
*  Prepare SPIM Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  xbee_spi_backup - used when non-retention registers are restored.
*  xbee_spi_txBufferWrite - modified every function call - resets to
*  zero.
*  xbee_spi_txBufferRead - modified every function call - resets to
*  zero.
*  xbee_spi_rxBufferWrite - modified every function call - resets to
*  zero.
*  xbee_spi_rxBufferRead - modified every function call - resets to
*  zero.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void xbee_spi_Wakeup(void) 
{
    #if(xbee_spi_RX_SOFTWARE_BUF_ENABLED)
        xbee_spi_rxBufferFull  = 0u;
        xbee_spi_rxBufferRead  = 0u;
        xbee_spi_rxBufferWrite = 0u;
    #endif /* (xbee_spi_RX_SOFTWARE_BUF_ENABLED) */

    #if(xbee_spi_TX_SOFTWARE_BUF_ENABLED)
        xbee_spi_txBufferFull  = 0u;
        xbee_spi_txBufferRead  = 0u;
        xbee_spi_txBufferWrite = 0u;
    #endif /* (xbee_spi_TX_SOFTWARE_BUF_ENABLED) */

    /* Clear any data from the RX and TX FIFO */
    xbee_spi_ClearFIFO();

    /* Restore components block enable state */
    if(0u != xbee_spi_backup.enableState)
    {
        xbee_spi_Enable();
    }
}


/* [] END OF FILE */
