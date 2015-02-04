/*******************************************************************************
* File Name: XBEE_SS.h  
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

#if !defined(CY_PINS_XBEE_SS_H) /* Pins XBEE_SS_H */
#define CY_PINS_XBEE_SS_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "XBEE_SS_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 XBEE_SS__PORT == 15 && ((XBEE_SS__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    XBEE_SS_Write(uint8 value) ;
void    XBEE_SS_SetDriveMode(uint8 mode) ;
uint8   XBEE_SS_ReadDataReg(void) ;
uint8   XBEE_SS_Read(void) ;
uint8   XBEE_SS_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define XBEE_SS_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define XBEE_SS_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define XBEE_SS_DM_RES_UP          PIN_DM_RES_UP
#define XBEE_SS_DM_RES_DWN         PIN_DM_RES_DWN
#define XBEE_SS_DM_OD_LO           PIN_DM_OD_LO
#define XBEE_SS_DM_OD_HI           PIN_DM_OD_HI
#define XBEE_SS_DM_STRONG          PIN_DM_STRONG
#define XBEE_SS_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define XBEE_SS_MASK               XBEE_SS__MASK
#define XBEE_SS_SHIFT              XBEE_SS__SHIFT
#define XBEE_SS_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define XBEE_SS_PS                     (* (reg8 *) XBEE_SS__PS)
/* Data Register */
#define XBEE_SS_DR                     (* (reg8 *) XBEE_SS__DR)
/* Port Number */
#define XBEE_SS_PRT_NUM                (* (reg8 *) XBEE_SS__PRT) 
/* Connect to Analog Globals */                                                  
#define XBEE_SS_AG                     (* (reg8 *) XBEE_SS__AG)                       
/* Analog MUX bux enable */
#define XBEE_SS_AMUX                   (* (reg8 *) XBEE_SS__AMUX) 
/* Bidirectional Enable */                                                        
#define XBEE_SS_BIE                    (* (reg8 *) XBEE_SS__BIE)
/* Bit-mask for Aliased Register Access */
#define XBEE_SS_BIT_MASK               (* (reg8 *) XBEE_SS__BIT_MASK)
/* Bypass Enable */
#define XBEE_SS_BYP                    (* (reg8 *) XBEE_SS__BYP)
/* Port wide control signals */                                                   
#define XBEE_SS_CTL                    (* (reg8 *) XBEE_SS__CTL)
/* Drive Modes */
#define XBEE_SS_DM0                    (* (reg8 *) XBEE_SS__DM0) 
#define XBEE_SS_DM1                    (* (reg8 *) XBEE_SS__DM1)
#define XBEE_SS_DM2                    (* (reg8 *) XBEE_SS__DM2) 
/* Input Buffer Disable Override */
#define XBEE_SS_INP_DIS                (* (reg8 *) XBEE_SS__INP_DIS)
/* LCD Common or Segment Drive */
#define XBEE_SS_LCD_COM_SEG            (* (reg8 *) XBEE_SS__LCD_COM_SEG)
/* Enable Segment LCD */
#define XBEE_SS_LCD_EN                 (* (reg8 *) XBEE_SS__LCD_EN)
/* Slew Rate Control */
#define XBEE_SS_SLW                    (* (reg8 *) XBEE_SS__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define XBEE_SS_PRTDSI__CAPS_SEL       (* (reg8 *) XBEE_SS__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define XBEE_SS_PRTDSI__DBL_SYNC_IN    (* (reg8 *) XBEE_SS__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define XBEE_SS_PRTDSI__OE_SEL0        (* (reg8 *) XBEE_SS__PRTDSI__OE_SEL0) 
#define XBEE_SS_PRTDSI__OE_SEL1        (* (reg8 *) XBEE_SS__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define XBEE_SS_PRTDSI__OUT_SEL0       (* (reg8 *) XBEE_SS__PRTDSI__OUT_SEL0) 
#define XBEE_SS_PRTDSI__OUT_SEL1       (* (reg8 *) XBEE_SS__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define XBEE_SS_PRTDSI__SYNC_OUT       (* (reg8 *) XBEE_SS__PRTDSI__SYNC_OUT) 


#if defined(XBEE_SS__INTSTAT)  /* Interrupt Registers */

    #define XBEE_SS_INTSTAT                (* (reg8 *) XBEE_SS__INTSTAT)
    #define XBEE_SS_SNAP                   (* (reg8 *) XBEE_SS__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_XBEE_SS_H */


/* [] END OF FILE */
