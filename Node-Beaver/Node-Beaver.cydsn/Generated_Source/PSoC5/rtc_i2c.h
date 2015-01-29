/*******************************************************************************
* File Name: rtc_i2c.h
* Version 3.30
*
* Description:
*  This file provides constants and parameter values for the I2C component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_I2C_rtc_i2c_H)
#define CY_I2C_rtc_i2c_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h"

/* Check if required defines such as CY_PSOC5LP are available in cy_boot */
#if !defined (CY_PSOC5LP)
    #error Component I2C_v3_30 requires cy_boot v3.10 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define rtc_i2c_IMPLEMENTATION     (1u)
#define rtc_i2c_MODE               (2u)
#define rtc_i2c_ENABLE_WAKEUP      (0u)
#define rtc_i2c_ADDR_DECODE        (1u)
#define rtc_i2c_UDB_INTRN_CLOCK    (0u)


/* I2C implementation enum */
#define rtc_i2c_UDB    (0x00u)
#define rtc_i2c_FF     (0x01u)

#define rtc_i2c_FF_IMPLEMENTED     (rtc_i2c_FF  == rtc_i2c_IMPLEMENTATION)
#define rtc_i2c_UDB_IMPLEMENTED    (rtc_i2c_UDB == rtc_i2c_IMPLEMENTATION)

#define rtc_i2c_UDB_INTRN_CLOCK_ENABLED    (rtc_i2c_UDB_IMPLEMENTED && \
                                                     (0u != rtc_i2c_UDB_INTRN_CLOCK))
/* I2C modes enum */
#define rtc_i2c_MODE_SLAVE                 (0x01u)
#define rtc_i2c_MODE_MASTER                (0x02u)
#define rtc_i2c_MODE_MULTI_MASTER          (0x06u)
#define rtc_i2c_MODE_MULTI_MASTER_SLAVE    (0x07u)
#define rtc_i2c_MODE_MULTI_MASTER_MASK     (0x04u)

#define rtc_i2c_MODE_SLAVE_ENABLED         (0u != (rtc_i2c_MODE_SLAVE  & rtc_i2c_MODE))
#define rtc_i2c_MODE_MASTER_ENABLED        (0u != (rtc_i2c_MODE_MASTER & rtc_i2c_MODE))
#define rtc_i2c_MODE_MULTI_MASTER_ENABLED  (0u != (rtc_i2c_MODE_MULTI_MASTER_MASK & \
                                                            rtc_i2c_MODE))
#define rtc_i2c_MODE_MULTI_MASTER_SLAVE_ENABLED    (rtc_i2c_MODE_MULTI_MASTER_SLAVE == \
                                                             rtc_i2c_MODE)

/* Address detection enum */
#define rtc_i2c_SW_DECODE      (0x00u)
#define rtc_i2c_HW_DECODE      (0x01u)

#define rtc_i2c_SW_ADRR_DECODE             (rtc_i2c_SW_DECODE == rtc_i2c_ADDR_DECODE)
#define rtc_i2c_HW_ADRR_DECODE             (rtc_i2c_HW_DECODE == rtc_i2c_ADDR_DECODE)

/* Wakeup enabled */
#define rtc_i2c_WAKEUP_ENABLED             (0u != rtc_i2c_ENABLE_WAKEUP)

/* Adds bootloader APIs to component */
#define rtc_i2c_BOOTLOADER_INTERFACE_ENABLED   (rtc_i2c_MODE_SLAVE_ENABLED && \
                                                            ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_rtc_i2c) || \
                                                             (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface)))

/* Timeout functionality */
#define rtc_i2c_TIMEOUT_ENABLE             (0u)
#define rtc_i2c_TIMEOUT_SCL_TMOUT_ENABLE   (0u)
#define rtc_i2c_TIMEOUT_SDA_TMOUT_ENABLE   (0u)
#define rtc_i2c_TIMEOUT_PRESCALER_ENABLE   (0u)
#define rtc_i2c_TIMEOUT_IMPLEMENTATION     (0u)

/* Convert to boolean */
#define rtc_i2c_TIMEOUT_ENABLED            (0u != rtc_i2c_TIMEOUT_ENABLE)
#define rtc_i2c_TIMEOUT_SCL_TMOUT_ENABLED  (0u != rtc_i2c_TIMEOUT_SCL_TMOUT_ENABLE)
#define rtc_i2c_TIMEOUT_SDA_TMOUT_ENABLED  (0u != rtc_i2c_TIMEOUT_SDA_TMOUT_ENABLE)
#define rtc_i2c_TIMEOUT_PRESCALER_ENABLED  (0u != rtc_i2c_TIMEOUT_PRESCALER_ENABLE)

/* Timeout implementation enum. */
#define rtc_i2c_TIMEOUT_UDB    (0x00u)
#define rtc_i2c_TIMEOUT_FF     (0x01u)

#define rtc_i2c_TIMEOUT_FF_IMPLEMENTED     (rtc_i2c_TIMEOUT_FF  == \
                                                        rtc_i2c_TIMEOUT_IMPLEMENTATION)
#define rtc_i2c_TIMEOUT_UDB_IMPLEMENTED    (rtc_i2c_TIMEOUT_UDB == \
                                                        rtc_i2c_TIMEOUT_IMPLEMENTATION)

#define rtc_i2c_TIMEOUT_FF_ENABLED         (rtc_i2c_TIMEOUT_ENABLED && \
                                                     rtc_i2c_TIMEOUT_FF_IMPLEMENTED && \
                                                     CY_PSOC5LP)

#define rtc_i2c_TIMEOUT_UDB_ENABLED        (rtc_i2c_TIMEOUT_ENABLED && \
                                                     rtc_i2c_TIMEOUT_UDB_IMPLEMENTED)

#define rtc_i2c_EXTERN_I2C_INTR_HANDLER    (0u)
#define rtc_i2c_EXTERN_TMOUT_INTR_HANDLER  (0u)

#define rtc_i2c_INTERN_I2C_INTR_HANDLER    (0u == rtc_i2c_EXTERN_I2C_INTR_HANDLER)
#define rtc_i2c_INTERN_TMOUT_INTR_HANDLER  (0u == rtc_i2c_EXTERN_TMOUT_INTR_HANDLER)


/***************************************
*       Type defines
***************************************/

/* Structure to save registers before go to sleep */
typedef struct
{
    uint8 enableState;

    #if(rtc_i2c_FF_IMPLEMENTED)
        uint8 xcfg;
        uint8 cfg;

        #if(rtc_i2c_MODE_SLAVE_ENABLED)
            uint8 addr;
        #endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */

        #if(CY_PSOC5A)
            uint8 clkDiv;
        #else
            uint8 clkDiv1;
            uint8 clkDiv2;
        #endif /* (CY_PSOC5A) */

    #else
        uint8 control;

        #if(CY_UDB_V0)
            uint8 intMask;

            #if(rtc_i2c_MODE_SLAVE_ENABLED)
                uint8 addr;
            #endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */
        #endif     /* (CY_UDB_V0) */

    #endif /* (rtc_i2c_FF_IMPLEMENTED) */

    #if(rtc_i2c_TIMEOUT_ENABLED)
        uint16 tmoutCfg;
        uint8  tmoutIntr;

        #if(rtc_i2c_TIMEOUT_PRESCALER_ENABLED && CY_UDB_V0)
            uint8 tmoutPrd;
        #endif /* (rtc_i2c_TIMEOUT_PRESCALER_ENABLED && CY_UDB_V0) */

    #endif /* (rtc_i2c_TIMEOUT_ENABLED) */

} rtc_i2c_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void rtc_i2c_Init(void)                            ;
void rtc_i2c_Enable(void)                          ;

void rtc_i2c_Start(void)                           ;
void rtc_i2c_Stop(void)                            ;

#define rtc_i2c_EnableInt()        CyIntEnable      (rtc_i2c_ISR_NUMBER)
#define rtc_i2c_DisableInt()       CyIntDisable     (rtc_i2c_ISR_NUMBER)
#define rtc_i2c_ClearPendingInt()  CyIntClearPending(rtc_i2c_ISR_NUMBER)
#define rtc_i2c_SetPendingInt()    CyIntSetPending  (rtc_i2c_ISR_NUMBER)

void rtc_i2c_SaveConfig(void)                      ;
void rtc_i2c_Sleep(void)                           ;
void rtc_i2c_RestoreConfig(void)                   ;
void rtc_i2c_Wakeup(void)                          ;

/* I2C Master functions prototypes */
#if(rtc_i2c_MODE_MASTER_ENABLED)
    /* Read and Clear status functions */
    uint8 rtc_i2c_MasterStatus(void)                ;
    uint8 rtc_i2c_MasterClearStatus(void)           ;

    /* Interrupt based operation functions */
    uint8 rtc_i2c_MasterWriteBuf(uint8 slaveAddress, uint8 * wrData, uint8 cnt, uint8 mode) \
                                                            ;
    uint8 rtc_i2c_MasterReadBuf(uint8 slaveAddress, uint8 * rdData, uint8 cnt, uint8 mode) \
                                                            ;
    uint8 rtc_i2c_MasterGetReadBufSize(void)       ;
    uint8 rtc_i2c_MasterGetWriteBufSize(void)      ;
    void  rtc_i2c_MasterClearReadBuf(void)         ;
    void  rtc_i2c_MasterClearWriteBuf(void)        ;

    /* Manual operation functions */
    uint8 rtc_i2c_MasterSendStart(uint8 slaveAddress, uint8 R_nW) \
                                                            ;
    uint8 rtc_i2c_MasterSendRestart(uint8 slaveAddress, uint8 R_nW) \
                                                            ;
    uint8 rtc_i2c_MasterSendStop(void)             ;
    uint8 rtc_i2c_MasterWriteByte(uint8 theByte)   ;
    uint8 rtc_i2c_MasterReadByte(uint8 acknNak)    ;

    /* This fake function use as workaround */
    void  rtc_i2c_Workaround(void)                 ;

#endif /* (rtc_i2c_MODE_MASTER_ENABLED) */

/* I2C Slave functions prototypes */
#if(rtc_i2c_MODE_SLAVE_ENABLED)
    /* Read and Clear status functions */
    uint8 rtc_i2c_SlaveStatus(void)                ;
    uint8 rtc_i2c_SlaveClearReadStatus(void)       ;
    uint8 rtc_i2c_SlaveClearWriteStatus(void)      ;

    void  rtc_i2c_SlaveSetAddress(uint8 address)   ;

    /* Interrupt based operation functions */
    void  rtc_i2c_SlaveInitReadBuf(uint8 * rdBuf, uint8 bufSize) \
                                                            ;
    void  rtc_i2c_SlaveInitWriteBuf(uint8 * wrBuf, uint8 bufSize) \
                                                            ;
    uint8 rtc_i2c_SlaveGetReadBufSize(void)        ;
    uint8 rtc_i2c_SlaveGetWriteBufSize(void)       ;
    void  rtc_i2c_SlaveClearReadBuf(void)          ;
    void  rtc_i2c_SlaveClearWriteBuf(void)         ;

    /* Communication bootloader I2C Slave APIs */
    #if defined(CYDEV_BOOTLOADER_IO_COMP) && (rtc_i2c_BOOTLOADER_INTERFACE_ENABLED)
        /* Physical layer functions */
        void     rtc_i2c_CyBtldrCommStart(void) CYSMALL \
                                                            ;
        void     rtc_i2c_CyBtldrCommStop(void)  CYSMALL \
                                                            ;
        void     rtc_i2c_CyBtldrCommReset(void) CYSMALL \
                                                            ;
        cystatus rtc_i2c_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) \
                                                        CYSMALL ;
        cystatus rtc_i2c_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)  CYSMALL \
                                                            ;

        #if(CYDEV_BOOTLOADER_IO_COMP == CyBtldr_rtc_i2c)
            #define CyBtldrCommStart    rtc_i2c_CyBtldrCommStart
            #define CyBtldrCommStop     rtc_i2c_CyBtldrCommStop
            #define CyBtldrCommReset    rtc_i2c_CyBtldrCommReset
            #define CyBtldrCommWrite    rtc_i2c_CyBtldrCommWrite
            #define CyBtldrCommRead     rtc_i2c_CyBtldrCommRead
        #endif /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_rtc_i2c) */

        /* Size of Read/Write buffers for I2C bootloader  */
        #define rtc_i2c_BTLDR_SIZEOF_READ_BUFFER   (0x80u)
        #define rtc_i2c_BTLDR_SIZEOF_WRITE_BUFFER  (0x80u)
        #define rtc_i2c_MIN_UNT16(a, b)            ( ((uint16)(a) < (b)) ? ((uint16) (a)) : ((uint16) (b)) )
        #define rtc_i2c_WAIT_1_MS                  (1u)

    #endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (rtc_i2c_BOOTLOADER_INTERFACE_ENABLED) */

#endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */

/* I2C interrupt handler */
CY_ISR_PROTO(rtc_i2c_ISR);
#if((rtc_i2c_FF_IMPLEMENTED) || (rtc_i2c_WAKEUP_ENABLED))
    CY_ISR_PROTO(rtc_i2c_WAKEUP_ISR);
#endif /* ((rtc_i2c_FF_IMPLEMENTED) || (rtc_i2c_WAKEUP_ENABLED)) */


/**********************************
*   Variable with external linkage
**********************************/

extern uint8 rtc_i2c_initVar;


/***************************************
*   Initial Parameter Constants
***************************************/

#define rtc_i2c_DATA_RATE          (400u)
#define rtc_i2c_DEFAULT_ADDR       (8u)
#define rtc_i2c_I2C_PAIR_SELECTED  (0u)

/* I2C pair enum */
#define rtc_i2c_I2C_PAIR_ANY   (0x01u) /* Any pins for I2C */
#define rtc_i2c_I2C_PAIR0      (0x01u) /* I2C0: (SCL = P12[4]) && (SCL = P12[5]) */
#define rtc_i2c_I2C_PAIR1      (0x02u) /* I2C1: (SCL = P12[0]) && (SDA = P12[1]) */

#define rtc_i2c_I2C1_SIO_PAIR  (rtc_i2c_I2C_PAIR1 == rtc_i2c_I2C_PAIR_SELECTED)
#define rtc_i2c_I2C0_SIO_PAIR  (rtc_i2c_I2C_PAIR0 == rtc_i2c_I2C_PAIR_SELECTED)


/***************************************
*            API Constants
***************************************/

/* Master/Slave control constants */
#define rtc_i2c_READ_XFER_MODE     (0x01u) /* Read */
#define rtc_i2c_WRITE_XFER_MODE    (0x00u) /* Write */
#define rtc_i2c_ACK_DATA           (0x01u) /* Send ACK */
#define rtc_i2c_NAK_DATA           (0x00u) /* Send NAK */
#define rtc_i2c_OVERFLOW_RETURN    (0xFFu) /* Senf on bus in case of overflow */

#if(rtc_i2c_MODE_MASTER_ENABLED)
    /* "Mode" constants for MasterWriteBuf() or MasterReadBuf() function */
    #define rtc_i2c_MODE_COMPLETE_XFER     (0x00u) /* Full transfer with Start and Stop */
    #define rtc_i2c_MODE_REPEAT_START      (0x01u) /* Begin with a ReStart instead of a Start */
    #define rtc_i2c_MODE_NO_STOP           (0x02u) /* Complete the transfer without a Stop */

    /* Master status */
    #define rtc_i2c_MSTAT_CLEAR            (0x00u) /* Clear (init) status value */

    #define rtc_i2c_MSTAT_RD_CMPLT         (0x01u) /* Read complete */
    #define rtc_i2c_MSTAT_WR_CMPLT         (0x02u) /* Write complete */
    #define rtc_i2c_MSTAT_XFER_INP         (0x04u) /* Master transfer in progress */
    #define rtc_i2c_MSTAT_XFER_HALT        (0x08u) /* Transfer is halted */

    #define rtc_i2c_MSTAT_ERR_MASK         (0xF0u) /* Mask for all errors */
    #define rtc_i2c_MSTAT_ERR_SHORT_XFER   (0x10u) /* Master NAKed before end of packet */
    #define rtc_i2c_MSTAT_ERR_ADDR_NAK     (0x20u) /* Slave did not ACK */
    #define rtc_i2c_MSTAT_ERR_ARB_LOST     (0x40u) /* Master lost arbitration during communication */
    #define rtc_i2c_MSTAT_ERR_XFER         (0x80u) /* Error during transfer */

    /* Master API returns */
    #define rtc_i2c_MSTR_NO_ERROR          (0x00u) /* Function complete without error */
    #define rtc_i2c_MSTR_BUS_BUSY          (0x01u) /* Bus is busy, process not started */
    #define rtc_i2c_MSTR_NOT_READY         (0x02u) /* Master not Master on the bus or */
                                                            /*  Slave operation in progress */
    #define rtc_i2c_MSTR_ERR_LB_NAK        (0x03u) /* Last Byte Naked */
    #define rtc_i2c_MSTR_ERR_ARB_LOST      (0x04u) /* Master lost arbitration during communication */
    #define rtc_i2c_MSTR_ERR_ABORT_START_GEN  (0x05u) /* Master did not generate Start, the Slave */
                                                               /* was addressed before */

#endif /* (rtc_i2c_MODE_MASTER_ENABLED) */

#if(rtc_i2c_MODE_SLAVE_ENABLED)
    /* Slave Status Constants */
    #define rtc_i2c_SSTAT_RD_CMPLT     (0x01u) /* Read transfer complete */
    #define rtc_i2c_SSTAT_RD_BUSY      (0x02u) /* Read transfer in progress */
    #define rtc_i2c_SSTAT_RD_ERR_OVFL  (0x04u) /* Read overflow Error */
    #define rtc_i2c_SSTAT_RD_MASK      (0x0Fu) /* Read Status Mask */
    #define rtc_i2c_SSTAT_RD_NO_ERR    (0x00u) /* Read no Error */

    #define rtc_i2c_SSTAT_WR_CMPLT     (0x10u) /* Write transfer complete */
    #define rtc_i2c_SSTAT_WR_BUSY      (0x20u) /* Write transfer in progress */
    #define rtc_i2c_SSTAT_WR_ERR_OVFL  (0x40u) /* Write overflow Error */
    #define rtc_i2c_SSTAT_WR_MASK      (0xF0u) /* Write Status Mask  */
    #define rtc_i2c_SSTAT_WR_NO_ERR    (0x00u) /* Write no Error */

    #define rtc_i2c_SSTAT_RD_CLEAR     (0x0Du) /* Read Status clear */
    #define rtc_i2c_SSTAT_WR_CLEAR     (0xD0u) /* Write Status Clear */

#endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */


/***************************************
*       I2C state machine constants
***************************************/

/* Default slave address states */
#define  rtc_i2c_SM_IDLE           (0x10u) /* Default state - IDLE */
#define  rtc_i2c_SM_EXIT_IDLE      (0x00u) /* Pass master and slave processing and go to IDLE */

/* Slave mode states */
#define  rtc_i2c_SM_SLAVE          (rtc_i2c_SM_IDLE) /* Any Slave state */
#define  rtc_i2c_SM_SL_WR_DATA     (0x11u) /* Master writes data to slzve  */
#define  rtc_i2c_SM_SL_RD_DATA     (0x12u) /* Master reads data from slave */

/* Master mode states */
#define  rtc_i2c_SM_MASTER         (0x40u) /* Any master state */

#define  rtc_i2c_SM_MSTR_RD        (0x08u) /* Any master read state          */
#define  rtc_i2c_SM_MSTR_RD_ADDR   (0x49u) /* Master sends address with read */
#define  rtc_i2c_SM_MSTR_RD_DATA   (0x4Au) /* Master reads data              */

#define  rtc_i2c_SM_MSTR_WR        (0x04u) /* Any master read state           */
#define  rtc_i2c_SM_MSTR_WR_ADDR   (0x45u) /* Master sends address with write */
#define  rtc_i2c_SM_MSTR_WR_DATA   (0x46u) /* Master writes data              */

#define  rtc_i2c_SM_MSTR_HALT      (0x60u) /* Master waits for ReStart */

#define rtc_i2c_CHECK_SM_MASTER    (0u != (rtc_i2c_SM_MASTER & rtc_i2c_state))
#define rtc_i2c_CHECK_SM_SLAVE     (0u != (rtc_i2c_SM_SLAVE  & rtc_i2c_state))


/***************************************
*              Registers
***************************************/

#if(rtc_i2c_FF_IMPLEMENTED)
    /* Fixed Function registers */
    #define rtc_i2c_XCFG_REG           (* (reg8 *) rtc_i2c_I2C_FF__XCFG)
    #define rtc_i2c_XCFG_PTR           (  (reg8 *) rtc_i2c_I2C_FF__XCFG)

    #define rtc_i2c_ADDR_REG           (* (reg8 *) rtc_i2c_I2C_FF__ADR)
    #define rtc_i2c_ADDR_PTR           (  (reg8 *) rtc_i2c_I2C_FF__ADR)

    #define rtc_i2c_CFG_REG            (* (reg8 *) rtc_i2c_I2C_FF__CFG)
    #define rtc_i2c_CFG_PTR            (  (reg8 *) rtc_i2c_I2C_FF__CFG)

    #define rtc_i2c_CSR_REG            (* (reg8 *) rtc_i2c_I2C_FF__CSR)
    #define rtc_i2c_CSR_PTR            (  (reg8 *) rtc_i2c_I2C_FF__CSR)

    #define rtc_i2c_DATA_REG           (* (reg8 *) rtc_i2c_I2C_FF__D)
    #define rtc_i2c_DATA_PTR           (  (reg8 *) rtc_i2c_I2C_FF__D)

    #define rtc_i2c_MCSR_REG           (* (reg8 *) rtc_i2c_I2C_FF__MCSR)
    #define rtc_i2c_MCSR_PTR           (  (reg8 *) rtc_i2c_I2C_FF__MCSR)

    #define rtc_i2c_ACT_PWRMGR_REG     (* (reg8 *) rtc_i2c_I2C_FF__PM_ACT_CFG)
    #define rtc_i2c_ACT_PWRMGR_PTR     (  (reg8 *) rtc_i2c_I2C_FF__PM_ACT_CFG)
    #define rtc_i2c_ACT_PWR_EN         (  (uint8)  rtc_i2c_I2C_FF__PM_ACT_MSK)

    #define rtc_i2c_STBY_PWRMGR_REG    (* (reg8 *) rtc_i2c_I2C_FF__PM_STBY_CFG)
    #define rtc_i2c_STBY_PWRMGR_PTR    (  (reg8 *) rtc_i2c_I2C_FF__PM_STBY_CFG)
    #define rtc_i2c_STBY_PWR_EN        (  (uint8)  rtc_i2c_I2C_FF__PM_STBY_MSK)

    #define rtc_i2c_PWRSYS_CR1_REG     (* (reg8 *) CYREG_PWRSYS_CR1)
    #define rtc_i2c_PWRSYS_CR1_PTR     (  (reg8 *) CYREG_PWRSYS_CR1)

    /* Clock divider register depends on silicon */
    #if(CY_PSOC5A)
        #define rtc_i2c_CLKDIV_REG     (* (reg8 *) rtc_i2c_I2C_FF__CLK_DIV)
        #define rtc_i2c_CLKDIV_PTR     (  (reg8 *) rtc_i2c_I2C_FF__CLK_DIV)

    #else
        #define rtc_i2c_CLKDIV1_REG    (* (reg8 *) rtc_i2c_I2C_FF__CLK_DIV1)
        #define rtc_i2c_CLKDIV1_PTR    (  (reg8 *) rtc_i2c_I2C_FF__CLK_DIV1)

        #define rtc_i2c_CLKDIV2_REG    (* (reg8 *) rtc_i2c_I2C_FF__CLK_DIV2)
        #define rtc_i2c_CLKDIV2_PTR    (  (reg8 *) rtc_i2c_I2C_FF__CLK_DIV2)

    #endif /* (CY_PSOC5A) */

#else
    /* UDB implementation registers */
    #define rtc_i2c_CFG_REG    (* (reg8 *) \
                                           rtc_i2c_bI2C_UDB_SyncCtl_CtrlReg__CONTROL_REG)
    #define rtc_i2c_CFG_PTR    (  (reg8 *) \
                                           rtc_i2c_bI2C_UDB_SyncCtl_CtrlReg__CONTROL_REG)

    #define rtc_i2c_CSR_REG        (* (reg8 *) rtc_i2c_bI2C_UDB_StsReg__STATUS_REG)
    #define rtc_i2c_CSR_PTR        (  (reg8 *) rtc_i2c_bI2C_UDB_StsReg__STATUS_REG)

    #define rtc_i2c_INT_MASK_REG   (* (reg8 *) rtc_i2c_bI2C_UDB_StsReg__MASK_REG)
    #define rtc_i2c_INT_MASK_PTR   (  (reg8 *) rtc_i2c_bI2C_UDB_StsReg__MASK_REG)

    #define rtc_i2c_INT_ENABLE_REG (* (reg8 *) rtc_i2c_bI2C_UDB_StsReg__STATUS_AUX_CTL_REG)
    #define rtc_i2c_INT_ENABLE_PTR (  (reg8 *) rtc_i2c_bI2C_UDB_StsReg__STATUS_AUX_CTL_REG)

    #define rtc_i2c_DATA_REG       (* (reg8 *) rtc_i2c_bI2C_UDB_Shifter_u0__A0_REG)
    #define rtc_i2c_DATA_PTR       (  (reg8 *) rtc_i2c_bI2C_UDB_Shifter_u0__A0_REG)

    #define rtc_i2c_GO_REG         (* (reg8 *) rtc_i2c_bI2C_UDB_Shifter_u0__F1_REG)
    #define rtc_i2c_GO_PTR         (  (reg8 *) rtc_i2c_bI2C_UDB_Shifter_u0__F1_REG)

    #define rtc_i2c_MCLK_PRD_REG   (* (reg8 *) rtc_i2c_bI2C_UDB_Master_ClkGen_u0__D0_REG)
    #define rtc_i2c_MCLK_PRD_PTR   (  (reg8 *) rtc_i2c_bI2C_UDB_Master_ClkGen_u0__D0_REG)

    #define rtc_i2c_MCLK_CMP_REG   (* (reg8 *) rtc_i2c_bI2C_UDB_Master_ClkGen_u0__D1_REG)
    #define rtc_i2c_MCLK_CMP_PTR   (  (reg8 *) rtc_i2c_bI2C_UDB_Master_ClkGen_u0__D1_REG)

    #if(rtc_i2c_MODE_SLAVE_ENABLED)
        #define rtc_i2c_ADDR_REG       (* (reg8 *) rtc_i2c_bI2C_UDB_Shifter_u0__D0_REG)
        #define rtc_i2c_ADDR_PTR       (  (reg8 *) rtc_i2c_bI2C_UDB_Shifter_u0__D0_REG)

        #define rtc_i2c_PERIOD_REG     (* (reg8 *) rtc_i2c_bI2C_UDB_Slave_BitCounter__PERIOD_REG)
        #define rtc_i2c_PERIOD_PTR     (  (reg8 *) rtc_i2c_bI2C_UDB_Slave_BitCounter__PERIOD_REG)

        #define rtc_i2c_COUNTER_REG    (* (reg8 *) rtc_i2c_bI2C_UDB_Slave_BitCounter__COUNT_REG)
        #define rtc_i2c_COUNTER_PTR    (  (reg8 *) rtc_i2c_bI2C_UDB_Slave_BitCounter__COUNT_REG)

        #define rtc_i2c_COUNTER_AUX_CTL_REG  (* (reg8 *) \
                                                        rtc_i2c_bI2C_UDB_Slave_BitCounter__CONTROL_AUX_CTL_REG)
        #define rtc_i2c_COUNTER_AUX_CTL_PTR  (  (reg8 *) \
                                                        rtc_i2c_bI2C_UDB_Slave_BitCounter__CONTROL_AUX_CTL_REG)

    #endif /* (rtc_i2c_MODE_SLAVE_ENABLED) */

#endif /* (rtc_i2c_FF_IMPLEMENTED) */


/***************************************
*        Registers Constants
***************************************/

/* rtc_i2c_I2C_IRQ */
#define rtc_i2c_ISR_NUMBER     ((uint8) rtc_i2c_I2C_IRQ__INTC_NUMBER)
#define rtc_i2c_ISR_PRIORITY   ((uint8) rtc_i2c_I2C_IRQ__INTC_PRIOR_NUM)

/* I2C Slave Data Register */
#define rtc_i2c_SLAVE_ADDR_MASK    (0x7Fu)
#define rtc_i2c_SLAVE_ADDR_SHIFT   (0x01u)
#define rtc_i2c_DATA_MASK          (0xFFu)
#define rtc_i2c_READ_FLAG          (0x01u)

#define rtc_i2c_FF_RESET_DELAY     (0x02u)

#if(rtc_i2c_FF_IMPLEMENTED)
    /* XCFG I2C Extended Configuration Register */
    #define rtc_i2c_XCFG_CLK_EN        (0x80u) /* Enable gated clock to block */
    #define rtc_i2c_XCFG_I2C_ON        (0x40u) /* Enable I2C as wake up source*/
    #define rtc_i2c_XCFG_RDY_TO_SLEEP  (0x20u) /* I2C ready go to sleep */
    #define rtc_i2c_XCFG_FORCE_NACK    (0x10u) /* Force NACK all incomming transactions */
    #define rtc_i2c_XCFG_NO_BC_INT     (0x08u) /* No interrupt on byte complete */
    #define rtc_i2c_XCFG_BUF_MODE      (0x02u) /* Enable buffer mode */
    #define rtc_i2c_XCFG_HDWR_ADDR_EN  (0x01u) /* Enable Hardware address match */

    /* CFG I2C Configuration Register */
    #define rtc_i2c_CFG_SIO_SELECT     (0x80u) /* Pin Select for SCL/SDA lines */
    #define rtc_i2c_CFG_PSELECT        (0x40u) /* Pin Select */
    #define rtc_i2c_CFG_BUS_ERR_IE     (0x20u) /* Bus Error Interrupt Enable */
    #define rtc_i2c_CFG_STOP_IE        (0x10u) /* Enable Interrupt on STOP condition */
    #define rtc_i2c_CFG_CLK_RATE_MSK   (0x0Cu) /* Clock rate select  **CHECK**  */
    #define rtc_i2c_CFG_CLK_RATE_100   (0x00u) /* Clock rate select 100K */
    #define rtc_i2c_CFG_CLK_RATE_400   (0x04u) /* Clock rate select 400K */
    #define rtc_i2c_CFG_CLK_RATE_050   (0x08u) /* Clock rate select 50K  */
    #define rtc_i2c_CFG_CLK_RATE_RSVD  (0x0Cu) /* Clock rate select Invalid */
    #define rtc_i2c_CFG_EN_MSTR        (0x02u) /* Enable Master operation */
    #define rtc_i2c_CFG_EN_SLAVE       (0x01u) /* Enable Slave operation */

    #define rtc_i2c_CFG_CLK_RATE_LESS_EQUAL_50 (0x04u) /* Clock rate select <= 50kHz */
    #define rtc_i2c_CFG_CLK_RATE_GRATER_50     (0x00u) /* Clock rate select > 50kHz */

    /* CSR I2C Control and Status Register */
    #define rtc_i2c_CSR_BUS_ERROR      (0x80u) /* Active high when bus error has occured */
    #define rtc_i2c_CSR_LOST_ARB       (0x40u) /* Set to 1 if lost arbitration in host mode */
    #define rtc_i2c_CSR_STOP_STATUS    (0x20u) /* Set if Stop has been detected */
    #define rtc_i2c_CSR_ACK            (0x10u) /* ACK response */
    #define rtc_i2c_CSR_NAK            (0x00u) /* NAK response */
    #define rtc_i2c_CSR_ADDRESS        (0x08u) /* Set in firmware 0 = status bit, 1 Address is slave */
    #define rtc_i2c_CSR_TRANSMIT       (0x04u) /* Set in firmware 1 = transmit, 0 = receive */
    #define rtc_i2c_CSR_LRB            (0x02u) /* Last received bit */
    #define rtc_i2c_CSR_LRB_ACK        (0x00u) /* Last received bit was an ACK */
    #define rtc_i2c_CSR_LRB_NAK        (0x02u) /* Last received bit was an NAK */
    #define rtc_i2c_CSR_BYTE_COMPLETE  (0x01u) /* Informs that last byte has been sent */
    #define rtc_i2c_CSR_STOP_GEN       (0x00u) /* Generate a stop condition */
    #define rtc_i2c_CSR_RDY_TO_RD      (0x00u) /* Set to recieve mode */

    /* MCSR I2C Master Control and Status Register */
    #define rtc_i2c_MCSR_STOP_GEN      (0x10u) /* Firmware sets this bit to initiate a Stop condition */
    #define rtc_i2c_MCSR_BUS_BUSY      (0x08u) /* Status bit, Set at Start and cleared at Stop condition */
    #define rtc_i2c_MCSR_MSTR_MODE     (0x04u) /* Status bit, Set at Start and cleared at Stop condition */
    #define rtc_i2c_MCSR_RESTART_GEN   (0x02u) /* Firmware sets this bit to initiate a ReStart condition */
    #define rtc_i2c_MCSR_START_GEN     (0x01u) /* Firmware sets this bit to initiate a Start condition */

    /* CLK_DIV I2C Clock Divide Factor Register */
    #define rtc_i2c_CLK_DIV_MSK    (0x07u) /* Status bit, Set at Start and cleared at Stop condition */
    #define rtc_i2c_CLK_DIV_1      (0x00u) /* Divide input clock by  1 */
    #define rtc_i2c_CLK_DIV_2      (0x01u) /* Divide input clock by  2 */
    #define rtc_i2c_CLK_DIV_4      (0x02u) /* Divide input clock by  4 */
    #define rtc_i2c_CLK_DIV_8      (0x03u) /* Divide input clock by  8 */
    #define rtc_i2c_CLK_DIV_16     (0x04u) /* Divide input clock by 16 */
    #define rtc_i2c_CLK_DIV_32     (0x05u) /* Divide input clock by 32 */
    #define rtc_i2c_CLK_DIV_64     (0x06u) /* Divide input clock by 64 */

    /* PWRSYS_CR1 to handle Sleep */
    #define rtc_i2c_PWRSYS_CR1_I2C_REG_BACKUP  (0x04u) /* Enables, power to I2C regs while sleep */

#else
    /* CONTROL REG bits location */
    #define rtc_i2c_CTRL_START_SHIFT           (7u)
    #define rtc_i2c_CTRL_STOP_SHIFT            (6u)
    #define rtc_i2c_CTRL_RESTART_SHIFT         (5u)
    #define rtc_i2c_CTRL_NACK_SHIFT            (4u)
    #define rtc_i2c_CTRL_ANY_ADDRESS_SHIFT     (3u)
    #define rtc_i2c_CTRL_TRANSMIT_SHIFT        (2u)
    #define rtc_i2c_CTRL_ENABLE_MASTER_SHIFT   (1u)
    #define rtc_i2c_CTRL_ENABLE_SLAVE_SHIFT    (0u)
    #define rtc_i2c_CTRL_START_MASK            ((uint8) (0x01u << rtc_i2c_CTRL_START_SHIFT))
    #define rtc_i2c_CTRL_STOP_MASK             ((uint8) (0x01u << rtc_i2c_CTRL_STOP_SHIFT))
    #define rtc_i2c_CTRL_RESTART_MASK          ((uint8) (0x01u << rtc_i2c_CTRL_RESTART_SHIFT))
    #define rtc_i2c_CTRL_NACK_MASK             ((uint8) (0x01u << rtc_i2c_CTRL_NACK_SHIFT))
    #define rtc_i2c_CTRL_ANY_ADDRESS_MASK      ((uint8) (0x01u << rtc_i2c_CTRL_ANY_ADDRESS_SHIFT))
    #define rtc_i2c_CTRL_TRANSMIT_MASK         ((uint8) (0x01u << rtc_i2c_CTRL_TRANSMIT_SHIFT))
    #define rtc_i2c_CTRL_ENABLE_MASTER_MASK    ((uint8) (0x01u << rtc_i2c_CTRL_ENABLE_MASTER_SHIFT))
    #define rtc_i2c_CTRL_ENABLE_SLAVE_MASK     ((uint8) (0x01u << rtc_i2c_CTRL_ENABLE_SLAVE_SHIFT))

    /* STATUS REG bits location */
    #define rtc_i2c_STS_LOST_ARB_SHIFT         (6u)
    #define rtc_i2c_STS_STOP_SHIFT             (5u)
    #define rtc_i2c_STS_BUSY_SHIFT             (4u)
    #define rtc_i2c_STS_ADDR_SHIFT             (3u)
    #define rtc_i2c_STS_MASTER_MODE_SHIFT      (2u)
    #define rtc_i2c_STS_LRB_SHIFT              (1u)
    #define rtc_i2c_STS_BYTE_COMPLETE_SHIFT    (0u)
    #define rtc_i2c_STS_LOST_ARB_MASK          ((uint8) (0x01u << rtc_i2c_STS_LOST_ARB_SHIFT))
    #define rtc_i2c_STS_STOP_MASK              ((uint8) (0x01u << rtc_i2c_STS_STOP_SHIFT))
    #define rtc_i2c_STS_BUSY_MASK              ((uint8) (0x01u << rtc_i2c_STS_BUSY_SHIFT))
    #define rtc_i2c_STS_ADDR_MASK              ((uint8) (0x01u << rtc_i2c_STS_ADDR_SHIFT))
    #define rtc_i2c_STS_MASTER_MODE_MASK       ((uint8) (0x01u << rtc_i2c_STS_MASTER_MODE_SHIFT))
    #define rtc_i2c_STS_LRB_MASK               ((uint8) (0x01u << rtc_i2c_STS_LRB_SHIFT))
    #define rtc_i2c_STS_BYTE_COMPLETE_MASK     ((uint8) (0x01u << rtc_i2c_STS_BYTE_COMPLETE_SHIFT))

    /* AUX_CTL bits definition */
    #define rtc_i2c_COUNTER_ENABLE_MASK        (0x20u) /* Enable 7-bit counter     */
    #define rtc_i2c_INT_ENABLE_MASK            (0x10u) /* Enable intr from statusi */
    #define rtc_i2c_CNT7_ENABLE                (rtc_i2c_COUNTER_ENABLE_MASK)
    #define rtc_i2c_INTR_ENABLE                (rtc_i2c_INT_ENABLE_MASK)

#endif /* (rtc_i2c_FF_IMPLEMENTED) */


/***************************************
*        Marco
***************************************/

/* ACK and NACK for data and address checks */
#define rtc_i2c_CHECK_ADDR_ACK(csr)    ((rtc_i2c_CSR_LRB_ACK | rtc_i2c_CSR_ADDRESS) == \
                                                 ((rtc_i2c_CSR_LRB    | rtc_i2c_CSR_ADDRESS) &  \
                                                  (csr)))


#define rtc_i2c_CHECK_ADDR_NAK(csr)    ((rtc_i2c_CSR_LRB_NAK | rtc_i2c_CSR_ADDRESS) == \
                                                 ((rtc_i2c_CSR_LRB    | rtc_i2c_CSR_ADDRESS) &  \
                                                  (csr)))

#define rtc_i2c_CHECK_DATA_ACK(csr)    (0u == ((csr) & rtc_i2c_CSR_LRB_NAK))

/* MCSR conditions check */
#define rtc_i2c_CHECK_BUS_FREE(mcsr)       (0u == ((mcsr) & rtc_i2c_MCSR_BUS_BUSY))
#define rtc_i2c_CHECK_MASTER_MODE(mcsr)    (0u != ((mcsr) & rtc_i2c_MCSR_MSTR_MODE))

/* CSR conditions check */
#define rtc_i2c_WAIT_BYTE_COMPLETE(csr)    (0u == ((csr) & rtc_i2c_CSR_BYTE_COMPLETE))
#define rtc_i2c_WAIT_STOP_COMPLETE(csr)    (0u == ((csr) & (rtc_i2c_CSR_BYTE_COMPLETE | \
                                                                     rtc_i2c_CSR_STOP_STATUS)))
#define rtc_i2c_CHECK_BYTE_COMPLETE(csr)   (0u != ((csr) & rtc_i2c_CSR_BYTE_COMPLETE))
#define rtc_i2c_CHECK_STOP_STS(csr)        (0u != ((csr) & rtc_i2c_CSR_STOP_STATUS))
#define rtc_i2c_CHECK_LOST_ARB(csr)        (0u != ((csr) & rtc_i2c_CSR_LOST_ARB))
#define rtc_i2c_CHECK_ADDRESS_STS(csr)     (0u != ((csr) & rtc_i2c_CSR_ADDRESS))

/* Software start and end of transaction check */
#define rtc_i2c_CHECK_RESTART(mstrCtrl)    (0u != ((mstrCtrl) & rtc_i2c_MODE_REPEAT_START))
#define rtc_i2c_CHECK_NO_STOP(mstrCtrl)    (0u != ((mstrCtrl) & rtc_i2c_MODE_NO_STOP))

/* Send read or write completion depends on state */
#define rtc_i2c_GET_MSTAT_CMPLT ((0u != (rtc_i2c_state & rtc_i2c_SM_MSTR_RD)) ? \
                                                 (rtc_i2c_MSTAT_RD_CMPLT) : (rtc_i2c_MSTAT_WR_CMPLT))

/* Returns 7-bit slave address and used for software address match */
#define rtc_i2c_GET_SLAVE_ADDR(dataReg)   (((dataReg) >> rtc_i2c_SLAVE_ADDR_SHIFT) & \
                                                                  rtc_i2c_SLAVE_ADDR_MASK)

#if(rtc_i2c_FF_IMPLEMENTED)
    /* Check enable of module */
    #define rtc_i2c_I2C_ENABLE_REG     (rtc_i2c_ACT_PWRMGR_REG)
    #define rtc_i2c_IS_I2C_ENABLE(reg) (0u != ((reg) & rtc_i2c_ACT_PWR_EN))
    #define rtc_i2c_IS_ENABLED         (0u != (rtc_i2c_ACT_PWRMGR_REG & rtc_i2c_ACT_PWR_EN))

    #define rtc_i2c_CHECK_PWRSYS_I2C_BACKUP    (0u != (rtc_i2c_PWRSYS_CR1_I2C_REG_BACKUP & \
                                                                rtc_i2c_PWRSYS_CR1_REG))

    /* Check start condition generation */
    #define rtc_i2c_CHECK_START_GEN(mcsr)  ((0u != ((mcsr) & rtc_i2c_MCSR_START_GEN)) && \
                                                     (0u == ((mcsr) & rtc_i2c_MCSR_MSTR_MODE)))

    #define rtc_i2c_CLEAR_START_GEN        do{ \
                                                        rtc_i2c_MCSR_REG &=                                   \
                                                                           ((uint8) ~rtc_i2c_MCSR_START_GEN); \
                                                    }while(0)

    /* Stop interrupt */
    #define rtc_i2c_ENABLE_INT_ON_STOP     do{ \
                                                        rtc_i2c_CFG_REG |= rtc_i2c_CFG_STOP_IE; \
                                                    }while(0)

    #define rtc_i2c_DISABLE_INT_ON_STOP    do{ \
                                                        rtc_i2c_CFG_REG &=                                 \
                                                                           ((uint8) ~rtc_i2c_CFG_STOP_IE); \
                                                    }while(0)

    /* Transmit data */
    #define rtc_i2c_TRANSMIT_DATA          do{ \
                                                        rtc_i2c_CSR_REG = rtc_i2c_CSR_TRANSMIT; \
                                                    }while(0)

    #define rtc_i2c_ACK_AND_TRANSMIT       do{ \
                                                        rtc_i2c_CSR_REG = (rtc_i2c_CSR_ACK |      \
                                                                                    rtc_i2c_CSR_TRANSMIT); \
                                                    }while(0)

    #define rtc_i2c_NAK_AND_TRANSMIT       do{ \
                                                        rtc_i2c_CSR_REG = rtc_i2c_CSR_NAK; \
                                                    }while(0)

    /* Special case: udb needs to ack, ff needs to nak */
    #define rtc_i2c_ACKNAK_AND_TRANSMIT    do{ \
                                                        rtc_i2c_CSR_REG  = (rtc_i2c_CSR_NAK |      \
                                                                                     rtc_i2c_CSR_TRANSMIT); \
                                                    }while(0)
    /* Receive data */
    #define rtc_i2c_ACK_AND_RECEIVE        do{ \
                                                        rtc_i2c_CSR_REG = rtc_i2c_CSR_ACK; \
                                                    }while(0)

    #define rtc_i2c_NAK_AND_RECEIVE        do{ \
                                                        rtc_i2c_CSR_REG = rtc_i2c_CSR_NAK; \
                                                    }while(0)

    #define rtc_i2c_READY_TO_READ          do{ \
                                                        rtc_i2c_CSR_REG = rtc_i2c_CSR_RDY_TO_RD; \
                                                    }while(0)

    /* Master condition generation */
    #define rtc_i2c_GENERATE_START         do{ \
                                                        rtc_i2c_MCSR_REG = rtc_i2c_MCSR_START_GEN; \
                                                    }while(0)

    #if(CY_PSOC5A)
        #define rtc_i2c_GENERATE_RESTART \
                        do{ \
                            rtc_i2c_MCSR_REG = rtc_i2c_MCSR_RESTART_GEN; \
                            rtc_i2c_CSR_REG  = rtc_i2c_CSR_NAK;          \
                        }while(0)

        #define rtc_i2c_GENERATE_STOP      do{ \
                                                        rtc_i2c_CSR_REG = rtc_i2c_CSR_STOP_GEN; \
                                                    }while(0)

    #else   /* PSoC3 ES3 handlees zero lenght packets */
        #define rtc_i2c_GENERATE_RESTART \
                        do{ \
                            rtc_i2c_MCSR_REG = (rtc_i2c_MCSR_RESTART_GEN | \
                                                         rtc_i2c_MCSR_STOP_GEN);    \
                            rtc_i2c_CSR_REG  = rtc_i2c_CSR_TRANSMIT;       \
                        }while(0)

        #define rtc_i2c_GENERATE_STOP \
                        do{ \
                            rtc_i2c_MCSR_REG = rtc_i2c_MCSR_STOP_GEN; \
                            rtc_i2c_CSR_REG  = rtc_i2c_CSR_TRANSMIT;  \
                        }while(0)
    #endif /* (CY_PSOC5A) */

    /* Master manual APIs compatible defines */
    #define rtc_i2c_GENERATE_RESTART_MANUAL    rtc_i2c_GENERATE_RESTART
    #define rtc_i2c_GENERATE_STOP_MANUAL       rtc_i2c_GENERATE_STOP
    #define rtc_i2c_TRANSMIT_DATA_MANUAL       rtc_i2c_TRANSMIT_DATA
    #define rtc_i2c_READY_TO_READ_MANUAL       rtc_i2c_READY_TO_READ
    #define rtc_i2c_ACK_AND_RECEIVE_MANUAL     rtc_i2c_ACK_AND_RECEIVE

#else

    /* Masks to enalbe interrupts from Status register */
    #define rtc_i2c_STOP_IE_MASK           (rtc_i2c_STS_STOP_MASK)
    #define rtc_i2c_BYTE_COMPLETE_IE_MASK  (rtc_i2c_STS_BYTE_COMPLETE_MASK)

    /* FF compatibility: CSR gegisters definitions */
    #define rtc_i2c_CSR_LOST_ARB       (rtc_i2c_STS_LOST_ARB_MASK)
    #define rtc_i2c_CSR_STOP_STATUS    (rtc_i2c_STS_STOP_MASK)
    #define rtc_i2c_CSR_BUS_ERROR      (0x00u)
    #define rtc_i2c_CSR_ADDRESS        (rtc_i2c_STS_ADDR_MASK)
    #define rtc_i2c_CSR_TRANSMIT       (rtc_i2c_CTRL_TRANSMIT_MASK)
    #define rtc_i2c_CSR_LRB            (rtc_i2c_STS_LRB_MASK)
    #define rtc_i2c_CSR_LRB_NAK        (rtc_i2c_STS_LRB_MASK)
    #define rtc_i2c_CSR_LRB_ACK        (0x00u)
    #define rtc_i2c_CSR_BYTE_COMPLETE  (rtc_i2c_STS_BYTE_COMPLETE_MASK)

    /* FF compatibility: MCSR gegisters definitions */
    #define rtc_i2c_MCSR_REG           (rtc_i2c_CSR_REG)   /* UDB incoporates master and slave regs */
    #define rtc_i2c_MCSR_BUS_BUSY      (rtc_i2c_STS_BUSY_MASK)       /* Is bus is busy              */
    #define rtc_i2c_MCSR_START_GEN     (rtc_i2c_CTRL_START_MASK)     /* Generate Sart condition     */
    #define rtc_i2c_MCSR_RESTART_GEN   (rtc_i2c_CTRL_RESTART_MASK)   /* Generates RESTART condition */
    #define rtc_i2c_MCSR_MSTR_MODE     (rtc_i2c_STS_MASTER_MODE_MASK)/* Define if active Master     */

    /* Data to write into TX FIFO to release FSM */
    #define rtc_i2c_RELEASE_FSM         (0x00u)
    
    /* Check enable of module */
    #define rtc_i2c_I2C_ENABLE_REG     (rtc_i2c_CFG_REG)
    #define rtc_i2c_IS_I2C_ENABLE(reg) ((0u != ((reg) & rtc_i2c_ENABLE_MASTER)) || \
                                                 (0u != ((reg) & rtc_i2c_ENABLE_SLAVE)))

    #define rtc_i2c_IS_ENABLED         (0u != (rtc_i2c_CFG_REG & rtc_i2c_ENABLE_MS))

    /* Check start condition generation */
    #define rtc_i2c_CHECK_START_GEN(mcsr)  ((0u != (rtc_i2c_CFG_REG &        \
                                                             rtc_i2c_MCSR_START_GEN)) \
                                                    &&                                         \
                                                    (0u == ((mcsr) & rtc_i2c_MCSR_MSTR_MODE)))

    #define rtc_i2c_CLEAR_START_GEN        do{ \
                                                        rtc_i2c_CFG_REG &=                 \
                                                        ((uint8) ~rtc_i2c_MCSR_START_GEN); \
                                                    }while(0)


    /* Stop interrupt */
    #define rtc_i2c_ENABLE_INT_ON_STOP     do{ \
                                                       rtc_i2c_INT_MASK_REG |= rtc_i2c_STOP_IE_MASK; \
                                                    }while(0)

    #define rtc_i2c_DISABLE_INT_ON_STOP    do{ \
                                                        rtc_i2c_INT_MASK_REG &=                               \
                                                                             ((uint8) ~rtc_i2c_STOP_IE_MASK); \
                                                    }while(0)


    /* Transmit data */
    #define rtc_i2c_TRANSMIT_DATA      do{ \
                                                    rtc_i2c_CFG_REG = (rtc_i2c_CTRL_TRANSMIT_MASK | \
                                                                                rtc_i2c_CTRL_DEFAULT);       \
                                                    rtc_i2c_GO_REG  = rtc_i2c_RELEASE_FSM;          \
                                                }while(0)

    #define rtc_i2c_ACK_AND_TRANSMIT   rtc_i2c_TRANSMIT_DATA


    #define rtc_i2c_NAK_AND_TRANSMIT   do{ \
                                                    rtc_i2c_CFG_REG = (rtc_i2c_CTRL_NACK_MASK     | \
                                                                                rtc_i2c_CTRL_TRANSMIT_MASK | \
                                                                                rtc_i2c_CTRL_DEFAULT);       \
                                                    rtc_i2c_GO_REG  =  rtc_i2c_RELEASE_FSM;         \
                                                }while(0)

    /* Receive data */
    #define rtc_i2c_READY_TO_READ      do{ \
                                                    rtc_i2c_CFG_REG = rtc_i2c_CTRL_DEFAULT; \
                                                    rtc_i2c_GO_REG  =  rtc_i2c_RELEASE_FSM; \
                                                }while(0)

    #define rtc_i2c_ACK_AND_RECEIVE    rtc_i2c_READY_TO_READ

    #define rtc_i2c_NAK_AND_RECEIVE    do{ \
                                                    rtc_i2c_CFG_REG = (rtc_i2c_CTRL_NACK_MASK | \
                                                                                rtc_i2c_CTRL_DEFAULT);   \
                                                    rtc_i2c_GO_REG  =  rtc_i2c_RELEASE_FSM;     \
                                                }while(0)

    /* Master condition generation */
    #define rtc_i2c_GENERATE_START     do{ \
                                                    rtc_i2c_CFG_REG = (rtc_i2c_CTRL_START_MASK | \
                                                                                 rtc_i2c_CTRL_DEFAULT);   \
                                                    rtc_i2c_GO_REG  =  rtc_i2c_RELEASE_FSM;      \
                                                }while(0)

    #define rtc_i2c_GENERATE_RESTART   do{ \
                                                    rtc_i2c_CFG_REG = (rtc_i2c_CTRL_RESTART_MASK | \
                                                                                rtc_i2c_CTRL_NACK_MASK    | \
                                                                                rtc_i2c_CTRL_DEFAULT);      \
                                                    rtc_i2c_GO_REG  =  rtc_i2c_RELEASE_FSM;        \
                                                }while(0)


    #define rtc_i2c_GENERATE_STOP      do{ \
                                                    rtc_i2c_CFG_REG = (rtc_i2c_CTRL_NACK_MASK | \
                                                                                rtc_i2c_CTRL_STOP_MASK | \
                                                                                rtc_i2c_CTRL_DEFAULT);   \
                                                    rtc_i2c_GO_REG  =  rtc_i2c_RELEASE_FSM;     \
                                                }while(0)

    /* Master manual APIs compatible defines */
    /* These defines wait while byte complete is cleared after command issued */
    #define rtc_i2c_GENERATE_RESTART_MANUAL    \
                                        do{             \
                                            rtc_i2c_GENERATE_RESTART;                                    \
                                            while(rtc_i2c_CHECK_BYTE_COMPLETE(rtc_i2c_CSR_REG)) \
                                            {                                                                     \
                                                ; /* Wait when byte complete is cleared */                        \
                                            }                                                                     \
                                        }while(0)

    #define rtc_i2c_GENERATE_STOP_MANUAL   \
                                        do{         \
                                            rtc_i2c_GENERATE_STOP;                                       \
                                            while(rtc_i2c_CHECK_BYTE_COMPLETE(rtc_i2c_CSR_REG)) \
                                            {                                                                     \
                                                ; /* Wait when byte complete is cleared */                        \
                                            }                                                                     \
                                        }while(0)

    #define rtc_i2c_TRANSMIT_DATA_MANUAL   \
                                        do{         \
                                            rtc_i2c_TRANSMIT_DATA;                                       \
                                            while(rtc_i2c_CHECK_BYTE_COMPLETE(rtc_i2c_CSR_REG)) \
                                            {                                                                     \
                                                ; /* Wait when byte complete is cleared */                        \
                                            }                                                                     \
                                        }while(0)

    #define rtc_i2c_READY_TO_READ_MANUAL   \
                                        do{         \
                                            rtc_i2c_READY_TO_READ;      \
                                            while(rtc_i2c_CHECK_BYTE_COMPLETE(rtc_i2c_CSR_REG)) \
                                            {                                                                     \
                                                ; /* Wait when byte complete is cleared */                        \
                                            }                                                                     \
                                        }while(0)

    #define rtc_i2c_ACK_AND_RECEIVE_MANUAL \
                                        do{         \
                                            rtc_i2c_ACK_AND_RECEIVE;                                     \
                                            while(rtc_i2c_CHECK_BYTE_COMPLETE(rtc_i2c_CSR_REG)) \
                                            {                                                                     \
                                                ; /* Wait when byte complete is cleared */                        \
                                            }                                                                     \
                                        }while(0)
#endif /* (rtc_i2c_FF_IMPLEMENTED) */

/* Comon for FF and UDB: used to release bus after lost arb */
#define rtc_i2c_BUS_RELEASE    rtc_i2c_READY_TO_READ


/***************************************
*     Default register init constants
***************************************/

#define rtc_i2c_DISABLE    (0u)
#define rtc_i2c_ENABLE     (1u)

#if(rtc_i2c_FF_IMPLEMENTED)
    /* rtc_i2c_XCFG_REG: bits definition */
    #define rtc_i2c_DEFAULT_XCFG_HW_ADDR_EN ((rtc_i2c_HW_ADRR_DECODE) ? \
                                                        (rtc_i2c_XCFG_HDWR_ADDR_EN) : (0u))

    #define rtc_i2c_DEFAULT_XCFG_I2C_ON    ((rtc_i2c_WAKEUP_ENABLED) ? \
                                                        (rtc_i2c_XCFG_I2C_ON) : (0u))


    #define rtc_i2c_DEFAULT_CFG_SIO_SELECT ((rtc_i2c_I2C1_SIO_PAIR) ? \
                                                        (rtc_i2c_CFG_SIO_SELECT) : (0u))


    /* rtc_i2c_CFG_REG: bits definition */
    #define rtc_i2c_DEFAULT_CFG_PSELECT    ((rtc_i2c_WAKEUP_ENABLED) ? \
                                                        (rtc_i2c_CFG_PSELECT) : (0u))

    #define rtc_i2c_DEFAULT_CLK_RATE0  ((rtc_i2c_DATA_RATE <= 50u) ?        \
                                                    (rtc_i2c_CFG_CLK_RATE_050) :     \
                                                    ((rtc_i2c_DATA_RATE <= 100u) ?   \
                                                        (rtc_i2c_CFG_CLK_RATE_100) : \
                                                        (rtc_i2c_CFG_CLK_RATE_400)))

    #define rtc_i2c_DEFAULT_CLK_RATE1  ((rtc_i2c_DATA_RATE <= 50u) ?           \
                                                 (rtc_i2c_CFG_CLK_RATE_LESS_EQUAL_50) : \
                                                 (rtc_i2c_CFG_CLK_RATE_GRATER_50))

    #define rtc_i2c_DEFAULT_CLK_RATE   ((CY_PSOC5A) ? (rtc_i2c_DEFAULT_CLK_RATE0) : \
                                                               (rtc_i2c_DEFAULT_CLK_RATE1))


    #define rtc_i2c_ENABLE_MASTER      ((rtc_i2c_MODE_MASTER_ENABLED) ? \
                                                 (rtc_i2c_CFG_EN_MSTR) : (0u))

    #define rtc_i2c_ENABLE_SLAVE       ((rtc_i2c_MODE_SLAVE_ENABLED) ? \
                                                 (rtc_i2c_CFG_EN_SLAVE) : (0u))

    #define rtc_i2c_ENABLE_MS      (rtc_i2c_ENABLE_MASTER | rtc_i2c_ENABLE_SLAVE)


    /* rtc_i2c_DEFAULT_XCFG_REG */
    #define rtc_i2c_DEFAULT_XCFG   (rtc_i2c_XCFG_CLK_EN         | \
                                             rtc_i2c_DEFAULT_XCFG_I2C_ON | \
                                             rtc_i2c_DEFAULT_XCFG_HW_ADDR_EN)

    /* rtc_i2c_DEFAULT_CFG_REG */
    #define rtc_i2c_DEFAULT_CFG    (rtc_i2c_DEFAULT_CFG_SIO_SELECT | \
                                             rtc_i2c_DEFAULT_CFG_PSELECT    | \
                                             rtc_i2c_DEFAULT_CLK_RATE       | \
                                             rtc_i2c_ENABLE_MASTER          | \
                                             rtc_i2c_ENABLE_SLAVE)

    /*rtc_i2c_DEFAULT_DIVIDE_FACTOR_REG */
    #define rtc_i2c_DEFAULT_DIVIDE_FACTOR  ((CY_PSOC5A) ? ((uint8) 2u) : ((uint16) 4u))

#else
    /* rtc_i2c_CFG_REG: bits definition  */
    #define rtc_i2c_ENABLE_MASTER  ((rtc_i2c_MODE_MASTER_ENABLED) ? \
                                             (rtc_i2c_CTRL_ENABLE_MASTER_MASK) : (0u))

    #define rtc_i2c_ENABLE_SLAVE   ((rtc_i2c_MODE_SLAVE_ENABLED) ? \
                                             (rtc_i2c_CTRL_ENABLE_SLAVE_MASK) : (0u))

    #define rtc_i2c_ENABLE_MS      (rtc_i2c_ENABLE_MASTER | rtc_i2c_ENABLE_SLAVE)


    #define rtc_i2c_DEFAULT_CTRL_ANY_ADDR   ((rtc_i2c_HW_ADRR_DECODE) ? \
                                                      (0u) : (rtc_i2c_CTRL_ANY_ADDRESS_MASK))

    /* rtc_i2c_DEFAULT_CFG_REG */
    #define rtc_i2c_DEFAULT_CFG    (rtc_i2c_DEFAULT_CTRL_ANY_ADDR)

    /* All CTRL default bits to be used in macro */
    #define rtc_i2c_CTRL_DEFAULT   (rtc_i2c_DEFAULT_CTRL_ANY_ADDR | rtc_i2c_ENABLE_MS)

    /* Master clock generator: d0 and d1 */
    #define rtc_i2c_MCLK_PERIOD_VALUE  (0x0Fu)
    #define rtc_i2c_MCLK_COMPARE_VALUE (0x08u)

    /* Slave bit-counter: contorol period */
    #define rtc_i2c_PERIOD_VALUE       (0x07u)

    /* rtc_i2c_DEFAULT_INT_MASK */
    #define rtc_i2c_DEFAULT_INT_MASK   (rtc_i2c_BYTE_COMPLETE_IE_MASK)

    /* rtc_i2c_DEFAULT_MCLK_PRD_REG */
    #define rtc_i2c_DEFAULT_MCLK_PRD   (rtc_i2c_MCLK_PERIOD_VALUE)

    /* rtc_i2c_DEFAULT_MCLK_CMP_REG */
    #define rtc_i2c_DEFAULT_MCLK_CMP   (rtc_i2c_MCLK_COMPARE_VALUE)

    /* rtc_i2c_DEFAULT_PERIOD_REG */
    #define rtc_i2c_DEFAULT_PERIOD     (rtc_i2c_PERIOD_VALUE)

#endif /* (rtc_i2c_FF_IMPLEMENTED) */


/***************************************
*       Obsolete
***************************************/

/* Following code are OBSOLETE and must not be used 
 * starting from I2C 3.20
 */
 
#define rtc_i2c_SSTAT_RD_ERR       (0x08u)
#define rtc_i2c_SSTAT_WR_ERR       (0x80u)
#define rtc_i2c_MSTR_SLAVE_BUSY    (rtc_i2c_MSTR_NOT_READY)
#define rtc_i2c_MSTAT_ERR_BUF_OVFL (0x80u)
#define rtc_i2c_SSTAT_RD_CMPT      (rtc_i2c_SSTAT_RD_CMPLT)
#define rtc_i2c_SSTAT_WR_CMPT      (rtc_i2c_SSTAT_WR_CMPLT)
#define rtc_i2c_MODE_MULTI_MASTER_ENABLE    (rtc_i2c_MODE_MULTI_MASTER_MASK)
#define rtc_i2c_DATA_RATE_50       (50u)
#define rtc_i2c_DATA_RATE_100      (100u)
#define rtc_i2c_DEV_MASK           (0xF0u)
#define rtc_i2c_SM_SL_STOP         (0x14u)
#define rtc_i2c_SM_MASTER_IDLE     (0x40u)
#define rtc_i2c_HDWR_DECODE        (0x01u)

#endif /* CY_I2C_rtc_i2c_H */


/* [] END OF FILE */
