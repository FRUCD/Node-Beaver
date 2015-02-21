/*******************************************************************************
* File Name: xbee_miso.c  
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
#include "xbee_miso.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 xbee_miso__PORT == 15 && ((xbee_miso__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: xbee_miso_Write
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
void xbee_miso_Write(uint8 value) 
{
    uint8 staticBits = (xbee_miso_DR & (uint8)(~xbee_miso_MASK));
    xbee_miso_DR = staticBits | ((uint8)(value << xbee_miso_SHIFT) & xbee_miso_MASK);
}


/*******************************************************************************
* Function Name: xbee_miso_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  xbee_miso_DM_STRONG     Strong Drive 
*  xbee_miso_DM_OD_HI      Open Drain, Drives High 
*  xbee_miso_DM_OD_LO      Open Drain, Drives Low 
*  xbee_miso_DM_RES_UP     Resistive Pull Up 
*  xbee_miso_DM_RES_DWN    Resistive Pull Down 
*  xbee_miso_DM_RES_UPDWN  Resistive Pull Up/Down 
*  xbee_miso_DM_DIG_HIZ    High Impedance Digital 
*  xbee_miso_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void xbee_miso_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(xbee_miso_0, mode);
}


/*******************************************************************************
* Function Name: xbee_miso_Read
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
*  Macro xbee_miso_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 xbee_miso_Read(void) 
{
    return (xbee_miso_PS & xbee_miso_MASK) >> xbee_miso_SHIFT;
}


/*******************************************************************************
* Function Name: xbee_miso_ReadDataReg
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
uint8 xbee_miso_ReadDataReg(void) 
{
    return (xbee_miso_DR & xbee_miso_MASK) >> xbee_miso_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(xbee_miso_INTSTAT) 

    /*******************************************************************************
    * Function Name: xbee_miso_ClearInterrupt
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
    uint8 xbee_miso_ClearInterrupt(void) 
    {
        return (xbee_miso_INTSTAT & xbee_miso_MASK) >> xbee_miso_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
