/*******************************************************************************
* File Name: power_comp.c
* Version 2.0
*
* Description:
*  This file provides the power management source code APIs for the
*  Comparator.
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

#include "power_comp.h"

static power_comp_backupStruct power_comp_backup;


/*******************************************************************************
* Function Name: power_comp_SaveConfig
********************************************************************************
*
* Summary:
*  Save the current user configuration
*
* Parameters:
*  void:
*
* Return:
*  void
*
*******************************************************************************/
void power_comp_SaveConfig(void) 
{
    /* Empty since all are system reset for retention flops */
}


/*******************************************************************************
* Function Name: power_comp_RestoreConfig
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
********************************************************************************/
void power_comp_RestoreConfig(void) 
{
    /* Empty since all are system reset for retention flops */    
}


/*******************************************************************************
* Function Name: power_comp_Sleep
********************************************************************************
*
* Summary:
*  Stop and Save the user configuration
*
* Parameters:
*  void:
*
* Return:
*  void
*
* Global variables:
*  power_comp_backup.enableState:  Is modified depending on the enable 
*   state of the block before entering sleep mode.
*
*******************************************************************************/
void power_comp_Sleep(void) 
{
    /* Save Comp's enable state */    
    if(power_comp_ACT_PWR_EN == (power_comp_PWRMGR & power_comp_ACT_PWR_EN))
    {
        /* Comp is enabled */
        power_comp_backup.enableState = 1u;
    }
    else
    {
        /* Comp is disabled */
        power_comp_backup.enableState = 0u;
    }    
    
    power_comp_Stop();
    power_comp_SaveConfig();
}


/*******************************************************************************
* Function Name: power_comp_Wakeup
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
*  power_comp_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void power_comp_Wakeup(void) 
{
    power_comp_RestoreConfig();
    
    if(power_comp_backup.enableState == 1u)
    {
        /* Enable Comp's operation */
        power_comp_Enable();

    } /* Do nothing if Comp was disabled before */ 
}


/* [] END OF FILE */
