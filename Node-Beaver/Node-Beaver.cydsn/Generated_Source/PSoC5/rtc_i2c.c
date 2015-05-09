/*******************************************************************************
* File Name: rtc_i2c.c
* Version 3.40
*
* Description:
*  This file provides the source code of APIs for the I2C component.
*  The actual protocol and operation code resides in the interrupt service
*  routine file.
*
*******************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "rtc_i2c_PVT.h"


/**********************************
*      System variables
**********************************/

uint8 rtc_i2c_initVar = 0u; /* Defines if component was initialized */

volatile uint8 rtc_i2c_state;  /* Current state of I2C FSM */


/*******************************************************************************
* Function Name: rtc_i2c_Init
********************************************************************************
*
* Summary:
*  Initializes I2C registers with initial values provided from customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void rtc_i2c_Init(void) 
{
#if (rtc_i2c_FF_IMPLEMENTED)
    /* Configure fixed function block */
    rtc_i2c_CFG_REG  = rtc_i2c_DEFAULT_CFG;
    rtc_i2c_XCFG_REG = rtc_i2c_DEFAULT_XCFG;
    rtc_i2c_ADDR_REG = rtc_i2c_DEFAULT_ADDR;
    rtc_i2c_CLKDIV1_REG = LO8(rtc_i2c_DEFAULT_DIVIDE_FACTOR);
    rtc_i2c_CLKDIV2_REG = HI8(rtc_i2c_DEFAULT_DIVIDE_FACTOR);

#else
    uint8 intState;

    /* Configure control and interrupt sources */
    rtc_i2c_CFG_REG      = rtc_i2c_DEFAULT_CFG;
    rtc_i2c_INT_MASK_REG = rtc_i2c_DEFAULT_INT_MASK;

    /* Enable interrupt generation in status */
    intState = CyEnterCriticalSection();
    rtc_i2c_INT_ENABLE_REG |= rtc_i2c_INTR_ENABLE;
    CyExitCriticalSection(intState);

    /* Configure bit counter */
    #if (rtc_i2c_MODE_SLAVE_ENABLED)
        rtc_i2c_PERIOD_REG = rtc_i2c_DEFAULT_PERIOD;
    #endif  /* (rtc_i2c_MODE_SLAVE_ENABLED) */

    /* Configure clock generator */
    #if (rtc_i2c_MODE_MASTER_ENABLED)
        rtc_i2c_MCLK_PRD_REG = rtc_i2c_DEFAULT_MCLK_PRD;
        rtc_i2c_MCLK_CMP_REG = rtc_i2c_DEFAULT_MCLK_CMP;
    #endif /* (rtc_i2c_MODE_MASTER_ENABLED) */
#endif /* (rtc_i2c_FF_IMPLEMENTED) */

#if (rtc_i2c_TIMEOUT_ENABLED)
    rtc_i2c_TimeoutInit();
#endif /* (rtc_i2c_TIMEOUT_ENABLED) */

    /* Configure internal interrupt */
    CyIntDisable    (rtc_i2c_ISR_NUMBER);
    CyIntSetPriority(rtc_i2c_ISR_NUMBER, rtc_i2c_ISR_PRIORITY);
    #if (rtc_i2c_INTERN_I2C_INTR_HANDLER)
        (void) CyIntSetVector(rtc_i2c_ISR_NUMBER, &rtc_i2c_ISR);
    #endif /* (rtc_i2c_INTERN_I2C_INTR_HANDLER) */

    /* Set FSM to default state */
    rtc_i2c_state = rtc_i2c_SM_IDLE;

#if (rtc_i2c_MODE_SLAVE_ENABLED)
    /* Clear status and buffers index */
    rtc_i2c_slStatus = 0u;
    rtc_i2c_slRdBufIndex = 0u;
    rtc_i2c_slWrBufIndex = 0u;

    /* Configure matched address */
    rtc_i2c_SlaveSetAddress(rtc_i2c_DEFAULT_ADDR);
#endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */

#if (rtc_i2c_MODE_MASTER_ENABLED)
    /* Clear status and buffers index */
    rtc_i2c_mstrStatus = 0u;
    rtc_i2c_mstrRdBufIndex = 0u;
    rtc_i2c_mstrWrBufIndex = 0u;
#endif /* (rtc_i2c_MODE_MASTER_ENABLED) */
}


/*******************************************************************************
* Function Name: rtc_i2c_Enable
********************************************************************************
*
* Summary:
*  Enables I2C operations.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  None.
*
*******************************************************************************/
void rtc_i2c_Enable(void) 
{
#if (rtc_i2c_FF_IMPLEMENTED)
    uint8 intState;

    /* Enable power to block */
    intState = CyEnterCriticalSection();
    rtc_i2c_ACT_PWRMGR_REG  |= rtc_i2c_ACT_PWR_EN;
    rtc_i2c_STBY_PWRMGR_REG |= rtc_i2c_STBY_PWR_EN;
    CyExitCriticalSection(intState);
#else
    #if (rtc_i2c_MODE_SLAVE_ENABLED)
        /* Enable bit counter */
        uint8 intState = CyEnterCriticalSection();
        rtc_i2c_COUNTER_AUX_CTL_REG |= rtc_i2c_CNT7_ENABLE;
        CyExitCriticalSection(intState);
    #endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */

    /* Enable slave or master bits */
    rtc_i2c_CFG_REG |= rtc_i2c_ENABLE_MS;
#endif /* (rtc_i2c_FF_IMPLEMENTED) */

#if (rtc_i2c_TIMEOUT_ENABLED)
    rtc_i2c_TimeoutEnable();
#endif /* (rtc_i2c_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: rtc_i2c_Start
********************************************************************************
*
* Summary:
*  Starts the I2C hardware. Enables Active mode power template bits or clock
*  gating as appropriate. It is required to be executed before I2C bus
*  operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  This component automatically enables its interrupt.  If I2C is enabled !
*  without the interrupt enabled, it can lock up the I2C bus.
*
* Global variables:
*  rtc_i2c_initVar - This variable is used to check the initial
*                             configuration, modified on the first
*                             function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void rtc_i2c_Start(void) 
{
    if (0u == rtc_i2c_initVar)
    {
        rtc_i2c_Init();
        rtc_i2c_initVar = 1u; /* Component initialized */
    }

    rtc_i2c_Enable();
    rtc_i2c_EnableInt();
}


/*******************************************************************************
* Function Name: rtc_i2c_Stop
********************************************************************************
*
* Summary:
*  Disables I2C hardware and disables I2C interrupt. Disables Active mode power
*  template bits or clock gating as appropriate.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void rtc_i2c_Stop(void) 
{
    rtc_i2c_DisableInt();

#if (rtc_i2c_TIMEOUT_ENABLED)
    rtc_i2c_TimeoutStop();
#endif  /* End (rtc_i2c_TIMEOUT_ENABLED) */

#if (rtc_i2c_FF_IMPLEMENTED)
    {
        uint8 intState;
        uint16 blockResetCycles;

        /* Store registers effected by block disable */
        rtc_i2c_backup.addr    = rtc_i2c_ADDR_REG;
        rtc_i2c_backup.clkDiv1 = rtc_i2c_CLKDIV1_REG;
        rtc_i2c_backup.clkDiv2 = rtc_i2c_CLKDIV2_REG;

        /* Calculate number of cycles to reset block */
        blockResetCycles = ((uint16) ((uint16) rtc_i2c_CLKDIV2_REG << 8u) | rtc_i2c_CLKDIV1_REG) + 1u;

        /* Disable block */
        rtc_i2c_CFG_REG &= (uint8) ~rtc_i2c_CFG_EN_SLAVE;
        /* Wait for block reset before disable power */
        CyDelayCycles((uint32) blockResetCycles);

        /* Disable power to block */
        intState = CyEnterCriticalSection();
        rtc_i2c_ACT_PWRMGR_REG  &= (uint8) ~rtc_i2c_ACT_PWR_EN;
        rtc_i2c_STBY_PWRMGR_REG &= (uint8) ~rtc_i2c_STBY_PWR_EN;
        CyExitCriticalSection(intState);

        /* Enable block */
        rtc_i2c_CFG_REG |= (uint8) rtc_i2c_ENABLE_MS;

        /* Restore registers effected by block disable. Ticket ID#198004 */
        rtc_i2c_ADDR_REG    = rtc_i2c_backup.addr;
        rtc_i2c_ADDR_REG    = rtc_i2c_backup.addr;
        rtc_i2c_CLKDIV1_REG = rtc_i2c_backup.clkDiv1;
        rtc_i2c_CLKDIV2_REG = rtc_i2c_backup.clkDiv2;
    }
#else

    /* Disable slave or master bits */
    rtc_i2c_CFG_REG &= (uint8) ~rtc_i2c_ENABLE_MS;

#if (rtc_i2c_MODE_SLAVE_ENABLED)
    {
        /* Disable bit counter */
        uint8 intState = CyEnterCriticalSection();
        rtc_i2c_COUNTER_AUX_CTL_REG &= (uint8) ~rtc_i2c_CNT7_ENABLE;
        CyExitCriticalSection(intState);
    }
#endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */

    /* Clear interrupt source register */
    (void) rtc_i2c_CSR_REG;
#endif /* (rtc_i2c_FF_IMPLEMENTED) */

    /* Disable interrupt on stop (enabled by write transaction) */
    rtc_i2c_DISABLE_INT_ON_STOP;
    rtc_i2c_ClearPendingInt();

    /* Reset FSM to default state */
    rtc_i2c_state = rtc_i2c_SM_IDLE;

    /* Clear busy statuses */
#if (rtc_i2c_MODE_SLAVE_ENABLED)
    rtc_i2c_slStatus &= (uint8) ~(rtc_i2c_SSTAT_RD_BUSY | rtc_i2c_SSTAT_WR_BUSY);
#endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */
}


/* [] END OF FILE */
