/*******************************************************************************
* File Name: rtc_int.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_rtc_int_H) /* Pins rtc_int_H */
#define CY_PINS_rtc_int_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "rtc_int_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 rtc_int__PORT == 15 && ((rtc_int__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    rtc_int_Write(uint8 value) ;
void    rtc_int_SetDriveMode(uint8 mode) ;
uint8   rtc_int_ReadDataReg(void) ;
uint8   rtc_int_Read(void) ;
uint8   rtc_int_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define rtc_int_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define rtc_int_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define rtc_int_DM_RES_UP          PIN_DM_RES_UP
#define rtc_int_DM_RES_DWN         PIN_DM_RES_DWN
#define rtc_int_DM_OD_LO           PIN_DM_OD_LO
#define rtc_int_DM_OD_HI           PIN_DM_OD_HI
#define rtc_int_DM_STRONG          PIN_DM_STRONG
#define rtc_int_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define rtc_int_MASK               rtc_int__MASK
#define rtc_int_SHIFT              rtc_int__SHIFT
#define rtc_int_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define rtc_int_PS                     (* (reg8 *) rtc_int__PS)
/* Data Register */
#define rtc_int_DR                     (* (reg8 *) rtc_int__DR)
/* Port Number */
#define rtc_int_PRT_NUM                (* (reg8 *) rtc_int__PRT) 
/* Connect to Analog Globals */                                                  
#define rtc_int_AG                     (* (reg8 *) rtc_int__AG)                       
/* Analog MUX bux enable */
#define rtc_int_AMUX                   (* (reg8 *) rtc_int__AMUX) 
/* Bidirectional Enable */                                                        
#define rtc_int_BIE                    (* (reg8 *) rtc_int__BIE)
/* Bit-mask for Aliased Register Access */
#define rtc_int_BIT_MASK               (* (reg8 *) rtc_int__BIT_MASK)
/* Bypass Enable */
#define rtc_int_BYP                    (* (reg8 *) rtc_int__BYP)
/* Port wide control signals */                                                   
#define rtc_int_CTL                    (* (reg8 *) rtc_int__CTL)
/* Drive Modes */
#define rtc_int_DM0                    (* (reg8 *) rtc_int__DM0) 
#define rtc_int_DM1                    (* (reg8 *) rtc_int__DM1)
#define rtc_int_DM2                    (* (reg8 *) rtc_int__DM2) 
/* Input Buffer Disable Override */
#define rtc_int_INP_DIS                (* (reg8 *) rtc_int__INP_DIS)
/* LCD Common or Segment Drive */
#define rtc_int_LCD_COM_SEG            (* (reg8 *) rtc_int__LCD_COM_SEG)
/* Enable Segment LCD */
#define rtc_int_LCD_EN                 (* (reg8 *) rtc_int__LCD_EN)
/* Slew Rate Control */
#define rtc_int_SLW                    (* (reg8 *) rtc_int__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define rtc_int_PRTDSI__CAPS_SEL       (* (reg8 *) rtc_int__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define rtc_int_PRTDSI__DBL_SYNC_IN    (* (reg8 *) rtc_int__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define rtc_int_PRTDSI__OE_SEL0        (* (reg8 *) rtc_int__PRTDSI__OE_SEL0) 
#define rtc_int_PRTDSI__OE_SEL1        (* (reg8 *) rtc_int__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define rtc_int_PRTDSI__OUT_SEL0       (* (reg8 *) rtc_int__PRTDSI__OUT_SEL0) 
#define rtc_int_PRTDSI__OUT_SEL1       (* (reg8 *) rtc_int__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define rtc_int_PRTDSI__SYNC_OUT       (* (reg8 *) rtc_int__PRTDSI__SYNC_OUT) 


#if defined(rtc_int__INTSTAT)  /* Interrupt Registers */

    #define rtc_int_INTSTAT                (* (reg8 *) rtc_int__INTSTAT)
    #define rtc_int_SNAP                   (* (reg8 *) rtc_int__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_rtc_int_H */


/* [] END OF FILE */
