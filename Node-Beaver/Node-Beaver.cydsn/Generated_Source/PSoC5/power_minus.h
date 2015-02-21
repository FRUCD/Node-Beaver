/*******************************************************************************
* File Name: power_minus.h  
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

#if !defined(CY_PINS_power_minus_H) /* Pins power_minus_H */
#define CY_PINS_power_minus_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "power_minus_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 power_minus__PORT == 15 && ((power_minus__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    power_minus_Write(uint8 value) ;
void    power_minus_SetDriveMode(uint8 mode) ;
uint8   power_minus_ReadDataReg(void) ;
uint8   power_minus_Read(void) ;
uint8   power_minus_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define power_minus_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define power_minus_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define power_minus_DM_RES_UP          PIN_DM_RES_UP
#define power_minus_DM_RES_DWN         PIN_DM_RES_DWN
#define power_minus_DM_OD_LO           PIN_DM_OD_LO
#define power_minus_DM_OD_HI           PIN_DM_OD_HI
#define power_minus_DM_STRONG          PIN_DM_STRONG
#define power_minus_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define power_minus_MASK               power_minus__MASK
#define power_minus_SHIFT              power_minus__SHIFT
#define power_minus_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define power_minus_PS                     (* (reg8 *) power_minus__PS)
/* Data Register */
#define power_minus_DR                     (* (reg8 *) power_minus__DR)
/* Port Number */
#define power_minus_PRT_NUM                (* (reg8 *) power_minus__PRT) 
/* Connect to Analog Globals */                                                  
#define power_minus_AG                     (* (reg8 *) power_minus__AG)                       
/* Analog MUX bux enable */
#define power_minus_AMUX                   (* (reg8 *) power_minus__AMUX) 
/* Bidirectional Enable */                                                        
#define power_minus_BIE                    (* (reg8 *) power_minus__BIE)
/* Bit-mask for Aliased Register Access */
#define power_minus_BIT_MASK               (* (reg8 *) power_minus__BIT_MASK)
/* Bypass Enable */
#define power_minus_BYP                    (* (reg8 *) power_minus__BYP)
/* Port wide control signals */                                                   
#define power_minus_CTL                    (* (reg8 *) power_minus__CTL)
/* Drive Modes */
#define power_minus_DM0                    (* (reg8 *) power_minus__DM0) 
#define power_minus_DM1                    (* (reg8 *) power_minus__DM1)
#define power_minus_DM2                    (* (reg8 *) power_minus__DM2) 
/* Input Buffer Disable Override */
#define power_minus_INP_DIS                (* (reg8 *) power_minus__INP_DIS)
/* LCD Common or Segment Drive */
#define power_minus_LCD_COM_SEG            (* (reg8 *) power_minus__LCD_COM_SEG)
/* Enable Segment LCD */
#define power_minus_LCD_EN                 (* (reg8 *) power_minus__LCD_EN)
/* Slew Rate Control */
#define power_minus_SLW                    (* (reg8 *) power_minus__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define power_minus_PRTDSI__CAPS_SEL       (* (reg8 *) power_minus__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define power_minus_PRTDSI__DBL_SYNC_IN    (* (reg8 *) power_minus__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define power_minus_PRTDSI__OE_SEL0        (* (reg8 *) power_minus__PRTDSI__OE_SEL0) 
#define power_minus_PRTDSI__OE_SEL1        (* (reg8 *) power_minus__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define power_minus_PRTDSI__OUT_SEL0       (* (reg8 *) power_minus__PRTDSI__OUT_SEL0) 
#define power_minus_PRTDSI__OUT_SEL1       (* (reg8 *) power_minus__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define power_minus_PRTDSI__SYNC_OUT       (* (reg8 *) power_minus__PRTDSI__SYNC_OUT) 


#if defined(power_minus__INTSTAT)  /* Interrupt Registers */

    #define power_minus_INTSTAT                (* (reg8 *) power_minus__INTSTAT)
    #define power_minus_SNAP                   (* (reg8 *) power_minus__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_power_minus_H */


/* [] END OF FILE */
