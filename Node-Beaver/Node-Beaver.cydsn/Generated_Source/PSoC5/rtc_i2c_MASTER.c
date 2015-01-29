/*******************************************************************************
* File Name: rtc_i2c_MASTER.c
* Version 3.30
*
* Description:
*  This file provides the source code of APIs for the I2C component Master mode.
*
* Note:
*
*******************************************************************************
* Copyright 2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "rtc_i2c_PVT.h"

#if(rtc_i2c_MODE_MASTER_ENABLED)

/**********************************
*      System variables
**********************************/

volatile uint8 rtc_i2c_mstrStatus;     /* Master Status byte  */
volatile uint8 rtc_i2c_mstrControl;    /* Master Control byte */

/* Transmit buffer variables */
volatile uint8 * rtc_i2c_mstrRdBufPtr;     /* Pointer to Master Read buffer */
volatile uint8   rtc_i2c_mstrRdBufSize;    /* Master Read buffer size       */
volatile uint8   rtc_i2c_mstrRdBufIndex;   /* Master Read buffer Index      */

/* Receive buffer variables */
volatile uint8 * rtc_i2c_mstrWrBufPtr;     /* Pointer to Master Write buffer */
volatile uint8   rtc_i2c_mstrWrBufSize;    /* Master Write buffer size       */
volatile uint8   rtc_i2c_mstrWrBufIndex;   /* Master Write buffer Index      */


/*******************************************************************************
* Function Name: rtc_i2c_MasterWriteBuf
********************************************************************************
*
* Summary:
*  Automatically writes an entire buffer of data to a slave device. Once the
*  data transfer is initiated by this function, further data transfer is handled
*  by the included ISR in byte by byte mode.
*
* Parameters:
*  slaveAddr: 7-bit slave address.
*  xferData:  Pointer to buffer of data to be sent.
*  cnt:       Size of buffer to send.
*  mode:      Transfer mode defines: start or restart condition generation at
*             begin of the transfer and complete the transfer or halt before
*             generating a stop.
*
* Return:
*  Status error - zero means no errors.
*
* Side Effects:
*  The included ISR will start transfer after start or restart condition will
*  be generated.
*
* Global variables:
*  rtc_i2c_mstrStatus  - used to store current status of I2C Master.
*  rtc_i2c_state       - used to store current state of software FSM.
*  rtc_i2c_mstrControl - used to control master end of transaction with
*  or without the Stop generation.
*  rtc_i2c_mstrWrBufPtr - used to store pointer to master write buffer.
*  rtc_i2c_mstrWrBufIndex - used to current index within master write
*  buffer.
*  rtc_i2c_mstrWrBufSize - used to store master write buffer size.
*
* Reentrant:
*  No
*
*******************************************************************************/
uint8 rtc_i2c_MasterWriteBuf(uint8 slaveAddress, uint8 * wrData, uint8 cnt, uint8 mode)
      
{
    uint8 errStatus;

    errStatus = rtc_i2c_MSTR_NOT_READY;

    if(NULL != wrData)
    {
        /* Check I2C state before transfer: valid are IDLE or HALT */
        if(rtc_i2c_SM_IDLE == rtc_i2c_state)
        {
            /* Check if free: Master is ready to transaction */
            if(rtc_i2c_CHECK_BUS_FREE(rtc_i2c_MCSR_REG))
            {
                errStatus = rtc_i2c_MSTR_NO_ERROR;
            }
            else
            {
                errStatus = rtc_i2c_MSTR_BUS_BUSY;
            }
        }
        else if(rtc_i2c_SM_MSTR_HALT == rtc_i2c_state)
        {
            errStatus = rtc_i2c_MSTR_NO_ERROR;

            CyIntClearPending(rtc_i2c_ISR_NUMBER);
            rtc_i2c_mstrStatus &= ((uint8) ~rtc_i2c_MSTAT_XFER_HALT);
        }
        else
        {
            /* errStatus = rtc_i2c_MSTR_NOT_READY was send before */
        }

        if(rtc_i2c_MSTR_NO_ERROR == errStatus)
        {
            rtc_i2c_state    = rtc_i2c_SM_MSTR_WR_ADDR;
            rtc_i2c_DATA_REG = ((uint8) (slaveAddress << rtc_i2c_SLAVE_ADDR_SHIFT));

            rtc_i2c_mstrWrBufIndex = 0u;
            rtc_i2c_mstrWrBufSize  = cnt;
            rtc_i2c_mstrWrBufPtr   = (volatile uint8 *) wrData;

            rtc_i2c_mstrControl = mode;    /* Save transaction mode */

            /* Generate a Start or ReStart depends on mode */
            if(rtc_i2c_CHECK_RESTART(mode))
            {
                rtc_i2c_GENERATE_RESTART;
            }
            else
            {
                rtc_i2c_GENERATE_START;
            }

            rtc_i2c_mstrStatus &= ((uint8) ~rtc_i2c_MSTAT_WR_CMPLT);

            rtc_i2c_EnableInt();   /* Enable intr to complete transfer */
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: rtc_i2c_MasterReadBuf
********************************************************************************
*
* Summary:
*  Automatically writes an entire buffer of data to a slave device. Once the
*  data transfer is initiated by this function, further data transfer is handled
*  by the included ISR in byte by byte mode.
*
* Parameters:
*  slaveAddr: 7-bit slave address.
*  xferData:  Pointer to buffer where to put data from slave.
*  cnt:       Size of buffer to read.
*  mode:      Transfer mode defines: start or restart condition generation at
*             begin of the transfer and complete the transfer or halt before
*             generating a stop.
*
* Return:
*  Status error - zero means no errors.
*
* Side Effects:
*  The included ISR will start transfer after start or restart condition will
*  be generated.
*
* Global variables:
*  rtc_i2c_mstrStatus  - used to store current status of I2C Master.
*  rtc_i2c_state       - used to store current state of software FSM.
*  rtc_i2c_mstrControl - used to control master end of transaction with
*  or without the Stop generation.
*  rtc_i2c_mstrRdBufPtr - used to store pointer to master write buffer.
*  rtc_i2c_mstrRdBufIndex - used to current index within master write
*  buffer.
*  rtc_i2c_mstrRdBufSize - used to store master write buffer size.
*
* Reentrant:
*  No
*
*******************************************************************************/
uint8 rtc_i2c_MasterReadBuf(uint8 slaveAddress, uint8 * rdData, uint8 cnt, uint8 mode)
      
{
    uint8 errStatus;

    errStatus = rtc_i2c_MSTR_NOT_READY;

    if(NULL != rdData)
    {
        /* Check I2C state before transfer: valid are IDLE or HALT */
        if(rtc_i2c_SM_IDLE == rtc_i2c_state)
        {
            /* Check if free: Master is ready to transaction */
            if(rtc_i2c_CHECK_BUS_FREE(rtc_i2c_MCSR_REG))
            {
                errStatus = rtc_i2c_MSTR_NO_ERROR;
            }
            else
            {
                errStatus = rtc_i2c_MSTR_BUS_BUSY;
            }
        }
        else if(rtc_i2c_SM_MSTR_HALT == rtc_i2c_state)
        {
            errStatus = rtc_i2c_MSTR_NO_ERROR;

            CyIntClearPending(rtc_i2c_ISR_NUMBER);
            rtc_i2c_mstrStatus &= ((uint8) ~rtc_i2c_MSTAT_XFER_HALT);
        }
        else
        {
            /* errStatus = rtc_i2c_MSTR_NOT_READY was send before */
        }

        if(rtc_i2c_MSTR_NO_ERROR == errStatus)
        {
            rtc_i2c_state    = rtc_i2c_SM_MSTR_RD_ADDR;
            rtc_i2c_DATA_REG = (((uint8) (slaveAddress << rtc_i2c_SLAVE_ADDR_SHIFT)) |
                                                   rtc_i2c_READ_FLAG);

            rtc_i2c_mstrRdBufIndex  = 0u;
            rtc_i2c_mstrRdBufSize   = cnt;
            rtc_i2c_mstrRdBufPtr    = (volatile uint8 *) rdData;

            rtc_i2c_mstrControl = mode;    /* Save transaction mode */

            /* Generate a Start or ReStart depends on mode */
            if(rtc_i2c_CHECK_RESTART(mode))
            {
                rtc_i2c_GENERATE_RESTART;
            }
            else
            {
                rtc_i2c_GENERATE_START;
            }

            rtc_i2c_mstrStatus &= ((uint8) ~rtc_i2c_MSTAT_RD_CMPLT);

            rtc_i2c_EnableInt();   /* Enable intr to complete transfer */
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: rtc_i2c_MasterSendStart
********************************************************************************
*
* Summary:
*  Generates Start condition and sends slave address with read/write bit.
*
* Parameters:
*  slaveAddress:  7-bit slave address.
*  R_nW:          Zero, send write command, non-zero send read command.
*
* Return:
*  Status error - zero means no errors.
*
* Side Effects:
*  This function is entered without a 'byte complete' bit set in the I2C_CSR
*  register. It does not exit until it will be set.
*
* Global variables:
*  rtc_i2c_state - used to store current state of software FSM.
*
* Reentrant:
*  No
*
*******************************************************************************/
uint8 rtc_i2c_MasterSendStart(uint8 slaveAddress, uint8 R_nW)
      
{
    uint8 errStatus;

    errStatus = rtc_i2c_MSTR_NOT_READY;

    /* If IDLE, check if bus is free */
    if(rtc_i2c_SM_IDLE == rtc_i2c_state)
    {
        /* If bus is free, generate Start condition */
        if(rtc_i2c_CHECK_BUS_FREE(rtc_i2c_MCSR_REG))
        {
            rtc_i2c_DisableInt();  /* Disable ISR for Manual functions */

            slaveAddress = ((uint8) (slaveAddress << rtc_i2c_SLAVE_ADDR_SHIFT)); /* Set Address */
            if(0u != R_nW)                                      /* Set the Read/Write flag */
            {
                slaveAddress |= rtc_i2c_READ_FLAG;
                rtc_i2c_state = rtc_i2c_SM_MSTR_RD_ADDR;
            }
            else
            {
                rtc_i2c_state = rtc_i2c_SM_MSTR_WR_ADDR;
            }
            rtc_i2c_DATA_REG = slaveAddress;   /* Write address to data reg */


            rtc_i2c_GENERATE_START;
            while(rtc_i2c_WAIT_BYTE_COMPLETE(rtc_i2c_CSR_REG))
            {
                ; /* Wait for the address to be transfered */
            }

            #if(rtc_i2c_MODE_MULTI_MASTER_SLAVE_ENABLED)
                if(rtc_i2c_CHECK_START_GEN(rtc_i2c_MCSR_REG))
                {
                    rtc_i2c_CLEAR_START_GEN;

                    /* Start condition was not generated: reset FSM to IDLE */
                    rtc_i2c_state = rtc_i2c_SM_IDLE;
                    errStatus = rtc_i2c_MSTR_ERR_ABORT_START_GEN;
                }
                else
            #endif /* (rtc_i2c_MODE_MULTI_MASTER_SLAVE_ENABLED) */

            #if(rtc_i2c_MODE_MULTI_MASTER_ENABLED)

                if(rtc_i2c_CHECK_LOST_ARB(rtc_i2c_CSR_REG))
                {
                    rtc_i2c_BUS_RELEASE;

                    /* Master lost arbitrage: reset FSM to IDLE */
                    rtc_i2c_state = rtc_i2c_SM_IDLE;
                    errStatus = rtc_i2c_MSTR_ERR_ARB_LOST;
                }
                else
            #endif /* (rtc_i2c_MODE_MULTI_MASTER_ENABLED) */

                if(rtc_i2c_CHECK_ADDR_NAK(rtc_i2c_CSR_REG))
                {
                    /* Address has been NACKed: reset FSM to IDLE */
                    rtc_i2c_state = rtc_i2c_SM_IDLE;
                    errStatus = rtc_i2c_MSTR_ERR_LB_NAK;
                }
                else
                {
                    /* Start was sent witout errors */
                    errStatus = rtc_i2c_MSTR_NO_ERROR;
                }
        }
        else
        {
            errStatus = rtc_i2c_MSTR_BUS_BUSY; /* Bus is busy */
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: rtc_i2c_MasterSendRestart
********************************************************************************
*
* Summary:
*  Generates ReStart condition and sends slave address with read/write bit.
*
* Parameters:
*  slaveAddress:  7-bit slave address.
*  R_nW:          Zero, send write command, non-zero send read command.
*
* Return:
*  Status error - zero means no errors.
*
* Side Effects:
*  This function is entered without a 'byte complete' bit set in the I2C_CSR
*  register. It does not exit until it will be set.
*
* Global variables:
*  rtc_i2c_state - used to store current state of software FSM.
*
* Reentrant:
*  No
*
*******************************************************************************/
uint8 rtc_i2c_MasterSendRestart(uint8 slaveAddress, uint8 R_nW)
      
{
    uint8 errStatus;

    errStatus = rtc_i2c_MSTR_NOT_READY;

    /* Check if START condition was generated */
    if(rtc_i2c_CHECK_MASTER_MODE(rtc_i2c_MCSR_REG))
    {
        slaveAddress = ((uint8) (slaveAddress << rtc_i2c_SLAVE_ADDR_SHIFT)); /* Set Address */
        if(0u != R_nW)  /* Set the Read/Write flag */
        {
            slaveAddress |= rtc_i2c_READ_FLAG;
            rtc_i2c_state = rtc_i2c_SM_MSTR_RD_ADDR;
        }
        else
        {
            rtc_i2c_state = rtc_i2c_SM_MSTR_WR_ADDR;
        }
        rtc_i2c_DATA_REG = slaveAddress;    /* Write address to data reg */


        rtc_i2c_GENERATE_RESTART_MANUAL;
        while(rtc_i2c_WAIT_BYTE_COMPLETE(rtc_i2c_CSR_REG))
        {
            ; /* Wait for the address to be transfered */
        }

        #if(rtc_i2c_MODE_MULTI_MASTER_ENABLED)
            if(rtc_i2c_CHECK_LOST_ARB(rtc_i2c_CSR_REG))
            {
                rtc_i2c_BUS_RELEASE;

                /* Master lost arbitrage: reset FSM to IDLE */
                rtc_i2c_state = rtc_i2c_SM_IDLE;
                errStatus = rtc_i2c_MSTR_ERR_ARB_LOST;
            }
            else
        #endif /* (rtc_i2c_MODE_MULTI_MASTER_ENABLED) */

            if(rtc_i2c_CHECK_ADDR_NAK(rtc_i2c_CSR_REG))
            {
                /* Address has been NACKed: reset FSM to IDLE */
                rtc_i2c_state = rtc_i2c_SM_IDLE;
                errStatus = rtc_i2c_MSTR_ERR_LB_NAK;
            }
            else
            {
                /* ReStart was sent witout errors */
                errStatus = rtc_i2c_MSTR_NO_ERROR;
            }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: rtc_i2c_MasterSendStop
********************************************************************************
*
* Summary:
*  Generates I2C Stop condition on bus. Function do nothing if Start or Restart
*  condition was failed before call this function.
*
* Parameters:
*  None
*
* Return:
*  Status error - zero means no errors.
*
* Side Effects:
*  The Stop generation is required to complete transaction.
*  This function does not wait whileStop condition will be generated.
*
* Global variables:
*  rtc_i2c_state - used to store current state of software FSM.
*
* Reentrant:
*  No
*
*******************************************************************************/
uint8 rtc_i2c_MasterSendStop(void) 
{
    uint8 errStatus;

    errStatus = rtc_i2c_MSTR_NOT_READY;

    /* Check if START condition was generated */
    if(rtc_i2c_CHECK_MASTER_MODE(rtc_i2c_MCSR_REG))
    {
        rtc_i2c_GENERATE_STOP_MANUAL;              /* Generate STOP */
        rtc_i2c_state = rtc_i2c_SM_IDLE;  /* Reset state to IDLE */

        while(rtc_i2c_WAIT_STOP_COMPLETE(rtc_i2c_CSR_REG))
        {
            ; /* Wait for Stop to be generated */
        }

        errStatus = rtc_i2c_MSTR_NO_ERROR;
        #if(rtc_i2c_MODE_MULTI_MASTER_ENABLED)
            if(rtc_i2c_CHECK_LOST_ARB(rtc_i2c_CSR_REG))
            {
                rtc_i2c_BUS_RELEASE;

                /* NACK was generated by enother instead Stop */
                errStatus = rtc_i2c_MSTR_ERR_ARB_LOST;
            }
        #endif /* (rtc_i2c_MODE_MULTI_MASTER_ENABLED) */
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: rtc_i2c_MasterWriteByte
********************************************************************************
*
* Summary:
*  Sends one byte to a slave. A valid Start or ReStart condition must be
*  generated before this call this function. Function do nothing if Start or
*  Restart condition was failed before call this function.
*
* Parameters:
*  data:  The data byte to send to the slave.
*
* Return:
*  Status error - zero means no errors.
*
* Side Effects:
*  This function is entered without a 'byte complete' bit set in the I2C_CSR
*  register. It does not exit until it will be set.
*
* Global variables:
*  rtc_i2c_state - used to store current state of software FSM.
*
*******************************************************************************/
uint8 rtc_i2c_MasterWriteByte(uint8 theByte) 
{
    uint8 errStatus;

    errStatus = rtc_i2c_MSTR_NOT_READY;

    /* Check if START condition was generated */
    if(rtc_i2c_CHECK_MASTER_MODE(rtc_i2c_MCSR_REG))
    {
        rtc_i2c_DATA_REG = theByte;                        /* Write DATA register */
        rtc_i2c_TRANSMIT_DATA_MANUAL;                      /* Set transmit mode */
        rtc_i2c_state = rtc_i2c_SM_MSTR_WR_DATA;  /* Set state WR_DATA */

        /* Make sure the last byte has been transfered first */
        while(rtc_i2c_WAIT_BYTE_COMPLETE(rtc_i2c_CSR_REG))
        {
            ; /* Wait for byte to be written */
        }

        #if(rtc_i2c_MODE_MULTI_MASTER_ENABLED)
            if(rtc_i2c_CHECK_LOST_ARB(rtc_i2c_CSR_REG))
            {
                rtc_i2c_BUS_RELEASE;

                /* Master lost arbitrage: reset FSM to IDLE */
                rtc_i2c_state = rtc_i2c_SM_IDLE;
                errStatus = rtc_i2c_MSTR_ERR_ARB_LOST;
            }
            /* Check LRB bit */
            else
        #endif /* (rtc_i2c_MODE_MULTI_MASTER_ENABLED) */

            if(rtc_i2c_CHECK_DATA_ACK(rtc_i2c_CSR_REG))
            {
                rtc_i2c_state = rtc_i2c_SM_MSTR_HALT;     /* Set state to HALT */
                errStatus = rtc_i2c_MSTR_NO_ERROR;                 /* The LRB was ACKed */
            }
            else
            {
                rtc_i2c_state = rtc_i2c_SM_MSTR_HALT;     /* Set state to HALT */
                errStatus = rtc_i2c_MSTR_ERR_LB_NAK;               /* The LRB was NACKed */
            }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: rtc_i2c_MasterReadByte
********************************************************************************
*
* Summary:
*  Reads one byte from a slave and ACK or NACK the transfer. A valid Start or
*  ReStart condition must be generated before this call this function. Function
*  do nothing if Start or Restart condition was failed before call this
*  function.
*
* Parameters:
*  acknNack:  Zero, response with NACK, if non-zero response with ACK.
*
* Return:
*  Byte read from slave.
*
* Side Effects:
*  This function is entered without a 'byte complete' bit set in the I2C_CSR
*  register. It does not exit until it will be set.
*
* Global variables:
*  rtc_i2c_state - used to store current state of software FSM.
*
* Reentrant:
*  No
*
*******************************************************************************/
uint8 rtc_i2c_MasterReadByte(uint8 acknNak) 
{
    uint8 theByte;

    theByte = 0u;

    /* Check if START condition was generated */
    if(rtc_i2c_CHECK_MASTER_MODE(rtc_i2c_MCSR_REG))
    {
        /* When address phase need release the bus and receive the byte, then decide ACK or NACK */
        if(rtc_i2c_SM_MSTR_RD_ADDR == rtc_i2c_state)
        {
            rtc_i2c_state = rtc_i2c_SM_MSTR_RD_DATA;
            rtc_i2c_READY_TO_READ_MANUAL;
        }

        while(rtc_i2c_WAIT_BYTE_COMPLETE(rtc_i2c_CSR_REG))
        {
            ; /* Wait for byte to be read */
        }

        theByte = rtc_i2c_DATA_REG;

        /* Now if the ACK flag was set, ACK the data which will release the bus and
           start the next byte in otherwise do NOTHING to the CSR reg.
           This will allow the calling routine to generate a repeat start or
           stop depending on it's preference. */
        if(acknNak != 0u)   /* Do ACK */
        {
            rtc_i2c_ACK_AND_RECEIVE_MANUAL;
        }
        else                /* Do NACK */
        {
            /* Do nothing to be able work with ReStart */
            rtc_i2c_state = rtc_i2c_SM_MSTR_HALT;
        }
    }

    return(theByte);
}


/*******************************************************************************
* Function Name: rtc_i2c_MasterStatus
********************************************************************************
*
* Summary:
*  Returns the master's communication status.
*
* Parameters:
*  None
*
* Return:
*  Current status of I2C master.
*
* Global variables:
*  rtc_i2c_mstrStatus - used to store current status of I2C Master.
*
*******************************************************************************/
uint8 rtc_i2c_MasterStatus(void) 
{
    uint8 status;

    status = rtc_i2c_mstrStatus;

    /* When in Master state only transaction is in progress */
    if(rtc_i2c_CHECK_SM_MASTER)
    {
        /* Add transaction in progress activity to master status */
        status |= rtc_i2c_MSTAT_XFER_INP;
    }
    else
    {
        /* Current master status is valid */
    }

    return(status);
}


/*******************************************************************************
* Function Name: rtc_i2c_MasterClearStatus
********************************************************************************
*
* Summary:
*  Clears all status flags and returns the master status.
*
* Parameters:
*  None
*
* Return:
*  Current status of I2C master.
*
* Global variables:
*  rtc_i2c_mstrStatus - used to store current status of I2C Master.
*
* Reentrant:
*  No
*
*******************************************************************************/
uint8 rtc_i2c_MasterClearStatus(void) 
{
    /* Current master status */
    uint8 status;

    /* Read and clear master status */
    status = rtc_i2c_mstrStatus;
    rtc_i2c_mstrStatus = rtc_i2c_MSTAT_CLEAR;

    return(status);
}


/*******************************************************************************
* Function Name: rtc_i2c_MasterGetReadBufSize
********************************************************************************
*
* Summary:
*  Returns the amount of bytes that has been transferred with an
*  I2C_MasterReadBuf command.
*
* Parameters:
*  None
*
* Return:
*  Byte count of transfer. If the transfer is not yet complete, it will return
*  the byte count transferred so far.
*
* Global variables:
*  rtc_i2c_mstrRdBufIndex - used to current index within master read
*  buffer.
*
*******************************************************************************/
uint8 rtc_i2c_MasterGetReadBufSize(void) 
{
    return(rtc_i2c_mstrRdBufIndex);
}


/*******************************************************************************
* Function Name: rtc_i2c_MasterGetWriteBufSize
********************************************************************************
*
* Summary:
*  Returns the amount of bytes that has been transferred with an
*  I2C_MasterWriteBuf command.
*
* Parameters:
*  None
*
* Return:
*  Byte count of transfer. If the transfer is not yet complete, it will return
*  the byte count transferred so far.
*
* Global variables:
*  rtc_i2c_mstrWrBufIndex - used to current index within master write
*  buffer.
*
*******************************************************************************/
uint8 rtc_i2c_MasterGetWriteBufSize(void) 
{
    return(rtc_i2c_mstrWrBufIndex);
}


/*******************************************************************************
* Function Name: rtc_i2c_MasterClearReadBuf
********************************************************************************
*
* Summary:
*  Resets the read buffer pointer back to the first byte in the buffer.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  rtc_i2c_mstrRdBufIndex - used to current index within master read
*   buffer.
*  rtc_i2c_mstrStatus - used to store current status of I2C Master.
*
* Reentrant:
*  No
*
*******************************************************************************/
void rtc_i2c_MasterClearReadBuf(void) 
{
    rtc_i2c_mstrRdBufIndex = 0u;
    rtc_i2c_mstrStatus    &= ((uint8) ~rtc_i2c_MSTAT_RD_CMPLT);
}


/*******************************************************************************
* Function Name: rtc_i2c_MasterClearWriteBuf
********************************************************************************
*
* Summary:
*  Resets the write buffer pointer back to the first byte in the buffer.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  rtc_i2c_mstrRdBufIndex - used to current index within master read
*   buffer.
*  rtc_i2c_mstrStatus - used to store current status of I2C Master.
*
* Reentrant:
*  No
*
*******************************************************************************/
void rtc_i2c_MasterClearWriteBuf(void) 
{
    rtc_i2c_mstrWrBufIndex = 0u;
    rtc_i2c_mstrStatus    &= ((uint8) ~rtc_i2c_MSTAT_WR_CMPLT);
}


/*******************************************************************************
* Function Name: rtc_i2c_Workaround
********************************************************************************
*
* Summary:
*  Do nothing. This fake fuction use as workaround for CDT 78083.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void rtc_i2c_Workaround(void) 
{
    /* Does nothing */
}

#endif /* (rtc_i2c_MODE_MASTER_ENABLED) */


/* [] END OF FILE */
