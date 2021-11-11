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

#define NO_SAMPLING 0b00000000
#define SAMPLING_TEMPERATURE 0b00000001
#define SAMPLING_LDR 0b00000010
#define SAMPLING_BOTH 0b00000011

#define MASK_STATUS_BITS 0b00000011

#define HIGH 1
#define LOW 0

uint8_t slaveBuffer[SLAVE_BUFFER_SIZE]; // Buffer for the slave device

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    EZI2C_Start();
    ADC_DelSig_Start();
    Timer_ISR_Start();
    isr_StartEx(Custom_ISR_ADC);
    
    //set Control Register 0: 5 samples RICORDARSI DI RIMETTERLA A 0!
    slaveBuffer[0]= 0; //0b00010111;
    
    //set Control Register 1: Timer Period equal to 4 RICORDARSI DI RIMETTERLA A 0!
    slaveBuffer[1]= 0; //0b00000100;
    
    //set WHO I AM
    slaveBuffer[2]= 0xBC;
    
    //set data byte (bits 3,4,5,6 slaveBuffer). Default: 0 
    slaveBuffer[3]=0;
    slaveBuffer[4]=0;
    slaveBuffer[5]=0;
    slaveBuffer[6]=0;
    
    // Set up EZI2C buffer
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, 2 ,slaveBuffer);
    
    for(;;)
    {
        //if the sampling is active for both channels, the LED is ON.
        if((slaveBuffer[0] & MASK_STATUS_BITS)== SAMPLING_BOTH){
            Pin_LED_Write(HIGH);      
        }
        else{
            Pin_LED_Write(LOW);
        }
    }
}

/* [] END OF FILE */
