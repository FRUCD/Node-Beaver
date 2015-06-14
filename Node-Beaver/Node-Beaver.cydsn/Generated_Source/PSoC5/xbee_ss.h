/*******************************************************************************
* File Name: xbee_ss.h  
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

#if !defined(CY_PINS_xbee_ss_H) /* Pins xbee_ss_H */
#define CY_PINS_xbee_ss_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "xbee_ss_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 xbee_ss__PORT == 15 && ((xbee_ss__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    xbee_ss_Write(uint8 value) ;
void    xbee_ss_SetDriveMode(uint8 mode) ;
uint8   xbee_ss_ReadDataReg(void) ;
uint8   xbee_ss_Read(void) ;
uint8   xbee_ss_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define xbee_ss_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define xbee_ss_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define xbee_ss_DM_RES_UP          PIN_DM_RES_UP
#define xbee_ss_DM_RES_DWN         PIN_DM_RES_DWN
#define xbee_ss_DM_OD_LO           PIN_DM_OD_LO
#define xbee_ss_DM_OD_HI           PIN_DM_OD_HI
#define xbee_ss_DM_STRONG          PIN_DM_STRONG
#define xbee_ss_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define xbee_ss_MASK               xbee_ss__MASK
#define xbee_ss_SHIFT              xbee_ss__SHIFT
#define xbee_ss_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define xbee_ss_PS                     (* (reg8 *) xbee_ss__PS)
/* Data Register */
#define xbee_ss_DR                     (* (reg8 *) xbee_ss__DR)
/* Port Number */
#define xbee_ss_PRT_NUM                (* (reg8 *) xbee_ss__PRT) 
/* Connect to Analog Globals */                                                  
#define xbee_ss_AG                     (* (reg8 *) xbee_ss__AG)                       
/* Analog MUX bux enable */
#define xbee_ss_AMUX                   (* (reg8 *) xbee_ss__AMUX) 
/* Bidirectional Enable */                                                        
#define xbee_ss_BIE                    (* (reg8 *) xbee_ss__BIE)
/* Bit-mask for Aliased Register Access */
#define xbee_ss_BIT_MASK               (* (reg8 *) xbee_ss__BIT_MASK)
/* Bypass Enable */
#define xbee_ss_BYP                    (* (reg8 *) xbee_ss__BYP)
/* Port wide control signals */                                                   
#define xbee_ss_CTL                    (* (reg8 *) xbee_ss__CTL)
/* Drive Modes */
#define xbee_ss_DM0                    (* (reg8 *) xbee_ss__DM0) 
#define xbee_ss_DM1                    (* (reg8 *) xbee_ss__DM1)
#define xbee_ss_DM2                    (* (reg8 *) xbee_ss__DM2) 
/* Input Buffer Disable Override */
#define xbee_ss_INP_DIS                (* (reg8 *) xbee_ss__INP_DIS)
/* LCD Common or Segment Drive */
#define xbee_ss_LCD_COM_SEG            (* (reg8 *) xbee_ss__LCD_COM_SEG)
/* Enable Segment LCD */
#define xbee_ss_LCD_EN                 (* (reg8 *) xbee_ss__LCD_EN)
/* Slew Rate Control */
#define xbee_ss_SLW                    (* (reg8 *) xbee_ss__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define xbee_ss_PRTDSI__CAPS_SEL       (* (reg8 *) xbee_ss__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define xbee_ss_PRTDSI__DBL_SYNC_IN    (* (reg8 *) xbee_ss__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define xbee_ss_PRTDSI__OE_SEL0        (* (reg8 *) xbee_ss__PRTDSI__OE_SEL0) 
#define xbee_ss_PRTDSI__OE_SEL1        (* (reg8 *) xbee_ss__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define xbee_ss_PRTDSI__OUT_SEL0       (* (reg8 *) xbee_ss__PRTDSI__OUT_SEL0) 
#define xbee_ss_PRTDSI__OUT_SEL1       (* (reg8 *) xbee_ss__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define xbee_ss_PRTDSI__SYNC_OUT       (* (reg8 *) xbee_ss__PRTDSI__SYNC_OUT) 


#if defined(xbee_ss__INTSTAT)  /* Interrupt Registers */

    #define xbee_ss_INTSTAT                (* (reg8 *) xbee_ss__INTSTAT)
    #define xbee_ss_SNAP                   (* (reg8 *) xbee_ss__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_xbee_ss_H */


/* [] END OF FILE */
