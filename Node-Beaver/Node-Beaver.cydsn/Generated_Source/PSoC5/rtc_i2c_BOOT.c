/*******************************************************************************
* File Name: rtc_i2c_BOOT.c
* Version 3.30
*
* Description:
*  This file provides the source code of bootloader communication APIs for the
*  I2C component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "rtc_i2c_PVT.h"

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (rtc_i2c_BOOTLOADER_INTERFACE_ENABLED)


/***************************************
*    Bootloader Internal Variables
***************************************/

/* I2C write buffer: Host writes command here */
static uint8 XDATA rtc_i2c_slReadBuf[rtc_i2c_BTLDR_SIZEOF_READ_BUFFER];

/* I2C read buffer: Host reads responses from it */
static uint8 XDATA rtc_i2c_slWriteBuf[rtc_i2c_BTLDR_SIZEOF_WRITE_BUFFER];


/*******************************************************************************
* Function Name: rtc_i2c_CyBtldrCommStart
********************************************************************************
*
* Summary:
*  Starts the communication component and enables the interrupt.
*  The read buffer initial state is full and the read always is 0xFFu.
*  The write buffer is clear and ready to receive a commmand.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This fucntion enables component interrupt. If I2C is enabled
*  without the interrupt enabled, it could lock up the I2C bus.
*
* Global variables:
*  rtc_i2c_slWriteBuf - used to store received command.
*  rtc_i2c_slReadBuf - used to store response.
*  rtc_i2c_slRdBufIndex - used to store current index within slave
*  read buffer.
*
*******************************************************************************/
void rtc_i2c_CyBtldrCommStart(void) CYSMALL 
{
    /* Set Write buffer */
    rtc_i2c_SlaveInitWriteBuf(rtc_i2c_slWriteBuf, rtc_i2c_BTLDR_SIZEOF_WRITE_BUFFER);

    /* Set Read buffer which has zero elements */
    rtc_i2c_SlaveInitReadBuf(rtc_i2c_slReadBuf, 0u);

    /* Enable power to I2C Module */
    rtc_i2c_Start();
}


/*******************************************************************************
* Function Name: rtc_i2c_CyBtldrCommStop
********************************************************************************
*
* Summary:
*  Disables the communication component and disables the interrupt.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void rtc_i2c_CyBtldrCommStop(void) CYSMALL 
{
    /* Stop I2C component */
    rtc_i2c_Stop();
}


/*******************************************************************************
* Function Name: rtc_i2c_CyBtldrCommReset
********************************************************************************
*
* Summary:
*  Set buffers to the initial state and reset the statuses.
*  The read buffer initial state is full and the read always is 0xFFu.
*  The write buffer is clear and ready to receive a commmand.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  rtc_i2c_slRdBufIndex - used to store current index within slave
*  read buffer.
*
*******************************************************************************/
void rtc_i2c_CyBtldrCommReset(void) CYSMALL 
{
    /* Make the Read buffer full */
    rtc_i2c_slRdBufSize = 0u;

    /* Reset Write buffer and Read buffer */
    rtc_i2c_slRdBufIndex = 0u;
    rtc_i2c_slWrBufIndex = 0u;

    /* Clear read and write status */
    rtc_i2c_slStatus = 0u;
}


/*******************************************************************************
* Function Name: rtc_i2c_CyBtldrCommWrite
********************************************************************************
*
* Summary:
*  Transmits the status of executed command to the Host.
*  The function updates the I2C read buffer with response and realeases it to
*  the host. All reads return 0xFF till the buffer will be released. All bytes
*  are transfered by the I2C ISR.
*  The function waits with timeout till all bytes will be read.
*  After exist this function the reads return 0xFF.
*
* Parameters:
*  pData:    pointer to data buffer with response command.
*  size:     number of bytes required to be transmitted.
*  count:    actual size of data was transmitted.
*  timeOut:  timeout value in tries of 10uS.
*
* Return:
*  Status of transmit operation.
*
* Global variables:
*  rtc_i2c_slReadBuf - used to store response.
*  rtc_i2c_slRdBufIndex - used to store current index within slave
*  read buffer.
*
*******************************************************************************/
cystatus rtc_i2c_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
         
{
    cystatus status;
    uint16 timeoutMs;

    status = CYRET_BAD_PARAM;

    if((NULL != pData) && (size > 0u))
    {
        status = CYRET_TIMEOUT;
        timeoutMs = ((uint16) 10u * timeOut);  /* Convert from 10mS check to 1mS checks */

        /* Copy pData to component buffer */
        (void) memcpy((void *) rtc_i2c_slReadBuf, (void *) pData, size);
        *count = size;  /* Buffer was copied to I2C buffer */

        /* The buffer is free to be read */
        rtc_i2c_slRdBufSize = ((uint8) size);

        while(0u != timeoutMs)  /* Wait till response will be read */
        {
            /* Check if host complete read */
            if(rtc_i2c_slRdBufIndex == ((uint8) size))
            {
                rtc_i2c_slRdBufSize  = 0u;
                rtc_i2c_slRdBufIndex = 0u;

                status = CYRET_SUCCESS;
                break;
            }

            CyDelay(rtc_i2c_WAIT_1_MS);
            timeoutMs--;
        }
    }

    return(status);
}


/*******************************************************************************
* Function Name: rtc_i2c_CyBtldrCommRead
********************************************************************************
*
* Summary:
*  Receives the command from the Host.
*  All bytes are received by the I2C ISR and stored in internal I2C buffer. The
*  function checks status with timeout to detemine the end of transfer and
*  then copy data to bootloader buffer.
*  After exist this function the I2C ISR is able to receive more data.
*
* Parameters:
*  pData:    pointer to data buffer to store command.
*  size:     maximum number of bytes which could to be passed back.
*  count:    actual size of data was received.
*  timeOut:  timeout value in tries of 10uS.
*
* Return:
*  Status of receive operation.
*
* Global variables:
*  rtc_i2c_slWriteBuf - used to store received command.
*
*******************************************************************************/
cystatus rtc_i2c_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
         
{
    cystatus status;
    uint16 timeoutMs;
    uint8 byteCount;

    status = CYRET_BAD_PARAM;

    if((NULL != pData) && (size > 0u))
    {
        status = CYRET_TIMEOUT;
        timeoutMs = ((uint16) 10u * timeOut);  /* Convert from 10mS checks to 1mS checks */

        while(0u != timeoutMs)  /* Wait for command from the host */
        {
            /* Check if the host complete write */
            if(0u != (rtc_i2c_slStatus & rtc_i2c_SSTAT_WR_CMPLT))
            {
                /* How many bytes the host has been written */
                byteCount = rtc_i2c_slWrBufIndex;
                *count = (uint16) byteCount;

                /* Copy command to pData buffer */
                (void) memcpy((void *) pData, (void *) rtc_i2c_slWriteBuf,
                              rtc_i2c_MIN_UNT16(byteCount, size));

                /* Clear I2C write buffer and status */
                rtc_i2c_slStatus     = 0u;
                rtc_i2c_slWrBufIndex = 0u;

                status = CYRET_SUCCESS;
                break;
            }

            CyDelay(rtc_i2c_WAIT_1_MS);
            timeoutMs--;
        }
    }

    return(status);
}

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (rtc_i2c_BOOTLOADER_INTERFACE_ENABLED) */


/* [] END OF FILE */
