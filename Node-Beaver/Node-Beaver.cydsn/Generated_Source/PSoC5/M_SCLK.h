/*******************************************************************************
* File Name: M_SCLK.h  
* Version 2.5
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

#if !defined(CY_PINS_M_SCLK_H) /* Pins M_SCLK_H */
#define CY_PINS_M_SCLK_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "M_SCLK_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_5 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 M_SCLK__PORT == 15 && ((M_SCLK__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    M_SCLK_Write(uint8 value) ;
void    M_SCLK_SetDriveMode(uint8 mode) ;
uint8   M_SCLK_ReadDataReg(void) ;
uint8   M_SCLK_Read(void) ;
uint8   M_SCLK_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define M_SCLK_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define M_SCLK_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define M_SCLK_DM_RES_UP          PIN_DM_RES_UP
#define M_SCLK_DM_RES_DWN         PIN_DM_RES_DWN
#define M_SCLK_DM_OD_LO           PIN_DM_OD_LO
#define M_SCLK_DM_OD_HI           PIN_DM_OD_HI
#define M_SCLK_DM_STRONG          PIN_DM_STRONG
#define M_SCLK_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define M_SCLK_MASK               M_SCLK__MASK
#define M_SCLK_SHIFT              M_SCLK__SHIFT
#define M_SCLK_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define M_SCLK_PS                     (* (reg8 *) M_SCLK__PS)
/* Data Register */
#define M_SCLK_DR                     (* (reg8 *) M_SCLK__DR)
/* Port Number */
#define M_SCLK_PRT_NUM                (* (reg8 *) M_SCLK__PRT) 
/* Connect to Analog Globals */                                                  
#define M_SCLK_AG                     (* (reg8 *) M_SCLK__AG)                       
/* Analog MUX bux enable */
#define M_SCLK_AMUX                   (* (reg8 *) M_SCLK__AMUX) 
/* Bidirectional Enable */                                                        
#define M_SCLK_BIE                    (* (reg8 *) M_SCLK__BIE)
/* Bit-mask for Aliased Register Access */
#define M_SCLK_BIT_MASK               (* (reg8 *) M_SCLK__BIT_MASK)
/* Bypass Enable */
#define M_SCLK_BYP                    (* (reg8 *) M_SCLK__BYP)
/* Port wide control signals */                                                   
#define M_SCLK_CTL                    (* (reg8 *) M_SCLK__CTL)
/* Drive Modes */
#define M_SCLK_DM0                    (* (reg8 *) M_SCLK__DM0) 
#define M_SCLK_DM1                    (* (reg8 *) M_SCLK__DM1)
#define M_SCLK_DM2                    (* (reg8 *) M_SCLK__DM2) 
/* Input Buffer Disable Override */
#define M_SCLK_INP_DIS                (* (reg8 *) M_SCLK__INP_DIS)
/* LCD Common or Segment Drive */
#define M_SCLK_LCD_COM_SEG            (* (reg8 *) M_SCLK__LCD_COM_SEG)
/* Enable Segment LCD */
#define M_SCLK_LCD_EN                 (* (reg8 *) M_SCLK__LCD_EN)
/* Slew Rate Control */
#define M_SCLK_SLW                    (* (reg8 *) M_SCLK__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define M_SCLK_PRTDSI__CAPS_SEL       (* (reg8 *) M_SCLK__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define M_SCLK_PRTDSI__DBL_SYNC_IN    (* (reg8 *) M_SCLK__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define M_SCLK_PRTDSI__OE_SEL0        (* (reg8 *) M_SCLK__PRTDSI__OE_SEL0) 
#define M_SCLK_PRTDSI__OE_SEL1        (* (reg8 *) M_SCLK__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define M_SCLK_PRTDSI__OUT_SEL0       (* (reg8 *) M_SCLK__PRTDSI__OUT_SEL0) 
#define M_SCLK_PRTDSI__OUT_SEL1       (* (reg8 *) M_SCLK__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define M_SCLK_PRTDSI__SYNC_OUT       (* (reg8 *) M_SCLK__PRTDSI__SYNC_OUT) 


#if defined(M_SCLK__INTSTAT)  /* Interrupt Registers */

    #define M_SCLK_INTSTAT                (* (reg8 *) M_SCLK__INTSTAT)
    #define M_SCLK_SNAP                   (* (reg8 *) M_SCLK__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_M_SCLK_H */


/* [] END OF FILE */
