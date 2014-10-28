/*******************************************************************************
* File Name: CAN_1.h
* Version 2.30
*
* Description:
*  Contains the function prototypes, constants and register definition
*  of the CAN Component.
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

#if !defined(CY_CAN_CAN_1_H)
#define CY_CAN_CAN_1_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h"

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component CAN_v2_30 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

extern uint8 CAN_1_initVar;

#define CAN_1_INT_ISR_DISABLE        (0u)


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define CAN_1_ARB_LOST               (0u)
#define CAN_1_OVERLOAD               (0u)
#define CAN_1_BIT_ERR                (0u)
#define CAN_1_STUFF_ERR              (0u)
#define CAN_1_ACK_ERR                (0u)
#define CAN_1_FORM_ERR               (0u)
#define CAN_1_CRC_ERR                (0u)
#define CAN_1_BUS_OFF                (1u)
#define CAN_1_RX_MSG_LOST            (0u)
#define CAN_1_TX_MESSAGE             (0u)
#define CAN_1_RX_MESSAGE             (1u)
#define CAN_1_ARB_LOST_USE_HELPER    (1u)
#define CAN_1_OVERLOAD_USE_HELPER    (1u)
#define CAN_1_BIT_ERR_USE_HELPER     (1u)
#define CAN_1_STUFF_ERR_USE_HELPER   (1u)
#define CAN_1_ACK_ERR_USE_HELPER     (1u)
#define CAN_1_FORM_ERR_USE_HELPER    (1u)
#define CAN_1_CRC_ERR_USE_HELPER     (1u)
#define CAN_1_BUS_OFF_USE_HELPER     (1u)
#define CAN_1_RX_MSG_LOST_USE_HELPER (1u)
#define CAN_1_TX_MESSAGE_USE_HELPER  (1u)
#define CAN_1_RX_MESSAGE_USE_HELPER  (1u)
#define CAN_1_ADVANCED_INTERRUPT_CFG (0u)

/* TX/RX Function Enable */
#define CAN_1_TX0_FUNC_ENABLE 0u
#define CAN_1_TX1_FUNC_ENABLE 0u
#define CAN_1_TX2_FUNC_ENABLE 0u
#define CAN_1_TX3_FUNC_ENABLE 0u
#define CAN_1_TX4_FUNC_ENABLE 0u
#define CAN_1_TX5_FUNC_ENABLE 0u
#define CAN_1_TX6_FUNC_ENABLE 0u
#define CAN_1_TX7_FUNC_ENABLE 0u
#define CAN_1_RX0_FUNC_ENABLE 0u
#define CAN_1_RX1_FUNC_ENABLE 0u
#define CAN_1_RX2_FUNC_ENABLE 0u
#define CAN_1_RX3_FUNC_ENABLE 0u
#define CAN_1_RX4_FUNC_ENABLE 0u
#define CAN_1_RX5_FUNC_ENABLE 0u
#define CAN_1_RX6_FUNC_ENABLE 0u
#define CAN_1_RX7_FUNC_ENABLE 0u
#define CAN_1_RX8_FUNC_ENABLE 0u
#define CAN_1_RX9_FUNC_ENABLE 0u
#define CAN_1_RX10_FUNC_ENABLE 0u
#define CAN_1_RX11_FUNC_ENABLE 0u
#define CAN_1_RX12_FUNC_ENABLE 0u
#define CAN_1_RX13_FUNC_ENABLE 0u
#define CAN_1_RX14_FUNC_ENABLE 0u
#define CAN_1_RX15_FUNC_ENABLE 0u
#define CAN_1_RX_MAILBOX_TYPE	0x0u
#define CAN_1_TX_MAILBOX_TYPE	0x0u


/***************************************
*        Data Struct Definition
***************************************/

/* Stuct for DATA of BASIC CAN mailbox */
typedef struct
{
    uint8 byte[8u];
} CAN_1_DATA_BYTES_MSG;

/* Stuct for DATA of CAN RX register */
typedef struct
{
    reg8 byte[8u];
} CAN_1_DATA_BYTES;

/* Stuct for 32-bit CAN register */
typedef struct
{
    reg8 byte[4u];
} CAN_1_REG_32;

/* Stuct for BASIC CAN mailbox to send messages */
typedef struct
{
    uint32 id;
    uint8 rtr;
    uint8 ide;
    uint8 dlc;
    uint8 irq;
    CAN_1_DATA_BYTES_MSG *msg;
} CAN_1_TX_MSG;

/* Constant configutaion of CAN RX */
typedef struct
{
    uint8 rxmailbox;
    uint32 rxcmd;
    uint32 rxamr;
    uint32 rxacr;
} CAN_1_RX_CFG;

/* Constant configutaion of CAN TX */
typedef struct
{
    uint8 txmailbox;
    uint32 txcmd;
    uint32 txid;
} CAN_1_TX_CFG;

/* CAN RX registers */
typedef struct
{
    CAN_1_REG_32 rxcmd;
    CAN_1_REG_32 rxid;
    CAN_1_DATA_BYTES rxdata;
    CAN_1_REG_32 rxamr;
    CAN_1_REG_32 rxacr;
    CAN_1_REG_32 rxamrd;
    CAN_1_REG_32 rxacrd;
} CAN_1_RX_STRUCT;

/* CAN TX registers */
typedef struct
{
    CAN_1_REG_32 txcmd;
    CAN_1_REG_32 txid;
    CAN_1_DATA_BYTES txdata;
} CAN_1_TX_STRUCT;

/* Sleep Mode API Support */
typedef struct
{
    uint8  enableState;
    uint32 intSr;
    uint32 intEn;
    uint32 cmd;
    uint32 cfg;    
} CAN_1_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

uint8 CAN_1_Init(void) ;
uint8 CAN_1_Start(void) ;
uint8 CAN_1_Stop(void) ;
uint8 CAN_1_Enable(void) ;
uint8 CAN_1_GlobalIntEnable(void) ;
uint8 CAN_1_GlobalIntDisable(void) ;
uint8 CAN_1_SetPreScaler(uint16 bitrate) ;
uint8 CAN_1_SetArbiter(uint8 arbiter) ;
uint8 CAN_1_SetTsegSample(uint8 cfgTseg1, uint8 cfgTseg2, uint8 sjw, uint8 sm)
                                                ;
uint8 CAN_1_SetRestartType(uint8 reset) ;
uint8 CAN_1_SetEdgeMode(uint8 edge) ;
uint8 CAN_1_SetOpMode(uint8 opMode) ;
uint8 CAN_1_RXRegisterInit(reg32 *regAddr, uint32 config)
                                            ;
uint8 CAN_1_SetIrqMask(uint16 mask) ;
uint8 CAN_1_GetTXErrorFlag(void) ;
uint8 CAN_1_GetRXErrorFlag(void) ;
uint8 CAN_1_GetTXErrorCount(void) ;
uint8 CAN_1_GetRXErrorCount(void) ;
uint8 CAN_1_GetErrorState(void) ;
void  CAN_1_Sleep(void) ;
void  CAN_1_Wakeup(void) ;
void  CAN_1_SaveConfig(void) ;
void  CAN_1_RestoreConfig(void) ;

#if (CAN_1_ARB_LOST)
    void CAN_1_ArbLostIsr(void) ;
#endif /* CAN_1_ARB_LOST */
#if (CAN_1_OVERLOAD)
    void CAN_1_OvrLdErrorIsr(void) ;
#endif /* CAN_1_OVERLOAD */
#if (CAN_1_BIT_ERR)
    void CAN_1_BitErrorIsr(void) ;
#endif /* CAN_1_BIT_ERR */
#if (CAN_1_STUFF_ERR)
    void CAN_1_BitStuffErrorIsr(void) ;
#endif /* CAN_1_STUFF_ERR */
#if (CAN_1_ACK_ERR)
    void CAN_1_AckErrorIsr(void) ;
#endif /* CAN_1_ACK_ERR */
#if (CAN_1_FORM_ERR)
    void CAN_1_MsgErrorIsr(void) ;
#endif /* CAN_1_FORM_ERR */
#if (CAN_1_CRC_ERR)
    void CAN_1_CrcErrorIsr(void) ;
#endif /* CAN_1_CRC_ERR */
#if (CAN_1_BUS_OFF)
    void CAN_1_BusOffIsr(void) ;
#endif /* CAN_1_BUS_OFF */
#if (CAN_1_RX_MSG_LOST)
    void CAN_1_MsgLostIsr(void) ;
#endif /* CAN_1_RX_MSG_LOST */
#if (CAN_1_TX_MESSAGE)
   void CAN_1_MsgTXIsr(void) ;
#endif /* CAN_1_TX_MESSAGE */
#if (CAN_1_RX_MESSAGE)
    void CAN_1_MsgRXIsr(void) ;
#endif /* CAN_1_RX_MESSAGE */

uint8 CAN_1_RxBufConfig(const CAN_1_RX_CFG *rxConfig) \
                                   ;
uint8 CAN_1_TxBufConfig(const CAN_1_TX_CFG *txConfig) \
                                   ;
uint8 CAN_1_SendMsg(const CAN_1_TX_MSG *message) ;
void  CAN_1_TxCancel(uint8 bufferId) ;
void  CAN_1_ReceiveMsg(uint8 rxMailbox) ;

#if (CAN_1_TX0_FUNC_ENABLE)
    uint8 CAN_1_SendMsg0(void) ;
#endif /* CAN_1_TX0_FUNC_ENABLE */
#if (CAN_1_TX1_FUNC_ENABLE)
    uint8 CAN_1_SendMsg1(void) ;
#endif /* CAN_1_TX1_FUNC_ENABLE */
#if (CAN_1_TX2_FUNC_ENABLE)
    uint8 CAN_1_SendMsg2(void) ;
#endif /* CAN_1_TX2_FUNC_ENABLE */
#if (CAN_1_TX3_FUNC_ENABLE)
    uint8 CAN_1_SendMsg3(void) ;
#endif /* CAN_1_TX3_FUNC_ENABLE */
#if (CAN_1_TX4_FUNC_ENABLE)
    uint8 CAN_1_SendMsg4(void) ;
#endif /* CAN_1_TX4_FUNC_ENABLE */
#if (CAN_1_TX5_FUNC_ENABLE)
    uint8 CAN_1_SendMsg5(void) ;
#endif /* CAN_1_TX5_FUNC_ENABLE */
#if (CAN_1_TX6_FUNC_ENABLE)
    uint8 CAN_1_SendMsg6(void) ;
#endif /* CAN_1_TX6_FUNC_ENABLE */
#if (CAN_1_TX7_FUNC_ENABLE)
    uint8 CAN_1_SendMsg7(void) ;
#endif /* CAN_1_TX7_FUNC_ENABLE */
#if (CAN_1_RX0_FUNC_ENABLE)
    void CAN_1_ReceiveMsg0(void) ;
#endif /* CAN_1_RX0_FUNC_ENABLE */
#if (CAN_1_RX1_FUNC_ENABLE)
    void CAN_1_ReceiveMsg1(void) ;
#endif /* CAN_1_RX1_FUNC_ENABLE */
#if (CAN_1_RX2_FUNC_ENABLE)
    void CAN_1_ReceiveMsg2(void) ;
#endif /* CAN_1_RX2_FUNC_ENABLE */
#if (CAN_1_RX3_FUNC_ENABLE)
    void CAN_1_ReceiveMsg3(void) ;
#endif /* CAN_1_RX3_FUNC_ENABLE */
#if (CAN_1_RX4_FUNC_ENABLE)
    void CAN_1_ReceiveMsg4(void) ;
#endif /* CAN_1_RX4_FUNC_ENABLE */
#if (CAN_1_RX5_FUNC_ENABLE)
    void CAN_1_ReceiveMsg5(void) ;
#endif /* CAN_1_RX5_FUNC_ENABLE */
#if (CAN_1_RX6_FUNC_ENABLE)
    void CAN_1_ReceiveMsg6(void) ;
#endif /* CAN_1_RX6_FUNC_ENABLE */
#if (CAN_1_RX7_FUNC_ENABLE)
    void CAN_1_ReceiveMsg7(void) ;
#endif /* CAN_1_RX7_FUNC_ENABLE */
#if (CAN_1_RX8_FUNC_ENABLE)
    void CAN_1_ReceiveMsg8(void) ;
#endif /* CAN_1_RX8_FUNC_ENABLE */
#if (CAN_1_RX9_FUNC_ENABLE)
    void CAN_1_ReceiveMsg9(void) ;
#endif /* CAN_1_RX9_FUNC_ENABLE */
#if (CAN_1_RX10_FUNC_ENABLE)
    void CAN_1_ReceiveMsg10(void) ;
#endif /* CAN_1_RX10_FUNC_ENABLE */
#if (CAN_1_RX11_FUNC_ENABLE)
    void CAN_1_ReceiveMsg11(void) ;
#endif /* CAN_1_RX11_FUNC_ENABLE */
#if (CAN_1_RX12_FUNC_ENABLE)
    void CAN_1_ReceiveMsg12(void) ;
#endif /* CAN_1_RX12_FUNC_ENABLE */
#if (CAN_1_RX13_FUNC_ENABLE)
    void CAN_1_ReceiveMsg13(void) ;
#endif /* CAN_1_RX13_FUNC_ENABLE */
#if (CAN_1_RX14_FUNC_ENABLE)
    void CAN_1_ReceiveMsg14(void) ;
#endif /* CAN_1_RX14_FUNC_ENABLE */
#if (CAN_1_RX15_FUNC_ENABLE)
    void CAN_1_ReceiveMsg15(void) ;
#endif /* CAN_1_RX15_FUNC_ENABLE */

#if(!CAN_1_INT_ISR_DISABLE)
    /* Interrupt handler */
    CY_ISR_PROTO(CAN_1_ISR);
#endif /* End CAN_1_INT_ISR_DISABLE */


/***************************************
*           API Constants
***************************************/

#if (!CAN_1_INT_ISR_DISABLE)
    /* Number of the CAN_1_isr interrupt */
    #define CAN_1_ISR_NUMBER             ((uint8) CAN_1_isr__INTC_NUMBER)
    /* Priority of the CAN_1_isr interrupt */
    #define CAN_1_ISR_PRIORITY           ((uint8) CAN_1_isr__INTC_PRIOR_NUM)
#endif /* End CAN_1_INT_ISR_DISABLE */

/* One or more parameters to the function were invalid. */
#define CAN_1_FAIL                       (0x01u)
#define CAN_1_OUT_OF_RANGE               (0x02u)

/* PM_ACT_CFG (Active Power Mode CFG Register) */
#define CAN_1_ACT_PWR_EN                 (CAN_1_CanIP__PM_ACT_MSK)    /* Power enable mask */

/* PM_STBY_CFG (Alternate Active (Standby) Power Mode CFG Register) */
#define CAN_1_STBY_PWR_EN                (CAN_1_CanIP__PM_STBY_MSK)   /* Power enable mask */

/* Number of TX and RX mailboxes */
#define CAN_1_NUMBER_OF_TX_MAILBOXES     (8u)
#define CAN_1_NUMBER_OF_RX_MAILBOXES     (16u) 

/* Error status of CAN */
#define CAN_1_ERROR_ACTIVE               (0x00u)
#define CAN_1_ERROR_PASIVE               (0x01u)
#define CAN_1_ERROR_BUS_OFF              (0x10u)


/***************************************
*    Initial Parameter Constants
***************************************/

/* General */
#define CAN_1_BITRATE             (0u)
#define CAN_1_CFG_REG_TSEG1       (13u - 1u)
#define CAN_1_CFG_REG_TSEG2       (2u - 1u)
#define CAN_1_CFG_REG_SJW         (1u - 1u)
#define CAN_1_SAMPLING_MODE       (0u)

#define CAN_1_ARBITER             (0u)
#define CAN_1_RESET_TYPE          (0u)
#define CAN_1_SYNC_EDGE           (0u)

/* Interrupts */
#define CAN_1_INT_ENABLE          (1u)
#define CAN_1_INIT_INTERRUPT_MASK (((uint16) CAN_1_INT_ENABLE) | \
                    ((uint16) ((uint16) CAN_1_ARB_LOST    <<  CAN_1_ARBITRATION_LOST_SHIFT)) | \
                    ((uint16) ((uint16) CAN_1_OVERLOAD    <<  CAN_1_OVERLOAD_ERROR_SHIFT))   | \
                    ((uint16) ((uint16) CAN_1_BIT_ERR     <<  CAN_1_BIT_ERROR_SHIFT))        | \
                    ((uint16) ((uint16) CAN_1_STUFF_ERR   <<  CAN_1_STUFF_ERROR_SHIFT))      | \
                    ((uint16) ((uint16) CAN_1_ACK_ERR     <<  CAN_1_ACK_ERROR_SHIFT))        | \
                    ((uint16) ((uint16) CAN_1_FORM_ERR    <<  CAN_1_FORM_ERROR_SHIFT))       | \
                    ((uint16) ((uint16) CAN_1_CRC_ERR     << (CAN_1_ONE_BYTE_OFFSET          + \
                                                               CAN_1_CRC_ERROR_SHIFT)))                 | \
                    ((uint16) ((uint16) CAN_1_BUS_OFF     << (CAN_1_ONE_BYTE_OFFSET          + \
                                                               CAN_1_BUS_OFF_SHIFT)))                   | \
                    ((uint16) ((uint16) CAN_1_RX_MSG_LOST << (CAN_1_ONE_BYTE_OFFSET          + \
                                                               CAN_1_RX_MSG_LOST_SHIFT)))               | \
                    ((uint16) ((uint16) CAN_1_TX_MESSAGE  << (CAN_1_ONE_BYTE_OFFSET          + \
                                                               CAN_1_TX_MESSAGE_SHIFT)))                | \
                    ((uint16) ((uint16) CAN_1_RX_MESSAGE  << (CAN_1_ONE_BYTE_OFFSET          + \
                                                                           CAN_1_RX_MESSAGE_SHIFT))))


/***************************************
*             Registers
***************************************/

#define CAN_1_TX          ( (volatile CAN_1_TX_STRUCT XDATA *) CAN_1_CanIP__TX0_CMD)
#define CAN_1_RX          ( (volatile CAN_1_RX_STRUCT XDATA *) CAN_1_CanIP__RX0_CMD)
#define CAN_1_INT_SR_REG  (*(volatile CAN_1_REG_32 XDATA *) CAN_1_CanIP__CSR_INT_SR)
#define CAN_1_INT_SR_PTR  ( (reg32 *) CAN_1_CanIP__CSR_INT_SR)
#define CAN_1_INT_EN_REG  (*(volatile CAN_1_REG_32 XDATA *) CAN_1_CanIP__CSR_INT_EN)
#define CAN_1_INT_EN_PTR  ( (reg32 *) CAN_1_CanIP__CSR_INT_EN)
#define CAN_1_BUF_SR_REG  (*(volatile CAN_1_REG_32 XDATA *) CAN_1_CanIP__CSR_BUF_SR)
#define CAN_1_BUF_SR_PTR  ( (reg32 *) CAN_1_CanIP__CSR_BUF_SR)
#define CAN_1_ERR_SR_REG  (*(volatile CAN_1_REG_32 XDATA *) CAN_1_CanIP__CSR_ERR_SR)
#define CAN_1_ERR_SR_PTR  ( (reg32 *) CAN_1_CanIP__CSR_ERR_SR)
#define CAN_1_CMD_REG     (*(volatile CAN_1_REG_32 XDATA *) CAN_1_CanIP__CSR_CMD)
#define CAN_1_CMD_PTR     ( (reg32 *) CAN_1_CanIP__CSR_CMD)
#define CAN_1_CFG_REG     (*(volatile CAN_1_REG_32 XDATA *) CAN_1_CanIP__CSR_CFG)
#define CAN_1_CFG_PTR     ( (reg32 *) CAN_1_CanIP__CSR_CFG)
#define CAN_1_PM_ACT_CFG_REG  (*(reg8 *) CAN_1_CanIP__PM_ACT_CFG)    /* Power manager */
#define CAN_1_PM_ACT_CFG_PTR  ( (reg8 *) CAN_1_CanIP__PM_ACT_CFG)    /* Power manager */
#define CAN_1_PM_STBY_CFG_REG (*(reg8 *) CAN_1_CanIP__PM_STBY_CFG)   /* Power manager */
#define CAN_1_PM_STBY_CFG_PTR ( (reg8 *) CAN_1_CanIP__PM_STBY_CFG)   /* Power manager */

#define CAN_1_RX_FIRST_REGISTER_PTR    ((reg32 *) CAN_1_CanIP__RX0_CMD)
#define CAN_1_RX_LAST_REGISTER_PTR     ((reg32 *) CAN_1_CanIP__RX15_ACRD)


/***************************************
*        Register Constants
***************************************/

/* Operation mode */
#define CAN_1_ACTIVE_MODE              (0x00u)
#define CAN_1_LISTEN_ONLY              (0x01u)

/* Run/Stop mode */
#define CAN_1_MODE_STOP                (0x00u)
#define CAN_1_MODE_START               (0x01u)

/* Transmit buffer arbiter */
#define CAN_1_ROUND_ROBIN              (0x00u)
#define CAN_1_FIXED_PRIORITY           (0x01u)

/* Restart type */
#define CAN_1_MANUAL_RESTART           (0x00u)
#define CAN_1_AUTO_RESTART             (0x01u)

/* Sampling mode */
#define CAN_1_ONE_SAMPLE_POINT         (0x00u)
#define CAN_1_THREE_SAMPLE_POINTS      (0x01u)

/* Edge mode */
#define CAN_1_EDGE_R_TO_D              (0x00u)
#define CAN_1_BOTH_EDGES               (0x01u)

/* Extended identifier */
#define CAN_1_STANDARD_MESSAGE         (0x00u)
#define CAN_1_EXTENDED_MESSAGE         (0x01u)

/* Write Protect Mask for Basic and Full mailboxes */
#define CAN_1_TX_WPN_SET             (((uint32) ((uint32) 0x00000001u << CAN_1_TX_WPNL_SHIFT)) | \
                                                ((uint32) ((uint32) 0x00000001u << (CAN_1_TWO_BYTE_OFFSET + \
                                                 CAN_1_TX_WPNH_SHIFT))))

#define CAN_1_TX_WPN_CLEAR             ((uint32) (~CAN_1_TX_WPN_SET))

#define CAN_1_RX_WPN_SET             (((uint32) ((uint32) 0x00000001u << CAN_1_RX_WPNL_SHIFT)) | \
                                                ((uint32) ((uint32) 0x00000001u << (CAN_1_TWO_BYTE_OFFSET + \
                                                 CAN_1_RX_WPNH_SHIFT))))

#define CAN_1_RX_WPN_CLEAR             ((uint32) (~CAN_1_RX_WPN_SET))

#define CAN_1_TX_RSVD_MASK             ((uint32) 0x00FF00FFu)

#define CAN_1_TX_READ_BACK_MASK        (CAN_1_TX_WPN_CLEAR & CAN_1_TX_RSVD_MASK)

#define CAN_1_RX_READ_BACK_MASK        (CAN_1_RX_WPN_CLEAR & CAN_1_TX_RSVD_MASK)

#define CAN_1_RX_CMD_REG_WIDTH         (0x20u)

/* TX send message */
#define CAN_1_TX_REQUEST_PENDING     (0x01u)
#define CAN_1_RETRY_NUMBER           (0x03u)
#define CAN_1_SEND_MESSAGE_SHIFT     (0u)
#define CAN_1_SEND_MESSAGE           ((uint32) ((uint32) 0x00000001u << CAN_1_SEND_MESSAGE_SHIFT))

/* Offsets to maintain bytes within uint32 */
#define CAN_1_ONE_BYTE_OFFSET          (8u)
#define CAN_1_TWO_BYTE_OFFSET          (16u)
#define CAN_1_TREE_BYTE_OFFSET         (24u)

/* Set/Clear bits macro for CAN_1_RX maibox(i) */
/* bit 0 within RX_CMD[i] */
#define CAN_1_RX_ACK_MSG_SHIFT         (0u)
#define CAN_1_RX_ACK_MSG               ((uint8) ((uint8) 0x01u << CAN_1_RX_ACK_MSG_SHIFT))
#define CAN_1_RX_ACK_MESSAGE(i)        (CAN_1_RX[i].rxcmd.byte[0u] |= CAN_1_RX_ACK_MSG)
/* bit 2 within RX_CMD[i] */
#define CAN_1_RX_RTR_ABORT_SHIFT       (2u)
#define CAN_1_RX_RTR_ABORT_MASK        ((uint8) ((uint8) 0x01u << CAN_1_RX_RTR_ABORT_SHIFT))
#define CAN_1_RX_RTR_ABORT_MESSAGE(i)  (CAN_1_RX[i].rxcmd.byte[0u] |= \
                                                   CAN_1_RX_RTR_ABORT_MASK)
/* bit 3 within RX_CMD[i] */
#define CAN_1_RX_BUF_ENABLE_SHIFT      (3u)
#define CAN_1_RX_BUF_ENABLE_MASK       ((uint8) ((uint8) 0x01u << CAN_1_RX_BUF_ENABLE_SHIFT))
#define CAN_1_RX_BUF_ENABLE(i)         (CAN_1_RX[i].rxcmd.byte[0u] |= \
                                                   CAN_1_RX_BUF_ENABLE_MASK)
#define CAN_1_RX_BUF_DISABLE(i)        (CAN_1_RX[i].rxcmd.byte[0u] &= \
                                                   (uint8) (~CAN_1_RX_BUF_ENABLE_MASK))
/* bit 4 within RX_CMD[i] */
#define CAN_1_RX_RTRREPLY_SHIFT        (4u)
#define CAN_1_RX_RTRREPLY_MASK         ((uint8) ((uint8) 0x01u << CAN_1_RX_RTRREPLY_SHIFT))
#define CAN_1_SET_RX_RTRREPLY(i)       (CAN_1_RX[i].rxcmd.byte[0u] |= \
                                                   CAN_1_RX_RTRREPLY_MASK)
#define CAN_1_CLEAR_RX_RTRREPLY(i)     (CAN_1_RX[i].rxcmd.byte[0u] &= \
                                                   (uint8) (~CAN_1_RX_RTRREPLY_MASK))
/* bit 5 within RX_CMD[i] */
#define CAN_1_RX_INT_ENABLE_SHIFT      (5u)
#define CAN_1_RX_INT_ENABLE_MASK       ((uint8) ((uint8) 0x01u << CAN_1_RX_INT_ENABLE_SHIFT))
#define CAN_1_RX_INT_ENABLE(i)         (CAN_1_RX[i].rxcmd.byte[0u] |= \
                                                   CAN_1_RX_INT_ENABLE_MASK)
#define CAN_1_RX_INT_DISABLE(i)        (CAN_1_RX[i].rxcmd.byte[0u] &= \
                                                   (uint8) (~CAN_1_RX_INT_ENABLE_MASK))
/* bit 6 within RX_CMD[i] */
#define CAN_1_RX_LINKING_SHIFT         (6u)
#define CAN_1_RX_LINKING_MASK          ((uint8) ((uint8) 0x01u << CAN_1_RX_LINKING_SHIFT))
#define CAN_1_SET_RX_LINKING(i)        (CAN_1_RX[i].rxcmd.byte[0u] |= \
                                                   CAN_1_RX_LINKING_MASK)
#define CAN_1_CLEAR_RX_LINKING(i)      (CAN_1_RX[i].rxcmd.byte[0u] &= \
                                                   (uint8) (~CAN_1_RX_LINKING_MASK))
/* bit 7 within RX_CMD[i] */
#define CAN_1_RX_WPNL_SHIFT          (7u)
#define CAN_1_RX_WPNL_MASK           ((uint8) ((uint8) 0x01u << CAN_1_RX_WPNL_SHIFT))
#define CAN_1_SET_RX_WNPL(i)         (CAN_1_RX[i].rxcmd.byte[0u] |= CAN_1_RX_WPNL_MASK)
#define CAN_1_CLEAR_RX_WNPL(i)       (CAN_1_RX[i].rxcmd.byte[0u] &= \
                                                 (uint8) (~CAN_1_RX_WPNL_MASK))

/* bit 23 within RX_CMD[i] */
#define CAN_1_RX_WPNH_SHIFT          (7u)
#define CAN_1_RX_WPNH_MASK           ((uint8) ((uint8) 0x01u << CAN_1_RX_WPNH_SHIFT))
#define CAN_1_SET_RX_WNPH(i)         (CAN_1_RX[i].rxcmd.byte[2u] |= CAN_1_RX_WPNH_MASK)
#define CAN_1_CLEAR_RX_WNPH(i)       (CAN_1_RX[i].rxcmd.byte[2u] &= \
                                                 (uint8) (~CAN_1_RX_WPNH_MASK))

/* bits 19-16 within TX_CMD[i] */
#define CAN_1_RX_DLC_VALUE_SHIFT      (0u)
#define CAN_1_RX_DLC_VALUE_MASK       ((uint8) ((uint8) 0x0Fu << CAN_1_RX_DLC_VALUE_SHIFT))
#define CAN_1_GET_DLC(mailbox)        (CAN_1_RX[mailbox].rxcmd.byte[2u] & \
                                                  CAN_1_RX_DLC_VALUE_MASK)

/* Set/Clear bits macro for CAN_1_TX mailbox(i) */
/* bit 0 within TX_CMD[i] */
#define CAN_1_TX_TRANSMIT_REQUEST_SHIFT (0u)
#define CAN_1_TX_TRANSMIT_REQUEST     ((uint8) ((uint8) 0x01u << CAN_1_TX_TRANSMIT_REQUEST_SHIFT))
#define CAN_1_TX_TRANSMIT_MESSAGE(i)  (CAN_1_TX[i].txcmd.byte[0u] |= \
                                                  CAN_1_TX_TRANSMIT_REQUEST)
/* bit 1 within TX_CMD[i] */
#define CAN_1_TX_ABORT_SHIFT        (1u)
#define CAN_1_TX_ABORT_MASK         ((uint8) ((uint8) 0x01u << CAN_1_TX_ABORT_SHIFT))
#define CAN_1_TX_ABORT_MESSAGE(i)   (CAN_1_TX[i].txcmd.byte[0u] |= CAN_1_TX_ABORT_MASK)

/* bit 2 within TX_CMD[i] */
#define CAN_1_TRANSMIT_INT_ENABLE   (0x01u)
#define CAN_1_TRANSMIT_INT_DISABLE  (0x00u)
#define CAN_1_TX_INT_ENABLE_SHIFT   (2u)
#define CAN_1_TX_INT_ENABLE_MASK    ((uint32) ((uint32) 0x00000001u << CAN_1_TX_INT_ENABLE_SHIFT))
#define CAN_1_TX_INT_ENABLE(i)      (CAN_1_TX[i].txcmd.byte[0u] |= \
                                                (uint8) CAN_1_TX_INT_ENABLE_MASK)
#define CAN_1_TX_INT_DISABLE(i)     (CAN_1_TX[i].txcmd.byte[0u] &= \
                                                (uint8) (~CAN_1_TX_INT_ENABLE_MASK))
/* bit 3 within TX_CMD[i] */
#define CAN_1_TX_WPNL_SHIFT         (3u)
#define CAN_1_TX_WPNL_MASK          ((uint8) ((uint8) 0x01u << CAN_1_TX_WPNL_SHIFT))
#define CAN_1_SET_TX_WNPL(i)        (CAN_1_TX[i].txcmd.byte[0u] |= CAN_1_TX_WPNL_MASK)
#define CAN_1_CLEAR_TX_WNPL(i)      (CAN_1_TX[i].txcmd.byte[0u] &= \
                                                (uint8) (~CAN_1_TX_WPNL_MASK))

/* bits 19-16 within TX_CMD[i] */
#define CAN_1_TX_DLC_VALUE_SHIFT       (0u)
#define CAN_1_TX_DLC_UPPER_VALUE_SHIFT (19u)
#define CAN_1_TX_DLC_UPPER_VALUE       ((uint32) ((uint32) 0x00000001u << \
                                                   CAN_1_TX_DLC_UPPER_VALUE_SHIFT))
#define CAN_1_TX_DLC_VALUE_MASK        ((uint8) ((uint8) 0x0Fu << CAN_1_TX_DLC_VALUE_SHIFT))
#define CAN_1_TX_DLC_MAX_VALUE         (8u)

/* bit 20 within TX_CMD[i] */
#define CAN_1_TX_IDE_SHIFT             (20u)
#define CAN_1_TX_IDE_MASK              ((uint32) ((uint32) 0x00000001u << CAN_1_TX_IDE_SHIFT))
#define CAN_1_SET_TX_IDE(i)            (CAN_1_TX[i].txcmd.byte[2u] |= \
                                                   (uint8) (CAN_1_TX_IDE_MASK >> \
                                                   CAN_1_TWO_BYTE_OFFSET))
#define CAN_1_CLEAR_TX_IDE(i)          (CAN_1_TX[i].txcmd.byte[2u] &= \
                                                   (uint8) (((uint32) (~CAN_1_TX_IDE_MASK)) >> \
                                                   CAN_1_TWO_BYTE_OFFSET))

/* bit 21 within TX_CMD[i] */
#define CAN_1_TX_RTR_SHIFT             (21u)
#define CAN_1_TX_RTR_MASK              ((uint32) ((uint32) 0x00000001u <<  CAN_1_TX_RTR_SHIFT))
#define CAN_1_SET_TX_RTR(i)            (CAN_1_TX[i].txcmd.byte[2u] |= \
                                                   (uint8) (CAN_1_TX_RTR_MASK >> \
                                                   CAN_1_TWO_BYTE_OFFSET))
#define CAN_1_CLEAR_TX_RTR(i)          (CAN_1_TX[i].txcmd.byte[2u] &= \
                                                   (uint8) (((uint32) (~CAN_1_TX_RTR_MASK)) >> \
                                                   CAN_1_TWO_BYTE_OFFSET))

/* bit 23 within TX_CMD[i] */
#define CAN_1_TX_WPNH_SHIFT           (7u)
#define CAN_1_TX_WPNH_MASK            ((uint8) ((uint8) 0x01u << CAN_1_TX_WPNH_SHIFT))
#define CAN_1_SET_TX_WNPH(i)          (CAN_1_TX[i].txcmd.byte[2u] |= CAN_1_TX_WPN_MASK)
#define CAN_1_CLEAR_TX_WNPH(i)        (CAN_1_TX[i].txcmd.byte[2u] &= \
                                                  (uint8) (~CAN_1_TX_WPN_MASK))

/* bit 4 within RX_CMD[i].byte[2] */
#define CAN_1_RX_IDE_SHIFT             (4u)
#define CAN_1_RX_IDE_MASK              ((uint8) ((uint8) 0x01u << CAN_1_RX_IDE_SHIFT))
#define CAN_1_GET_RX_IDE(i)            (CAN_1_RX[i].rxcmd.byte[2u] & CAN_1_RX_IDE_MASK)
#define CAN_1_GET_RX_ID(i)             ((CAN_1_GET_RX_IDE(i)) ? ((CY_GET_REG32((reg32 *) & \
                                                    CAN_1_RX[i].rxid)) >> 3u) : ((CY_GET_REG32((reg32 *) & \
                                                    CAN_1_RX[i].rxid)) >> 21u))

#define CAN_1_RX_DATA_BYTE(mailbox, i) (CAN_1_RX[mailbox].rxdata.byte[((i) > 3u) ? \
                                                   (7u - ((i) - 4u)) : (3u - (i))])
#define CAN_1_TX_DATA_BYTE(mailbox, i) (CAN_1_TX[mailbox].txdata.byte[((i) > 3u) ? \
                                                   (7u - ((i) - 4u)) : (3u - (i))])

/* Macros for access to RX DATA for mailbox(i) */
#define CAN_1_RX_DATA_BYTE1(i)         CAN_1_RX[i].rxdata.byte[3u]
#define CAN_1_RX_DATA_BYTE2(i)         CAN_1_RX[i].rxdata.byte[2u]
#define CAN_1_RX_DATA_BYTE3(i)         CAN_1_RX[i].rxdata.byte[1u]
#define CAN_1_RX_DATA_BYTE4(i)         CAN_1_RX[i].rxdata.byte[0u]
#define CAN_1_RX_DATA_BYTE5(i)         CAN_1_RX[i].rxdata.byte[7u]
#define CAN_1_RX_DATA_BYTE6(i)         CAN_1_RX[i].rxdata.byte[6u]
#define CAN_1_RX_DATA_BYTE7(i)         CAN_1_RX[i].rxdata.byte[5u]
#define CAN_1_RX_DATA_BYTE8(i)         CAN_1_RX[i].rxdata.byte[4u]

/* Macros for access to TX DATA for mailbox(i) */
#define CAN_1_TX_DATA_BYTE1(i)         CAN_1_TX[i].txdata.byte[3u]
#define CAN_1_TX_DATA_BYTE2(i)         CAN_1_TX[i].txdata.byte[2u]
#define CAN_1_TX_DATA_BYTE3(i)         CAN_1_TX[i].txdata.byte[1u]
#define CAN_1_TX_DATA_BYTE4(i)         CAN_1_TX[i].txdata.byte[0u]
#define CAN_1_TX_DATA_BYTE5(i)         CAN_1_TX[i].txdata.byte[7u]
#define CAN_1_TX_DATA_BYTE6(i)         CAN_1_TX[i].txdata.byte[6u]
#define CAN_1_TX_DATA_BYTE7(i)         CAN_1_TX[i].txdata.byte[5u]
#define CAN_1_TX_DATA_BYTE8(i)         CAN_1_TX[i].txdata.byte[4u]

/* Macros for set Tx Msg Indentifier in CAN_1_TX_ID register */
#define CAN_1_SET_TX_ID_STANDARD_MSG_SHIFT  (21u)
#define CAN_1_SET_TX_ID_EXTENDED_MSG_SHIFT  (3u)
#define CAN_1_SET_TX_ID_STANDARD_MSG(i, id) (CY_SET_REG32((reg32 *) &CAN_1_TX[i].txid, \
                                        ((uint32) ((uint32) (id) << CAN_1_SET_TX_ID_STANDARD_MSG_SHIFT))))
#define CAN_1_SET_TX_ID_EXTENDED_MSG(i, id) (CY_SET_REG32((reg32 *) &CAN_1_TX[i].txid, \
                                        ((uint32) ((uint32) (id) << CAN_1_SET_TX_ID_EXTENDED_MSG_SHIFT))))

/* Mask for bits within CAN_1_CSR_CFG */
#define CAN_1_EDGE_MODE_SHIFT               (0u)
/* bit 0 within CSR_CFG */
#define CAN_1_EDGE_MODE_MASK                ((uint8) ((uint8) 0x01u << CAN_1_EDGE_MODE_SHIFT))
#define CAN_1_SAMPLE_MODE_SHIFT             (1u)
/* bit 1 within CSR_CFG */
#define CAN_1_SAMPLE_MODE_MASK              ((uint8) ((uint8) 0x01u << CAN_1_SAMPLE_MODE_SHIFT))
#define CAN_1_CFG_REG_SJW_SHIFT             (2u)
/* bits 3-2 within CSR_CFG */
#define CAN_1_CFG_REG_SJW_MASK              ((uint8) ((uint8) 0x03u << CAN_1_CFG_REG_SJW_SHIFT))
#define CAN_1_CFG_REG_SJW_LOWER_LIMIT       (0x03u)  /* the lowest allowed value of cfg_sjw */
#define CAN_1_RESET_SHIFT                   (4u)
/* bit 4 within CSR_CFG    */
#define CAN_1_RESET_MASK                    ((uint8) ((uint8) 0x01u << CAN_1_RESET_SHIFT))
#define CAN_1_CFG_REG_TSEG2_SHIFT           (5u)
/* bits 7-5 within CSR_CFG */
#define CAN_1_CFG_REG_TSEG2_MASK            ((uint8) ((uint8) 0x07u << CAN_1_CFG_REG_TSEG2_SHIFT))
/* the highest allowed value of cfg_tseg2 */
#define CAN_1_CFG_REG_TSEG2_UPPER_LIMIT     (0x07u)
/* the lowest allowed value of cfg_tseg2 */
#define CAN_1_CFG_REG_TSEG2_LOWER_LIMIT     (0x02u)
 /* the lowest allowed value of cfg_tseg2 if sample point is one point */
#define CAN_1_CFG_REG_TSEG2_EXCEPTION       (0x01u)  
/* bits 11-8 within CSR_CFG */
#define CAN_1_CFG_REG_TSEG1_SHIFT           (8u)
#define CAN_1_CFG_REG_TSEG1_MASK            (0x0Fu)
/* the highest allowed value of cfg_tseg1 */
#define CAN_1_CFG_REG_TSEG1_UPPER_LIMIT     (0x0Fu)
/* the lowest allowed value of cfg_tseg1 */
#define CAN_1_CFG_REG_TSEG1_LOWER_LIMIT     (0x02u)
#define CAN_1_ARBITRATION_SHIFT             (4u)
#define CAN_1_ARBITER_SHIFT                 (12u)
/* bit 12 within CSR_CFG */
#define CAN_1_ARBITRATION_MASK              ((uint8) ((uint8) 0x01u << CAN_1_ARBITRATION_SHIFT))
/* bits 23-16 within CSR_CFG */
#define CAN_1_BITRATE_SHIFT                 (16u)
#define CAN_1_BITRATE_MASK                  (0x7FFFu)

/* Mask for bits within CAN_1_CSR_CMD */
#define CAN_1_MODE_SHIFT                    (0u)
/* bit 0 within CSR_CMD */
#define CAN_1_MODE_MASK                     ((uint8) ((uint8) 0x01u << CAN_1_MODE_SHIFT))
#define CAN_1_OPMODE_MASK_SHIFT             (1u)
/* bit 1 within CSR_CMD */
#define CAN_1_OPMODE_MASK                   ((uint8) ((uint8) 0x01u << CAN_1_OPMODE_MASK_SHIFT))

/* Mask for bits within CAN_1_CSR_CMD */
#define CAN_1_ERROR_STATE_SHIFT             (0u)
/* bit 17-16 within ERR_SR */
#define CAN_1_ERROR_STATE_MASK              ((uint8) ((uint8) 0x03u << CAN_1_ERROR_STATE_SHIFT))
#define CAN_1_TX_ERROR_FLAG_SHIFT           (2u)
/* bit 18 within ERR_SR */
#define CAN_1_TX_ERROR_FLAG_MASK            ((uint8) ((uint8) 0x01u << CAN_1_TX_ERROR_FLAG_SHIFT))
#define CAN_1_RX_ERROR_FLAG_SHIFT           (3u)
/* bit 19 within ERR_SR */
#define CAN_1_RX_ERROR_FLAG_MASK            ((uint8) ((uint8) 0x01u << CAN_1_RX_ERROR_FLAG_SHIFT))

/* Mask and Macros for bits within CAN_1_INT_EN_REG */
#define CAN_1_GLOBAL_INT_SHIFT              (0u)
/* bit 0 within INT_EN */
#define CAN_1_GLOBAL_INT_MASK               ((uint8) ((uint8) 0x01u << CAN_1_GLOBAL_INT_SHIFT))
#define CAN_1_ARBITRATION_LOST_SHIFT        (2u)
/* bit 2 within INT_EN and INT_SR */
#define CAN_1_ARBITRATION_LOST_MASK      ((uint8) ((uint8) 0x01u << CAN_1_ARBITRATION_LOST_SHIFT))
#define CAN_1_ARBITRATION_LOST_INT_ENABLE   (CAN_1_INT_EN_REG.byte[0u] |= \
                                                        CAN_1_ARBITRATION_LOST_MASK)
#define CAN_1_ARBITRATION_LOST_INT_DISABLE  (CAN_1_INT_EN_REG.byte[0u] &= \
                                                       (uint8) (~CAN_1_ARBITRATION_LOST_MASK))
#define CAN_1_OVERLOAD_ERROR_SHIFT          (3u)
/* bit 3 within INT_EN and INT_SR */
#define CAN_1_OVERLOAD_ERROR_MASK          ((uint8) ((uint8) 0x01u << CAN_1_OVERLOAD_ERROR_SHIFT))
#define CAN_1_OVERLOAD_ERROR_INT_ENABLE    (CAN_1_INT_EN_REG.byte[0u] |= \
                                                       CAN_1_OVERLOAD_ERROR_MASK)
#define CAN_1_OVERLOAD_ERROR_INT_DISABLE   (CAN_1_INT_EN_REG.byte[0u] &= \
                                                      (uint8) (~CAN_1_OVERLOAD_ERROR_MASK))
#define CAN_1_BIT_ERROR_SHIFT               (4u)
/* bit 4 within INT_EN and INT_SR */
#define CAN_1_BIT_ERROR_MASK                ((uint8) ((uint8) 0x01u << CAN_1_BIT_ERROR_SHIFT))
#define CAN_1_BIT_ERROR_LOST_INT_ENABLE     (CAN_1_INT_EN_REG.byte[0u] |= \
                                                        CAN_1_BIT_ERROR_MASK)
#define CAN_1_BIT_ERROR_LOST_INT_DISABLE    (CAN_1_INT_EN_REG.byte[0u] &= \
                                                       (uint8) (~CAN_1_BIT_ERROR_MASK))
#define CAN_1_STUFF_ERROR_SHIFT             (5u)
/* bit 5 within INT_EN and INT_SR */
#define CAN_1_STUFF_ERROR_MASK              ((uint8) ((uint8) 0x01u << CAN_1_STUFF_ERROR_SHIFT))
#define CAN_1_STUFF_ERROR_INT_ENABLE        (CAN_1_INT_EN_REG.byte[0u] |= \
                                                        CAN_1_STUFF_ERROR_MASK)
#define CAN_1_STUFF_ERROR_INT_DISABLE       (CAN_1_INT_EN_REG.byte[0u] &= \
                                                       (uint8) (~CAN_1_STUFF_ERROR_MASK))
#define CAN_1_ACK_ERROR_SHIFT               (6u)
/* bit 6 within INT_EN and INT_SR */
#define CAN_1_ACK_ERROR_MASK                ((uint8) ((uint8) 0x01u << CAN_1_ACK_ERROR_SHIFT))
#define CAN_1_ACK_ERROR_INT_ENABLE          (CAN_1_INT_EN_REG.byte[0u] |= \
                                                        CAN_1_ACK_ERROR_MASK)
#define CAN_1_ACK_ERROR_INT_DISABLE         (CAN_1_INT_EN_REG.byte[0u] &= \
                                                       (uint8) (~CAN_1_ACK_ERROR_MASK))
#define CAN_1_FORM_ERROR_SHIFT              (7u)
/* bit 7 within INT_EN and INT_SR */
#define CAN_1_FORM_ERROR_MASK               ((uint8) ((uint8) 0x01u << CAN_1_FORM_ERROR_SHIFT))
#define CAN_1_FORM_ERROR_INT_ENABLE         (CAN_1_INT_EN_REG.byte[0u] |= \
                                                        CAN_1_FORM_ERROR_MASK)
#define CAN_1_FORM_ERROR_INT_DISABLE        (CAN_1_INT_EN_REG.byte[0u] &= \
                                                        (uint8) (~CAN_1_FORM_ERROR_MASK))
#define CAN_1_CRC_ERROR_SHIFT               (0u)
/* bit 8 within INT_EN and INT_SR */
#define CAN_1_CRC_ERROR_MASK                ((uint8) ((uint8) 0x01u << CAN_1_CRC_ERROR_SHIFT))
#define CAN_1_CRC_ERROR_INT_ENABLE          (CAN_1_INT_EN_REG.byte[1u] |= \
                                                        CAN_1_CRC_ERROR_MASK)
#define CAN_1_CRC_ERROR_INT_DISABLE         (CAN_1_INT_EN_REG.byte[1u] &= \
                                                        (uint8) (~CAN_1_CRC_ERROR_MASK))
#define CAN_1_BUS_OFF_SHIFT                 (1u)
/* bit 9 within INT_EN and INT_SR */
#define CAN_1_BUS_OFF_MASK                  ((uint8) ((uint8) 0x01u << CAN_1_BUS_OFF_SHIFT))
#define CAN_1_BUS_OFF_INT_ENABLE            (CAN_1_INT_EN_REG.byte[1u] |= \
                                                        CAN_1_BUS_OFF_MASK)
#define CAN_1_BUS_OFF_INT_DISABLE           (CAN_1_INT_EN_REG.byte[1u] &= \
                                                        (uint8) (~CAN_1_BUS_OFF_MASK))
#define CAN_1_RX_MSG_LOST_SHIFT             (2u)
/* bit 10 within INT_EN and INT_SR */
#define CAN_1_RX_MSG_LOST_MASK              ((uint8) ((uint8) 0x01u << CAN_1_RX_MSG_LOST_SHIFT))
#define CAN_1_RX_MSG_LOST_INT_ENABLE        (CAN_1_INT_EN_REG.byte[1u] |= \
                                                        CAN_1_RX_MSG_LOST_MASK)
#define CAN_1_RX_MSG_LOST_INT_DISABLE       (CAN_1_INT_EN_REG.byte[1u] &= \
                                                        (uint8) (~CAN_1_RX_MSG_LOST_MASK))
#define CAN_1_TX_MESSAGE_SHIFT              (3u)
/* bit 11 within INT_EN and INT_SR */
#define CAN_1_TX_MESSAGE_MASK               ((uint8) ((uint8) 0x01u << CAN_1_TX_MESSAGE_SHIFT))
#define CAN_1_TX_MSG_INT_ENABLE             (CAN_1_INT_EN_REG.byte[1u] |= \
                                                        CAN_1_TX_MESSAGE_MASK)
#define CAN_1_TX_MSG_INT_DISABLE            (CAN_1_INT_EN_REG.byte[1u] &= \
                                                        (uint8) (~CAN_1_TX_MESSAGE_MASK))
#define CAN_1_RX_MESSAGE_SHIFT              (4u)
/* bit 12 within INT_EN and INT_SR */
#define CAN_1_RX_MESSAGE_MASK               ((uint8) ((uint8) 0x01u << CAN_1_RX_MESSAGE_SHIFT))
#define CAN_1_RX_MSG_INT_ENABLE             (CAN_1_INT_EN_REG.byte[1u] |= \
                                                        CAN_1_RX_MESSAGE_MASK)
#define CAN_1_RX_MSG_INT_DISABLE            (CAN_1_INT_EN_REG.byte[1u] &= \
                                                        (uint8) (~CAN_1_RX_MESSAGE_MASK))

#define CAN_1_GLOBAL_INT_ENABLE_SHIFT       (0u)
#define CAN_1_ARBITRATION_LOST_ENABLE_SHIFT (2u)
#define CAN_1_OVERLOAD_ERROR_ENABLE_SHIFT   (3u)
#define CAN_1_BIT_ERROR_ENABLE_SHIFT        (4u)
#define CAN_1_STUFF_ERROR_ENABLE_SHIFT      (5u)
#define CAN_1_ACK_ERROR_ENABLE_SHIFT        (6u)
#define CAN_1_FORM_ERROR_ENABLE_SHIFT       (7u)
#define CAN_1_CRC_ERROR_ENABLE_SHIFT        (8u)
#define CAN_1_BUS_OFF_ENABLE_SHIFT          (9u)
#define CAN_1_RX_MSG_LOST_ENABLE_SHIFT      (10u)
#define CAN_1_TX_MESSAGE_ENABLE_SHIFT       (11u)
#define CAN_1_RX_MESSAGE_ENABLE_SHIFT       (12u)

#define CAN_1_GLOBAL_INT_ENABLE       ((uint16) ((uint16) 0x01u << CAN_1_GLOBAL_INT_ENABLE_SHIFT))
#define CAN_1_ARBITRATION_LOST_ENABLE ((uint16) ((uint16) 0x01u << \
                                                            CAN_1_ARBITRATION_LOST_ENABLE_SHIFT))
#define CAN_1_OVERLOAD_ERROR_ENABLE   ((uint16) ((uint16) 0x01u << \
                                                            CAN_1_OVERLOAD_ERROR_ENABLE_SHIFT))
#define CAN_1_BIT_ERROR_ENABLE       ((uint16) ((uint16) 0x01u << CAN_1_BIT_ERROR_ENABLE_SHIFT))
#define CAN_1_STUFF_ERROR_ENABLE     ((uint16) ((uint16) 0x01u << CAN_1_STUFF_ERROR_ENABLE_SHIFT))
#define CAN_1_ACK_ERROR_ENABLE       ((uint16) ((uint16) 0x01u << CAN_1_ACK_ERROR_ENABLE_SHIFT))
#define CAN_1_FORM_ERROR_ENABLE      ((uint16) ((uint16) 0x01u << CAN_1_FORM_ERROR_ENABLE_SHIFT))
#define CAN_1_CRC_ERROR_ENABLE       ((uint16) ((uint16) 0x01u << CAN_1_CRC_ERROR_ENABLE_SHIFT))
#define CAN_1_BUS_OFF_ENABLE         ((uint16) ((uint16) 0x01u << CAN_1_BUS_OFF_ENABLE_SHIFT))
#define CAN_1_RX_MSG_LOST_ENABLE     ((uint16) ((uint16) 0x01u << CAN_1_RX_MSG_LOST_ENABLE_SHIFT))
#define CAN_1_TX_MESSAGE_ENABLE      ((uint16) ((uint16) 0x01u << CAN_1_TX_MESSAGE_ENABLE_SHIFT))
#define CAN_1_RX_MESSAGE_ENABLE      ((uint16) ((uint16) 0x01u << CAN_1_RX_MESSAGE_ENABLE_SHIFT))

/******************************************************************************************
* Following code are OBSOLETE and must not be used starting from CAN 2.30
******************************************************************************************/
#define CAN_1_RESTART_BY_HAND               (0x00u)

#endif /* CY_CAN_CAN_1_H */


/* [] END OF FILE */
