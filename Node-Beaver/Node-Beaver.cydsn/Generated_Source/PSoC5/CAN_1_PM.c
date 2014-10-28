/*******************************************************************************
* File Name: CAN_1_PM.c
* Version 2.30
*
* Description:
*  This file contains the setup, control and status commands to support 
*  component operations in low power mode.  
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CAN_1.h"

static CAN_1_BACKUP_STRUCT CAN_1_backup = 
{ 0u,
  0u,
  CAN_1_INIT_INTERRUPT_MASK,
  CAN_1_MODE_MASK,
  (((uint32) ((uint32) CAN_1_SYNC_EDGE     << CAN_1_EDGE_MODE_SHIFT))     | 
   ((uint32) ((uint32) CAN_1_SAMPLING_MODE << CAN_1_SAMPLE_MODE_SHIFT))   |
   ((uint32) ((uint32) CAN_1_CFG_REG_SJW   << CAN_1_CFG_REG_SJW_SHIFT))   | 
   ((uint32) ((uint32) CAN_1_RESET_TYPE    << CAN_1_RESET_SHIFT))         |
   ((uint32) ((uint32) CAN_1_CFG_REG_TSEG2 << CAN_1_CFG_REG_TSEG2_SHIFT)) |
   ((uint32) ((uint32) CAN_1_CFG_REG_TSEG1 << CAN_1_CFG_REG_TSEG1_SHIFT)) |
   ((uint32) ((uint32) CAN_1_ARBITER       << CAN_1_ARBITER_SHIFT))       |
   ((uint32) ((uint32) CAN_1_BITRATE       << CAN_1_BITRATE_SHIFT))) };


/*******************************************************************************
* Function Name: CAN_1_SaveConfig
********************************************************************************
*
* Summary:
*  Save CAN configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
* 
* Global Variables:
*  CAN_1_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void CAN_1_SaveConfig(void) 
{
    CAN_1_backup.intSr = (CY_GET_REG32(CAN_1_INT_SR_PTR));
    CAN_1_backup.intEn = (CY_GET_REG32(CAN_1_INT_EN_PTR));
    CAN_1_backup.cmd = (CY_GET_REG32(CAN_1_CMD_PTR));
    CAN_1_backup.cfg = (CY_GET_REG32(CAN_1_CFG_PTR));
}


/*******************************************************************************
* Function Name: CAN_1_RestoreConfig
********************************************************************************
*
* Summary:
*  Restore CAN configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  CAN_1_backup - used when non-retention registers are restored.
*
* Side Effects:
*  If this API is called without first calling SaveConfig then in the following
*  registers will be default values from Customizer: CAN_1_INT_SR, 
*  CAN_1_INT_EN, CAN_1_CMD, CAN_1_CFG.
*
*******************************************************************************/
void CAN_1_RestoreConfig(void) 
{
    CY_SET_REG32(CAN_1_INT_SR_PTR, CAN_1_backup.intSr);
    CY_SET_REG32(CAN_1_INT_EN_PTR, CAN_1_backup.intEn);
    CY_SET_REG32(CAN_1_CMD_PTR, CAN_1_backup.cmd);
    CY_SET_REG32(CAN_1_CFG_PTR, CAN_1_backup.cfg);
}


/*******************************************************************************
* Function Name: CAN_1_Sleep
********************************************************************************
* 
* Summary:
*  Prepare CAN Component goes to sleep.
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  CAN_1_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void CAN_1_Sleep(void) 
{   
    if (0u != (CY_GET_REG32(CAN_1_CMD_PTR) & CAN_1_MODE_MASK))
    {
        CAN_1_backup.enableState = 1u;
    }
    else /* The CAN block is disabled */
    {
        CAN_1_backup.enableState = 0u;
    }
    
    CAN_1_SaveConfig();
    (void) CAN_1_Stop();
}


/*******************************************************************************
* Function Name: CAN_1_Wakeup
********************************************************************************
* 
* Summary:
*  Prepare CAN Component to wake up.
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  CAN_1_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void CAN_1_Wakeup(void) 
{           
    /* Enable power to CAN */
    CAN_1_PM_ACT_CFG_REG |= CAN_1_ACT_PWR_EN;
    
    CAN_1_RestoreConfig();
    
    if (CAN_1_backup.enableState != 0u)
    {
        /* Enable component's operation */
        (void) CAN_1_Enable();
    } /* Do nothing if component's block was disabled before */
}


/* [] END OF FILE */
