/*******************************************************************************
* File Name: time_refresh.c
* Version 2.70
*
* Description:
*  The Timer component consists of a 8, 16, 24 or 32-bit timer with
*  a selectable period between 2 and 2^Width - 1.  The timer may free run
*  or be used as a capture timer as well.  The capture can be initiated
*  by a positive or negative edge signal as well as via software.
*  A trigger input can be programmed to enable the timer on rising edge
*  falling edge, either edge or continous run.
*  Interrupts may be generated due to a terminal count condition
*  or a capture event.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "time_refresh.h"

uint8 time_refresh_initVar = 0u;


/*******************************************************************************
* Function Name: time_refresh_Init
********************************************************************************
*
* Summary:
*  Initialize to the schematic state
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void time_refresh_Init(void) 
{
    #if(!time_refresh_UsingFixedFunction)
            /* Interrupt State Backup for Critical Region*/
            uint8 time_refresh_interruptState;
    #endif /* Interrupt state back up for Fixed Function only */

    #if (time_refresh_UsingFixedFunction)
        /* Clear all bits but the enable bit (if it's already set) for Timer operation */
        time_refresh_CONTROL &= time_refresh_CTRL_ENABLE;

        /* Clear the mode bits for continuous run mode */
        #if (CY_PSOC5A)
            time_refresh_CONTROL2 &= ((uint8)(~time_refresh_CTRL_MODE_MASK));
        #endif /* Clear bits in CONTROL2 only in PSOC5A */

        #if (CY_PSOC3 || CY_PSOC5LP)
            time_refresh_CONTROL3 &= ((uint8)(~time_refresh_CTRL_MODE_MASK));
        #endif /* CONTROL3 register exists only in PSoC3 OR PSoC5LP */

        /* Check if One Shot mode is enabled i.e. RunMode !=0*/
        #if (time_refresh_RunModeUsed != 0x0u)
            /* Set 3rd bit of Control register to enable one shot mode */
            time_refresh_CONTROL |= 0x04u;
        #endif /* One Shot enabled only when RunModeUsed is not Continuous*/

        #if (time_refresh_RunModeUsed == 2)
            #if (CY_PSOC5A)
                /* Set last 2 bits of control2 register if one shot(halt on
                interrupt) is enabled*/
                time_refresh_CONTROL2 |= 0x03u;
            #endif /* Set One-Shot Halt on Interrupt bit in CONTROL2 for PSoC5A */

            #if (CY_PSOC3 || CY_PSOC5LP)
                /* Set last 2 bits of control3 register if one shot(halt on
                interrupt) is enabled*/
                time_refresh_CONTROL3 |= 0x03u;
            #endif /* Set One-Shot Halt on Interrupt bit in CONTROL3 for PSoC3 or PSoC5LP */

        #endif /* Remove section if One Shot Halt on Interrupt is not enabled */

        #if (time_refresh_UsingHWEnable != 0)
            #if (CY_PSOC5A)
                /* Set the default Run Mode of the Timer to Continuous */
                time_refresh_CONTROL2 |= time_refresh_CTRL_MODE_PULSEWIDTH;
            #endif /* Set Continuous Run Mode in CONTROL2 for PSoC5A */

            #if (CY_PSOC3 || CY_PSOC5LP)
                /* Clear and Set ROD and COD bits of CFG2 register */
                time_refresh_CONTROL3 &= ((uint8)(~time_refresh_CTRL_RCOD_MASK));
                time_refresh_CONTROL3 |= time_refresh_CTRL_RCOD;

                /* Clear and Enable the HW enable bit in CFG2 register */
                time_refresh_CONTROL3 &= ((uint8)(~time_refresh_CTRL_ENBL_MASK));
                time_refresh_CONTROL3 |= time_refresh_CTRL_ENBL;

                /* Set the default Run Mode of the Timer to Continuous */
                time_refresh_CONTROL3 |= time_refresh_CTRL_MODE_CONTINUOUS;
            #endif /* Set Continuous Run Mode in CONTROL3 for PSoC3ES3 or PSoC5A */

        #endif /* Configure Run Mode with hardware enable */

        /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        time_refresh_RT1 &= ((uint8)(~time_refresh_RT1_MASK));
        time_refresh_RT1 |= time_refresh_SYNC;

        /*Enable DSI Sync all all inputs of the Timer*/
        time_refresh_RT1 &= ((uint8)(~time_refresh_SYNCDSI_MASK));
        time_refresh_RT1 |= time_refresh_SYNCDSI_EN;

        /* Set the IRQ to use the status register interrupts */
        time_refresh_CONTROL2 |= time_refresh_CTRL2_IRQ_SEL;
    #endif /* Configuring registers of fixed function implementation */

    /* Set Initial values from Configuration */
    time_refresh_WritePeriod(time_refresh_INIT_PERIOD);
    time_refresh_WriteCounter(time_refresh_INIT_PERIOD);

    #if (time_refresh_UsingHWCaptureCounter)/* Capture counter is enabled */
        time_refresh_CAPTURE_COUNT_CTRL |= time_refresh_CNTR_ENABLE;
        time_refresh_SetCaptureCount(time_refresh_INIT_CAPTURE_COUNT);
    #endif /* Configure capture counter value */

    #if (!time_refresh_UsingFixedFunction)
        #if (time_refresh_SoftwareCaptureMode)
            time_refresh_SetCaptureMode(time_refresh_INIT_CAPTURE_MODE);
        #endif /* Set Capture Mode for UDB implementation if capture mode is software controlled */

        #if (time_refresh_SoftwareTriggerMode)
            #if (!time_refresh_UDB_CONTROL_REG_REMOVED)
                if (0u == (time_refresh_CONTROL & time_refresh__B_TIMER__TM_SOFTWARE))
                {
                    time_refresh_SetTriggerMode(time_refresh_INIT_TRIGGER_MODE);
                }
            #endif /* (!time_refresh_UDB_CONTROL_REG_REMOVED) */
        #endif /* Set trigger mode for UDB Implementation if trigger mode is software controlled */

        /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
        /* Enter Critical Region*/
        time_refresh_interruptState = CyEnterCriticalSection();

        /* Use the interrupt output of the status register for IRQ output */
        time_refresh_STATUS_AUX_CTRL |= time_refresh_STATUS_ACTL_INT_EN_MASK;

        /* Exit Critical Region*/
        CyExitCriticalSection(time_refresh_interruptState);

        #if (time_refresh_EnableTriggerMode)
            time_refresh_EnableTrigger();
        #endif /* Set Trigger enable bit for UDB implementation in the control register*/
		
		
        #if (time_refresh_InterruptOnCaptureCount && !time_refresh_UDB_CONTROL_REG_REMOVED)
            time_refresh_SetInterruptCount(time_refresh_INIT_INT_CAPTURE_COUNT);
        #endif /* Set interrupt count in UDB implementation if interrupt count feature is checked.*/

        time_refresh_ClearFIFO();
    #endif /* Configure additional features of UDB implementation */

    time_refresh_SetInterruptMode(time_refresh_INIT_INTERRUPT_MODE);
}


/*******************************************************************************
* Function Name: time_refresh_Enable
********************************************************************************
*
* Summary:
*  Enable the Timer
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void time_refresh_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (time_refresh_UsingFixedFunction)
        time_refresh_GLOBAL_ENABLE |= time_refresh_BLOCK_EN_MASK;
        time_refresh_GLOBAL_STBY_ENABLE |= time_refresh_BLOCK_STBY_EN_MASK;
    #endif /* Set Enable bit for enabling Fixed function timer*/

    /* Remove assignment if control register is removed */
    #if (!time_refresh_UDB_CONTROL_REG_REMOVED || time_refresh_UsingFixedFunction)
        time_refresh_CONTROL |= time_refresh_CTRL_ENABLE;
    #endif /* Remove assignment if control register is removed */
}


/*******************************************************************************
* Function Name: time_refresh_Start
********************************************************************************
*
* Summary:
*  The start function initializes the timer with the default values, the
*  enables the timerto begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  time_refresh_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void time_refresh_Start(void) 
{
    if(time_refresh_initVar == 0u)
    {
        time_refresh_Init();

        time_refresh_initVar = 1u;   /* Clear this bit for Initialization */
    }

    /* Enable the Timer */
    time_refresh_Enable();
}


/*******************************************************************************
* Function Name: time_refresh_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the timer, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the timer.
*
*******************************************************************************/
void time_refresh_Stop(void) 
{
    /* Disable Timer */
    #if(!time_refresh_UDB_CONTROL_REG_REMOVED || time_refresh_UsingFixedFunction)
        time_refresh_CONTROL &= ((uint8)(~time_refresh_CTRL_ENABLE));
    #endif /* Remove assignment if control register is removed */

    /* Globally disable the Fixed Function Block chosen */
    #if (time_refresh_UsingFixedFunction)
        time_refresh_GLOBAL_ENABLE &= ((uint8)(~time_refresh_BLOCK_EN_MASK));
        time_refresh_GLOBAL_STBY_ENABLE &= ((uint8)(~time_refresh_BLOCK_STBY_EN_MASK));
    #endif /* Disable global enable for the Timer Fixed function block to stop the Timer*/
}


/*******************************************************************************
* Function Name: time_refresh_SetInterruptMode
********************************************************************************
*
* Summary:
*  This function selects which of the interrupt inputs may cause an interrupt.
*  The twosources are caputure and terminal.  One, both or neither may
*  be selected.
*
* Parameters:
*  interruptMode:   This parameter is used to enable interrups on either/or
*                   terminal count or capture.
*
* Return:
*  void
*
*******************************************************************************/
void time_refresh_SetInterruptMode(uint8 interruptMode) 
{
    time_refresh_STATUS_MASK = interruptMode;
}


/*******************************************************************************
* Function Name: time_refresh_SoftwareCapture
********************************************************************************
*
* Summary:
*  This function forces a capture independent of the capture signal.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Side Effects:
*  An existing hardware capture could be overwritten.
*
*******************************************************************************/
void time_refresh_SoftwareCapture(void) 
{
    /* Generate a software capture by reading the counter register */
    #if(time_refresh_UsingFixedFunction)
        (void)CY_GET_REG16(time_refresh_COUNTER_LSB_PTR);
    #else
        (void)CY_GET_REG8(time_refresh_COUNTER_LSB_PTR_8BIT);
    #endif/* (time_refresh_UsingFixedFunction) */
    /* Capture Data is now in the FIFO */
}


/*******************************************************************************
* Function Name: time_refresh_ReadStatusRegister
********************************************************************************
*
* Summary:
*  Reads the status register and returns it's state. This function should use
*  defined types for the bit-field information as the bits in this register may
*  be permuteable.
*
* Parameters:
*  void
*
* Return:
*  The contents of the status register
*
* Side Effects:
*  Status register bits may be clear on read.
*
*******************************************************************************/
uint8   time_refresh_ReadStatusRegister(void) 
{
    return (time_refresh_STATUS);
}


#if (!time_refresh_UDB_CONTROL_REG_REMOVED) /* Remove API if control register is unused */


/*******************************************************************************
* Function Name: time_refresh_ReadControlRegister
********************************************************************************
*
* Summary:
*  Reads the control register and returns it's value.
*
* Parameters:
*  void
*
* Return:
*  The contents of the control register
*
*******************************************************************************/
uint8 time_refresh_ReadControlRegister(void) 
{
    #if (!time_refresh_UDB_CONTROL_REG_REMOVED) 
        return ((uint8)time_refresh_CONTROL);
    #else
        return (0);
    #endif /* (!time_refresh_UDB_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: time_refresh_WriteControlRegister
********************************************************************************
*
* Summary:
*  Sets the bit-field of the control register.
*
* Parameters:
*  control: The contents of the control register
*
* Return:
*
*******************************************************************************/
void time_refresh_WriteControlRegister(uint8 control) 
{
    #if (!time_refresh_UDB_CONTROL_REG_REMOVED) 
        time_refresh_CONTROL = control;
    #else
        control = 0u;
    #endif /* (!time_refresh_UDB_CONTROL_REG_REMOVED) */
}

#endif /* Remove API if control register is unused */


/*******************************************************************************
* Function Name: time_refresh_ReadPeriod
********************************************************************************
*
* Summary:
*  This function returns the current value of the Period.
*
* Parameters:
*  void
*
* Return:
*  The present value of the counter.
*
*******************************************************************************/
uint32 time_refresh_ReadPeriod(void) 
{
   #if(time_refresh_UsingFixedFunction)
       return ((uint32)CY_GET_REG16(time_refresh_PERIOD_LSB_PTR));
   #else
       return (CY_GET_REG24(time_refresh_PERIOD_LSB_PTR));
   #endif /* (time_refresh_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: time_refresh_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period: This value may be between 1 and (2^Resolution)-1.  A value of 0 will
*          result in the counter remaining at zero.
*
* Return:
*  void
*
*******************************************************************************/
void time_refresh_WritePeriod(uint32 period) 
{
    #if(time_refresh_UsingFixedFunction)
        uint16 period_temp = (uint16)period;
        CY_SET_REG16(time_refresh_PERIOD_LSB_PTR, period_temp);
    #else
        CY_SET_REG24(time_refresh_PERIOD_LSB_PTR, period);
    #endif /*Write Period value with appropriate resolution suffix depending on UDB or fixed function implementation */
}


/*******************************************************************************
* Function Name: time_refresh_ReadCapture
********************************************************************************
*
* Summary:
*  This function returns the last value captured.
*
* Parameters:
*  void
*
* Return:
*  Present Capture value.
*
*******************************************************************************/
uint32 time_refresh_ReadCapture(void) 
{
   #if(time_refresh_UsingFixedFunction)
       return ((uint32)CY_GET_REG16(time_refresh_CAPTURE_LSB_PTR));
   #else
       return (CY_GET_REG24(time_refresh_CAPTURE_LSB_PTR));
   #endif /* (time_refresh_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: time_refresh_WriteCounter
********************************************************************************
*
* Summary:
*  This funtion is used to set the counter to a specific value
*
* Parameters:
*  counter:  New counter value.
*
* Return:
*  void
*
*******************************************************************************/
void time_refresh_WriteCounter(uint32 counter) 
{
   #if(time_refresh_UsingFixedFunction)
        /* This functionality is removed until a FixedFunction HW update to
         * allow this register to be written
         */
        CY_SET_REG16(time_refresh_COUNTER_LSB_PTR, (uint16)counter);
        
    #else
        CY_SET_REG24(time_refresh_COUNTER_LSB_PTR, counter);
    #endif /* Set Write Counter only for the UDB implementation (Write Counter not available in fixed function Timer */
}


/*******************************************************************************
* Function Name: time_refresh_ReadCounter
********************************************************************************
*
* Summary:
*  This function returns the current counter value.
*
* Parameters:
*  void
*
* Return:
*  Present compare value.
*
*******************************************************************************/
uint32 time_refresh_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(time_refresh_UsingFixedFunction)
        (void)CY_GET_REG16(time_refresh_COUNTER_LSB_PTR);
    #else
        (void)CY_GET_REG8(time_refresh_COUNTER_LSB_PTR_8BIT);
    #endif/* (time_refresh_UsingFixedFunction) */

    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(time_refresh_UsingFixedFunction)
        return ((uint32)CY_GET_REG16(time_refresh_CAPTURE_LSB_PTR));
    #else
        return (CY_GET_REG24(time_refresh_CAPTURE_LSB_PTR));
    #endif /* (time_refresh_UsingFixedFunction) */
}


#if(!time_refresh_UsingFixedFunction) /* UDB Specific Functions */

    
/*******************************************************************************
 * The functions below this point are only available using the UDB
 * implementation.  If a feature is selected, then the API is enabled.
 ******************************************************************************/


#if (time_refresh_SoftwareCaptureMode)


/*******************************************************************************
* Function Name: time_refresh_SetCaptureMode
********************************************************************************
*
* Summary:
*  This function sets the capture mode to either rising or falling edge.
*
* Parameters:
*  captureMode: This parameter sets the capture mode of the UDB capture feature
*  The parameter values are defined using the
*  #define time_refresh__B_TIMER__CM_NONE 0
#define time_refresh__B_TIMER__CM_RISINGEDGE 1
#define time_refresh__B_TIMER__CM_FALLINGEDGE 2
#define time_refresh__B_TIMER__CM_EITHEREDGE 3
#define time_refresh__B_TIMER__CM_SOFTWARE 4
 identifiers
*  The following are the possible values of the parameter
*  time_refresh__B_TIMER__CM_NONE        - Set Capture mode to None
*  time_refresh__B_TIMER__CM_RISINGEDGE  - Rising edge of Capture input
*  time_refresh__B_TIMER__CM_FALLINGEDGE - Falling edge of Capture input
*  time_refresh__B_TIMER__CM_EITHEREDGE  - Either edge of Capture input
*
* Return:
*  void
*
*******************************************************************************/
void time_refresh_SetCaptureMode(uint8 captureMode) 
{
    /* This must only set to two bits of the control register associated */
    captureMode = ((uint8)((uint8)captureMode << time_refresh_CTRL_CAP_MODE_SHIFT));
    captureMode &= (time_refresh_CTRL_CAP_MODE_MASK);

    #if (!time_refresh_UDB_CONTROL_REG_REMOVED)
        /* Clear the Current Setting */
        time_refresh_CONTROL &= ((uint8)(~time_refresh_CTRL_CAP_MODE_MASK));

        /* Write The New Setting */
        time_refresh_CONTROL |= captureMode;
    #endif /* (!time_refresh_UDB_CONTROL_REG_REMOVED) */
}
#endif /* Remove API if Capture Mode is not Software Controlled */


#if (time_refresh_SoftwareTriggerMode)


/*******************************************************************************
* Function Name: time_refresh_SetTriggerMode
********************************************************************************
*
* Summary:
*  This function sets the trigger input mode
*
* Parameters:
*  triggerMode: Pass one of the pre-defined Trigger Modes (except Software)
    #define time_refresh__B_TIMER__TM_NONE 0x00u
    #define time_refresh__B_TIMER__TM_RISINGEDGE 0x04u
    #define time_refresh__B_TIMER__TM_FALLINGEDGE 0x08u
    #define time_refresh__B_TIMER__TM_EITHEREDGE 0x0Cu
    #define time_refresh__B_TIMER__TM_SOFTWARE 0x10u
*
* Return:
*  void
*
*******************************************************************************/
void time_refresh_SetTriggerMode(uint8 triggerMode) 
{
    /* This must only set to two bits of the control register associated */
    triggerMode &= time_refresh_CTRL_TRIG_MODE_MASK;

    #if (!time_refresh_UDB_CONTROL_REG_REMOVED)   /* Remove assignment if control register is removed */
    
        /* Clear the Current Setting */
        time_refresh_CONTROL &= ((uint8)(~time_refresh_CTRL_TRIG_MODE_MASK));

        /* Write The New Setting */
        time_refresh_CONTROL |= (triggerMode | time_refresh__B_TIMER__TM_SOFTWARE);
    #endif /* Remove code section if control register is not used */
}
#endif /* Remove API if Trigger Mode is not Software Controlled */

#if (time_refresh_EnableTriggerMode)


/*******************************************************************************
* Function Name: time_refresh_EnableTrigger
********************************************************************************
*
* Summary:
*  Sets the control bit enabling Hardware Trigger mode
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void time_refresh_EnableTrigger(void) 
{
    #if (!time_refresh_UDB_CONTROL_REG_REMOVED)   /* Remove assignment if control register is removed */
        time_refresh_CONTROL |= time_refresh_CTRL_TRIG_EN;
    #endif /* Remove code section if control register is not used */
}


/*******************************************************************************
* Function Name: time_refresh_DisableTrigger
********************************************************************************
*
* Summary:
*  Clears the control bit enabling Hardware Trigger mode
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void time_refresh_DisableTrigger(void) 
{
    #if (!time_refresh_UDB_CONTROL_REG_REMOVED )   /* Remove assignment if control register is removed */
        time_refresh_CONTROL &= ((uint8)(~time_refresh_CTRL_TRIG_EN));
    #endif /* Remove code section if control register is not used */
}
#endif /* Remove API is Trigger Mode is set to None */

#if(time_refresh_InterruptOnCaptureCount)


/*******************************************************************************
* Function Name: time_refresh_SetInterruptCount
********************************************************************************
*
* Summary:
*  This function sets the capture count before an interrupt is triggered.
*
* Parameters:
*  interruptCount:  A value between 0 and 3 is valid.  If the value is 0, then
*                   an interrupt will occur each time a capture occurs.
*                   A value of 1 to 3 will cause the interrupt
*                   to delay by the same number of captures.
*
* Return:
*  void
*
*******************************************************************************/
void time_refresh_SetInterruptCount(uint8 interruptCount) 
{
    /* This must only set to two bits of the control register associated */
    interruptCount &= time_refresh_CTRL_INTCNT_MASK;

    #if (!time_refresh_UDB_CONTROL_REG_REMOVED)
        /* Clear the Current Setting */
        time_refresh_CONTROL &= ((uint8)(~time_refresh_CTRL_INTCNT_MASK));
        /* Write The New Setting */
        time_refresh_CONTROL |= interruptCount;
    #endif /* (!time_refresh_UDB_CONTROL_REG_REMOVED) */
}
#endif /* time_refresh_InterruptOnCaptureCount */


#if (time_refresh_UsingHWCaptureCounter)


/*******************************************************************************
* Function Name: time_refresh_SetCaptureCount
********************************************************************************
*
* Summary:
*  This function sets the capture count
*
* Parameters:
*  captureCount: A value between 2 and 127 inclusive is valid.  A value of 1
*                to 127 will cause the interrupt to delay by the same number of
*                captures.
*
* Return:
*  void
*
*******************************************************************************/
void time_refresh_SetCaptureCount(uint8 captureCount) 
{
    time_refresh_CAP_COUNT = captureCount;
}


/*******************************************************************************
* Function Name: time_refresh_ReadCaptureCount
********************************************************************************
*
* Summary:
*  This function reads the capture count setting
*
* Parameters:
*  void
*
* Return:
*  Returns the Capture Count Setting
*
*******************************************************************************/
uint8 time_refresh_ReadCaptureCount(void) 
{
    return ((uint8)time_refresh_CAP_COUNT);
}
#endif /* time_refresh_UsingHWCaptureCounter */


/*******************************************************************************
* Function Name: time_refresh_ClearFIFO
********************************************************************************
*
* Summary:
*  This function clears all capture data from the capture FIFO
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void time_refresh_ClearFIFO(void) 
{
    while(0u != (time_refresh_ReadStatusRegister() & time_refresh_STATUS_FIFONEMP))
    {
        (void)time_refresh_ReadCapture();
    }
}

#endif /* UDB Specific Functions */


/* [] END OF FILE */
