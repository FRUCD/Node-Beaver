/*******************************************************************************
* File Name: rtc_i2c_PM.c
* Version 3.30
*
* Description:
*  This file provides Low power mode APIs for I2C component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "rtc_i2c_PVT.h"

rtc_i2c_BACKUP_STRUCT rtc_i2c_backup =
{
    rtc_i2c_DISABLE, /* enableState */

    #if(rtc_i2c_FF_IMPLEMENTED)
        rtc_i2c_DEFAULT_XCFG,  /* xcfg */
        rtc_i2c_DEFAULT_CFG,   /* cfg  */

        #if(rtc_i2c_MODE_SLAVE_ENABLED)
            rtc_i2c_DEFAULT_ADDR, /* addr */
        #endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */

        #if(CY_PSOC5A)
            LO8(rtc_i2c_DEFAULT_DIVIDE_FACTOR),  /* div */
        #else
            LO8(rtc_i2c_DEFAULT_DIVIDE_FACTOR), /* div1 */
            HI8(rtc_i2c_DEFAULT_DIVIDE_FACTOR), /* div2 */
        #endif /* (CY_PSOC5A) */

    #else  /* (rtc_i2c_UDB_IMPLEMENTED) */
        rtc_i2c_DEFAULT_CFG,    /* control */

        #if(CY_UDB_V0)
            rtc_i2c_INT_ENABLE_MASK, /* aux_ctl */

            #if(rtc_i2c_MODE_SLAVE_ENABLED)
                rtc_i2c_DEFAULT_ADDR, /* addr_d0 */
            #endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */
        #endif /* (CY_UDB_V0) */
    #endif /* (rtc_i2c_FF_IMPLEMENTED) */

    #if(rtc_i2c_TIMEOUT_ENABLED)
        rtc_i2c_DEFAULT_TMOUT_PERIOD,
        rtc_i2c_DEFAULT_TMOUT_INTR_MASK,

        #if(rtc_i2c_TIMEOUT_PRESCALER_ENABLED && CY_UDB_V0)
            rtc_i2c_DEFAULT_TMOUT_PRESCALER_PRD,
        #endif /* (rtc_i2c_TIMEOUT_PRESCALER_ENABLED) */

    #endif /* (rtc_i2c_TIMEOUT_ENABLED) */
};

#if((rtc_i2c_FF_IMPLEMENTED) && (rtc_i2c_WAKEUP_ENABLED))
    volatile uint8 rtc_i2c_wakeupSource;
#endif /* ((rtc_i2c_FF_IMPLEMENTED) && (rtc_i2c_WAKEUP_ENABLED)) */


/*******************************************************************************
* Function Name: rtc_i2c_SaveConfig
********************************************************************************
*
* Summary:
*  Wakeup on address match enabled: disables I2C Master(if was enabled before go
*  to sleep), enables I2C backup regulator. Waits while on-going transaction be
*  will completed and I2C will be ready go to sleep. All incoming transaction
*  will be NACKed till power down will be asserted. The address match event
*  wakes up the chip.
*  Wakeup on address match disabled: saves I2C configuration and non-retention
*  register values.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  rtc_i2c_backup - used to save component configuration and
*       none-retention registers before enter sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void rtc_i2c_SaveConfig(void) 
{
    #if(rtc_i2c_FF_IMPLEMENTED)
        #if(rtc_i2c_WAKEUP_ENABLED)
            uint8 enableInterrupts;
        #endif /* (rtc_i2c_WAKEUP_ENABLED) */

        /* Store regiters in either Sleep mode */
        rtc_i2c_backup.cfg  = rtc_i2c_CFG_REG;
        rtc_i2c_backup.xcfg = rtc_i2c_XCFG_REG;

        #if(rtc_i2c_MODE_SLAVE_ENABLED)
            rtc_i2c_backup.addr = rtc_i2c_ADDR_REG;
        #endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */

        #if(CY_PSOC5A)
            rtc_i2c_backup.clkDiv   = rtc_i2c_CLKDIV_REG;
        #else
            rtc_i2c_backup.clkDiv1  = rtc_i2c_CLKDIV1_REG;
            rtc_i2c_backup.clkDiv2  = rtc_i2c_CLKDIV2_REG;
        #endif /* (CY_PSOC5A) */

        #if(rtc_i2c_WAKEUP_ENABLED)
            /* Need to disable Master */
            rtc_i2c_CFG_REG &= ((uint8) ~rtc_i2c_ENABLE_MASTER);

            /* Enable the I2C regulator backup */
            enableInterrupts = CyEnterCriticalSection();
            rtc_i2c_PWRSYS_CR1_REG |= rtc_i2c_PWRSYS_CR1_I2C_REG_BACKUP;
            CyExitCriticalSection(enableInterrupts);

            /* 1) Set force NACK to ignore I2C transactions
               2) Wait while I2C will be ready go to Sleep
               3) These bits are cleared on wake up */
            rtc_i2c_XCFG_REG |= rtc_i2c_XCFG_FORCE_NACK;
            while(0u == (rtc_i2c_XCFG_REG & rtc_i2c_XCFG_RDY_TO_SLEEP))
            {
                ; /* Wait when block is ready to Sleep */
            }

            /* Setup wakeup interrupt */
            rtc_i2c_DisableInt();
            (void) CyIntSetVector(rtc_i2c_ISR_NUMBER, &rtc_i2c_WAKEUP_ISR);
            rtc_i2c_wakeupSource = 0u;
            rtc_i2c_EnableInt();

        #endif /* (rtc_i2c_WAKEUP_ENABLED) */

    #else
        /* Store only address match bit */
        rtc_i2c_backup.control = (rtc_i2c_CFG_REG & rtc_i2c_CTRL_ANY_ADDRESS_MASK);

        #if(CY_UDB_V0)
            /* Store interrupt mask bits */
            rtc_i2c_backup.intMask = rtc_i2c_INT_MASK_REG;

            #if(rtc_i2c_MODE & rtc_i2c_MODE_SLAVE)
                rtc_i2c_backup.addr = rtc_i2c_ADDR_REG;
            #endif /* (rtc_i2c_MODE & rtc_i2c_MODE_SLAVE) */

        #endif /* (CY_UDB_V0) */

    #endif /* (rtc_i2c_FF_IMPLEMENTED) */

    #if(rtc_i2c_TIMEOUT_ENABLED)
        rtc_i2c_TimeoutSaveConfig();   /* Save Timeout config */
    #endif /* (rtc_i2c_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: rtc_i2c_Sleep
********************************************************************************
*
* Summary:
*  Wakeup on address match enabled: All incoming transaction will be NACKed till
*  power down will be asserted. The address match event wakes up the chip.
*  Wakeup on address match disabled: Disables active mode power template bits or
*  clock gating as appropriate. Saves I2C configuration and non-retention
*  register values.
*  Disables I2C interrupt.
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
void rtc_i2c_Sleep(void) 
{
    #if(rtc_i2c_WAKEUP_ENABLED)
        /* The I2C block should be always enabled if used as wakeup source */
        rtc_i2c_backup.enableState = rtc_i2c_DISABLE;

        #if(rtc_i2c_TIMEOUT_ENABLED)
            rtc_i2c_TimeoutStop();
        #endif /* (rtc_i2c_TIMEOUT_ENABLED) */

    #else

        rtc_i2c_backup.enableState = ((uint8) rtc_i2c_IS_ENABLED);

        if(rtc_i2c_IS_ENABLED)
        {
            rtc_i2c_Stop();
        }
    #endif /* (rtc_i2c_WAKEUP_ENABLED) */

    rtc_i2c_SaveConfig();
}


/*******************************************************************************
* Function Name: rtc_i2c_RestoreConfig
********************************************************************************
*
* Summary:
*  Wakeup on address match enabled: enables I2C Master (if was enabled before go
*  to sleep), disables I2C backup regulator.
*  Wakeup on address match disabled: Restores I2C configuration and
*  non-retention register values.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  rtc_i2c_backup - used to save component configuration and
*  none-retention registers before exit sleep mode.
*
*******************************************************************************/
void rtc_i2c_RestoreConfig(void) 
{
    #if(rtc_i2c_FF_IMPLEMENTED)
        uint8 enableInterrupts;

        if(rtc_i2c_CHECK_PWRSYS_I2C_BACKUP)    /* Enabled if was in Sleep */
        {
            /* Disable back-up regulator */
            enableInterrupts = CyEnterCriticalSection();
            rtc_i2c_PWRSYS_CR1_REG &= ((uint8) ~rtc_i2c_PWRSYS_CR1_I2C_REG_BACKUP);
            CyExitCriticalSection(enableInterrupts);

            /* Re-enable Master */
            rtc_i2c_CFG_REG = rtc_i2c_backup.cfg;
        }
        else /* The I2C_REG_BACKUP was cleaned by PM API: it means Hibernate or wake-up not set */
        {
            #if(rtc_i2c_WAKEUP_ENABLED)
                /* Disable power to I2C block before register restore */
                enableInterrupts = CyEnterCriticalSection();
                rtc_i2c_ACT_PWRMGR_REG  &= ((uint8) ~rtc_i2c_ACT_PWR_EN);
                rtc_i2c_STBY_PWRMGR_REG &= ((uint8) ~rtc_i2c_STBY_PWR_EN);
                CyExitCriticalSection(enableInterrupts);

                /* Enable component after restore complete */
                rtc_i2c_backup.enableState = rtc_i2c_ENABLE;
            #endif /* (rtc_i2c_WAKEUP_ENABLED) */

            /* Restore component registers: Hibernate disable power */
            rtc_i2c_XCFG_REG = rtc_i2c_backup.xcfg;
            rtc_i2c_CFG_REG  = rtc_i2c_backup.cfg;

            #if(rtc_i2c_MODE_SLAVE_ENABLED)
                rtc_i2c_ADDR_REG = rtc_i2c_backup.addr;
            #endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */

            #if(CY_PSOC5A)
                rtc_i2c_CLKDIV_REG  = rtc_i2c_backup.clkDiv;
            #else
                rtc_i2c_CLKDIV1_REG = rtc_i2c_backup.clkDiv1;
                rtc_i2c_CLKDIV2_REG = rtc_i2c_backup.clkDiv2;
            #endif /* (CY_PSOC5A) */
        }

        #if(rtc_i2c_WAKEUP_ENABLED)
            rtc_i2c_DisableInt();
            (void) CyIntSetVector(rtc_i2c_ISR_NUMBER, &rtc_i2c_ISR);
            if(0u != rtc_i2c_wakeupSource)
            {
                rtc_i2c_SetPendingInt();   /* Generate interrupt to process incomming transcation */
            }
            rtc_i2c_EnableInt();
        #endif /* (rtc_i2c_WAKEUP_ENABLED) */

    #else

        #if(CY_UDB_V0)
            uint8 enableInterrupts;

            rtc_i2c_INT_MASK_REG |= rtc_i2c_backup.intMask;

            enableInterrupts = CyEnterCriticalSection();
            rtc_i2c_INT_ENABLE_REG |= rtc_i2c_INT_ENABLE_MASK;
            CyExitCriticalSection(enableInterrupts);

            #if(rtc_i2c_MODE_MASTER_ENABLED)
                /* Restore Master Clock generator */
                rtc_i2c_MCLK_PRD_REG = rtc_i2c_DEFAULT_MCLK_PRD;
                rtc_i2c_MCLK_CMP_REG = rtc_i2c_DEFAULT_MCLK_CMP;
            #endif /* (rtc_i2c_MODE_MASTER_ENABLED) */

            #if(rtc_i2c_MODE_SLAVE_ENABLED)
                rtc_i2c_ADDR_REG = rtc_i2c_backup.addr;

                /* Restore slave bit counter period */
                rtc_i2c_PERIOD_REG = rtc_i2c_DEFAULT_PERIOD;
            #endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */

        #endif /* (CY_UDB_V0) */

        rtc_i2c_CFG_REG = rtc_i2c_backup.control;

    #endif /* (rtc_i2c_FF_IMPLEMENTED) */

    #if(rtc_i2c_TIMEOUT_ENABLED)
        rtc_i2c_TimeoutRestoreConfig();
    #endif /* (rtc_i2c_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: rtc_i2c_Wakeup
********************************************************************************
*
* Summary:
*  Wakeup on address match enabled: enables I2C Master (if was enabled before go
*  to sleep) and disables I2C backup regulator.
*  Wakeup on address match disabled: Restores I2C configuration and
*  non-retention register values. Restores Active mode power template bits or
*  clock gating as appropriate.
*  The I2C interrupt remains disabled after function call.
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
void rtc_i2c_Wakeup(void) 
{
    rtc_i2c_RestoreConfig();   /* Restore I2C register settings */

    /* Restore component enable state */
    if(0u != rtc_i2c_backup.enableState)
    {
        rtc_i2c_Enable();
        rtc_i2c_EnableInt();
    }
    else
    {
        #if(rtc_i2c_TIMEOUT_ENABLED)
            rtc_i2c_TimeoutEnable();
        #endif /* (rtc_i2c_TIMEOUT_ENABLED) */
    }
}


/* [] END OF FILE */
