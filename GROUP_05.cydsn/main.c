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
#include "project.h"
#include "Interrupt_Routines.h"

#define SLAVE_BUFFER_SIZE 7
uint8_t slaveBuffer[SLAVE_BUFFER_SIZE]; // Buffer for the slave device

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    //set Control Register 0: 5 samples 
    slaveBuffer[0]=0b00010100;
    
    //set Control Register 1: Timer Period equal to 10
    slaveBuffer[1]=0b00001010;
    
    //set WHO I AM
    slaveBuffer[2]=0xBC;
    
    //set 3,4,5,6 slaveBuffer
    slaveBuffer[3]=0;
    slaveBuffer[4]=0;
    slaveBuffer[5]=0;
    slaveBuffer[6]=0;
    
    // Set up EZI2C buffer
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, 2 ,slaveBuffer);
    
    for(;;)
    {
        if((slaveBuffer[0] & 0b00000011)==3){
            Pin_LED_Write(1);      
        }
        else{
            Pin_LED_Write(0);
        }
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
