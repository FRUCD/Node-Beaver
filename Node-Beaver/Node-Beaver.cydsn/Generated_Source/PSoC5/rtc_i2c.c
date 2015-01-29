/*******************************************************************************
* File Name: rtc_i2c.c
* Version 3.30
*
* Description:
*  This file provides the source code of APIs for the I2C component.
*  Actual protocol and operation code resides in the interrupt service routine
*  file.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "rtc_i2c_PVT.h"


/**********************************
*      System variables
**********************************/

uint8 rtc_i2c_initVar = 0u;    /* Defines if component was initialized */

volatile uint8 rtc_i2c_state;  /* Current state of I2C FSM */


/*******************************************************************************
* Function Name: rtc_i2c_Init
********************************************************************************
*
* Summary:
*  Initializes I2C registers with initial values provided from customizer.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void rtc_i2c_Init(void) 
{
    #if(rtc_i2c_FF_IMPLEMENTED)
        rtc_i2c_CFG_REG  = rtc_i2c_DEFAULT_CFG;
        rtc_i2c_XCFG_REG = rtc_i2c_DEFAULT_XCFG;

        #if(CY_PSOC5A)
            rtc_i2c_CLKDIV_REG  = LO8(rtc_i2c_DEFAULT_DIVIDE_FACTOR);
        #else
            rtc_i2c_CLKDIV1_REG = LO8(rtc_i2c_DEFAULT_DIVIDE_FACTOR);
            rtc_i2c_CLKDIV2_REG = HI8(rtc_i2c_DEFAULT_DIVIDE_FACTOR);
        #endif /* (CY_PSOC5A) */

    #else
        uint8 enableInterrupts;

        rtc_i2c_CFG_REG      = rtc_i2c_DEFAULT_CFG;      /* control  */
        rtc_i2c_INT_MASK_REG = rtc_i2c_DEFAULT_INT_MASK; /* int_mask */

        /* Enable interrupts from block */
        enableInterrupts = CyEnterCriticalSection();
        rtc_i2c_INT_ENABLE_REG |= rtc_i2c_INTR_ENABLE; /* aux_ctl */
        CyExitCriticalSection(enableInterrupts);

        #if(rtc_i2c_MODE_MASTER_ENABLED)
            rtc_i2c_MCLK_PRD_REG = rtc_i2c_DEFAULT_MCLK_PRD;
            rtc_i2c_MCLK_CMP_REG = rtc_i2c_DEFAULT_MCLK_CMP;
         #endif /* (rtc_i2c_MODE_MASTER_ENABLED) */

        #if(rtc_i2c_MODE_SLAVE_ENABLED)
            rtc_i2c_PERIOD_REG = rtc_i2c_DEFAULT_PERIOD;
        #endif  /* (rtc_i2c_MODE_SLAVE_ENABLED) */

    #endif /* (rtc_i2c_FF_IMPLEMENTED) */

    #if(rtc_i2c_TIMEOUT_ENABLED)
        rtc_i2c_TimeoutInit();
    #endif /* (rtc_i2c_TIMEOUT_ENABLED) */

    /* Disable Interrupt and set vector and priority */
    CyIntDisable    (rtc_i2c_ISR_NUMBER);
    CyIntSetPriority(rtc_i2c_ISR_NUMBER, rtc_i2c_ISR_PRIORITY);
    #if(rtc_i2c_INTERN_I2C_INTR_HANDLER)
        (void) CyIntSetVector(rtc_i2c_ISR_NUMBER, &rtc_i2c_ISR);
    #endif /* (rtc_i2c_INTERN_I2C_INTR_HANDLER) */


    /* Put state machine in idle state */
    rtc_i2c_state = rtc_i2c_SM_IDLE;

    #if(rtc_i2c_MODE_SLAVE_ENABLED)
        /* Reset status and buffers index */
        rtc_i2c_SlaveClearReadBuf();
        rtc_i2c_SlaveClearWriteBuf();
        rtc_i2c_slStatus = 0u; /* Reset slave status */

        /* Set default address */
        rtc_i2c_SlaveSetAddress(rtc_i2c_DEFAULT_ADDR);
    #endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */

    #if(rtc_i2c_MODE_MASTER_ENABLED)
        /* Reset status and buffers index */
        rtc_i2c_MasterClearReadBuf();
        rtc_i2c_MasterClearWriteBuf();
        (void) rtc_i2c_MasterClearStatus();
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
*  None
*
* Return:
*  None
*
* Global variables:
*  None
*
*******************************************************************************/
void rtc_i2c_Enable(void) 
{
    #if(rtc_i2c_FF_IMPLEMENTED)
        uint8 enableInterrupts;

        /* Enable power to I2C FF block */
        enableInterrupts = CyEnterCriticalSection();
        rtc_i2c_ACT_PWRMGR_REG  |= rtc_i2c_ACT_PWR_EN;
        rtc_i2c_STBY_PWRMGR_REG |= rtc_i2c_STBY_PWR_EN;
        CyExitCriticalSection(enableInterrupts);

    #else

        #if(rtc_i2c_MODE_SLAVE_ENABLED)
            uint8 enableInterrupts;
        #endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */

        #if(rtc_i2c_MODE_SLAVE_ENABLED)
            /* Enable slave bit counter */
            enableInterrupts = CyEnterCriticalSection();
            rtc_i2c_COUNTER_AUX_CTL_REG |= rtc_i2c_CNT7_ENABLE;   /* aux_ctl */
            CyExitCriticalSection(enableInterrupts);
        #endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */

        rtc_i2c_CFG_REG |= rtc_i2c_ENABLE_MS;

    #endif /* (rtc_i2c_FF_IMPLEMENTED) */

    #if(rtc_i2c_TIMEOUT_ENABLED)
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
*  None
*
* Return:
*  None
*
* Side Effects:
*  This component automatically enables it's interrupt.  If I2C is enabled
*  without the interrupt enabled, it could lock up the I2C bus.
*
* Global variables:
*  rtc_i2c_initVar - used to check initial configuration, modified
*  on first function call.
*
* Reentrant:
*  No
*
*******************************************************************************/
void rtc_i2c_Start(void) 
{
    /* Initialize I2C registers, reset I2C buffer index and clears status */
    if(0u == rtc_i2c_initVar)
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
*  None
*
* Return:
*  None
*
*******************************************************************************/
void rtc_i2c_Stop(void) 
{
    #if((rtc_i2c_FF_IMPLEMENTED)  || \
        (rtc_i2c_UDB_IMPLEMENTED && rtc_i2c_MODE_SLAVE_ENABLED))
        uint8 enableInterrupts;
    #endif /* ((rtc_i2c_FF_IMPLEMENTED)  || \
               (rtc_i2c_UDB_IMPLEMENTED && rtc_i2c_MODE_SLAVE_ENABLED)) */

    rtc_i2c_DisableInt();

    rtc_i2c_DISABLE_INT_ON_STOP;   /* Interrupt on Stop can be enabled by write */
    (void) rtc_i2c_CSR_REG;        /* Clear CSR reg */
    
    #if(rtc_i2c_TIMEOUT_ENABLED)
        rtc_i2c_TimeoutStop();
    #endif  /* End (rtc_i2c_TIMEOUT_ENABLED) */

    #if(rtc_i2c_FF_IMPLEMENTED)
        #if(CY_PSOC3 || CY_PSOC5LP)
            /* Store registers which are held in reset when Master and Slave bits are cleared */
            #if(rtc_i2c_MODE_SLAVE_ENABLED)
                rtc_i2c_backup.addr = rtc_i2c_ADDR_REG;
            #endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */

            rtc_i2c_backup.clkDiv1  = rtc_i2c_CLKDIV1_REG;
            rtc_i2c_backup.clkDiv2  = rtc_i2c_CLKDIV2_REG;


            /* Reset FF block */
            rtc_i2c_CFG_REG &= ((uint8) ~rtc_i2c_ENABLE_MS);
            CyDelayUs(rtc_i2c_FF_RESET_DELAY);
            rtc_i2c_CFG_REG |= ((uint8)  rtc_i2c_ENABLE_MS);


            /* Restore registers */
            #if(rtc_i2c_MODE_SLAVE_ENABLED)
                rtc_i2c_ADDR_REG = rtc_i2c_backup.addr;
            #endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */

            rtc_i2c_CLKDIV1_REG = rtc_i2c_backup.clkDiv1;
            rtc_i2c_CLKDIV2_REG = rtc_i2c_backup.clkDiv2;

        #endif /* (CY_PSOC3 || CY_PSOC5LP) */

        /* Disable power to I2C block */
        enableInterrupts = CyEnterCriticalSection();
        rtc_i2c_ACT_PWRMGR_REG  &= ((uint8) ~rtc_i2c_ACT_PWR_EN);
        rtc_i2c_STBY_PWRMGR_REG &= ((uint8) ~rtc_i2c_STBY_PWR_EN);
        CyExitCriticalSection(enableInterrupts);

    #else

        #if(rtc_i2c_MODE_SLAVE_ENABLED)
            /* Disable slave bit counter */
            enableInterrupts = CyEnterCriticalSection();
            rtc_i2c_COUNTER_AUX_CTL_REG &= ((uint8) ~rtc_i2c_CNT7_ENABLE);
            CyExitCriticalSection(enableInterrupts);
        #endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */

        rtc_i2c_CFG_REG &= ((uint8) ~rtc_i2c_ENABLE_MS);

    #endif /* (rtc_i2c_FF_IMPLEMENTED) */

    rtc_i2c_ClearPendingInt();  /* Clear interrupt triggers on reset */

    rtc_i2c_state = rtc_i2c_SM_IDLE;  /* Reset software FSM */
}


/* [] END OF FILE */
