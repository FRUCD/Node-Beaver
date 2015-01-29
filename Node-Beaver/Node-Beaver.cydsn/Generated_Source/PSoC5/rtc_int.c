/*******************************************************************************
* File Name: rtc_int.c  
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
#include "rtc_int.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 rtc_int__PORT == 15 && ((rtc_int__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: rtc_int_Write
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
void rtc_int_Write(uint8 value) 
{
    uint8 staticBits = (rtc_int_DR & (uint8)(~rtc_int_MASK));
    rtc_int_DR = staticBits | ((uint8)(value << rtc_int_SHIFT) & rtc_int_MASK);
}


/*******************************************************************************
* Function Name: rtc_int_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  rtc_int_DM_STRONG     Strong Drive 
*  rtc_int_DM_OD_HI      Open Drain, Drives High 
*  rtc_int_DM_OD_LO      Open Drain, Drives Low 
*  rtc_int_DM_RES_UP     Resistive Pull Up 
*  rtc_int_DM_RES_DWN    Resistive Pull Down 
*  rtc_int_DM_RES_UPDWN  Resistive Pull Up/Down 
*  rtc_int_DM_DIG_HIZ    High Impedance Digital 
*  rtc_int_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void rtc_int_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(rtc_int_0, mode);
}


/*******************************************************************************
* Function Name: rtc_int_Read
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
*  Macro rtc_int_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 rtc_int_Read(void) 
{
    return (rtc_int_PS & rtc_int_MASK) >> rtc_int_SHIFT;
}


/*******************************************************************************
* Function Name: rtc_int_ReadDataReg
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
uint8 rtc_int_ReadDataReg(void) 
{
    return (rtc_int_DR & rtc_int_MASK) >> rtc_int_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(rtc_int_INTSTAT) 

    /*******************************************************************************
    * Function Name: rtc_int_ClearInterrupt
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
    uint8 rtc_int_ClearInterrupt(void) 
    {
        return (rtc_int_INTSTAT & rtc_int_MASK) >> rtc_int_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
