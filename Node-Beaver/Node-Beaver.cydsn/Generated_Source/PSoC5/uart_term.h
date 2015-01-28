/*******************************************************************************
* File Name: uart_term.h  
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

#if !defined(CY_PINS_uart_term_H) /* Pins uart_term_H */
#define CY_PINS_uart_term_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "uart_term_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 uart_term__PORT == 15 && ((uart_term__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    uart_term_Write(uint8 value) ;
void    uart_term_SetDriveMode(uint8 mode) ;
uint8   uart_term_ReadDataReg(void) ;
uint8   uart_term_Read(void) ;
uint8   uart_term_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define uart_term_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define uart_term_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define uart_term_DM_RES_UP          PIN_DM_RES_UP
#define uart_term_DM_RES_DWN         PIN_DM_RES_DWN
#define uart_term_DM_OD_LO           PIN_DM_OD_LO
#define uart_term_DM_OD_HI           PIN_DM_OD_HI
#define uart_term_DM_STRONG          PIN_DM_STRONG
#define uart_term_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define uart_term_MASK               uart_term__MASK
#define uart_term_SHIFT              uart_term__SHIFT
#define uart_term_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define uart_term_PS                     (* (reg8 *) uart_term__PS)
/* Data Register */
#define uart_term_DR                     (* (reg8 *) uart_term__DR)
/* Port Number */
#define uart_term_PRT_NUM                (* (reg8 *) uart_term__PRT) 
/* Connect to Analog Globals */                                                  
#define uart_term_AG                     (* (reg8 *) uart_term__AG)                       
/* Analog MUX bux enable */
#define uart_term_AMUX                   (* (reg8 *) uart_term__AMUX) 
/* Bidirectional Enable */                                                        
#define uart_term_BIE                    (* (reg8 *) uart_term__BIE)
/* Bit-mask for Aliased Register Access */
#define uart_term_BIT_MASK               (* (reg8 *) uart_term__BIT_MASK)
/* Bypass Enable */
#define uart_term_BYP                    (* (reg8 *) uart_term__BYP)
/* Port wide control signals */                                                   
#define uart_term_CTL                    (* (reg8 *) uart_term__CTL)
/* Drive Modes */
#define uart_term_DM0                    (* (reg8 *) uart_term__DM0) 
#define uart_term_DM1                    (* (reg8 *) uart_term__DM1)
#define uart_term_DM2                    (* (reg8 *) uart_term__DM2) 
/* Input Buffer Disable Override */
#define uart_term_INP_DIS                (* (reg8 *) uart_term__INP_DIS)
/* LCD Common or Segment Drive */
#define uart_term_LCD_COM_SEG            (* (reg8 *) uart_term__LCD_COM_SEG)
/* Enable Segment LCD */
#define uart_term_LCD_EN                 (* (reg8 *) uart_term__LCD_EN)
/* Slew Rate Control */
#define uart_term_SLW                    (* (reg8 *) uart_term__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define uart_term_PRTDSI__CAPS_SEL       (* (reg8 *) uart_term__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define uart_term_PRTDSI__DBL_SYNC_IN    (* (reg8 *) uart_term__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define uart_term_PRTDSI__OE_SEL0        (* (reg8 *) uart_term__PRTDSI__OE_SEL0) 
#define uart_term_PRTDSI__OE_SEL1        (* (reg8 *) uart_term__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define uart_term_PRTDSI__OUT_SEL0       (* (reg8 *) uart_term__PRTDSI__OUT_SEL0) 
#define uart_term_PRTDSI__OUT_SEL1       (* (reg8 *) uart_term__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define uart_term_PRTDSI__SYNC_OUT       (* (reg8 *) uart_term__PRTDSI__SYNC_OUT) 


#if defined(uart_term__INTSTAT)  /* Interrupt Registers */

    #define uart_term_INTSTAT                (* (reg8 *) uart_term__INTSTAT)
    #define uart_term_SNAP                   (* (reg8 *) uart_term__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_uart_term_H */


/* [] END OF FILE */
