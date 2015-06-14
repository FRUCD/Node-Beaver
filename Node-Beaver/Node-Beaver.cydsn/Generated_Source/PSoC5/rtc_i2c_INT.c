/*******************************************************************************
* File Name: rtc_i2c_INT.c
* Version 3.50
*
* Description:
*  This file provides the source code of Interrupt Service Routine (ISR)
*  for the I2C component.
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "rtc_i2c_PVT.h"


/*******************************************************************************
*  Place your includes, defines and code here.
********************************************************************************/
/* `#START rtc_i2c_ISR_intc` */

/* `#END` */


/*******************************************************************************
* Function Name: rtc_i2c_ISR
********************************************************************************
*
* Summary:
*  The handler for the I2C interrupt. The slave and master operations are
*  handled here.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
CY_ISR(rtc_i2c_ISR)
{
#if (rtc_i2c_MODE_SLAVE_ENABLED)
   uint8  tmp8;
#endif  /* (rtc_i2c_MODE_SLAVE_ENABLED) */

    uint8  tmpCsr;

#if(rtc_i2c_TIMEOUT_FF_ENABLED)
    if(0u != rtc_i2c_TimeoutGetStatus())
    {
        rtc_i2c_TimeoutReset();
        rtc_i2c_state = rtc_i2c_SM_EXIT_IDLE;
        /* rtc_i2c_CSR_REG should be cleared after reset */
    }
#endif /* (rtc_i2c_TIMEOUT_FF_ENABLED) */


    tmpCsr = rtc_i2c_CSR_REG;      /* Make copy as interrupts clear */

#if(rtc_i2c_MODE_MULTI_MASTER_SLAVE_ENABLED)
    if(rtc_i2c_CHECK_START_GEN(rtc_i2c_MCSR_REG))
    {
        rtc_i2c_CLEAR_START_GEN;

        /* Set transfer complete and error flags */
        rtc_i2c_mstrStatus |= (rtc_i2c_MSTAT_ERR_XFER |
                                        rtc_i2c_GET_MSTAT_CMPLT);

        /* Slave was addressed */
        rtc_i2c_state = rtc_i2c_SM_SLAVE;
    }
#endif /* (rtc_i2c_MODE_MULTI_MASTER_SLAVE_ENABLED) */


#if(rtc_i2c_MODE_MULTI_MASTER_ENABLED)
    if(rtc_i2c_CHECK_LOST_ARB(tmpCsr))
    {
        /* Set errors */
        rtc_i2c_mstrStatus |= (rtc_i2c_MSTAT_ERR_XFER     |
                                        rtc_i2c_MSTAT_ERR_ARB_LOST |
                                        rtc_i2c_GET_MSTAT_CMPLT);

        rtc_i2c_DISABLE_INT_ON_STOP; /* Interrupt on Stop is enabled by write */

        #if(rtc_i2c_MODE_MULTI_MASTER_SLAVE_ENABLED)
            if(rtc_i2c_CHECK_ADDRESS_STS(tmpCsr))
            {
                /* Slave was addressed */
                rtc_i2c_state = rtc_i2c_SM_SLAVE;
            }
            else
            {
                rtc_i2c_BUS_RELEASE;

                rtc_i2c_state = rtc_i2c_SM_EXIT_IDLE;
            }
        #else
            rtc_i2c_BUS_RELEASE;

            rtc_i2c_state = rtc_i2c_SM_EXIT_IDLE;

        #endif /* (rtc_i2c_MODE_MULTI_MASTER_SLAVE_ENABLED) */
    }
#endif /* (rtc_i2c_MODE_MULTI_MASTER_ENABLED) */

    /* Check for master operation mode */
    if(rtc_i2c_CHECK_SM_MASTER)
    {
    #if(rtc_i2c_MODE_MASTER_ENABLED)
        if(rtc_i2c_CHECK_BYTE_COMPLETE(tmpCsr))
        {
            switch (rtc_i2c_state)
            {
            case rtc_i2c_SM_MSTR_WR_ADDR:  /* After address is sent, write data */
            case rtc_i2c_SM_MSTR_RD_ADDR:  /* After address is sent, read data */

                tmpCsr &= ((uint8) ~rtc_i2c_CSR_STOP_STATUS); /* Clear Stop bit history on address phase */

                if(rtc_i2c_CHECK_ADDR_ACK(tmpCsr))
                {
                    /* Setup for transmit or receive of data */
                    if(rtc_i2c_state == rtc_i2c_SM_MSTR_WR_ADDR)   /* TRANSMIT data */
                    {
                        /* Check if at least one byte to transfer */
                        if(rtc_i2c_mstrWrBufSize > 0u)
                        {
                            /* Load the 1st data byte */
                            rtc_i2c_DATA_REG = rtc_i2c_mstrWrBufPtr[0u];
                            rtc_i2c_TRANSMIT_DATA;
                            rtc_i2c_mstrWrBufIndex = 1u;   /* Set index to 2nd element */

                            /* Set transmit state until done */
                            rtc_i2c_state = rtc_i2c_SM_MSTR_WR_DATA;
                        }
                        /* End of buffer: complete writing */
                        else if(rtc_i2c_CHECK_NO_STOP(rtc_i2c_mstrControl))
                        {
                            /* Set write complete and master halted */
                            rtc_i2c_mstrStatus |= (rtc_i2c_MSTAT_XFER_HALT |
                                                            rtc_i2c_MSTAT_WR_CMPLT);

                            rtc_i2c_state = rtc_i2c_SM_MSTR_HALT; /* Expect ReStart */
                            rtc_i2c_DisableInt();
                        }
                        else
                        {
                            rtc_i2c_ENABLE_INT_ON_STOP; /* Enable interrupt on Stop, to catch it */
                            rtc_i2c_GENERATE_STOP;
                        }
                    }
                    else  /* Master receive data */
                    {
                        rtc_i2c_READY_TO_READ; /* Release bus to read data */

                        rtc_i2c_state  = rtc_i2c_SM_MSTR_RD_DATA;
                    }
                }
                /* Address is NACKed */
                else if(rtc_i2c_CHECK_ADDR_NAK(tmpCsr))
                {
                    /* Set Address NAK error */
                    rtc_i2c_mstrStatus |= (rtc_i2c_MSTAT_ERR_XFER |
                                                    rtc_i2c_MSTAT_ERR_ADDR_NAK);

                    if(rtc_i2c_CHECK_NO_STOP(rtc_i2c_mstrControl))
                    {
                        rtc_i2c_mstrStatus |= (rtc_i2c_MSTAT_XFER_HALT |
                                                        rtc_i2c_GET_MSTAT_CMPLT);

                        rtc_i2c_state = rtc_i2c_SM_MSTR_HALT; /* Expect RESTART */
                        rtc_i2c_DisableInt();
                    }
                    else  /* Do normal Stop */
                    {
                        rtc_i2c_ENABLE_INT_ON_STOP; /* Enable interrupt on Stop, to catch it */
                        rtc_i2c_GENERATE_STOP;
                    }
                }
                else
                {
                    /* Address phase is not set for some reason: error */
                    #if(rtc_i2c_TIMEOUT_ENABLED)
                        /* Exit interrupt to take chance for timeout timer to handle this case */
                        rtc_i2c_DisableInt();
                        rtc_i2c_ClearPendingInt();
                    #else
                        /* Block execution flow: unexpected condition */
                        CYASSERT(0u != 0u);
                    #endif /* (rtc_i2c_TIMEOUT_ENABLED) */
                }
                break;

            case rtc_i2c_SM_MSTR_WR_DATA:

                if(rtc_i2c_CHECK_DATA_ACK(tmpCsr))
                {
                    /* Check if end of buffer */
                    if(rtc_i2c_mstrWrBufIndex  < rtc_i2c_mstrWrBufSize)
                    {
                        rtc_i2c_DATA_REG =
                                                 rtc_i2c_mstrWrBufPtr[rtc_i2c_mstrWrBufIndex];
                        rtc_i2c_TRANSMIT_DATA;
                        rtc_i2c_mstrWrBufIndex++;
                    }
                    /* End of buffer: complete writing */
                    else if(rtc_i2c_CHECK_NO_STOP(rtc_i2c_mstrControl))
                    {
                        /* Set write complete and master halted */
                        rtc_i2c_mstrStatus |= (rtc_i2c_MSTAT_XFER_HALT |
                                                        rtc_i2c_MSTAT_WR_CMPLT);

                        rtc_i2c_state = rtc_i2c_SM_MSTR_HALT;    /* Expect restart */
                        rtc_i2c_DisableInt();
                    }
                    else  /* Do normal Stop */
                    {
                        rtc_i2c_ENABLE_INT_ON_STOP;    /* Enable interrupt on Stop, to catch it */
                        rtc_i2c_GENERATE_STOP;
                    }
                }
                /* Last byte NAKed: end writing */
                else if(rtc_i2c_CHECK_NO_STOP(rtc_i2c_mstrControl))
                {
                    /* Set write complete, short transfer and master halted */
                    rtc_i2c_mstrStatus |= (rtc_i2c_MSTAT_ERR_XFER       |
                                                    rtc_i2c_MSTAT_ERR_SHORT_XFER |
                                                    rtc_i2c_MSTAT_XFER_HALT      |
                                                    rtc_i2c_MSTAT_WR_CMPLT);

                    rtc_i2c_state = rtc_i2c_SM_MSTR_HALT;    /* Expect ReStart */
                    rtc_i2c_DisableInt();
                }
                else  /* Do normal Stop */
                {
                    rtc_i2c_ENABLE_INT_ON_STOP;    /* Enable interrupt on Stop, to catch it */
                    rtc_i2c_GENERATE_STOP;

                    /* Set short transfer and error flag */
                    rtc_i2c_mstrStatus |= (rtc_i2c_MSTAT_ERR_SHORT_XFER |
                                                    rtc_i2c_MSTAT_ERR_XFER);
                }

                break;

            case rtc_i2c_SM_MSTR_RD_DATA:

                rtc_i2c_mstrRdBufPtr[rtc_i2c_mstrRdBufIndex] = rtc_i2c_DATA_REG;
                rtc_i2c_mstrRdBufIndex++;

                /* Check if end of buffer */
                if(rtc_i2c_mstrRdBufIndex < rtc_i2c_mstrRdBufSize)
                {
                    rtc_i2c_ACK_AND_RECEIVE;       /* ACK and receive byte */
                }
                /* End of buffer: complete reading */
                else if(rtc_i2c_CHECK_NO_STOP(rtc_i2c_mstrControl))
                {
                    /* Set read complete and master halted */
                    rtc_i2c_mstrStatus |= (rtc_i2c_MSTAT_XFER_HALT |
                                                    rtc_i2c_MSTAT_RD_CMPLT);

                    rtc_i2c_state = rtc_i2c_SM_MSTR_HALT;    /* Expect ReStart */
                    rtc_i2c_DisableInt();
                }
                else
                {
                    rtc_i2c_ENABLE_INT_ON_STOP;
                    rtc_i2c_NAK_AND_RECEIVE;       /* NACK and TRY to generate Stop */
                }
                break;

            default: /* This is an invalid state and should not occur */

            #if(rtc_i2c_TIMEOUT_ENABLED)
                /* Exit interrupt to take chance for timeout timer to handles this case */
                rtc_i2c_DisableInt();
                rtc_i2c_ClearPendingInt();
            #else
                /* Block execution flow: unexpected condition */
                CYASSERT(0u != 0u);
            #endif /* (rtc_i2c_TIMEOUT_ENABLED) */

                break;
            }
        }

        /* Catches Stop: end of transaction */
        if(rtc_i2c_CHECK_STOP_STS(tmpCsr))
        {
            rtc_i2c_mstrStatus |= rtc_i2c_GET_MSTAT_CMPLT;

            rtc_i2c_DISABLE_INT_ON_STOP;
            rtc_i2c_state = rtc_i2c_SM_IDLE;
        }
    #endif /* (rtc_i2c_MODE_MASTER_ENABLED) */
    }
    else if(rtc_i2c_CHECK_SM_SLAVE)
    {
    #if(rtc_i2c_MODE_SLAVE_ENABLED)

        if((rtc_i2c_CHECK_STOP_STS(tmpCsr)) || /* Stop || Restart */
           (rtc_i2c_CHECK_BYTE_COMPLETE(tmpCsr) && rtc_i2c_CHECK_ADDRESS_STS(tmpCsr)))
        {
            /* Catch end of master write transaction: use interrupt on Stop */
            /* The Stop bit history on address phase does not have correct state */
            if(rtc_i2c_SM_SL_WR_DATA == rtc_i2c_state)
            {
                rtc_i2c_DISABLE_INT_ON_STOP;

                rtc_i2c_slStatus &= ((uint8) ~rtc_i2c_SSTAT_WR_BUSY);
                rtc_i2c_slStatus |= ((uint8)  rtc_i2c_SSTAT_WR_CMPLT);

                rtc_i2c_state = rtc_i2c_SM_IDLE;
            }
        }

        if(rtc_i2c_CHECK_BYTE_COMPLETE(tmpCsr))
        {
            /* The address only issued after Start or ReStart: so check the address
               to catch these events:
                FF : sets an address phase with a byte_complete interrupt trigger.
                UDB: sets an address phase immediately after Start or ReStart. */
            if(rtc_i2c_CHECK_ADDRESS_STS(tmpCsr))
            {
            /* Check for software address detection */
            #if(rtc_i2c_SW_ADRR_DECODE)
                tmp8 = rtc_i2c_GET_SLAVE_ADDR(rtc_i2c_DATA_REG);

                if(tmp8 == rtc_i2c_slAddress)   /* Check for address match */
                {
                    if(0u != (rtc_i2c_DATA_REG & rtc_i2c_READ_FLAG))
                    {
                        /* Place code to prepare read buffer here                  */
                        /* `#START rtc_i2c_SW_PREPARE_READ_BUF_interrupt` */

                        /* `#END` */

                        /* Prepare next operation to read, get data and place in data register */
                        if(rtc_i2c_slRdBufIndex < rtc_i2c_slRdBufSize)
                        {
                            /* Load first data byte from array */
                            rtc_i2c_DATA_REG = rtc_i2c_slRdBufPtr[rtc_i2c_slRdBufIndex];
                            rtc_i2c_ACK_AND_TRANSMIT;
                            rtc_i2c_slRdBufIndex++;

                            rtc_i2c_slStatus |= rtc_i2c_SSTAT_RD_BUSY;
                        }
                        else    /* Overflow: provide 0xFF on bus */
                        {
                            rtc_i2c_DATA_REG = rtc_i2c_OVERFLOW_RETURN;
                            rtc_i2c_ACK_AND_TRANSMIT;

                            rtc_i2c_slStatus  |= (rtc_i2c_SSTAT_RD_BUSY |
                                                           rtc_i2c_SSTAT_RD_ERR_OVFL);
                        }

                        rtc_i2c_state = rtc_i2c_SM_SL_RD_DATA;
                    }
                    else  /* Write transaction: receive 1st byte */
                    {
                        rtc_i2c_ACK_AND_RECEIVE;
                        rtc_i2c_state = rtc_i2c_SM_SL_WR_DATA;

                        rtc_i2c_slStatus |= rtc_i2c_SSTAT_WR_BUSY;
                        rtc_i2c_ENABLE_INT_ON_STOP;
                    }
                }
                else
                {
                    /*     Place code to compare for additional address here    */
                    /* `#START rtc_i2c_SW_ADDR_COMPARE_interruptStart` */

                    /* `#END` */

                    rtc_i2c_NAK_AND_RECEIVE;   /* NACK address */

                    /* Place code to end of condition for NACK generation here */
                    /* `#START rtc_i2c_SW_ADDR_COMPARE_interruptEnd`  */

                    /* `#END` */
                }

            #else /* (rtc_i2c_HW_ADRR_DECODE) */

                if(0u != (rtc_i2c_DATA_REG & rtc_i2c_READ_FLAG))
                {
                    /* Place code to prepare read buffer here                  */
                    /* `#START rtc_i2c_HW_PREPARE_READ_BUF_interrupt` */

                    /* `#END` */

                    /* Prepare next operation to read, get data and place in data register */
                    if(rtc_i2c_slRdBufIndex < rtc_i2c_slRdBufSize)
                    {
                        /* Load first data byte from array */
                        rtc_i2c_DATA_REG = rtc_i2c_slRdBufPtr[rtc_i2c_slRdBufIndex];
                        rtc_i2c_ACK_AND_TRANSMIT;
                        rtc_i2c_slRdBufIndex++;

                        rtc_i2c_slStatus |= rtc_i2c_SSTAT_RD_BUSY;
                    }
                    else    /* Overflow: provide 0xFF on bus */
                    {
                        rtc_i2c_DATA_REG = rtc_i2c_OVERFLOW_RETURN;
                        rtc_i2c_ACK_AND_TRANSMIT;

                        rtc_i2c_slStatus  |= (rtc_i2c_SSTAT_RD_BUSY |
                                                       rtc_i2c_SSTAT_RD_ERR_OVFL);
                    }

                    rtc_i2c_state = rtc_i2c_SM_SL_RD_DATA;
                }
                else  /* Write transaction: receive 1st byte */
                {
                    rtc_i2c_ACK_AND_RECEIVE;
                    rtc_i2c_state = rtc_i2c_SM_SL_WR_DATA;

                    rtc_i2c_slStatus |= rtc_i2c_SSTAT_WR_BUSY;
                    rtc_i2c_ENABLE_INT_ON_STOP;
                }

            #endif /* (rtc_i2c_SW_ADRR_DECODE) */
            }
            /* Data states */
            /* Data master writes into slave */
            else if(rtc_i2c_state == rtc_i2c_SM_SL_WR_DATA)
            {
                if(rtc_i2c_slWrBufIndex < rtc_i2c_slWrBufSize)
                {
                    tmp8 = rtc_i2c_DATA_REG;
                    rtc_i2c_ACK_AND_RECEIVE;
                    rtc_i2c_slWrBufPtr[rtc_i2c_slWrBufIndex] = tmp8;
                    rtc_i2c_slWrBufIndex++;
                }
                else  /* of array: complete write, send NACK */
                {
                    rtc_i2c_NAK_AND_RECEIVE;

                    rtc_i2c_slStatus |= rtc_i2c_SSTAT_WR_ERR_OVFL;
                }
            }
            /* Data master reads from slave */
            else if(rtc_i2c_state == rtc_i2c_SM_SL_RD_DATA)
            {
                if(rtc_i2c_CHECK_DATA_ACK(tmpCsr))
                {
                    if(rtc_i2c_slRdBufIndex < rtc_i2c_slRdBufSize)
                    {
                         /* Get data from array */
                        rtc_i2c_DATA_REG = rtc_i2c_slRdBufPtr[rtc_i2c_slRdBufIndex];
                        rtc_i2c_TRANSMIT_DATA;
                        rtc_i2c_slRdBufIndex++;
                    }
                    else   /* Overflow: provide 0xFF on bus */
                    {
                        rtc_i2c_DATA_REG = rtc_i2c_OVERFLOW_RETURN;
                        rtc_i2c_TRANSMIT_DATA;

                        rtc_i2c_slStatus |= rtc_i2c_SSTAT_RD_ERR_OVFL;
                    }
                }
                else  /* Last byte was NACKed: read complete */
                {
                    /* Only NACK appears on bus */
                    rtc_i2c_DATA_REG = rtc_i2c_OVERFLOW_RETURN;
                    rtc_i2c_NAK_AND_TRANSMIT;

                    rtc_i2c_slStatus &= ((uint8) ~rtc_i2c_SSTAT_RD_BUSY);
                    rtc_i2c_slStatus |= ((uint8)  rtc_i2c_SSTAT_RD_CMPLT);

                    rtc_i2c_state = rtc_i2c_SM_IDLE;
                }
            }
            else
            {
            #if(rtc_i2c_TIMEOUT_ENABLED)
                /* Exit interrupt to take chance for timeout timer to handle this case */
                rtc_i2c_DisableInt();
                rtc_i2c_ClearPendingInt();
            #else
                /* Block execution flow: unexpected condition */
                CYASSERT(0u != 0u);
            #endif /* (rtc_i2c_TIMEOUT_ENABLED) */
            }
        }
    #endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */
    }
    else
    {
        /* The FSM skips master and slave processing: return to IDLE */
        rtc_i2c_state = rtc_i2c_SM_IDLE;
    }
}


#if ((rtc_i2c_FF_IMPLEMENTED) && (rtc_i2c_WAKEUP_ENABLED))
    /*******************************************************************************
    * Function Name: rtc_i2c_WAKEUP_ISR
    ********************************************************************************
    *
    * Summary:
    *  The interrupt handler to trigger after a wakeup.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(rtc_i2c_WAKEUP_ISR)
    {
         /* Set flag to notify that matched address is received */
        rtc_i2c_wakeupSource = 1u;

        /* SCL is stretched until the I2C_Wake() is called */
    }
#endif /* ((rtc_i2c_FF_IMPLEMENTED) && (rtc_i2c_WAKEUP_ENABLED)) */


/* [] END OF FILE */
