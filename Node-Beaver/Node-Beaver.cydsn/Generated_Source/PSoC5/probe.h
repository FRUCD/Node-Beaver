/*******************************************************************************
* File Name: probe.h  
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

#if !defined(CY_PINS_probe_H) /* Pins probe_H */
#define CY_PINS_probe_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "probe_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 probe__PORT == 15 && ((probe__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    probe_Write(uint8 value) ;
void    probe_SetDriveMode(uint8 mode) ;
uint8   probe_ReadDataReg(void) ;
uint8   probe_Read(void) ;
uint8   probe_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define probe_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define probe_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define probe_DM_RES_UP          PIN_DM_RES_UP
#define probe_DM_RES_DWN         PIN_DM_RES_DWN
#define probe_DM_OD_LO           PIN_DM_OD_LO
#define probe_DM_OD_HI           PIN_DM_OD_HI
#define probe_DM_STRONG          PIN_DM_STRONG
#define probe_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define probe_MASK               probe__MASK
#define probe_SHIFT              probe__SHIFT
#define probe_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define probe_PS                     (* (reg8 *) probe__PS)
/* Data Register */
#define probe_DR                     (* (reg8 *) probe__DR)
/* Port Number */
#define probe_PRT_NUM                (* (reg8 *) probe__PRT) 
/* Connect to Analog Globals */                                                  
#define probe_AG                     (* (reg8 *) probe__AG)                       
/* Analog MUX bux enable */
#define probe_AMUX                   (* (reg8 *) probe__AMUX) 
/* Bidirectional Enable */                                                        
#define probe_BIE                    (* (reg8 *) probe__BIE)
/* Bit-mask for Aliased Register Access */
#define probe_BIT_MASK               (* (reg8 *) probe__BIT_MASK)
/* Bypass Enable */
#define probe_BYP                    (* (reg8 *) probe__BYP)
/* Port wide control signals */                                                   
#define probe_CTL                    (* (reg8 *) probe__CTL)
/* Drive Modes */
#define probe_DM0                    (* (reg8 *) probe__DM0) 
#define probe_DM1                    (* (reg8 *) probe__DM1)
#define probe_DM2                    (* (reg8 *) probe__DM2) 
/* Input Buffer Disable Override */
#define probe_INP_DIS                (* (reg8 *) probe__INP_DIS)
/* LCD Common or Segment Drive */
#define probe_LCD_COM_SEG            (* (reg8 *) probe__LCD_COM_SEG)
/* Enable Segment LCD */
#define probe_LCD_EN                 (* (reg8 *) probe__LCD_EN)
/* Slew Rate Control */
#define probe_SLW                    (* (reg8 *) probe__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define probe_PRTDSI__CAPS_SEL       (* (reg8 *) probe__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define probe_PRTDSI__DBL_SYNC_IN    (* (reg8 *) probe__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define probe_PRTDSI__OE_SEL0        (* (reg8 *) probe__PRTDSI__OE_SEL0) 
#define probe_PRTDSI__OE_SEL1        (* (reg8 *) probe__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define probe_PRTDSI__OUT_SEL0       (* (reg8 *) probe__PRTDSI__OUT_SEL0) 
#define probe_PRTDSI__OUT_SEL1       (* (reg8 *) probe__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define probe_PRTDSI__SYNC_OUT       (* (reg8 *) probe__PRTDSI__SYNC_OUT) 


#if defined(probe__INTSTAT)  /* Interrupt Registers */

    #define probe_INTSTAT                (* (reg8 *) probe__INTSTAT)
    #define probe_SNAP                   (* (reg8 *) probe__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_probe_H */


/* [] END OF FILE */
