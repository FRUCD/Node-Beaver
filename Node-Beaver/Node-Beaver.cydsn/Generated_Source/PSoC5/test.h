/*******************************************************************************
* File Name: test.h  
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

#if !defined(CY_PINS_test_H) /* Pins test_H */
#define CY_PINS_test_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "test_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 test__PORT == 15 && ((test__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    test_Write(uint8 value) ;
void    test_SetDriveMode(uint8 mode) ;
uint8   test_ReadDataReg(void) ;
uint8   test_Read(void) ;
uint8   test_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define test_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define test_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define test_DM_RES_UP          PIN_DM_RES_UP
#define test_DM_RES_DWN         PIN_DM_RES_DWN
#define test_DM_OD_LO           PIN_DM_OD_LO
#define test_DM_OD_HI           PIN_DM_OD_HI
#define test_DM_STRONG          PIN_DM_STRONG
#define test_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define test_MASK               test__MASK
#define test_SHIFT              test__SHIFT
#define test_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define test_PS                     (* (reg8 *) test__PS)
/* Data Register */
#define test_DR                     (* (reg8 *) test__DR)
/* Port Number */
#define test_PRT_NUM                (* (reg8 *) test__PRT) 
/* Connect to Analog Globals */                                                  
#define test_AG                     (* (reg8 *) test__AG)                       
/* Analog MUX bux enable */
#define test_AMUX                   (* (reg8 *) test__AMUX) 
/* Bidirectional Enable */                                                        
#define test_BIE                    (* (reg8 *) test__BIE)
/* Bit-mask for Aliased Register Access */
#define test_BIT_MASK               (* (reg8 *) test__BIT_MASK)
/* Bypass Enable */
#define test_BYP                    (* (reg8 *) test__BYP)
/* Port wide control signals */                                                   
#define test_CTL                    (* (reg8 *) test__CTL)
/* Drive Modes */
#define test_DM0                    (* (reg8 *) test__DM0) 
#define test_DM1                    (* (reg8 *) test__DM1)
#define test_DM2                    (* (reg8 *) test__DM2) 
/* Input Buffer Disable Override */
#define test_INP_DIS                (* (reg8 *) test__INP_DIS)
/* LCD Common or Segment Drive */
#define test_LCD_COM_SEG            (* (reg8 *) test__LCD_COM_SEG)
/* Enable Segment LCD */
#define test_LCD_EN                 (* (reg8 *) test__LCD_EN)
/* Slew Rate Control */
#define test_SLW                    (* (reg8 *) test__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define test_PRTDSI__CAPS_SEL       (* (reg8 *) test__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define test_PRTDSI__DBL_SYNC_IN    (* (reg8 *) test__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define test_PRTDSI__OE_SEL0        (* (reg8 *) test__PRTDSI__OE_SEL0) 
#define test_PRTDSI__OE_SEL1        (* (reg8 *) test__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define test_PRTDSI__OUT_SEL0       (* (reg8 *) test__PRTDSI__OUT_SEL0) 
#define test_PRTDSI__OUT_SEL1       (* (reg8 *) test__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define test_PRTDSI__SYNC_OUT       (* (reg8 *) test__PRTDSI__SYNC_OUT) 


#if defined(test__INTSTAT)  /* Interrupt Registers */

    #define test_INTSTAT                (* (reg8 *) test__INTSTAT)
    #define test_SNAP                   (* (reg8 *) test__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_test_H */


/* [] END OF FILE */
