/*******************************************************************************
* File Name: time_refresh_isr.c  
* Version 1.70
*
*  Description:
*   API for controlling the state of an interrupt.
*
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#include <cydevice_trm.h>
#include <CyLib.h>
#include <time_refresh_isr.h>

#if !defined(time_refresh_isr__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START time_refresh_isr_intc` */

/* `#END` */

#ifndef CYINT_IRQ_BASE
#define CYINT_IRQ_BASE      16
#endif /* CYINT_IRQ_BASE */
#ifndef CYINT_VECT_TABLE
#define CYINT_VECT_TABLE    ((cyisraddress **) CYREG_NVIC_VECT_OFFSET)
#endif /* CYINT_VECT_TABLE */

/* Declared in startup, used to set unused interrupts to. */
CY_ISR_PROTO(IntDefaultHandler);


/*******************************************************************************
* Function Name: time_refresh_isr_Start
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void time_refresh_isr_Start(void)
{
    /* For all we know the interrupt is active. */
    time_refresh_isr_Disable();

    /* Set the ISR to point to the time_refresh_isr Interrupt. */
    time_refresh_isr_SetVector(&time_refresh_isr_Interrupt);

    /* Set the priority. */
    time_refresh_isr_SetPriority((uint8)time_refresh_isr_INTC_PRIOR_NUMBER);

    /* Enable it. */
    time_refresh_isr_Enable();
}


/*******************************************************************************
* Function Name: time_refresh_isr_StartEx
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it.
*
* Parameters:  
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void time_refresh_isr_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    time_refresh_isr_Disable();

    /* Set the ISR to point to the time_refresh_isr Interrupt. */
    time_refresh_isr_SetVector(address);

    /* Set the priority. */
    time_refresh_isr_SetPriority((uint8)time_refresh_isr_INTC_PRIOR_NUMBER);

    /* Enable it. */
    time_refresh_isr_Enable();
}


/*******************************************************************************
* Function Name: time_refresh_isr_Stop
********************************************************************************
*
* Summary:
*   Disables and removes the interrupt.
*
* Parameters:  
*
* Return:
*   None
*
*******************************************************************************/
void time_refresh_isr_Stop(void)
{
    /* Disable this interrupt. */
    time_refresh_isr_Disable();

    /* Set the ISR to point to the passive one. */
    time_refresh_isr_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: time_refresh_isr_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for time_refresh_isr.
*
*   Add custom code between the coments to keep the next version of this file
*   from over writting your code.
*
* Parameters:  
*
* Return:
*   None
*
*******************************************************************************/
CY_ISR(time_refresh_isr_Interrupt)
{
    /*  Place your Interrupt code here. */
    /* `#START time_refresh_isr_Interrupt` */

    /* `#END` */
}


/*******************************************************************************
* Function Name: time_refresh_isr_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling time_refresh_isr_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use time_refresh_isr_StartEx instead.
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void time_refresh_isr_SetVector(cyisraddress address)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    ramVectorTable[CYINT_IRQ_BASE + (uint32)time_refresh_isr__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: time_refresh_isr_GetVector
********************************************************************************
*
* Summary:
*   Gets the "address" of the current ISR vector for the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress time_refresh_isr_GetVector(void)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    return ramVectorTable[CYINT_IRQ_BASE + (uint32)time_refresh_isr__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: time_refresh_isr_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. Note calling time_refresh_isr_Start
*   or time_refresh_isr_StartEx will override any effect this method 
*   would have had. This method should only be called after 
*   time_refresh_isr_Start or time_refresh_isr_StartEx has been called. To set 
*   the initial priority for the component use the cydwr file in the tool.
*
* Parameters:
*   priority: Priority of the interrupt. 0 - 7, 0 being the highest.
*
* Return:
*   None
*
*******************************************************************************/
void time_refresh_isr_SetPriority(uint8 priority)
{
    *time_refresh_isr_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: time_refresh_isr_GetPriority
********************************************************************************
*
* Summary:
*   Gets the Priority of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Priority of the interrupt. 0 - 7, 0 being the highest.
*
*******************************************************************************/
uint8 time_refresh_isr_GetPriority(void)
{
    uint8 priority;


    priority = *time_refresh_isr_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: time_refresh_isr_Enable
********************************************************************************
*
* Summary:
*   Enables the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void time_refresh_isr_Enable(void)
{
    /* Enable the general interrupt. */
    *time_refresh_isr_INTC_SET_EN = time_refresh_isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: time_refresh_isr_GetState
********************************************************************************
*
* Summary:
*   Gets the state (enabled, disabled) of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   1 if enabled, 0 if disabled.
*
*******************************************************************************/
uint8 time_refresh_isr_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*time_refresh_isr_INTC_SET_EN & (uint32)time_refresh_isr__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: time_refresh_isr_Disable
********************************************************************************
*
* Summary:
*   Disables the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void time_refresh_isr_Disable(void)
{
    /* Disable the general interrupt. */
    *time_refresh_isr_INTC_CLR_EN = time_refresh_isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: time_refresh_isr_SetPending
********************************************************************************
*
* Summary:
*   Causes the Interrupt to enter the pending state, a software method of
*   generating the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void time_refresh_isr_SetPending(void)
{
    *time_refresh_isr_INTC_SET_PD = time_refresh_isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: time_refresh_isr_ClearPending
********************************************************************************
*
* Summary:
*   Clears a pending interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void time_refresh_isr_ClearPending(void)
{
    *time_refresh_isr_INTC_CLR_PD = time_refresh_isr__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
