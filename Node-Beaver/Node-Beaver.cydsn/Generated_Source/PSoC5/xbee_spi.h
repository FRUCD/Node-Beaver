/*******************************************************************************
* File Name: xbee_spi.h
* Version 2.40
*
* Description:
*  Contains the function prototypes, constants and register definition
*  of the SPI Master Component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SPIM_xbee_spi_H)
#define CY_SPIM_xbee_spi_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component SPI_Master_v2_40 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define xbee_spi_INTERNAL_CLOCK             (0u)

#if(0u != xbee_spi_INTERNAL_CLOCK)
    #include "xbee_spi_IntClock.h"
#endif /* (0u != xbee_spi_INTERNAL_CLOCK) */

#define xbee_spi_MODE                       (1u)
#define xbee_spi_DATA_WIDTH                 (8u)
#define xbee_spi_MODE_USE_ZERO              (1u)
#define xbee_spi_BIDIRECTIONAL_MODE         (0u)

/* Internal interrupt handling */
#define xbee_spi_TX_BUFFER_SIZE             (4u)
#define xbee_spi_RX_BUFFER_SIZE             (4u)
#define xbee_spi_INTERNAL_TX_INT_ENABLED    (0u)
#define xbee_spi_INTERNAL_RX_INT_ENABLED    (0u)

#define xbee_spi_SINGLE_REG_SIZE            (8u)
#define xbee_spi_USE_SECOND_DATAPATH        (xbee_spi_DATA_WIDTH > xbee_spi_SINGLE_REG_SIZE)

#define xbee_spi_FIFO_SIZE                  (4u)
#define xbee_spi_TX_SOFTWARE_BUF_ENABLED    ((0u != xbee_spi_INTERNAL_TX_INT_ENABLED) && \
                                                     (xbee_spi_TX_BUFFER_SIZE > xbee_spi_FIFO_SIZE))

#define xbee_spi_RX_SOFTWARE_BUF_ENABLED    ((0u != xbee_spi_INTERNAL_RX_INT_ENABLED) && \
                                                     (xbee_spi_RX_BUFFER_SIZE > xbee_spi_FIFO_SIZE))


/***************************************
*        Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
    uint8 cntrPeriod;
    #if(CY_UDB_V0)
        uint8 saveSrTxIntMask;
        uint8 saveSrRxIntMask;
    #endif /* (CY_UDB_V0) */

} xbee_spi_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  xbee_spi_Init(void)                           ;
void  xbee_spi_Enable(void)                         ;
void  xbee_spi_Start(void)                          ;
void  xbee_spi_Stop(void)                           ;

void  xbee_spi_EnableTxInt(void)                    ;
void  xbee_spi_EnableRxInt(void)                    ;
void  xbee_spi_DisableTxInt(void)                   ;
void  xbee_spi_DisableRxInt(void)                   ;

void  xbee_spi_Sleep(void)                          ;
void  xbee_spi_Wakeup(void)                         ;
void  xbee_spi_SaveConfig(void)                     ;
void  xbee_spi_RestoreConfig(void)                  ;

void  xbee_spi_SetTxInterruptMode(uint8 intSrc)     ;
void  xbee_spi_SetRxInterruptMode(uint8 intSrc)     ;
uint8 xbee_spi_ReadTxStatus(void)                   ;
uint8 xbee_spi_ReadRxStatus(void)                   ;
void  xbee_spi_WriteTxData(uint8 txData)  \
                                                            ;
uint8 xbee_spi_ReadRxData(void) \
                                                            ;
uint8 xbee_spi_GetRxBufferSize(void)                ;
uint8 xbee_spi_GetTxBufferSize(void)                ;
void  xbee_spi_ClearRxBuffer(void)                  ;
void  xbee_spi_ClearTxBuffer(void)                  ;
void  xbee_spi_ClearFIFO(void)                              ;
void  xbee_spi_PutArray(const uint8 buffer[], uint8 byteCount) \
                                                            ;

#if(0u != xbee_spi_BIDIRECTIONAL_MODE)
    void  xbee_spi_TxEnable(void)                   ;
    void  xbee_spi_TxDisable(void)                  ;
#endif /* (0u != xbee_spi_BIDIRECTIONAL_MODE) */

CY_ISR_PROTO(xbee_spi_TX_ISR);
CY_ISR_PROTO(xbee_spi_RX_ISR);


/**********************************
*   Variable with external linkage
**********************************/

extern uint8 xbee_spi_initVar;


/***************************************
*           API Constants
***************************************/

#define xbee_spi_TX_ISR_NUMBER     ((uint8) (xbee_spi_TxInternalInterrupt__INTC_NUMBER))
#define xbee_spi_RX_ISR_NUMBER     ((uint8) (xbee_spi_RxInternalInterrupt__INTC_NUMBER))

#define xbee_spi_TX_ISR_PRIORITY   ((uint8) (xbee_spi_TxInternalInterrupt__INTC_PRIOR_NUM))
#define xbee_spi_RX_ISR_PRIORITY   ((uint8) (xbee_spi_RxInternalInterrupt__INTC_PRIOR_NUM))


/***************************************
*    Initial Parameter Constants
***************************************/

#define xbee_spi_INT_ON_SPI_DONE    ((uint8) (0u   << xbee_spi_STS_SPI_DONE_SHIFT))
#define xbee_spi_INT_ON_TX_EMPTY    ((uint8) (0u   << xbee_spi_STS_TX_FIFO_EMPTY_SHIFT))
#define xbee_spi_INT_ON_TX_NOT_FULL ((uint8) (0u << \
                                                                           xbee_spi_STS_TX_FIFO_NOT_FULL_SHIFT))
#define xbee_spi_INT_ON_BYTE_COMP   ((uint8) (0u  << xbee_spi_STS_BYTE_COMPLETE_SHIFT))
#define xbee_spi_INT_ON_SPI_IDLE    ((uint8) (0u   << xbee_spi_STS_SPI_IDLE_SHIFT))

/* Disable TX_NOT_FULL if software buffer is used */
#define xbee_spi_INT_ON_TX_NOT_FULL_DEF ((xbee_spi_TX_SOFTWARE_BUF_ENABLED) ? \
                                                                        (0u) : (xbee_spi_INT_ON_TX_NOT_FULL))

/* TX interrupt mask */
#define xbee_spi_TX_INIT_INTERRUPTS_MASK    (xbee_spi_INT_ON_SPI_DONE  | \
                                                     xbee_spi_INT_ON_TX_EMPTY  | \
                                                     xbee_spi_INT_ON_TX_NOT_FULL_DEF | \
                                                     xbee_spi_INT_ON_BYTE_COMP | \
                                                     xbee_spi_INT_ON_SPI_IDLE)

#define xbee_spi_INT_ON_RX_FULL         ((uint8) (0u << \
                                                                          xbee_spi_STS_RX_FIFO_FULL_SHIFT))
#define xbee_spi_INT_ON_RX_NOT_EMPTY    ((uint8) (0u << \
                                                                          xbee_spi_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define xbee_spi_INT_ON_RX_OVER         ((uint8) (0u << \
                                                                          xbee_spi_STS_RX_FIFO_OVERRUN_SHIFT))

/* RX interrupt mask */
#define xbee_spi_RX_INIT_INTERRUPTS_MASK    (xbee_spi_INT_ON_RX_FULL      | \
                                                     xbee_spi_INT_ON_RX_NOT_EMPTY | \
                                                     xbee_spi_INT_ON_RX_OVER)
/* Nubmer of bits to receive/transmit */
#define xbee_spi_BITCTR_INIT            (((uint8) (xbee_spi_DATA_WIDTH << 1u)) - 1u)


/***************************************
*             Registers
***************************************/

#if(CY_PSOC3 || CY_PSOC5)
    #define xbee_spi_TXDATA_REG (* (reg8 *) \
                                                xbee_spi_BSPIM_sR8_Dp_u0__F0_REG)
    #define xbee_spi_TXDATA_PTR (  (reg8 *) \
                                                xbee_spi_BSPIM_sR8_Dp_u0__F0_REG)
    #define xbee_spi_RXDATA_REG (* (reg8 *) \
                                                xbee_spi_BSPIM_sR8_Dp_u0__F1_REG)
    #define xbee_spi_RXDATA_PTR (  (reg8 *) \
                                                xbee_spi_BSPIM_sR8_Dp_u0__F1_REG)
#else   /* PSOC4 */
    #if(xbee_spi_USE_SECOND_DATAPATH)
        #define xbee_spi_TXDATA_REG (* (reg16 *) \
                                          xbee_spi_BSPIM_sR8_Dp_u0__16BIT_F0_REG)
        #define xbee_spi_TXDATA_PTR (  (reg16 *) \
                                          xbee_spi_BSPIM_sR8_Dp_u0__16BIT_F0_REG)
        #define xbee_spi_RXDATA_REG (* (reg16 *) \
                                          xbee_spi_BSPIM_sR8_Dp_u0__16BIT_F1_REG)
        #define xbee_spi_RXDATA_PTR         (  (reg16 *) \
                                          xbee_spi_BSPIM_sR8_Dp_u0__16BIT_F1_REG)
    #else
        #define xbee_spi_TXDATA_REG (* (reg8 *) \
                                                xbee_spi_BSPIM_sR8_Dp_u0__F0_REG)
        #define xbee_spi_TXDATA_PTR (  (reg8 *) \
                                                xbee_spi_BSPIM_sR8_Dp_u0__F0_REG)
        #define xbee_spi_RXDATA_REG (* (reg8 *) \
                                                xbee_spi_BSPIM_sR8_Dp_u0__F1_REG)
        #define xbee_spi_RXDATA_PTR (  (reg8 *) \
                                                xbee_spi_BSPIM_sR8_Dp_u0__F1_REG)
    #endif /* (xbee_spi_USE_SECOND_DATAPATH) */
#endif     /* (CY_PSOC3 || CY_PSOC5) */

#define xbee_spi_AUX_CONTROL_DP0_REG (* (reg8 *) \
                                        xbee_spi_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG)
#define xbee_spi_AUX_CONTROL_DP0_PTR (  (reg8 *) \
                                        xbee_spi_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG)

#if(xbee_spi_USE_SECOND_DATAPATH)
    #define xbee_spi_AUX_CONTROL_DP1_REG  (* (reg8 *) \
                                        xbee_spi_BSPIM_sR8_Dp_u1__DP_AUX_CTL_REG)
    #define xbee_spi_AUX_CONTROL_DP1_PTR  (  (reg8 *) \
                                        xbee_spi_BSPIM_sR8_Dp_u1__DP_AUX_CTL_REG)
#endif /* (xbee_spi_USE_SECOND_DATAPATH) */

#define xbee_spi_COUNTER_PERIOD_REG     (* (reg8 *) xbee_spi_BSPIM_BitCounter__PERIOD_REG)
#define xbee_spi_COUNTER_PERIOD_PTR     (  (reg8 *) xbee_spi_BSPIM_BitCounter__PERIOD_REG)
#define xbee_spi_COUNTER_CONTROL_REG    (* (reg8 *) xbee_spi_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)
#define xbee_spi_COUNTER_CONTROL_PTR    (  (reg8 *) xbee_spi_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)

#define xbee_spi_TX_STATUS_REG          (* (reg8 *) xbee_spi_BSPIM_TxStsReg__STATUS_REG)
#define xbee_spi_TX_STATUS_PTR          (  (reg8 *) xbee_spi_BSPIM_TxStsReg__STATUS_REG)
#define xbee_spi_RX_STATUS_REG          (* (reg8 *) xbee_spi_BSPIM_RxStsReg__STATUS_REG)
#define xbee_spi_RX_STATUS_PTR          (  (reg8 *) xbee_spi_BSPIM_RxStsReg__STATUS_REG)

#define xbee_spi_CONTROL_REG            (* (reg8 *) \
                                      xbee_spi_BSPIM_BidirMode_SyncCtl_CtrlReg__CONTROL_REG)
#define xbee_spi_CONTROL_PTR            (  (reg8 *) \
                                      xbee_spi_BSPIM_BidirMode_SyncCtl_CtrlReg__CONTROL_REG)

#define xbee_spi_TX_STATUS_MASK_REG     (* (reg8 *) xbee_spi_BSPIM_TxStsReg__MASK_REG)
#define xbee_spi_TX_STATUS_MASK_PTR     (  (reg8 *) xbee_spi_BSPIM_TxStsReg__MASK_REG)
#define xbee_spi_RX_STATUS_MASK_REG     (* (reg8 *) xbee_spi_BSPIM_RxStsReg__MASK_REG)
#define xbee_spi_RX_STATUS_MASK_PTR     (  (reg8 *) xbee_spi_BSPIM_RxStsReg__MASK_REG)

#define xbee_spi_TX_STATUS_ACTL_REG     (* (reg8 *) xbee_spi_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define xbee_spi_TX_STATUS_ACTL_PTR     (  (reg8 *) xbee_spi_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define xbee_spi_RX_STATUS_ACTL_REG     (* (reg8 *) xbee_spi_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)
#define xbee_spi_RX_STATUS_ACTL_PTR     (  (reg8 *) xbee_spi_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)

#if(xbee_spi_USE_SECOND_DATAPATH)
    #define xbee_spi_AUX_CONTROLDP1     (xbee_spi_AUX_CONTROL_DP1_REG)
#endif /* (xbee_spi_USE_SECOND_DATAPATH) */


/***************************************
*       Register Constants
***************************************/

/* Status Register Definitions */
#define xbee_spi_STS_SPI_DONE_SHIFT             (0x00u)
#define xbee_spi_STS_TX_FIFO_EMPTY_SHIFT        (0x01u)
#define xbee_spi_STS_TX_FIFO_NOT_FULL_SHIFT     (0x02u)
#define xbee_spi_STS_BYTE_COMPLETE_SHIFT        (0x03u)
#define xbee_spi_STS_SPI_IDLE_SHIFT             (0x04u)
#define xbee_spi_STS_RX_FIFO_FULL_SHIFT         (0x04u)
#define xbee_spi_STS_RX_FIFO_NOT_EMPTY_SHIFT    (0x05u)
#define xbee_spi_STS_RX_FIFO_OVERRUN_SHIFT      (0x06u)

#define xbee_spi_STS_SPI_DONE           ((uint8) (0x01u << xbee_spi_STS_SPI_DONE_SHIFT))
#define xbee_spi_STS_TX_FIFO_EMPTY      ((uint8) (0x01u << xbee_spi_STS_TX_FIFO_EMPTY_SHIFT))
#define xbee_spi_STS_TX_FIFO_NOT_FULL   ((uint8) (0x01u << xbee_spi_STS_TX_FIFO_NOT_FULL_SHIFT))
#define xbee_spi_STS_BYTE_COMPLETE      ((uint8) (0x01u << xbee_spi_STS_BYTE_COMPLETE_SHIFT))
#define xbee_spi_STS_SPI_IDLE           ((uint8) (0x01u << xbee_spi_STS_SPI_IDLE_SHIFT))
#define xbee_spi_STS_RX_FIFO_FULL       ((uint8) (0x01u << xbee_spi_STS_RX_FIFO_FULL_SHIFT))
#define xbee_spi_STS_RX_FIFO_NOT_EMPTY  ((uint8) (0x01u << xbee_spi_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define xbee_spi_STS_RX_FIFO_OVERRUN    ((uint8) (0x01u << xbee_spi_STS_RX_FIFO_OVERRUN_SHIFT))

/* TX and RX masks for clear on read bits */
#define xbee_spi_TX_STS_CLR_ON_RD_BYTES_MASK    (0x09u)
#define xbee_spi_RX_STS_CLR_ON_RD_BYTES_MASK    (0x40u)

/* StatusI Register Interrupt Enable Control Bits */
/* As defined by the Register map for the AUX Control Register */
#define xbee_spi_INT_ENABLE     (0x10u) /* Enable interrupt from statusi */
#define xbee_spi_TX_FIFO_CLR    (0x01u) /* F0 - TX FIFO */
#define xbee_spi_RX_FIFO_CLR    (0x02u) /* F1 - RX FIFO */
#define xbee_spi_FIFO_CLR       (xbee_spi_TX_FIFO_CLR | xbee_spi_RX_FIFO_CLR)

/* Bit Counter (7-bit) Control Register Bit Definitions */
/* As defined by the Register map for the AUX Control Register */
#define xbee_spi_CNTR_ENABLE    (0x20u) /* Enable CNT7 */

/* Bi-Directional mode control bit */
#define xbee_spi_CTRL_TX_SIGNAL_EN  (0x01u)

/* Datapath Auxillary Control Register definitions */
#define xbee_spi_AUX_CTRL_FIFO0_CLR         (0x01u)
#define xbee_spi_AUX_CTRL_FIFO1_CLR         (0x02u)
#define xbee_spi_AUX_CTRL_FIFO0_LVL         (0x04u)
#define xbee_spi_AUX_CTRL_FIFO1_LVL         (0x08u)
#define xbee_spi_STATUS_ACTL_INT_EN_MASK    (0x10u)

/* Component disabled */
#define xbee_spi_DISABLED   (0u)


/***************************************
*       Macros
***************************************/

/* Returns true if componentn enabled */
#define xbee_spi_IS_ENABLED (0u != (xbee_spi_TX_STATUS_ACTL_REG & xbee_spi_INT_ENABLE))

/* Retuns TX status register */
#define xbee_spi_GET_STATUS_TX(swTxSts) ( (uint8)(xbee_spi_TX_STATUS_REG | \
                                                          ((swTxSts) & xbee_spi_TX_STS_CLR_ON_RD_BYTES_MASK)) )
/* Retuns RX status register */
#define xbee_spi_GET_STATUS_RX(swRxSts) ( (uint8)(xbee_spi_RX_STATUS_REG | \
                                                          ((swRxSts) & xbee_spi_RX_STS_CLR_ON_RD_BYTES_MASK)) )


/***************************************
*       Obsolete definitions
***************************************/

/* Following definitions are for version compatibility.
*  They are obsolete in SPIM v2_30.
*  Please do not use it in new projects
*/

#define xbee_spi_WriteByte   xbee_spi_WriteTxData
#define xbee_spi_ReadByte    xbee_spi_ReadRxData
void  xbee_spi_SetInterruptMode(uint8 intSrc)       ;
uint8 xbee_spi_ReadStatus(void)                     ;
void  xbee_spi_EnableInt(void)                      ;
void  xbee_spi_DisableInt(void)                     ;

/* Obsolete register names. Not to be used in new designs */
#define xbee_spi_TXDATA                 (xbee_spi_TXDATA_REG)
#define xbee_spi_RXDATA                 (xbee_spi_RXDATA_REG)
#define xbee_spi_AUX_CONTROLDP0         (xbee_spi_AUX_CONTROL_DP0_REG)
#define xbee_spi_TXBUFFERREAD           (xbee_spi_txBufferRead)
#define xbee_spi_TXBUFFERWRITE          (xbee_spi_txBufferWrite)
#define xbee_spi_RXBUFFERREAD           (xbee_spi_rxBufferRead)
#define xbee_spi_RXBUFFERWRITE          (xbee_spi_rxBufferWrite)

#define xbee_spi_COUNTER_PERIOD         (xbee_spi_COUNTER_PERIOD_REG)
#define xbee_spi_COUNTER_CONTROL        (xbee_spi_COUNTER_CONTROL_REG)
#define xbee_spi_STATUS                 (xbee_spi_TX_STATUS_REG)
#define xbee_spi_CONTROL                (xbee_spi_CONTROL_REG)
#define xbee_spi_STATUS_MASK            (xbee_spi_TX_STATUS_MASK_REG)
#define xbee_spi_STATUS_ACTL            (xbee_spi_TX_STATUS_ACTL_REG)

#define xbee_spi_INIT_INTERRUPTS_MASK  (xbee_spi_INT_ON_SPI_DONE     | \
                                                xbee_spi_INT_ON_TX_EMPTY     | \
                                                xbee_spi_INT_ON_TX_NOT_FULL_DEF  | \
                                                xbee_spi_INT_ON_RX_FULL      | \
                                                xbee_spi_INT_ON_RX_NOT_EMPTY | \
                                                xbee_spi_INT_ON_RX_OVER      | \
                                                xbee_spi_INT_ON_BYTE_COMP)
                                                
/* Following definitions are for version Compatibility.
*  They are obsolete in SPIM v2_40.
*  Please do not use it in new projects
*/

#define xbee_spi_DataWidth                  (xbee_spi_DATA_WIDTH)
#define xbee_spi_InternalClockUsed          (xbee_spi_INTERNAL_CLOCK)
#define xbee_spi_InternalTxInterruptEnabled (xbee_spi_INTERNAL_TX_INT_ENABLED)
#define xbee_spi_InternalRxInterruptEnabled (xbee_spi_INTERNAL_RX_INT_ENABLED)
#define xbee_spi_ModeUseZero                (xbee_spi_MODE_USE_ZERO)
#define xbee_spi_BidirectionalMode          (xbee_spi_BIDIRECTIONAL_MODE)
#define xbee_spi_Mode                       (xbee_spi_MODE)
#define xbee_spi_DATAWIDHT                  (xbee_spi_DATA_WIDTH)
#define xbee_spi_InternalInterruptEnabled   (0u)

#define xbee_spi_TXBUFFERSIZE   (xbee_spi_TX_BUFFER_SIZE)
#define xbee_spi_RXBUFFERSIZE   (xbee_spi_RX_BUFFER_SIZE)

#define xbee_spi_TXBUFFER       xbee_spi_txBuffer
#define xbee_spi_RXBUFFER       xbee_spi_rxBuffer

#endif /* (CY_SPIM_xbee_spi_H) */


/* [] END OF FILE */
