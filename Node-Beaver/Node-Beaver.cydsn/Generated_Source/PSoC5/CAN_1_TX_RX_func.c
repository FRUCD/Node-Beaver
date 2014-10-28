/*******************************************************************************
* File Name: CAN_1_TX_RX_func.c
* Version 2.30
*
* Description:
*  There are fucntions process "Full" Receive and Transmit mailboxes:
*     - CAN_1_SendMsg0-7();
*     - CAN_1_ReceiveMsg0-15();
*  Transmition of message, and receive routine for "Basic" mailboxes:
*     - CAN_1_SendMsg();
*     - CAN_1_TxCancel();
*     - CAN_1_ReceiveMsg();
*
*  Note:
*   None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CAN_1.h"

/* `#START TX_RX_FUNCTION` */

/* `#END` */


/*******************************************************************************
* FUNCTION NAME:   CAN_1_SendMsg
********************************************************************************
*
* Summary:
*  This function Send Message from one of Basic mailboxes. Function loop through
*  the transmit message buffer designed as Basic CAN mailboxes for first free
*  available and send from it. The number of retries is limited.
*
* Parameters:
*  message: Pointer to structure that contain all required data to send message.
*
* Return:
*  Indication if message has been sent.
*   Define                             Description
*    CYRET_SUCCESS                      Function passed successfully
*    CAN_1_FAIL              Function failed
*
*******************************************************************************/
uint8 CAN_1_SendMsg(const CAN_1_TX_MSG *message) 
{
    uint8 i, j, shift;
    uint8 retry = 0u;
    uint8 result = CAN_1_FAIL;
    uint32 regTemp;

    while (retry < CAN_1_RETRY_NUMBER)
    {
        shift = 1u;
        for (i = 0u; i < CAN_1_NUMBER_OF_TX_MAILBOXES; i++)
        {
            /* Find Basic TX mailboxes */
            if ((CAN_1_TX_MAILBOX_TYPE & shift) == 0u)
            {
                /* Find free mailbox */
                if ((CAN_1_BUF_SR_REG.byte[2] & shift) == 0u)
                {
                    regTemp = 0u;

                    /* Set message parameters */                   
                    if ((message->ide) == CAN_1_STANDARD_MESSAGE)
                    {
                        CAN_1_SET_TX_ID_STANDARD_MSG(i, message->id);                        
                    }
                    else
                    {
                        regTemp = CAN_1_TX_IDE_MASK;
                        CAN_1_SET_TX_ID_EXTENDED_MSG(i, message->id);
                    }
                    if (message->dlc < CAN_1_TX_DLC_MAX_VALUE)
                    {
                        regTemp |= ((uint32) message->dlc) << CAN_1_TWO_BYTE_OFFSET;
                    }
                    else
                    {
                        regTemp |= CAN_1_TX_DLC_UPPER_VALUE;
                    }
                    if ((message->irq) != CAN_1_TRANSMIT_INT_DISABLE)
                    {
                        regTemp |= CAN_1_TX_INT_ENABLE_MASK;    /* Transmit Interrupt Enable */
                    }

                    for (j = 0u; (j < message->dlc) && (j < CAN_1_TX_DLC_MAX_VALUE); j++)
                    {
                        CAN_1_TX_DATA_BYTE(i, j) = message->msg->byte[j];
                    }
                    
    /* Disable isr */
    CyIntDisable(CAN_1_ISR_NUMBER);

                    /* WPN[23] and WPN[3] set to 1 for write to CAN Control reg */
                    CY_SET_REG32((reg32 *) &CAN_1_TX[i].txcmd, (regTemp | CAN_1_TX_WPN_SET));
                    CY_SET_REG32((reg32 *) &CAN_1_TX[i].txcmd, CAN_1_SEND_MESSAGE);
                    
    /* Enable isr */
    CyIntEnable(CAN_1_ISR_NUMBER);

                    result = CYRET_SUCCESS;
                }
            }
            shift <<= 1u;
            if (result == CYRET_SUCCESS)
            {
                break;
            }
        }
        if (result == CYRET_SUCCESS)
        {
            break;
        }
        else
        {
            retry++;
        }
    }

    return (result);
}


/*******************************************************************************
* FUNCTION NAME:   CAN_1_TxCancel
********************************************************************************
*
* Summary:
*  This function cancel transmission of a message that has been queued for
*  transmitted. Values between 0 and 15 are valid.
*
* Parameters:
*  bufferId: Mailbox number.
*
* Return:
*  None.
*
*******************************************************************************/
void CAN_1_TxCancel(uint8 bufferId) 
{
    if (bufferId < CAN_1_NUMBER_OF_TX_MAILBOXES)
    {
        CAN_1_TX_ABORT_MESSAGE(bufferId);
    }
}


#if (CAN_1_TX0_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_SendMsg0
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Transmit Message 0. Function check
    *  if mailbox 0 doesn't already have an un-transmitted messages waiting for
    *  arbitration. If not initiate transmission of the message.
    *  Only generated for Transmit mailbox designed as Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Indication if Message has been sent.
    *   Define                             Description
    *    CYRET_SUCCESS                      Function passed successfully
    *    CAN_1_FAIL              Function failed
    *
    *******************************************************************************/
    uint8 CAN_1_SendMsg0(void) 
    {
        uint8 result = CYRET_SUCCESS;
        
        if ((CAN_1_TX[0u].txcmd.byte[0u] & CAN_1_TX_REQUEST_PENDING) ==
            CAN_1_TX_REQUEST_PENDING)
        {
            result = CAN_1_FAIL;
        }
        else
        {
            /* `#START MESSAGE_0_TRASMITTED` */

            /* `#END` */
            
            CY_SET_REG32((reg32 *) &CAN_1_TX[0u].txcmd, CAN_1_SEND_MESSAGE);
        }
    
        return (result);
    }    
#endif /* CAN_1_TX0_FUNC_ENABLE */


#if (CAN_1_TX1_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_SendMsg1
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Transmit Message 1. Function check
    *  if mailbox 1 doesn't already have an un-transmitted messages waiting for
    *  arbitration. If not initiate transmission of the message.
    *  Only generated for Transmit mailbox designed as Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Indication if Message has been sent.
    *   Define                             Description
    *    CYRET_SUCCESS                      Function passed successfully
    *    CAN_1_FAIL              Function failed
    *
    *******************************************************************************/
    uint8 CAN_1_SendMsg1(void) 
    {
        uint8 result = CYRET_SUCCESS;
        
        if ((CAN_1_TX[1u].txcmd.byte[0u] & CAN_1_TX_REQUEST_PENDING) ==
            CAN_1_TX_REQUEST_PENDING)
        {
            result = CAN_1_FAIL;
        }
        else
        {
            /* `#START MESSAGE_1_TRASMITTED` */

            /* `#END` */
            
            CY_SET_REG32((reg32 *) & CAN_1_TX[1u].txcmd, CAN_1_SEND_MESSAGE);
        }
    
        return (result);
    }    
#endif /* CAN_1_TX1_FUNC_ENABLE */


#if (CAN_1_TX2_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_SendMsg2
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Transmit Message 2. Function check
    *  if mailbox 2 doesn't already have an un-transmitted messages waiting for
    *  arbitration. If not initiate transmission of the message.
    *  Only generated for Transmit mailbox designed as Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Indication if Message has been sent.
    *   Define                             Description
    *    CYRET_SUCCESS                      Function passed successfully
    *    CAN_1_FAIL              Function failed
    *
    *******************************************************************************/
    uint8 CAN_1_SendMsg2(void) 
    {
        uint8 result = CYRET_SUCCESS;
        
        if ((CAN_1_TX[2u].txcmd.byte[0u] & CAN_1_TX_REQUEST_PENDING) ==
            CAN_1_TX_REQUEST_PENDING)
        {
            result = CAN_1_FAIL;
        }
        else
        {
            /* `#START MESSAGE_2_TRASMITTED` */

            /* `#END` */
            
            CY_SET_REG32((reg32 *) & CAN_1_TX[2u].txcmd, CAN_1_SEND_MESSAGE);
        }
    
        return (result);
    }
#endif /* CAN_1_TX2_FUNC_ENABLE */


#if (CAN_1_TX3_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_SendMsg3
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Transmit Message 3. Function check
    *  if mailbox 3 doesn't already have an un-transmitted messages waiting for
    *  arbitration. If not initiate transmission of the message.
    *  Only generated for Transmit mailbox designed as Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Indication if Message has been sent.
    *   Define                             Description
    *    CYRET_SUCCESS                      Function passed successfully
    *    CAN_1_FAIL              Function failed
    *
    *******************************************************************************/
    uint8 CAN_1_SendMsg3(void) 
    {
        uint8 result = CYRET_SUCCESS;
        
        if ((CAN_1_TX[3u].txcmd.byte[0u] & CAN_1_TX_REQUEST_PENDING) ==
            CAN_1_TX_REQUEST_PENDING)
        {
            result = CAN_1_FAIL;
        }
        else
        {
            /* `#START MESSAGE_3_TRASMITTED` */

            /* `#END` */
            
            CY_SET_REG32((reg32 *) & CAN_1_TX[3u].txcmd, CAN_1_SEND_MESSAGE);
        }
    
        return (result);
    }    
#endif /* CAN_1_TX3_FUNC_ENABLE */


#if (CAN_1_TX4_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_SendMsg4
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Transmit Message 4. Function check if mailbox
    *  4 doesn't already have an un-transmitted messages waiting for arbitration. 
    *  If not initiate transmission of the message. Only generated for Transmit 
    *  mailbox designed as Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Indication if Message has been sent.
    *   Define                             Description
    *    CYRET_SUCCESS                      Function passed successfully
    *    CAN_1_FAIL              Function failed
    *
    *******************************************************************************/
    uint8 CAN_1_SendMsg4(void) 
    {
        uint8 result = CYRET_SUCCESS;
        
        if ((CAN_1_TX[4u].txcmd.byte[0u] & CAN_1_TX_REQUEST_PENDING) ==
            CAN_1_TX_REQUEST_PENDING)
        {
            result = CAN_1_FAIL;
        }
        else
        {
            /* `#START MESSAGE_4_TRASMITTED` */

            /* `#END` */
            
            CY_SET_REG32((reg32 *) & CAN_1_TX[4u].txcmd, CAN_1_SEND_MESSAGE);
        }
    
        return (result);
    }    
#endif /* CAN_1_TX4_FUNC_ENABLE */


#if (CAN_1_TX5_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_SendMsg5
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Transmit Message 5. Function check
    *  if mailbox 5 doesn't already have an un-transmitted messages waiting for
    *  arbitration. If not initiate transmission of the message. Only generated for
    *  Transmit mailbox designed as Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Indication if Message has been sent.
    *   Define                             Description
    *    CYRET_SUCCESS                      Function passed successfully
    *    CAN_1_FAIL              Function failed
    *
    *******************************************************************************/
    uint8 CAN_1_SendMsg5(void) 
    {
        uint8 result = CYRET_SUCCESS;
        
        if ((CAN_1_TX[5u].txcmd.byte[0u] & CAN_1_TX_REQUEST_PENDING) ==
            CAN_1_TX_REQUEST_PENDING)
        {
            result = CAN_1_FAIL;
        }
        else
        {
            /* `#START MESSAGE_5_TRASMITTED` */

            /* `#END` */
            
            CY_SET_REG32((reg32 *) & CAN_1_TX[5u].txcmd, CAN_1_SEND_MESSAGE);
        }
    
        return (result);
    }    
#endif /* CAN_1_TX5_FUNC_ENABLE */


#if (CAN_1_TX6_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_SendMsg6
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Transmit Message 6. Function check
    *  if mailbox 6 doesn't already have an un-transmitted messages waiting for
    *  arbitration. If not initiate transmission of the message. Only generated for
    *  Transmit mailbox designed as Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Indication if Message has been sent.
    *   Define                             Description
    *    CYRET_SUCCESS                      Function passed successfully
    *    CAN_1_FAIL              Function failed
    *
    *******************************************************************************/
    uint8 CAN_1_SendMsg6(void) 
    {
        uint8 result = CYRET_SUCCESS;
        
        if ((CAN_1_TX[6u].txcmd.byte[0u] & CAN_1_TX_REQUEST_PENDING) ==
            CAN_1_TX_REQUEST_PENDING)
        {
            result = CAN_1_FAIL;
        }
        else
        {
            /* `#START MESSAGE_6_TRASMITTED` */

            /* `#END` */
            
            CY_SET_REG32((reg32 *) & CAN_1_TX[6u].txcmd, CAN_1_SEND_MESSAGE);
        }
    
        return (result);
    }    
#endif /* CAN_1_TX6_FUNC_ENABLE */


#if (CAN_1_TX7_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_SendMsg7)
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Transmit Message 7. Function check
    *  if mailbox 7 doesn't already have an un-transmitted messages waiting for
    *  arbitration. If not initiate transmission of the message. Only generated for
    *  Transmit mailbox designed as Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Indication if Message has been sent.
    *   Define                             Description
    *    CYRET_SUCCESS                      Function passed successfully
    *    CAN_1_FAIL              Function failed
    *
    *******************************************************************************/
    uint8 CAN_1_SendMsg7(void) 
    {
        uint8 result = CYRET_SUCCESS;
        
        if ((CAN_1_TX[7u].txcmd.byte[0u] & CAN_1_TX_REQUEST_PENDING) ==
            CAN_1_TX_REQUEST_PENDING)
        {
            result = CAN_1_FAIL;
        }
        else
        {
            /* `#START MESSAGE_7_TRASMITTED` */

            /* `#END` */
            
            CY_SET_REG32((reg32 *) & CAN_1_TX[7u].txcmd, CAN_1_SEND_MESSAGE);
        }
    
        return (result);
    }    
#endif /* CAN_1_TX7_FUNC_ENABLE */


/*******************************************************************************
* FUNCTION NAME:   CAN_1_ReceiveMsg
********************************************************************************
*
* Summary:
*  This function is entry point to Receive Message Interrupt for Basic 
*  mailboxes. Clears Receive particular Message interrupt flag. Only generated 
*  if one of Receive mailboxes designed as Basic.
*
* Parameters:
*  rxMailbox: Mailbox number that trig Receive Message Interrupt.
*
* Return:
*  None.
*
* Reentrant:
*  Depends on Customer code.
*
*******************************************************************************/
void CAN_1_ReceiveMsg(uint8 rxMailbox) 
{
    if ((CAN_1_RX[rxMailbox].rxcmd.byte[0u] & CAN_1_RX_ACK_MSG) == CAN_1_RX_ACK_MSG)
    {
        /* `#START MESSAGE_BASIC_RECEIVED` */

        /* `#END` */
        
        CAN_1_RX[rxMailbox].rxcmd.byte[0u] |= CAN_1_RX_ACK_MSG;
    }
}


#if (CAN_1_RX0_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_ReceiveMsg0
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Receive Message 0 Interrupt. Clears Receive
    *  Message 0 interrupt flag. Only generated for Receive mailbox designed as 
    *  Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void CAN_1_ReceiveMsg0(void) 
    {
        /* `#START MESSAGE_0_RECEIVED` */

        /* `#END` */
    
        CAN_1_RX[0u].rxcmd.byte[0u] |= CAN_1_RX_ACK_MSG;

    }
#endif /* CAN_1_RX0_FUNC_ENABLE */


#if (CAN_1_RX1_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:    CAN_1_ReceiveMsg1
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Receive Message 1 Interrupt. Clears Receive
    *  Message 1 interrupt flag. Only generated for Receive mailbox designed as 
    *  Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void CAN_1_ReceiveMsg1(void) 
    {
        /* `#START MESSAGE_1_RECEIVED` */

        /* `#END` */
    
        CAN_1_RX[1u].rxcmd.byte[0u] |= CAN_1_RX_ACK_MSG;

    }    
#endif /* CAN_1_RX1_FUNC_ENABLE */


#if (CAN_1_RX2_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_ReceiveMsg2
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Receive Message 2 Interrupt. Clears Receive
    *  Message 2 interrupt flag. Only generated for Receive mailbox designed as
    *  Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void CAN_1_ReceiveMsg2(void) 
    {
        /* `#START MESSAGE_2_RECEIVED` */

        /* `#END` */
    
        CAN_1_RX[2u].rxcmd.byte[0u] |= CAN_1_RX_ACK_MSG;

    }    
#endif /* CAN_1_RX2_FUNC_ENABLE */


#if (CAN_1_RX3_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_ReceiveMsg3
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Receive Message 3 Interrupt. Clears Receive
    *  Message 3 interrupt flag. Only generated for Receive mailbox designed as
    *  Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void CAN_1_ReceiveMsg3(void) 
    {
        /* `#START MESSAGE_3_RECEIVED` */

        /* `#END` */
    
        CAN_1_RX[3u].rxcmd.byte[0u] |= CAN_1_RX_ACK_MSG;

    }    
#endif /* CAN_1_RX3_FUNC_ENABLE */


#if (CAN_1_RX4_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_ReceiveMsg4
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Receive Message 4 Interrupt. Clears Receive
    *  Message 4 interrupt flag. Only generated for Receive mailbox designed as
    *  Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void CAN_1_ReceiveMsg4(void) 
    {
        /* `#START MESSAGE_4_RECEIVED` */

        /* `#END` */
    
        CAN_1_RX[4u].rxcmd.byte[0u] |= CAN_1_RX_ACK_MSG;

    }    
#endif /* CAN_1_RX4_FUNC_ENABLE */


#if (CAN_1_RX5_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_ReceiveMsg5
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Receive Message 5 Interrupt. Clears Receive
    *  Message 5 interrupt flag. Only generated for Receive mailbox designed as
    *  Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void CAN_1_ReceiveMsg5(void) 
    {
        /* `#START MESSAGE_5_RECEIVED` */

        /* `#END` */
    
        CAN_1_RX[5u].rxcmd.byte[0u] |= CAN_1_RX_ACK_MSG;

    }
#endif /* CAN_1_RX5_FUNC_ENABLE */


#if (CAN_1_RX6_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_ReceiveMsg6
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Receive Message 6 Interrupt. Clears Receive
    *  Message 6 interrupt flag. Only generated for Receive mailbox designed as
    *  Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void CAN_1_ReceiveMsg6(void) 
    {
        /* `#START MESSAGE_6_RECEIVED` */

        /* `#END` */
    
        CAN_1_RX[6u].rxcmd.byte[0u] |= CAN_1_RX_ACK_MSG;

    }    
#endif /* CAN_1_RX6_FUNC_ENABLE */


#if (CAN_1_RX7_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_ReceiveMsg7
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Receive Message 7 Interrupt. Clears Receive
    *  Message 7 interrupt flag. Only generated for Receive mailbox designed as
    *  Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void CAN_1_ReceiveMsg7(void) 
    {
        /* `#START MESSAGE_7_RECEIVED` */

        /* `#END` */
    
        CAN_1_RX[7u].rxcmd.byte[0u] |= CAN_1_RX_ACK_MSG;

    }    
#endif /* CAN_1_RX7_FUNC_ENABLE */


#if (CAN_1_RX8_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_ReceiveMsg8
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Receive Message 8 Interrupt. Clears Receive
    *  Message 8 interrupt flag. Only generated for Receive mailbox designed as
    *  Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void CAN_1_ReceiveMsg8(void) 
    {
        /* `#START MESSAGE_8_RECEIVED` */

        /* `#END` */
    
        CAN_1_RX[8u].rxcmd.byte[0u] |= CAN_1_RX_ACK_MSG;
        
    }    
#endif /* CAN_1_RX8_FUNC_ENABLE */


#if (CAN_1_RX9_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_ReceiveMsg9
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Receive Message 9 Interrupt. Clears Receive
    *  Message 9 interrupt flag. Only generated for Receive mailbox designed as
    *  Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void CAN_1_ReceiveMsg9(void) 
    {
        /* `#START MESSAGE_9_RECEIVED` */

        /* `#END` */
    
        CAN_1_RX[9u].rxcmd.byte[0u] |= CAN_1_RX_ACK_MSG;

    }    
#endif /* CAN_1_RX9_FUNC_ENABLE */


#if (CAN_1_RX10_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_ReceiveMsg10
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Receive Message 10 Interrupt. Clears Receive
    *  Message 10 interrupt flag. Only generated for Receive mailbox designed as
    *  Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void CAN_1_ReceiveMsg10(void) 
    {
        /* `#START MESSAGE_10_RECEIVED` */

        /* `#END` */
    
        CAN_1_RX[10u].rxcmd.byte[0u] |= CAN_1_RX_ACK_MSG;

    }    
#endif /* CAN_1_RX10_FUNC_ENABLE */


#if (CAN_1_RX11_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_ReceiveMsg11
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Receive Message 11 Interrupt. Clears Receive
    *  Message 11 interrupt flag. Only generated for Receive mailbox designed as
    *  Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void CAN_1_ReceiveMsg11(void) 
    {
        /* `#START MESSAGE_11_RECEIVED` */

        /* `#END` */
    
        CAN_1_RX[11u].rxcmd.byte[0u] |= CAN_1_RX_ACK_MSG;

    }    
#endif /* CAN_1_RX11_FUNC_ENABLE */


#if (CAN_1_RX12_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_ReceiveMsg12
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Receive Message 12 Interrupt. Clears Receive
    *  Message 12 interrupt flag. Only generated for Receive mailbox designed as
    *  Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void CAN_1_ReceiveMsg12(void) 
    {
        /* `#START MESSAGE_12_RECEIVED` */

        /* `#END` */
    
        CAN_1_RX[12u].rxcmd.byte[0u] |= CAN_1_RX_ACK_MSG;
        
    }    
#endif /* CAN_1_RX12_FUNC_ENABLE */


#if (CAN_1_RX13_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_ReceiveMsg13
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Receive Message 13 Interrupt. Clears Receive
    *  Message 13 interrupt flag. Only generated for Receive mailbox designed as
    *  Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void CAN_1_ReceiveMsg13(void) 
    {
        /* `#START MESSAGE_13_RECEIVED` */

        /* `#END` */
    
        CAN_1_RX[13u].rxcmd.byte[0u] |= CAN_1_RX_ACK_MSG;
 
    }    
#endif /* CAN_1_RX13_FUNC_ENABLE */


#if (CAN_1_RX14_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_ReceiveMsg14
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Receive Message 14 Interrupt. Clears Receive
    *  Message 14 interrupt flag. Only generated for Receive mailbox designed as
    *  Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void CAN_1_ReceiveMsg14(void) 
    {
        /* `#START MESSAGE_14_RECEIVED` */

        /* `#END` */
    
        CAN_1_RX[14u].rxcmd.byte[0u] |= CAN_1_RX_ACK_MSG;

    }    
#endif /* CAN_1_RX14_FUNC_ENABLE */


#if (CAN_1_RX15_FUNC_ENABLE)
    /*******************************************************************************
    * FUNCTION NAME:   CAN_1_ReceiveMsg15
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Receive Message 15 Interrupt. Clears Receive
    *  Message 15 interrupt flag. Only generated for Receive mailbox designed as
    *  Full.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void CAN_1_ReceiveMsg15(void) 
    {
        /* `#START MESSAGE_15_RECEIVED` */

        /* `#END` */
    
        CAN_1_RX[15u].rxcmd.byte[0u] |= CAN_1_RX_ACK_MSG;

    }    
#endif /* CAN_1_RX15_FUNC_ENABLE */


/* [] END OF FILE */
