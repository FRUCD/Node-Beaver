/*******************************************************************************
* File Name: xbee_spi_INT.c
* Version 2.40
*
* Description:
*  This file provides all Interrupt Service Routine (ISR) for the SPI Master
*  component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "xbee_spi_PVT.h"

/* User code required at start of ISR */
/* `#START xbee_spi_ISR_START_DEF` */

/* `#END` */


/*******************************************************************************
* Function Name: xbee_spi_TX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for TX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  xbee_spi_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  xbee_spi_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified when exist data to
*  sending and FIFO Not Full.
*  xbee_spi_txBuffer[xbee_spi_TX_BUFFER_SIZE] - used to store
*  data to sending.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(xbee_spi_TX_ISR)
{
    #if(xbee_spi_TX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
    #endif /* (xbee_spi_TX_SOFTWARE_BUF_ENABLED) */

    /* User code required at start of ISR */
    /* `#START xbee_spi_TX_ISR_START` */

    /* `#END` */

    #if(xbee_spi_TX_SOFTWARE_BUF_ENABLED)
        /* Check if TX data buffer is not empty and there is space in TX FIFO */
        while(xbee_spi_txBufferRead != xbee_spi_txBufferWrite)
        {
            tmpStatus = xbee_spi_GET_STATUS_TX(xbee_spi_swStatusTx);
            xbee_spi_swStatusTx = tmpStatus;

            if(0u != (xbee_spi_swStatusTx & xbee_spi_STS_TX_FIFO_NOT_FULL))
            {
                if(0u == xbee_spi_txBufferFull)
                {
                   xbee_spi_txBufferRead++;

                    if(xbee_spi_txBufferRead >= xbee_spi_TX_BUFFER_SIZE)
                    {
                        xbee_spi_txBufferRead = 0u;
                    }
                }
                else
                {
                    xbee_spi_txBufferFull = 0u;
                }

                /* Move data from the Buffer to the FIFO */
                CY_SET_REG8(xbee_spi_TXDATA_PTR,
                    xbee_spi_txBuffer[xbee_spi_txBufferRead]);
            }
            else
            {
                break;
            }
        }

        if(xbee_spi_txBufferRead == xbee_spi_txBufferWrite)
        {
            /* TX Buffer is EMPTY: disable interrupt on TX NOT FULL */
            xbee_spi_TX_STATUS_MASK_REG &= ((uint8) ~xbee_spi_STS_TX_FIFO_NOT_FULL);
        }

    #endif /* (xbee_spi_TX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START xbee_spi_TX_ISR_END` */

    /* `#END` */
}


/*******************************************************************************
* Function Name: xbee_spi_RX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for RX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  xbee_spi_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer modified when FIFO contains
*  new data.
*  xbee_spi_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified when overflow occurred.
*  xbee_spi_rxBuffer[xbee_spi_RX_BUFFER_SIZE] - used to store
*  received data, modified when FIFO contains new data.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(xbee_spi_RX_ISR)
{
    #if(xbee_spi_RX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
        uint8 rxData;
    #endif /* (xbee_spi_RX_SOFTWARE_BUF_ENABLED) */

    /* User code required at start of ISR */
    /* `#START xbee_spi_RX_ISR_START` */

    /* `#END` */

    #if(xbee_spi_RX_SOFTWARE_BUF_ENABLED)

        tmpStatus = xbee_spi_GET_STATUS_RX(xbee_spi_swStatusRx);
        xbee_spi_swStatusRx = tmpStatus;

        /* Check if RX data FIFO has some data to be moved into the RX Buffer */
        while(0u != (xbee_spi_swStatusRx & xbee_spi_STS_RX_FIFO_NOT_EMPTY))
        {
            rxData = CY_GET_REG8(xbee_spi_RXDATA_PTR);

            /* Set next pointer. */
            xbee_spi_rxBufferWrite++;
            if(xbee_spi_rxBufferWrite >= xbee_spi_RX_BUFFER_SIZE)
            {
                xbee_spi_rxBufferWrite = 0u;
            }

            if(xbee_spi_rxBufferWrite == xbee_spi_rxBufferRead)
            {
                xbee_spi_rxBufferRead++;
                if(xbee_spi_rxBufferRead >= xbee_spi_RX_BUFFER_SIZE)
                {
                    xbee_spi_rxBufferRead = 0u;
                }

                xbee_spi_rxBufferFull = 1u;
            }

            /* Move data from the FIFO to the Buffer */
            xbee_spi_rxBuffer[xbee_spi_rxBufferWrite] = rxData;

            tmpStatus = xbee_spi_GET_STATUS_RX(xbee_spi_swStatusRx);
            xbee_spi_swStatusRx = tmpStatus;
        }

    #endif /* (xbee_spi_RX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START xbee_spi_RX_ISR_END` */

    /* `#END` */
}

/* [] END OF FILE */
