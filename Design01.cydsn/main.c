/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    /* CyGlobalIntEnable; */ /* Uncomment this line to enable global interrupts. */
    PWM_1_Init();
    PWM_2_Init();
    Tick_Start();
    
    for(;;)
    {
        /* Place your application code here. */
        if(Tick_Status_Read())
            LED4_Write(~LED4_ReadDataReg());
    }
}

/* [] END OF FILE */
