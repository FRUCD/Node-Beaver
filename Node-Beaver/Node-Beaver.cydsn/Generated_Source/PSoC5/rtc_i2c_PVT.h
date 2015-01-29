/*******************************************************************************
* File Name: .h
* Version 3.30
*
* Description:
*  This file provides private constants and parameter values for the I2C
*  component.
*
* Note:
*
********************************************************************************
* Copyright 2012, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_I2C_PVT_rtc_i2c_H)
#define CY_I2C_PVT_rtc_i2c_H

#include "rtc_i2c.h"

#define rtc_i2c_TIMEOUT_ENABLED_INC    (0u)
#if(0u != rtc_i2c_TIMEOUT_ENABLED_INC)
    #include "rtc_i2c_TMOUT.h"
#endif /* (0u != rtc_i2c_TIMEOUT_ENABLED_INC) */


/**********************************
*   Variables with external linkage
**********************************/

extern rtc_i2c_BACKUP_STRUCT rtc_i2c_backup;

extern volatile uint8 rtc_i2c_state;   /* Current state of I2C FSM */

/* Master variables */
#if(rtc_i2c_MODE_MASTER_ENABLED)
    extern volatile uint8 rtc_i2c_mstrStatus;   /* Master Status byte  */
    extern volatile uint8 rtc_i2c_mstrControl;  /* Master Control byte */

    /* Transmit buffer variables */
    extern volatile uint8 * rtc_i2c_mstrRdBufPtr;   /* Pointer to Master Read buffer */
    extern volatile uint8   rtc_i2c_mstrRdBufSize;  /* Master Read buffer size       */
    extern volatile uint8   rtc_i2c_mstrRdBufIndex; /* Master Read buffer Index      */

    /* Receive buffer variables */
    extern volatile uint8 * rtc_i2c_mstrWrBufPtr;   /* Pointer to Master Write buffer */
    extern volatile uint8   rtc_i2c_mstrWrBufSize;  /* Master Write buffer size       */
    extern volatile uint8   rtc_i2c_mstrWrBufIndex; /* Master Write buffer Index      */

#endif /* (rtc_i2c_MODE_MASTER_ENABLED) */

/* Slave variables */
#if(rtc_i2c_MODE_SLAVE_ENABLED)
    extern volatile uint8 rtc_i2c_slStatus;         /* Slave Status  */

    /* Transmit buffer variables */
    extern volatile uint8 * rtc_i2c_slRdBufPtr;     /* Pointer to Transmit buffer  */
    extern volatile uint8   rtc_i2c_slRdBufSize;    /* Slave Transmit buffer size  */
    extern volatile uint8   rtc_i2c_slRdBufIndex;   /* Slave Transmit buffer Index */

    /* Receive buffer variables */
    extern volatile uint8 * rtc_i2c_slWrBufPtr;     /* Pointer to Receive buffer  */
    extern volatile uint8   rtc_i2c_slWrBufSize;    /* Slave Receive buffer size  */
    extern volatile uint8   rtc_i2c_slWrBufIndex;   /* Slave Receive buffer Index */

    #if(rtc_i2c_SW_ADRR_DECODE)
        extern volatile uint8 rtc_i2c_slAddress;     /* Software address variable */
    #endif   /* (rtc_i2c_SW_ADRR_DECODE) */

#endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */

#if((rtc_i2c_FF_IMPLEMENTED) && (rtc_i2c_WAKEUP_ENABLED))
    extern volatile uint8 rtc_i2c_wakeupSource;
#endif /* ((rtc_i2c_FF_IMPLEMENTED) && (rtc_i2c_WAKEUP_ENABLED)) */


#endif /* CY_I2C_PVT_rtc_i2c_H */


/* [] END OF FILE */
