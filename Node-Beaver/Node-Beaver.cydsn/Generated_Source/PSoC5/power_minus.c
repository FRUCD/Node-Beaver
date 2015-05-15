/*******************************************************************************
* File Name: power_minus.c  
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
#include "power_minus.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 power_minus__PORT == 15 && ((power_minus__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: power_minus_Write
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
void power_minus_Write(uint8 value) 
{
    uint8 staticBits = (power_minus_DR & (uint8)(~power_minus_MASK));
    power_minus_DR = staticBits | ((uint8)(value << power_minus_SHIFT) & power_minus_MASK);
}


/*******************************************************************************
* Function Name: power_minus_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  power_minus_DM_STRONG     Strong Drive 
*  power_minus_DM_OD_HI      Open Drain, Drives High 
*  power_minus_DM_OD_LO      Open Drain, Drives Low 
*  power_minus_DM_RES_UP     Resistive Pull Up 
*  power_minus_DM_RES_DWN    Resistive Pull Down 
*  power_minus_DM_RES_UPDWN  Resistive Pull Up/Down 
*  power_minus_DM_DIG_HIZ    High Impedance Digital 
*  power_minus_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void power_minus_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(power_minus_0, mode);
}


/*******************************************************************************
* Function Name: power_minus_Read
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
*  Macro power_minus_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 power_minus_Read(void) 
{
    return (power_minus_PS & power_minus_MASK) >> power_minus_SHIFT;
}


/*******************************************************************************
* Function Name: power_minus_ReadDataReg
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
uint8 power_minus_ReadDataReg(void) 
{
    return (power_minus_DR & power_minus_MASK) >> power_minus_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(power_minus_INTSTAT) 

    /*******************************************************************************
    * Function Name: power_minus_ClearInterrupt
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
    uint8 power_minus_ClearInterrupt(void) 
    {
        return (power_minus_INTSTAT & power_minus_MASK) >> power_minus_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
