/*******************************************************************************
* File Name: millis_timer_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "millis_timer.h"

static millis_timer_backupStruct millis_timer_backup;


/*******************************************************************************
* Function Name: millis_timer_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  millis_timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void millis_timer_SaveConfig(void) 
{
    #if (!millis_timer_UsingFixedFunction)
        millis_timer_backup.TimerUdb = millis_timer_ReadCounter();
        millis_timer_backup.InterruptMaskValue = millis_timer_STATUS_MASK;
        #if (millis_timer_UsingHWCaptureCounter)
            millis_timer_backup.TimerCaptureCounter = millis_timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!millis_timer_UDB_CONTROL_REG_REMOVED)
            millis_timer_backup.TimerControlRegister = millis_timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: millis_timer_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  millis_timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void millis_timer_RestoreConfig(void) 
{   
    #if (!millis_timer_UsingFixedFunction)

        millis_timer_WriteCounter(millis_timer_backup.TimerUdb);
        millis_timer_STATUS_MASK =millis_timer_backup.InterruptMaskValue;
        #if (millis_timer_UsingHWCaptureCounter)
            millis_timer_SetCaptureCount(millis_timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!millis_timer_UDB_CONTROL_REG_REMOVED)
            millis_timer_WriteControlRegister(millis_timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: millis_timer_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  millis_timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void millis_timer_Sleep(void) 
{
    #if(!millis_timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(millis_timer_CTRL_ENABLE == (millis_timer_CONTROL & millis_timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            millis_timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            millis_timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    millis_timer_Stop();
    millis_timer_SaveConfig();
}


/*******************************************************************************
* Function Name: millis_timer_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  millis_timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void millis_timer_Wakeup(void) 
{
    millis_timer_RestoreConfig();
    #if(!millis_timer_UDB_CONTROL_REG_REMOVED)
        if(millis_timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                millis_timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
