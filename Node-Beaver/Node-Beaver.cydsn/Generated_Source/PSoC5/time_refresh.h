/*******************************************************************************
* File Name: time_refresh.h
* Version 2.70
*
*  Description:
*     Contains the function prototypes and constants available to the timer
*     user module.
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_Timer_v2_60_time_refresh_H)
#define CY_Timer_v2_60_time_refresh_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 time_refresh_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_70 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define time_refresh_Resolution                 24u
#define time_refresh_UsingFixedFunction         0u
#define time_refresh_UsingHWCaptureCounter      0u
#define time_refresh_SoftwareCaptureMode        0u
#define time_refresh_SoftwareTriggerMode        0u
#define time_refresh_UsingHWEnable              0u
#define time_refresh_EnableTriggerMode          0u
#define time_refresh_InterruptOnCaptureCount    0u
#define time_refresh_RunModeUsed                0u
#define time_refresh_ControlRegRemoved          0u

#if defined(time_refresh_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define time_refresh_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (time_refresh_UsingFixedFunction)
    #define time_refresh_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define time_refresh_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End time_refresh_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!time_refresh_UsingFixedFunction)

        uint32 TimerUdb;
        uint8 InterruptMaskValue;
        #if (time_refresh_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!time_refresh_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}time_refresh_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    time_refresh_Start(void) ;
void    time_refresh_Stop(void) ;

void    time_refresh_SetInterruptMode(uint8 interruptMode) ;
uint8   time_refresh_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define time_refresh_GetInterruptSource() time_refresh_ReadStatusRegister()

#if(!time_refresh_UDB_CONTROL_REG_REMOVED)
    uint8   time_refresh_ReadControlRegister(void) ;
    void    time_refresh_WriteControlRegister(uint8 control) ;
#endif /* (!time_refresh_UDB_CONTROL_REG_REMOVED) */

uint32  time_refresh_ReadPeriod(void) ;
void    time_refresh_WritePeriod(uint32 period) ;
uint32  time_refresh_ReadCounter(void) ;
void    time_refresh_WriteCounter(uint32 counter) ;
uint32  time_refresh_ReadCapture(void) ;
void    time_refresh_SoftwareCapture(void) ;

#if(!time_refresh_UsingFixedFunction) /* UDB Prototypes */
    #if (time_refresh_SoftwareCaptureMode)
        void    time_refresh_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!time_refresh_UsingFixedFunction) */

    #if (time_refresh_SoftwareTriggerMode)
        void    time_refresh_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (time_refresh_SoftwareTriggerMode) */

    #if (time_refresh_EnableTriggerMode)
        void    time_refresh_EnableTrigger(void) ;
        void    time_refresh_DisableTrigger(void) ;
    #endif /* (time_refresh_EnableTriggerMode) */


    #if(time_refresh_InterruptOnCaptureCount)
        void    time_refresh_SetInterruptCount(uint8 interruptCount) ;
    #endif /* (time_refresh_InterruptOnCaptureCount) */

    #if (time_refresh_UsingHWCaptureCounter)
        void    time_refresh_SetCaptureCount(uint8 captureCount) ;
        uint8   time_refresh_ReadCaptureCount(void) ;
    #endif /* (time_refresh_UsingHWCaptureCounter) */

    void time_refresh_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void time_refresh_Init(void)          ;
void time_refresh_Enable(void)        ;
void time_refresh_SaveConfig(void)    ;
void time_refresh_RestoreConfig(void) ;
void time_refresh_Sleep(void)         ;
void time_refresh_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define time_refresh__B_TIMER__CM_NONE 0
#define time_refresh__B_TIMER__CM_RISINGEDGE 1
#define time_refresh__B_TIMER__CM_FALLINGEDGE 2
#define time_refresh__B_TIMER__CM_EITHEREDGE 3
#define time_refresh__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define time_refresh__B_TIMER__TM_NONE 0x00u
#define time_refresh__B_TIMER__TM_RISINGEDGE 0x04u
#define time_refresh__B_TIMER__TM_FALLINGEDGE 0x08u
#define time_refresh__B_TIMER__TM_EITHEREDGE 0x0Cu
#define time_refresh__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define time_refresh_INIT_PERIOD             9999999u
#define time_refresh_INIT_CAPTURE_MODE       ((uint8)((uint8)0u << time_refresh_CTRL_CAP_MODE_SHIFT))
#define time_refresh_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << time_refresh_CTRL_TRIG_MODE_SHIFT))
#if (time_refresh_UsingFixedFunction)
    #define time_refresh_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << time_refresh_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << time_refresh_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define time_refresh_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << time_refresh_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << time_refresh_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << time_refresh_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (time_refresh_UsingFixedFunction) */
#define time_refresh_INIT_CAPTURE_COUNT      (2u)
#define time_refresh_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << time_refresh_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (time_refresh_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define time_refresh_STATUS         (*(reg8 *) time_refresh_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define time_refresh_STATUS_MASK    (*(reg8 *) time_refresh_TimerHW__SR0 )
    #define time_refresh_CONTROL        (*(reg8 *) time_refresh_TimerHW__CFG0)
    #define time_refresh_CONTROL2       (*(reg8 *) time_refresh_TimerHW__CFG1)
    #define time_refresh_CONTROL2_PTR   ( (reg8 *) time_refresh_TimerHW__CFG1)
    #define time_refresh_RT1            (*(reg8 *) time_refresh_TimerHW__RT1)
    #define time_refresh_RT1_PTR        ( (reg8 *) time_refresh_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define time_refresh_CONTROL3       (*(reg8 *) time_refresh_TimerHW__CFG2)
        #define time_refresh_CONTROL3_PTR   ( (reg8 *) time_refresh_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define time_refresh_GLOBAL_ENABLE  (*(reg8 *) time_refresh_TimerHW__PM_ACT_CFG)
    #define time_refresh_GLOBAL_STBY_ENABLE  (*(reg8 *) time_refresh_TimerHW__PM_STBY_CFG)

    #define time_refresh_CAPTURE_LSB         (* (reg16 *) time_refresh_TimerHW__CAP0 )
    #define time_refresh_CAPTURE_LSB_PTR       ((reg16 *) time_refresh_TimerHW__CAP0 )
    #define time_refresh_PERIOD_LSB          (* (reg16 *) time_refresh_TimerHW__PER0 )
    #define time_refresh_PERIOD_LSB_PTR        ((reg16 *) time_refresh_TimerHW__PER0 )
    #define time_refresh_COUNTER_LSB         (* (reg16 *) time_refresh_TimerHW__CNT_CMP0 )
    #define time_refresh_COUNTER_LSB_PTR       ((reg16 *) time_refresh_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define time_refresh_BLOCK_EN_MASK                     time_refresh_TimerHW__PM_ACT_MSK
    #define time_refresh_BLOCK_STBY_EN_MASK                time_refresh_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define time_refresh_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define time_refresh_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define time_refresh_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define time_refresh_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define time_refresh_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define time_refresh_CTRL_ENABLE                        ((uint8)((uint8)0x01u << time_refresh_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define time_refresh_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define time_refresh_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << time_refresh_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define time_refresh_CTRL_MODE_SHIFT                 0x01u
        #define time_refresh_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << time_refresh_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define time_refresh_CTRL_RCOD_SHIFT        0x02u
        #define time_refresh_CTRL_ENBL_SHIFT        0x00u
        #define time_refresh_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define time_refresh_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << time_refresh_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define time_refresh_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << time_refresh_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define time_refresh_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << time_refresh_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define time_refresh_CTRL_RCOD       ((uint8)((uint8)0x03u << time_refresh_CTRL_RCOD_SHIFT))
        #define time_refresh_CTRL_ENBL       ((uint8)((uint8)0x80u << time_refresh_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define time_refresh_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define time_refresh_RT1_MASK                        ((uint8)((uint8)0x03u << time_refresh_RT1_SHIFT))
    #define time_refresh_SYNC                            ((uint8)((uint8)0x03u << time_refresh_RT1_SHIFT))
    #define time_refresh_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define time_refresh_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << time_refresh_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define time_refresh_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << time_refresh_SYNCDSI_SHIFT))

    #define time_refresh_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << time_refresh_CTRL_MODE_SHIFT))
    #define time_refresh_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << time_refresh_CTRL_MODE_SHIFT))
    #define time_refresh_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << time_refresh_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define time_refresh_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define time_refresh_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define time_refresh_STATUS_TC_INT_MASK_SHIFT        (time_refresh_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define time_refresh_STATUS_CAPTURE_INT_MASK_SHIFT   (time_refresh_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define time_refresh_STATUS_TC                       ((uint8)((uint8)0x01u << time_refresh_STATUS_TC_SHIFT))
    #define time_refresh_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << time_refresh_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define time_refresh_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << time_refresh_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define time_refresh_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << time_refresh_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define time_refresh_STATUS              (* (reg8 *) time_refresh_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define time_refresh_STATUS_MASK         (* (reg8 *) time_refresh_TimerUDB_rstSts_stsreg__MASK_REG)
    #define time_refresh_STATUS_AUX_CTRL     (* (reg8 *) time_refresh_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define time_refresh_CONTROL             (* (reg8 *) time_refresh_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(time_refresh_Resolution <= 8u) /* 8-bit Timer */
        #define time_refresh_CAPTURE_LSB         (* (reg8 *) time_refresh_TimerUDB_sT24_timerdp_u0__F0_REG )
        #define time_refresh_CAPTURE_LSB_PTR       ((reg8 *) time_refresh_TimerUDB_sT24_timerdp_u0__F0_REG )
        #define time_refresh_PERIOD_LSB          (* (reg8 *) time_refresh_TimerUDB_sT24_timerdp_u0__D0_REG )
        #define time_refresh_PERIOD_LSB_PTR        ((reg8 *) time_refresh_TimerUDB_sT24_timerdp_u0__D0_REG )
        #define time_refresh_COUNTER_LSB         (* (reg8 *) time_refresh_TimerUDB_sT24_timerdp_u0__A0_REG )
        #define time_refresh_COUNTER_LSB_PTR       ((reg8 *) time_refresh_TimerUDB_sT24_timerdp_u0__A0_REG )
    #elif(time_refresh_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define time_refresh_CAPTURE_LSB         (* (reg16 *) time_refresh_TimerUDB_sT24_timerdp_u0__F0_REG )
            #define time_refresh_CAPTURE_LSB_PTR       ((reg16 *) time_refresh_TimerUDB_sT24_timerdp_u0__F0_REG )
            #define time_refresh_PERIOD_LSB          (* (reg16 *) time_refresh_TimerUDB_sT24_timerdp_u0__D0_REG )
            #define time_refresh_PERIOD_LSB_PTR        ((reg16 *) time_refresh_TimerUDB_sT24_timerdp_u0__D0_REG )
            #define time_refresh_COUNTER_LSB         (* (reg16 *) time_refresh_TimerUDB_sT24_timerdp_u0__A0_REG )
            #define time_refresh_COUNTER_LSB_PTR       ((reg16 *) time_refresh_TimerUDB_sT24_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define time_refresh_CAPTURE_LSB         (* (reg16 *) time_refresh_TimerUDB_sT24_timerdp_u0__16BIT_F0_REG )
            #define time_refresh_CAPTURE_LSB_PTR       ((reg16 *) time_refresh_TimerUDB_sT24_timerdp_u0__16BIT_F0_REG )
            #define time_refresh_PERIOD_LSB          (* (reg16 *) time_refresh_TimerUDB_sT24_timerdp_u0__16BIT_D0_REG )
            #define time_refresh_PERIOD_LSB_PTR        ((reg16 *) time_refresh_TimerUDB_sT24_timerdp_u0__16BIT_D0_REG )
            #define time_refresh_COUNTER_LSB         (* (reg16 *) time_refresh_TimerUDB_sT24_timerdp_u0__16BIT_A0_REG )
            #define time_refresh_COUNTER_LSB_PTR       ((reg16 *) time_refresh_TimerUDB_sT24_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(time_refresh_Resolution <= 24u)/* 24-bit Timer */
        #define time_refresh_CAPTURE_LSB         (* (reg32 *) time_refresh_TimerUDB_sT24_timerdp_u0__F0_REG )
        #define time_refresh_CAPTURE_LSB_PTR       ((reg32 *) time_refresh_TimerUDB_sT24_timerdp_u0__F0_REG )
        #define time_refresh_PERIOD_LSB          (* (reg32 *) time_refresh_TimerUDB_sT24_timerdp_u0__D0_REG )
        #define time_refresh_PERIOD_LSB_PTR        ((reg32 *) time_refresh_TimerUDB_sT24_timerdp_u0__D0_REG )
        #define time_refresh_COUNTER_LSB         (* (reg32 *) time_refresh_TimerUDB_sT24_timerdp_u0__A0_REG )
        #define time_refresh_COUNTER_LSB_PTR       ((reg32 *) time_refresh_TimerUDB_sT24_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define time_refresh_CAPTURE_LSB         (* (reg32 *) time_refresh_TimerUDB_sT24_timerdp_u0__F0_REG )
            #define time_refresh_CAPTURE_LSB_PTR       ((reg32 *) time_refresh_TimerUDB_sT24_timerdp_u0__F0_REG )
            #define time_refresh_PERIOD_LSB          (* (reg32 *) time_refresh_TimerUDB_sT24_timerdp_u0__D0_REG )
            #define time_refresh_PERIOD_LSB_PTR        ((reg32 *) time_refresh_TimerUDB_sT24_timerdp_u0__D0_REG )
            #define time_refresh_COUNTER_LSB         (* (reg32 *) time_refresh_TimerUDB_sT24_timerdp_u0__A0_REG )
            #define time_refresh_COUNTER_LSB_PTR       ((reg32 *) time_refresh_TimerUDB_sT24_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define time_refresh_CAPTURE_LSB         (* (reg32 *) time_refresh_TimerUDB_sT24_timerdp_u0__32BIT_F0_REG )
            #define time_refresh_CAPTURE_LSB_PTR       ((reg32 *) time_refresh_TimerUDB_sT24_timerdp_u0__32BIT_F0_REG )
            #define time_refresh_PERIOD_LSB          (* (reg32 *) time_refresh_TimerUDB_sT24_timerdp_u0__32BIT_D0_REG )
            #define time_refresh_PERIOD_LSB_PTR        ((reg32 *) time_refresh_TimerUDB_sT24_timerdp_u0__32BIT_D0_REG )
            #define time_refresh_COUNTER_LSB         (* (reg32 *) time_refresh_TimerUDB_sT24_timerdp_u0__32BIT_A0_REG )
            #define time_refresh_COUNTER_LSB_PTR       ((reg32 *) time_refresh_TimerUDB_sT24_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define time_refresh_COUNTER_LSB_PTR_8BIT       ((reg8 *) time_refresh_TimerUDB_sT24_timerdp_u0__A0_REG )
    
    #if (time_refresh_UsingHWCaptureCounter)
        #define time_refresh_CAP_COUNT              (*(reg8 *) time_refresh_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define time_refresh_CAP_COUNT_PTR          ( (reg8 *) time_refresh_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define time_refresh_CAPTURE_COUNT_CTRL     (*(reg8 *) time_refresh_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define time_refresh_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) time_refresh_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (time_refresh_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define time_refresh_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define time_refresh_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define time_refresh_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define time_refresh_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define time_refresh_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define time_refresh_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << time_refresh_CTRL_INTCNT_SHIFT))
    #define time_refresh_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << time_refresh_CTRL_TRIG_MODE_SHIFT))
    #define time_refresh_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << time_refresh_CTRL_TRIG_EN_SHIFT))
    #define time_refresh_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << time_refresh_CTRL_CAP_MODE_SHIFT))
    #define time_refresh_CTRL_ENABLE                    ((uint8)((uint8)0x01u << time_refresh_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define time_refresh_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define time_refresh_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define time_refresh_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define time_refresh_STATUS_TC_INT_MASK_SHIFT       time_refresh_STATUS_TC_SHIFT
    #define time_refresh_STATUS_CAPTURE_INT_MASK_SHIFT  time_refresh_STATUS_CAPTURE_SHIFT
    #define time_refresh_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define time_refresh_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define time_refresh_STATUS_FIFOFULL_INT_MASK_SHIFT time_refresh_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define time_refresh_STATUS_TC                      ((uint8)((uint8)0x01u << time_refresh_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define time_refresh_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << time_refresh_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define time_refresh_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << time_refresh_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define time_refresh_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << time_refresh_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define time_refresh_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << time_refresh_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define time_refresh_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << time_refresh_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define time_refresh_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << time_refresh_STATUS_FIFOFULL_SHIFT))

    #define time_refresh_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define time_refresh_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define time_refresh_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define time_refresh_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define time_refresh_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define time_refresh_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_Timer_v2_30_time_refresh_H */


/* [] END OF FILE */
