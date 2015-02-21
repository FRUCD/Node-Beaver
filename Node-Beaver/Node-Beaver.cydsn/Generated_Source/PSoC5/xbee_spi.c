/*******************************************************************************
* File Name: xbee_spi.c
* Version 2.40
*
* Description:
*  This file provides all API functionality of the SPI Master component.
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

#if(xbee_spi_TX_SOFTWARE_BUF_ENABLED)
    volatile uint8 xbee_spi_txBuffer[xbee_spi_TX_BUFFER_SIZE] = {0u};
    volatile uint8 xbee_spi_txBufferFull;
    volatile uint8 xbee_spi_txBufferRead;
    volatile uint8 xbee_spi_txBufferWrite;
#endif /* (xbee_spi_TX_SOFTWARE_BUF_ENABLED) */

#if(xbee_spi_RX_SOFTWARE_BUF_ENABLED)
    volatile uint8 xbee_spi_rxBuffer[xbee_spi_RX_BUFFER_SIZE] = {0u};
    volatile uint8 xbee_spi_rxBufferFull;
    volatile uint8 xbee_spi_rxBufferRead;
    volatile uint8 xbee_spi_rxBufferWrite;
#endif /* (xbee_spi_RX_SOFTWARE_BUF_ENABLED) */

uint8 xbee_spi_initVar = 0u;

volatile uint8 xbee_spi_swStatusTx;
volatile uint8 xbee_spi_swStatusRx;


/*******************************************************************************
* Function Name: xbee_spi_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default SPIM configuration provided with customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  When this function is called it initializes all of the necessary parameters
*  for execution. i.e. setting the initial interrupt mask, configuring the
*  interrupt service routine, configuring the bit-counter parameters and
*  clearing the FIFO and Status Register.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void xbee_spi_Init(void) 
{
    /* Initialize the Bit counter */
    xbee_spi_COUNTER_PERIOD_REG = xbee_spi_BITCTR_INIT;

    /* Init TX ISR  */
    #if(0u != xbee_spi_INTERNAL_TX_INT_ENABLED)
        CyIntDisable         (xbee_spi_TX_ISR_NUMBER);
        CyIntSetPriority     (xbee_spi_TX_ISR_NUMBER,  xbee_spi_TX_ISR_PRIORITY);
        (void) CyIntSetVector(xbee_spi_TX_ISR_NUMBER, &xbee_spi_TX_ISR);
    #endif /* (0u != xbee_spi_INTERNAL_TX_INT_ENABLED) */

    /* Init RX ISR  */
    #if(0u != xbee_spi_INTERNAL_RX_INT_ENABLED)
        CyIntDisable         (xbee_spi_RX_ISR_NUMBER);
        CyIntSetPriority     (xbee_spi_RX_ISR_NUMBER,  xbee_spi_RX_ISR_PRIORITY);
        (void) CyIntSetVector(xbee_spi_RX_ISR_NUMBER, &xbee_spi_RX_ISR);
    #endif /* (0u != xbee_spi_INTERNAL_RX_INT_ENABLED) */

    /* Clear any stray data from the RX and TX FIFO */
    xbee_spi_ClearFIFO();

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

    (void) xbee_spi_ReadTxStatus(); /* Clear Tx status and swStatusTx */
    (void) xbee_spi_ReadRxStatus(); /* Clear Rx status and swStatusRx */

    /* Configure TX and RX interrupt mask */
    xbee_spi_TX_STATUS_MASK_REG = xbee_spi_TX_INIT_INTERRUPTS_MASK;
    xbee_spi_RX_STATUS_MASK_REG = xbee_spi_RX_INIT_INTERRUPTS_MASK;
}


/*******************************************************************************
* Function Name: xbee_spi_Enable
********************************************************************************
*
* Summary:
*  Enable SPIM component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void xbee_spi_Enable(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    xbee_spi_COUNTER_CONTROL_REG |= xbee_spi_CNTR_ENABLE;
    xbee_spi_TX_STATUS_ACTL_REG  |= xbee_spi_INT_ENABLE;
    xbee_spi_RX_STATUS_ACTL_REG  |= xbee_spi_INT_ENABLE;
    CyExitCriticalSection(enableInterrupts);

    #if(0u != xbee_spi_INTERNAL_CLOCK)
        xbee_spi_IntClock_Enable();
    #endif /* (0u != xbee_spi_INTERNAL_CLOCK) */

    xbee_spi_EnableTxInt();
    xbee_spi_EnableRxInt();
}


/*******************************************************************************
* Function Name: xbee_spi_Start
********************************************************************************
*
* Summary:
*  Initialize and Enable the SPI Master component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  xbee_spi_initVar - used to check initial configuration, modified on
*  first function call.
*
* Theory:
*  Enable the clock input to enable operation.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void xbee_spi_Start(void) 
{
    if(0u == xbee_spi_initVar)
    {
        xbee_spi_Init();
        xbee_spi_initVar = 1u;
    }

    xbee_spi_Enable();
}


/*******************************************************************************
* Function Name: xbee_spi_Stop
********************************************************************************
*
* Summary:
*  Disable the SPI Master component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the clock input to enable operation.
*
*******************************************************************************/
void xbee_spi_Stop(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    xbee_spi_TX_STATUS_ACTL_REG &= ((uint8) ~xbee_spi_INT_ENABLE);
    xbee_spi_RX_STATUS_ACTL_REG &= ((uint8) ~xbee_spi_INT_ENABLE);
    CyExitCriticalSection(enableInterrupts);

    #if(0u != xbee_spi_INTERNAL_CLOCK)
        xbee_spi_IntClock_Disable();
    #endif /* (0u != xbee_spi_INTERNAL_CLOCK) */

    xbee_spi_DisableTxInt();
    xbee_spi_DisableRxInt();
}


/*******************************************************************************
* Function Name: xbee_spi_EnableTxInt
********************************************************************************
*
* Summary:
*  Enable internal Tx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal Tx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void xbee_spi_EnableTxInt(void) 
{
    #if(0u != xbee_spi_INTERNAL_TX_INT_ENABLED)
        CyIntEnable(xbee_spi_TX_ISR_NUMBER);
    #endif /* (0u != xbee_spi_INTERNAL_TX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: xbee_spi_EnableRxInt
********************************************************************************
*
* Summary:
*  Enable internal Rx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal Rx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void xbee_spi_EnableRxInt(void) 
{
    #if(0u != xbee_spi_INTERNAL_RX_INT_ENABLED)
        CyIntEnable(xbee_spi_RX_ISR_NUMBER);
    #endif /* (0u != xbee_spi_INTERNAL_RX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: xbee_spi_DisableTxInt
********************************************************************************
*
* Summary:
*  Disable internal Tx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal Tx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void xbee_spi_DisableTxInt(void) 
{
    #if(0u != xbee_spi_INTERNAL_TX_INT_ENABLED)
        CyIntDisable(xbee_spi_TX_ISR_NUMBER);
    #endif /* (0u != xbee_spi_INTERNAL_TX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: xbee_spi_DisableRxInt
********************************************************************************
*
* Summary:
*  Disable internal Rx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal Rx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void xbee_spi_DisableRxInt(void) 
{
    #if(0u != xbee_spi_INTERNAL_RX_INT_ENABLED)
        CyIntDisable(xbee_spi_RX_ISR_NUMBER);
    #endif /* (0u != xbee_spi_INTERNAL_RX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: xbee_spi_SetTxInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void xbee_spi_SetTxInterruptMode(uint8 intSrc) 
{
    xbee_spi_TX_STATUS_MASK_REG = intSrc;
}


/*******************************************************************************
* Function Name: xbee_spi_SetRxInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void xbee_spi_SetRxInterruptMode(uint8 intSrc) 
{
    xbee_spi_RX_STATUS_MASK_REG  = intSrc;
}


/*******************************************************************************
* Function Name: xbee_spi_ReadTxStatus
********************************************************************************
*
* Summary:
*  Read the Tx status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the Tx status register.
*
* Global variables:
*  xbee_spi_swStatusTx - used to store in software status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the Tx status register for error
*  detection and flow control.
*
* Side Effects:
*  Clear Tx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 xbee_spi_ReadTxStatus(void) 
{
    uint8 tmpStatus;

    #if(xbee_spi_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        xbee_spi_DisableTxInt();

        tmpStatus = xbee_spi_GET_STATUS_TX(xbee_spi_swStatusTx);
        xbee_spi_swStatusTx = 0u;

        xbee_spi_EnableTxInt();

    #else

        tmpStatus = xbee_spi_TX_STATUS_REG;

    #endif /* (xbee_spi_TX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/*******************************************************************************
* Function Name: xbee_spi_ReadRxStatus
********************************************************************************
*
* Summary:
*  Read the Rx status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the Rx status register.
*
* Global variables:
*  xbee_spi_swStatusRx - used to store in software Rx status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the Rx status register for error
*  detection and flow control.
*
* Side Effects:
*  Clear Rx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 xbee_spi_ReadRxStatus(void) 
{
    uint8 tmpStatus;

    #if(xbee_spi_RX_SOFTWARE_BUF_ENABLED)
        /* Disable RX interrupt to protect global veriables */
        xbee_spi_DisableRxInt();

        tmpStatus = xbee_spi_GET_STATUS_RX(xbee_spi_swStatusRx);
        xbee_spi_swStatusRx = 0u;

        xbee_spi_EnableRxInt();

    #else

        tmpStatus = xbee_spi_RX_STATUS_REG;

    #endif /* (xbee_spi_RX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/*******************************************************************************
* Function Name: xbee_spi_WriteTxData
********************************************************************************
*
* Summary:
*  Write a byte of data to be sent across the SPI.
*
* Parameters:
*  txDataByte: The data value to send across the SPI.
*
* Return:
*  None.
*
* Global variables:
*  xbee_spi_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer, modified every function
*  call if TX Software Buffer is used.
*  xbee_spi_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer.
*  xbee_spi_txBuffer[xbee_spi_TX_BUFFER_SIZE] - used to store
*  data to sending, modified every function call if TX Software Buffer is used.
*
* Theory:
*  Allows the user to transmit any byte of data in a single transfer.
*
* Side Effects:
*  If this function is called again before the previous byte is finished then
*  the next byte will be appended to the transfer with no time between
*  the byte transfers. Clear Tx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void xbee_spi_WriteTxData(uint8 txData) 
{
    #if(xbee_spi_TX_SOFTWARE_BUF_ENABLED)

        uint8 tempStatus;
        uint8 tmpTxBufferRead;

        /* Block if TX buffer is FULL: don't overwrite */
        do
        {
            tmpTxBufferRead = xbee_spi_txBufferRead;
            if(0u == tmpTxBufferRead)
            {
                tmpTxBufferRead = (xbee_spi_TX_BUFFER_SIZE - 1u);
            }
            else
            {
                tmpTxBufferRead--;
            }

        }while(tmpTxBufferRead == xbee_spi_txBufferWrite);

        /* Disable TX interrupt to protect global veriables */
        xbee_spi_DisableTxInt();

        tempStatus = xbee_spi_GET_STATUS_TX(xbee_spi_swStatusTx);
        xbee_spi_swStatusTx = tempStatus;


        if((xbee_spi_txBufferRead == xbee_spi_txBufferWrite) &&
           (0u != (xbee_spi_swStatusTx & xbee_spi_STS_TX_FIFO_NOT_FULL)))
        {
            /* Add directly to the TX FIFO */
            CY_SET_REG8(xbee_spi_TXDATA_PTR, txData);
        }
        else
        {
            /* Add to the TX software buffer */
            xbee_spi_txBufferWrite++;
            if(xbee_spi_txBufferWrite >= xbee_spi_TX_BUFFER_SIZE)
            {
                xbee_spi_txBufferWrite = 0u;
            }

            if(xbee_spi_txBufferWrite == xbee_spi_txBufferRead)
            {
                xbee_spi_txBufferRead++;
                if(xbee_spi_txBufferRead >= xbee_spi_TX_BUFFER_SIZE)
                {
                    xbee_spi_txBufferRead = 0u;
                }
                xbee_spi_txBufferFull = 1u;
            }

            xbee_spi_txBuffer[xbee_spi_txBufferWrite] = txData;

            xbee_spi_TX_STATUS_MASK_REG |= xbee_spi_STS_TX_FIFO_NOT_FULL;
        }

        xbee_spi_EnableTxInt();

    #else

        while(0u == (xbee_spi_TX_STATUS_REG & xbee_spi_STS_TX_FIFO_NOT_FULL))
        {
            ; /* Wait for room in FIFO */
        }

        /* Put byte in TX FIFO */
        CY_SET_REG8(xbee_spi_TXDATA_PTR, txData);

    #endif /* (xbee_spi_TX_SOFTWARE_BUF_ENABLED) */
}


/*******************************************************************************
* Function Name: xbee_spi_ReadRxData
********************************************************************************
*
* Summary:
*  Read the next byte of data received across the SPI.
*
* Parameters:
*  None.
*
* Return:
*  The next byte of data read from the FIFO.
*
* Global variables:
*  xbee_spi_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer.
*  xbee_spi_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified every function
*  call if RX Software Buffer is used.
*  xbee_spi_rxBuffer[xbee_spi_RX_BUFFER_SIZE] - used to store
*  received data.
*
* Theory:
*  Allows the user to read a byte of data received.
*
* Side Effects:
*  Will return invalid data if the FIFO is empty. The user should Call
*  GetRxBufferSize() and if it returns a non-zero value then it is safe to call
*  ReadByte() function.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 xbee_spi_ReadRxData(void) 
{
    uint8 rxData;

    #if(xbee_spi_RX_SOFTWARE_BUF_ENABLED)

        /* Disable RX interrupt to protect global veriables */
        xbee_spi_DisableRxInt();

        if(xbee_spi_rxBufferRead != xbee_spi_rxBufferWrite)
        {
            if(0u == xbee_spi_rxBufferFull)
            {
                xbee_spi_rxBufferRead++;
                if(xbee_spi_rxBufferRead >= xbee_spi_RX_BUFFER_SIZE)
                {
                    xbee_spi_rxBufferRead = 0u;
                }
            }
            else
            {
                xbee_spi_rxBufferFull = 0u;
            }
        }

        rxData = xbee_spi_rxBuffer[xbee_spi_rxBufferRead];

        xbee_spi_EnableRxInt();

    #else

        rxData = CY_GET_REG8(xbee_spi_RXDATA_PTR);

    #endif /* (xbee_spi_RX_SOFTWARE_BUF_ENABLED) */

    return(rxData);
}


/*******************************************************************************
* Function Name: xbee_spi_GetRxBufferSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes/words of data currently held in the RX buffer.
*  If RX Software Buffer not used then function return 0 if FIFO empty or 1 if
*  FIFO not empty. In another case function return size of RX Software Buffer.
*
* Parameters:
*  None.
*
* Return:
*  Integer count of the number of bytes/words in the RX buffer.
*
* Global variables:
*  xbee_spi_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer.
*  xbee_spi_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
uint8 xbee_spi_GetRxBufferSize(void) 
{
    uint8 size;

    #if(xbee_spi_RX_SOFTWARE_BUF_ENABLED)

        /* Disable RX interrupt to protect global veriables */
        xbee_spi_DisableRxInt();

        if(xbee_spi_rxBufferRead == xbee_spi_rxBufferWrite)
        {
            size = 0u;
        }
        else if(xbee_spi_rxBufferRead < xbee_spi_rxBufferWrite)
        {
            size = (xbee_spi_rxBufferWrite - xbee_spi_rxBufferRead);
        }
        else
        {
            size = (xbee_spi_RX_BUFFER_SIZE - xbee_spi_rxBufferRead) + xbee_spi_rxBufferWrite;
        }

        xbee_spi_EnableRxInt();

    #else

        /* We can only know if there is data in the RX FIFO */
        size = (0u != (xbee_spi_RX_STATUS_REG & xbee_spi_STS_RX_FIFO_NOT_EMPTY)) ? 1u : 0u;

    #endif /* (xbee_spi_TX_SOFTWARE_BUF_ENABLED) */

    return(size);
}


/*******************************************************************************
* Function Name: xbee_spi_GetTxBufferSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes/words of data currently held in the TX buffer.
*  If TX Software Buffer not used then function return 0 - if FIFO empty, 1 - if
*  FIFO not full, 4 - if FIFO full. In another case function return size of TX
*  Software Buffer.
*
* Parameters:
*  None.
*
* Return:
*  Integer count of the number of bytes/words in the TX buffer.
*
* Global variables:
*  xbee_spi_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  xbee_spi_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
uint8  xbee_spi_GetTxBufferSize(void) 
{
    uint8 size;

    #if(xbee_spi_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        xbee_spi_DisableTxInt();

        if(xbee_spi_txBufferRead == xbee_spi_txBufferWrite)
        {
            size = 0u;
        }
        else if(xbee_spi_txBufferRead < xbee_spi_txBufferWrite)
        {
            size = (xbee_spi_txBufferWrite - xbee_spi_txBufferRead);
        }
        else
        {
            size = (xbee_spi_TX_BUFFER_SIZE - xbee_spi_txBufferRead) + xbee_spi_txBufferWrite;
        }

        xbee_spi_EnableTxInt();

    #else

        size = xbee_spi_TX_STATUS_REG;

        if(0u != (size & xbee_spi_STS_TX_FIFO_EMPTY))
        {
            size = 0u;
        }
        else if(0u != (size & xbee_spi_STS_TX_FIFO_NOT_FULL))
        {
            size = 1u;
        }
        else
        {
            size = xbee_spi_FIFO_SIZE;
        }

    #endif /* (xbee_spi_TX_SOFTWARE_BUF_ENABLED) */

    return(size);
}


/*******************************************************************************
* Function Name: xbee_spi_ClearRxBuffer
********************************************************************************
*
* Summary:
*  Clear the RX RAM buffer by setting the read and write pointers both to zero.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  xbee_spi_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer, modified every function
*  call - resets to zero.
*  xbee_spi_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified every function call -
*  resets to zero.
*
* Theory:
*  Setting the pointers to zero makes the system believe there is no data to
*  read and writing will resume at address 0 overwriting any data that may have
*  remained in the RAM.
*
* Side Effects:
*  Any received data not read from the RAM buffer will be lost when overwritten.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void xbee_spi_ClearRxBuffer(void) 
{
    /* Clear Hardware RX FIFO */
    while(0u !=(xbee_spi_RX_STATUS_REG & xbee_spi_STS_RX_FIFO_NOT_EMPTY))
    {
        (void) CY_GET_REG8(xbee_spi_RXDATA_PTR);
    }

    #if(xbee_spi_RX_SOFTWARE_BUF_ENABLED)
        /* Disable RX interrupt to protect global veriables */
        xbee_spi_DisableRxInt();

        xbee_spi_rxBufferFull  = 0u;
        xbee_spi_rxBufferRead  = 0u;
        xbee_spi_rxBufferWrite = 0u;

        xbee_spi_EnableRxInt();
    #endif /* (xbee_spi_RX_SOFTWARE_BUF_ENABLED) */
}


/*******************************************************************************
* Function Name: xbee_spi_ClearTxBuffer
********************************************************************************
*
* Summary:
*  Clear the TX RAM buffer by setting the read and write pointers both to zero.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  xbee_spi_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer, modified every function
*  call - resets to zero.
*  xbee_spi_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified every function call -
*  resets to zero.
*
* Theory:
*  Setting the pointers to zero makes the system believe there is no data to
*  read and writing will resume at address 0 overwriting any data that may have
*  remained in the RAM.
*
* Side Effects:
*  Any data not yet transmitted from the RAM buffer will be lost when
*  overwritten.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void xbee_spi_ClearTxBuffer(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    /* Clear TX FIFO */
    xbee_spi_AUX_CONTROL_DP0_REG |= ((uint8)  xbee_spi_TX_FIFO_CLR);
    xbee_spi_AUX_CONTROL_DP0_REG &= ((uint8) ~xbee_spi_TX_FIFO_CLR);

    #if(xbee_spi_USE_SECOND_DATAPATH)
        /* Clear TX FIFO for 2nd Datapath */
        xbee_spi_AUX_CONTROL_DP1_REG |= ((uint8)  xbee_spi_TX_FIFO_CLR);
        xbee_spi_AUX_CONTROL_DP1_REG &= ((uint8) ~xbee_spi_TX_FIFO_CLR);
    #endif /* (xbee_spi_USE_SECOND_DATAPATH) */
    CyExitCriticalSection(enableInterrupts);

    #if(xbee_spi_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        xbee_spi_DisableTxInt();

        xbee_spi_txBufferFull  = 0u;
        xbee_spi_txBufferRead  = 0u;
        xbee_spi_txBufferWrite = 0u;

        /* Buffer is EMPTY: disable TX FIFO NOT FULL interrupt */
        xbee_spi_TX_STATUS_MASK_REG &= ((uint8) ~xbee_spi_STS_TX_FIFO_NOT_FULL);

        xbee_spi_EnableTxInt();
    #endif /* (xbee_spi_TX_SOFTWARE_BUF_ENABLED) */
}


#if(0u != xbee_spi_BIDIRECTIONAL_MODE)
    /*******************************************************************************
    * Function Name: xbee_spi_TxEnable
    ********************************************************************************
    *
    * Summary:
    *  If the SPI master is configured to use a single bi-directional pin then this
    *  will set the bi-directional pin to transmit.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void xbee_spi_TxEnable(void) 
    {
        xbee_spi_CONTROL_REG |= xbee_spi_CTRL_TX_SIGNAL_EN;
    }


    /*******************************************************************************
    * Function Name: xbee_spi_TxDisable
    ********************************************************************************
    *
    * Summary:
    *  If the SPI master is configured to use a single bi-directional pin then this
    *  will set the bi-directional pin to receive.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void xbee_spi_TxDisable(void) 
    {
        xbee_spi_CONTROL_REG &= ((uint8) ~xbee_spi_CTRL_TX_SIGNAL_EN);
    }

#endif /* (0u != xbee_spi_BIDIRECTIONAL_MODE) */


/*******************************************************************************
* Function Name: xbee_spi_PutArray
********************************************************************************
*
* Summary:
*  Write available data from ROM/RAM to the TX buffer while space is available
*  in the TX buffer. Keep trying until all data is passed to the TX buffer.
*
* Parameters:
*  *buffer: Pointer to the location in RAM containing the data to send
*  byteCount: The number of bytes to move to the transmit buffer.
*
* Return:
*  None.
*
* Side Effects:
*  Will stay in this routine until all data has been sent.  May get locked in
*  this loop if data is not being initiated by the master if there is not
*  enough room in the TX FIFO.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void xbee_spi_PutArray(const uint8 buffer[], uint8 byteCount)
                                                                          
{
    uint8 bufIndex;

    bufIndex = 0u;

    while(byteCount > 0u)
    {
        xbee_spi_WriteTxData(buffer[bufIndex]);
        bufIndex++;
        byteCount--;
    }
}


/*******************************************************************************
* Function Name: xbee_spi_ClearFIFO
********************************************************************************
*
* Summary:
*  Clear the RX and TX FIFO's of all data for a fresh start.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
void xbee_spi_ClearFIFO(void) 
{
    uint8 enableInterrupts;

    /* Clear Hardware RX FIFO */
    while(0u !=(xbee_spi_RX_STATUS_REG & xbee_spi_STS_RX_FIFO_NOT_EMPTY))
    {
        (void) CY_GET_REG8(xbee_spi_RXDATA_PTR);
    }

    enableInterrupts = CyEnterCriticalSection();
    /* Clear TX FIFO */
    xbee_spi_AUX_CONTROL_DP0_REG |= ((uint8)  xbee_spi_TX_FIFO_CLR);
    xbee_spi_AUX_CONTROL_DP0_REG &= ((uint8) ~xbee_spi_TX_FIFO_CLR);

    #if(xbee_spi_USE_SECOND_DATAPATH)
        /* Clear TX FIFO for 2nd Datapath */
        xbee_spi_AUX_CONTROL_DP1_REG |= ((uint8)  xbee_spi_TX_FIFO_CLR);
        xbee_spi_AUX_CONTROL_DP1_REG &= ((uint8) ~xbee_spi_TX_FIFO_CLR);
    #endif /* (xbee_spi_USE_SECOND_DATAPATH) */
    CyExitCriticalSection(enableInterrupts);
}


/* Following functions are for version Compatibility, they are obsolete.
*  Please do not use it in new projects.
*/


/*******************************************************************************
* Function Name: xbee_spi_EnableInt
********************************************************************************
*
* Summary:
*  Enable internal interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void xbee_spi_EnableInt(void) 
{
    xbee_spi_EnableRxInt();
    xbee_spi_EnableTxInt();
}


/*******************************************************************************
* Function Name: xbee_spi_DisableInt
********************************************************************************
*
* Summary:
*  Disable internal interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void xbee_spi_DisableInt(void) 
{
    xbee_spi_DisableTxInt();
    xbee_spi_DisableRxInt();
}


/*******************************************************************************
* Function Name: xbee_spi_SetInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void xbee_spi_SetInterruptMode(uint8 intSrc) 
{
    xbee_spi_TX_STATUS_MASK_REG  = (intSrc & ((uint8) ~xbee_spi_STS_SPI_IDLE));
    xbee_spi_RX_STATUS_MASK_REG  =  intSrc;
}


/*******************************************************************************
* Function Name: xbee_spi_ReadStatus
********************************************************************************
*
* Summary:
*  Read the status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the status register.
*
* Global variables:
*  xbee_spi_swStatus - used to store in software status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the status register for error detection
*  and flow control.
*
* Side Effects:
*  Clear status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 xbee_spi_ReadStatus(void) 
{
    uint8 tmpStatus;

    #if(xbee_spi_TX_SOFTWARE_BUF_ENABLED || xbee_spi_RX_SOFTWARE_BUF_ENABLED)

        xbee_spi_DisableInt();

        tmpStatus  = xbee_spi_GET_STATUS_RX(xbee_spi_swStatusRx);
        tmpStatus |= xbee_spi_GET_STATUS_TX(xbee_spi_swStatusTx);
        tmpStatus &= ((uint8) ~xbee_spi_STS_SPI_IDLE);

        xbee_spi_swStatusTx = 0u;
        xbee_spi_swStatusRx = 0u;

        xbee_spi_EnableInt();

    #else

        tmpStatus  = xbee_spi_RX_STATUS_REG;
        tmpStatus |= xbee_spi_TX_STATUS_REG;
        tmpStatus &= ((uint8) ~xbee_spi_STS_SPI_IDLE);

    #endif /* (xbee_spi_TX_SOFTWARE_BUF_ENABLED || xbee_spi_RX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/* [] END OF FILE */
