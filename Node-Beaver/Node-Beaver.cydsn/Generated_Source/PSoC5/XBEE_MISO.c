/*******************************************************************************
* File Name: XBEE_MISO.c  
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
#include "XBEE_MISO.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 XBEE_MISO__PORT == 15 && ((XBEE_MISO__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: XBEE_MISO_Write
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
void XBEE_MISO_Write(uint8 value) 
{
    uint8 staticBits = (XBEE_MISO_DR & (uint8)(~XBEE_MISO_MASK));
    XBEE_MISO_DR = staticBits | ((uint8)(value << XBEE_MISO_SHIFT) & XBEE_MISO_MASK);
}


/*******************************************************************************
* Function Name: XBEE_MISO_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  XBEE_MISO_DM_STRONG     Strong Drive 
*  XBEE_MISO_DM_OD_HI      Open Drain, Drives High 
*  XBEE_MISO_DM_OD_LO      Open Drain, Drives Low 
*  XBEE_MISO_DM_RES_UP     Resistive Pull Up 
*  XBEE_MISO_DM_RES_DWN    Resistive Pull Down 
*  XBEE_MISO_DM_RES_UPDWN  Resistive Pull Up/Down 
*  XBEE_MISO_DM_DIG_HIZ    High Impedance Digital 
*  XBEE_MISO_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void XBEE_MISO_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(XBEE_MISO_0, mode);
}


/*******************************************************************************
* Function Name: XBEE_MISO_Read
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
*  Macro XBEE_MISO_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 XBEE_MISO_Read(void) 
{
    return (XBEE_MISO_PS & XBEE_MISO_MASK) >> XBEE_MISO_SHIFT;
}


/*******************************************************************************
* Function Name: XBEE_MISO_ReadDataReg
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
uint8 XBEE_MISO_ReadDataReg(void) 
{
    return (XBEE_MISO_DR & XBEE_MISO_MASK) >> XBEE_MISO_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(XBEE_MISO_INTSTAT) 

    /*******************************************************************************
    * Function Name: XBEE_MISO_ClearInterrupt
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
    uint8 XBEE_MISO_ClearInterrupt(void) 
    {
        return (XBEE_MISO_INTSTAT & XBEE_MISO_MASK) >> XBEE_MISO_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
