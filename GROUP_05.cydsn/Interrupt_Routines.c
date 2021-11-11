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
#include "Interrupt_Routines.h"
#include "project.h"

#define NO_SAMPLING 0b00000000
#define SAMPLING_TEMPERATURE 0b00000001
#define SAMPLING_LDR 0b00000010
#define SAMPLING_BOTH 0b00000011

#define MASK_STATUS_BIT0 0b00000001
#define MASK_STATUS_BIT1 0b00000010
#define MASK_STATUS_BITS 0b00000011

#define MASK_N_SAMPLES 0b00111100

extern uint8_t slaveBuffer[];
uint32 value_digit_TEMP=0;
uint32 value_digit_LDR=0;
float value_mv_TEMP=0;
float value_mv_LDR=0;
uint16 value_TEMP=0;
float R_LDR=0;
uint16 value_Lux=0;
uint8 n_cycles=0;
uint8 n_samples=0;
int16 TEMP_avg=0;
uint16 LDR_avg=0;


CY_ISR(Custom_ISR_ADC)
{
    Timer_ISR_ReadStatusRegister();
    
    if (n_samples==0){
        n_samples = (slaveBuffer[0] & MASK_N_SAMPLES)>>2;
        Timer_ISR_WritePeriod(slaveBuffer[1]);
    }
    
    else{
    
    // Temperature sampling (if active)
    if((slaveBuffer[0] & MASK_STATUS_BIT0)==SAMPLING_TEMPERATURE){
        AMux_Select(0);
        value_digit_TEMP=ADC_DelSig_Read32();
        value_mv_TEMP=ADC_DelSig_CountsTo_mVolts(value_digit_TEMP);
        value_TEMP+=0.1*value_mv_TEMP-50; //conversion from mV to Celsius Degrees (calculated from datasheet)
    }
    
    // LDR sampling (if active)
    if((slaveBuffer[0] & MASK_STATUS_BIT1)==SAMPLING_LDR){
        AMux_Select(1);
        value_digit_LDR=ADC_DelSig_Read32();
        value_mv_LDR=ADC_DelSig_CountsTo_mVolts(value_digit_LDR);
        R_LDR=10*((5000/value_mv_LDR)-1); //conversion from mV to resistance in kOhm (due to the voltage divider)
        value_Lux+=(-100*R_LDR+10000); //conversion from kOhm to lux (calculated from datasheet)
    }
    
    n_cycles++;  //number of times that the ISR has occurred
    if (n_cycles==n_samples){
        n_cycles=0;
        
        //if the temperature sampling is active, we calculate the mean
        if((slaveBuffer[0] & MASK_STATUS_BIT0) == SAMPLING_TEMPERATURE){
            TEMP_avg=value_TEMP/n_samples;
            slaveBuffer[3]=TEMP_avg>>8;
            slaveBuffer[4]=TEMP_avg & 0XFF;
            value_TEMP=0;
        }

        //if the LDR sampling is active, we calculate the mean
        if((slaveBuffer[0] & MASK_STATUS_BIT1) == SAMPLING_LDR){
            LDR_avg=value_Lux/n_samples;
            slaveBuffer[5]=LDR_avg>>8;
            slaveBuffer[6]=LDR_avg & 0XFF;
            value_Lux=0;
        }
        
        /* The user can change both the number of samples and the Timer Period (ISR period of sampling).
        Updating the values of the control register (number of samples and timer period, changed
        by the user).
        If the number of samples inserted by the user, is not coherent with the period inserted (or
        the period inserted is too short to sample that number of samples, in order to send data 
        at 50 Hz), we set the period as the minimum period between the two.*/
        
        n_samples = (slaveBuffer[0] & MASK_N_SAMPLES)>>2;
        Timer_ISR_WritePeriod(slaveBuffer[1]);
        
        if((1000/(50*n_samples)<slaveBuffer[1])){
            Timer_ISR_WritePeriod(1000/(50*n_samples));
            
        /*
            Timer_ISR_WritePeriod(1000/slaveBuffer[1]*n_samples)); //if slaveBuffer[1] contains the transmission frequency
        */
        }  
        
    }
    
 }
}

/* [] END OF FILE */
