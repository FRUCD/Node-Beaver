/*******************************************************************************
* File Name: power_plus.h  
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

#if !defined(CY_PINS_power_plus_H) /* Pins power_plus_H */
#define CY_PINS_power_plus_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "power_plus_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 power_plus__PORT == 15 && ((power_plus__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    power_plus_Write(uint8 value) ;
void    power_plus_SetDriveMode(uint8 mode) ;
uint8   power_plus_ReadDataReg(void) ;
uint8   power_plus_Read(void) ;
uint8   power_plus_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define power_plus_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define power_plus_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define power_plus_DM_RES_UP          PIN_DM_RES_UP
#define power_plus_DM_RES_DWN         PIN_DM_RES_DWN
#define power_plus_DM_OD_LO           PIN_DM_OD_LO
#define power_plus_DM_OD_HI           PIN_DM_OD_HI
#define power_plus_DM_STRONG          PIN_DM_STRONG
#define power_plus_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define power_plus_MASK               power_plus__MASK
#define power_plus_SHIFT              power_plus__SHIFT
#define power_plus_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define power_plus_PS                     (* (reg8 *) power_plus__PS)
/* Data Register */
#define power_plus_DR                     (* (reg8 *) power_plus__DR)
/* Port Number */
#define power_plus_PRT_NUM                (* (reg8 *) power_plus__PRT) 
/* Connect to Analog Globals */                                                  
#define power_plus_AG                     (* (reg8 *) power_plus__AG)                       
/* Analog MUX bux enable */
#define power_plus_AMUX                   (* (reg8 *) power_plus__AMUX) 
/* Bidirectional Enable */                                                        
#define power_plus_BIE                    (* (reg8 *) power_plus__BIE)
/* Bit-mask for Aliased Register Access */
#define power_plus_BIT_MASK               (* (reg8 *) power_plus__BIT_MASK)
/* Bypass Enable */
#define power_plus_BYP                    (* (reg8 *) power_plus__BYP)
/* Port wide control signals */                                                   
#define power_plus_CTL                    (* (reg8 *) power_plus__CTL)
/* Drive Modes */
#define power_plus_DM0                    (* (reg8 *) power_plus__DM0) 
#define power_plus_DM1                    (* (reg8 *) power_plus__DM1)
#define power_plus_DM2                    (* (reg8 *) power_plus__DM2) 
/* Input Buffer Disable Override */
#define power_plus_INP_DIS                (* (reg8 *) power_plus__INP_DIS)
/* LCD Common or Segment Drive */
#define power_plus_LCD_COM_SEG            (* (reg8 *) power_plus__LCD_COM_SEG)
/* Enable Segment LCD */
#define power_plus_LCD_EN                 (* (reg8 *) power_plus__LCD_EN)
/* Slew Rate Control */
#define power_plus_SLW                    (* (reg8 *) power_plus__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define power_plus_PRTDSI__CAPS_SEL       (* (reg8 *) power_plus__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define power_plus_PRTDSI__DBL_SYNC_IN    (* (reg8 *) power_plus__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define power_plus_PRTDSI__OE_SEL0        (* (reg8 *) power_plus__PRTDSI__OE_SEL0) 
#define power_plus_PRTDSI__OE_SEL1        (* (reg8 *) power_plus__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define power_plus_PRTDSI__OUT_SEL0       (* (reg8 *) power_plus__PRTDSI__OUT_SEL0) 
#define power_plus_PRTDSI__OUT_SEL1       (* (reg8 *) power_plus__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define power_plus_PRTDSI__SYNC_OUT       (* (reg8 *) power_plus__PRTDSI__SYNC_OUT) 


#if defined(power_plus__INTSTAT)  /* Interrupt Registers */

    #define power_plus_INTSTAT                (* (reg8 *) power_plus__INTSTAT)
    #define power_plus_SNAP                   (* (reg8 *) power_plus__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_power_plus_H */


/* [] END OF FILE */
