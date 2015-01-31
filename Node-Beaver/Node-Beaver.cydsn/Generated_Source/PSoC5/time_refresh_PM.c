/*******************************************************************************
* File Name: time_refresh_PM.c
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

#include "time_refresh.h"

static time_refresh_backupStruct time_refresh_backup;


/*******************************************************************************
* Function Name: time_refresh_SaveConfig
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
*  time_refresh_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void time_refresh_SaveConfig(void) 
{
    #if (!time_refresh_UsingFixedFunction)
        time_refresh_backup.TimerUdb = time_refresh_ReadCounter();
        time_refresh_backup.InterruptMaskValue = time_refresh_STATUS_MASK;
        #if (time_refresh_UsingHWCaptureCounter)
            time_refresh_backup.TimerCaptureCounter = time_refresh_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!time_refresh_UDB_CONTROL_REG_REMOVED)
            time_refresh_backup.TimerControlRegister = time_refresh_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: time_refresh_RestoreConfig
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
*  time_refresh_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void time_refresh_RestoreConfig(void) 
{   
    #if (!time_refresh_UsingFixedFunction)

        time_refresh_WriteCounter(time_refresh_backup.TimerUdb);
        time_refresh_STATUS_MASK =time_refresh_backup.InterruptMaskValue;
        #if (time_refresh_UsingHWCaptureCounter)
            time_refresh_SetCaptureCount(time_refresh_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!time_refresh_UDB_CONTROL_REG_REMOVED)
            time_refresh_WriteControlRegister(time_refresh_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: time_refresh_Sleep
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
*  time_refresh_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void time_refresh_Sleep(void) 
{
    #if(!time_refresh_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(time_refresh_CTRL_ENABLE == (time_refresh_CONTROL & time_refresh_CTRL_ENABLE))
        {
            /* Timer is enabled */
            time_refresh_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            time_refresh_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    time_refresh_Stop();
    time_refresh_SaveConfig();
}


/*******************************************************************************
* Function Name: time_refresh_Wakeup
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
*  time_refresh_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void time_refresh_Wakeup(void) 
{
    time_refresh_RestoreConfig();
    #if(!time_refresh_UDB_CONTROL_REG_REMOVED)
        if(time_refresh_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                time_refresh_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
