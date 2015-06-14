/*******************************************************************************
* File Name: probe.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "probe.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 probe__PORT == 15 && ((probe__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: probe_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void probe_Write(uint8 value) 
{
    uint8 staticBits = (probe_DR & (uint8)(~probe_MASK));
    probe_DR = staticBits | ((uint8)(value << probe_SHIFT) & probe_MASK);
}


/*******************************************************************************
* Function Name: probe_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  probe_DM_STRONG     Strong Drive 
*  probe_DM_OD_HI      Open Drain, Drives High 
*  probe_DM_OD_LO      Open Drain, Drives Low 
*  probe_DM_RES_UP     Resistive Pull Up 
*  probe_DM_RES_DWN    Resistive Pull Down 
*  probe_DM_RES_UPDWN  Resistive Pull Up/Down 
*  probe_DM_DIG_HIZ    High Impedance Digital 
*  probe_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void probe_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(probe_0, mode);
}


/*******************************************************************************
* Function Name: probe_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro probe_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 probe_Read(void) 
{
    return (probe_PS & probe_MASK) >> probe_SHIFT;
}


/*******************************************************************************
* Function Name: probe_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 probe_ReadDataReg(void) 
{
    return (probe_DR & probe_MASK) >> probe_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(probe_INTSTAT) 

    /*******************************************************************************
    * Function Name: probe_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 probe_ClearInterrupt(void) 
    {
        return (probe_INTSTAT & probe_MASK) >> probe_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
