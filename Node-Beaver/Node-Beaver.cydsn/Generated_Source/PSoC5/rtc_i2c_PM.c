/*******************************************************************************
* File Name: rtc_i2c_PM.c
* Version 3.40
*
* Description:
*  This file provides low power mode APIs for the I2C component.
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "rtc_i2c_PVT.h"

rtc_i2c_BACKUP_STRUCT rtc_i2c_backup =
{
    rtc_i2c_DISABLE,

#if (rtc_i2c_FF_IMPLEMENTED)
    rtc_i2c_DEFAULT_XCFG,
    rtc_i2c_DEFAULT_CFG,
    rtc_i2c_DEFAULT_ADDR,
    LO8(rtc_i2c_DEFAULT_DIVIDE_FACTOR),
    HI8(rtc_i2c_DEFAULT_DIVIDE_FACTOR),
#else  /* (rtc_i2c_UDB_IMPLEMENTED) */
    rtc_i2c_DEFAULT_CFG,
#endif /* (rtc_i2c_FF_IMPLEMENTED) */

#if (rtc_i2c_TIMEOUT_ENABLED)
    rtc_i2c_DEFAULT_TMOUT_PERIOD,
    rtc_i2c_DEFAULT_TMOUT_INTR_MASK,
#endif /* (rtc_i2c_TIMEOUT_ENABLED) */
};

#if ((rtc_i2c_FF_IMPLEMENTED) && (rtc_i2c_WAKEUP_ENABLED))
    volatile uint8 rtc_i2c_wakeupSource;
#endif /* ((rtc_i2c_FF_IMPLEMENTED) && (rtc_i2c_WAKEUP_ENABLED)) */


/*******************************************************************************
* Function Name: rtc_i2c_SaveConfig
********************************************************************************
*
* Summary:
*  The Enable wakeup from Sleep Mode selection influences this function
*  implementation:
*   Unchecked: Stores the component non-retention configuration registers.
*   Checked:   Disables the master, if it was enabled before, and enables
*              backup regulator of the I2C hardware. If a transaction intended
*              for component executes during this function call, it waits until
*              the current transaction is completed and I2C hardware is ready
*              to enter sleep mode. All subsequent I2C traffic is NAKed until
*              the device is put into sleep mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  rtc_i2c_backup - The global variable used to save the component
*                            configuration and non-retention registers before
*                            entering the sleep mode.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void rtc_i2c_SaveConfig(void) 
{
#if (rtc_i2c_FF_IMPLEMENTED)
    #if (rtc_i2c_WAKEUP_ENABLED)
        uint8 intState;
    #endif /* (rtc_i2c_WAKEUP_ENABLED) */

    /* Store registers before enter low power mode */
    rtc_i2c_backup.cfg     = rtc_i2c_CFG_REG;
    rtc_i2c_backup.xcfg    = rtc_i2c_XCFG_REG;
    rtc_i2c_backup.addr    = rtc_i2c_ADDR_REG;
    rtc_i2c_backup.clkDiv1 = rtc_i2c_CLKDIV1_REG;
    rtc_i2c_backup.clkDiv2 = rtc_i2c_CLKDIV2_REG;

#if (rtc_i2c_WAKEUP_ENABLED)
    /* Disable master */
    rtc_i2c_CFG_REG &= (uint8) ~rtc_i2c_ENABLE_MASTER;

    /* Enable backup regulator to keep block powered in low power mode */
    intState = CyEnterCriticalSection();
    rtc_i2c_PWRSYS_CR1_REG |= rtc_i2c_PWRSYS_CR1_I2C_REG_BACKUP;
    CyExitCriticalSection(intState);

    /* 1) Set force NACK to ignore I2C transactions;
    *  2) Wait unti I2C is ready go to Sleep; !!
    *  3) These bits are cleared on wake up.
    */
    /* Wait when block is ready for sleep */
    rtc_i2c_XCFG_REG |= rtc_i2c_XCFG_FORCE_NACK;
    while (0u == (rtc_i2c_XCFG_REG & rtc_i2c_XCFG_RDY_TO_SLEEP))
    {
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
#endif /* (rtc_i2c_FF_IMPLEMENTED) */

#if (rtc_i2c_TIMEOUT_ENABLED)
    rtc_i2c_TimeoutSaveConfig();
#endif /* (rtc_i2c_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: rtc_i2c_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred method to prepare the component before device enters
*  sleep mode. The Enable wakeup from Sleep Mode selection influences this
*  function implementation:
*   Unchecked: Checks current I2C component state, saves it, and disables the
*              component by calling I2C_Stop() if it is currently enabled.
*              I2C_SaveConfig() is then called to save the component
*              non-retention configuration registers.
*   Checked:   If a transaction intended for component executes during this
*              function call, it waits until the current transaction is
*              completed. All subsequent I2C traffic intended for component
*              is NAKed until the device is put to sleep mode. The address
*              match event wakes up the device.
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
void rtc_i2c_Sleep(void) 
{
#if (rtc_i2c_WAKEUP_ENABLED)
    /* Do not enable block after exit low power mode if it is wakeup source */
    rtc_i2c_backup.enableState = rtc_i2c_DISABLE;

    #if (rtc_i2c_TIMEOUT_ENABLED)
        rtc_i2c_TimeoutStop();
    #endif /* (rtc_i2c_TIMEOUT_ENABLED) */

#else
    /* Store enable state */
    rtc_i2c_backup.enableState = (uint8) rtc_i2c_IS_ENABLED;

    if (0u != rtc_i2c_backup.enableState)
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
*  The Enable wakeup from Sleep Mode selection influences this function
*  implementation:
*   Unchecked: Restores the component non-retention configuration registers
*              to the state they were in before I2C_Sleep() or I2C_SaveConfig()
*              was called.
*   Checked:   Disables the backup regulator of the I2C hardware. Sets up the
*              regular component interrupt handler and generates the component
*              interrupt if it was wake up source to release the bus and
*              continue in-coming I2C transaction.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  rtc_i2c_backup - The global variable used to save the component
*                            configuration and non-retention registers before
*                            exiting the sleep mode.
*
* Reentrant:
*  No.
*
* Side Effects:
*  Calling this function before rtc_i2c_SaveConfig() or
*  rtc_i2c_Sleep() will lead to unpredictable results.
*
*******************************************************************************/
void rtc_i2c_RestoreConfig(void) 
{
#if (rtc_i2c_FF_IMPLEMENTED)
    uint8 intState;

    if (rtc_i2c_CHECK_PWRSYS_I2C_BACKUP)
    /* Low power mode was Sleep - backup regulator is enabled */
    {
        /* Enable backup regulator in active mode */
        intState = CyEnterCriticalSection();
        rtc_i2c_PWRSYS_CR1_REG &= (uint8) ~rtc_i2c_PWRSYS_CR1_I2C_REG_BACKUP;
        CyExitCriticalSection(intState);

        /* Restore master */
        rtc_i2c_CFG_REG = rtc_i2c_backup.cfg;
    }
    else
    /* Low power mode was Hibernate - backup regulator is disabled. All registers are cleared */
    {
    #if (rtc_i2c_WAKEUP_ENABLED)
        /* Disable power to block before register restore */
        intState = CyEnterCriticalSection();
        rtc_i2c_ACT_PWRMGR_REG  &= (uint8) ~rtc_i2c_ACT_PWR_EN;
        rtc_i2c_STBY_PWRMGR_REG &= (uint8) ~rtc_i2c_STBY_PWR_EN;
        CyExitCriticalSection(intState);

        /* Enable component in I2C_Wakeup() after register restore */
        rtc_i2c_backup.enableState = rtc_i2c_ENABLE;
    #endif /* (rtc_i2c_WAKEUP_ENABLED) */

        /* Restore component registers after Hibernate */
        rtc_i2c_XCFG_REG    = rtc_i2c_backup.xcfg;
        rtc_i2c_CFG_REG     = rtc_i2c_backup.cfg;
        rtc_i2c_ADDR_REG    = rtc_i2c_backup.addr;
        rtc_i2c_CLKDIV1_REG = rtc_i2c_backup.clkDiv1;
        rtc_i2c_CLKDIV2_REG = rtc_i2c_backup.clkDiv2;
    }

#if (rtc_i2c_WAKEUP_ENABLED)
    rtc_i2c_DisableInt();
    (void) CyIntSetVector(rtc_i2c_ISR_NUMBER, &rtc_i2c_ISR);
    if (0u != rtc_i2c_wakeupSource)
    {
        /* Generate interrupt to process incoming transaction */
        rtc_i2c_SetPendingInt();
    }
    rtc_i2c_EnableInt();
#endif /* (rtc_i2c_WAKEUP_ENABLED) */

#else
    rtc_i2c_CFG_REG = rtc_i2c_backup.control;
#endif /* (rtc_i2c_FF_IMPLEMENTED) */

#if (rtc_i2c_TIMEOUT_ENABLED)
    rtc_i2c_TimeoutRestoreConfig();
#endif /* (rtc_i2c_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: rtc_i2c_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred method to prepare the component for active mode
*  operation (when device exits sleep mode). The Enable wakeup from Sleep Mode
*  selection influences this function implementation:
*   Unchecked: Restores the component non-retention configuration registers
*              by calling I2C_RestoreConfig(). If the component was enabled
*              before the I2C_Sleep() function was called, I2C_Wakeup()
*              re-enables it.
*   Checked:   Enables  master functionality if it was enabled before sleep,
*              and disables the backup regulator of the I2C hardware.
*              The incoming transaction continues as soon as the regular
*              I2C interrupt handler is set up (global interrupts has to be
*              enabled to service I2C component interrupt).
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
* Side Effects:
*  Calling this function before rtc_i2c_SaveConfig() or
*  rtc_i2c_Sleep() will lead to unpredictable results.
*
*******************************************************************************/
void rtc_i2c_Wakeup(void) 
{
    rtc_i2c_RestoreConfig();

    /* Restore component enable state */
    if (0u != rtc_i2c_backup.enableState)
    {
        rtc_i2c_Enable();
        rtc_i2c_EnableInt();
    }
    else
    {
    #if (rtc_i2c_TIMEOUT_ENABLED)
        rtc_i2c_TimeoutEnable();
    #endif /* (rtc_i2c_TIMEOUT_ENABLED) */
    }
}


/* [] END OF FILE */
