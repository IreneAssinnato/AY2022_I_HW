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
extern uint8_t slaveBuffer[];
uint32 value_digit_TEMP=0;
uint32 value_digit_LDR=0;
uint16 value_mv_TEMP=0;
uint16 value_mv_LDR=0;
uint16 value_TEMP=0;
uint8 n_cycles=0;
uint8 n_samples=0;
int16 TEMP_avg=0;
uint16 LDR_avg=0;



 
CY_ISR(Custom_ISR_ADC)
{
    Timer_ISR_ReadStatusRegister();
           
    
    
    if((slaveBuffer[0] & 0b00000001)==1){
        AMux_Select(0);
        value_digit_TEMP=ADC_DelSig_Read32();
        if (value_digit_TEMP<0)  value_digit_TEMP=0;
        if (value_digit_TEMP>4294967295)  value_digit_TEMP=4294967295;
        value_mv_TEMP=ADC_DelSig_CountsTo_mVolts(value_digit_TEMP);
        value_TEMP+=0.1*value_mv_TEMP-50;
    }
    
    if((slaveBuffer[0] & 0b00000010)==2){
        AMux_Select(1);
        value_digit_LDR=ADC_DelSig_Read32();
        if (value_digit_LDR<0)  value_digit_LDR=0;
        if (value_digit_LDR>4294967295)  value_digit_LDR=4294967295;
        value_mv_LDR+=ADC_DelSig_CountsTo_mVolts(value_digit_LDR);
        //AGGIUNGERE CONVERSIONE DA MV A R E DA R A INTENSITà LUCE
    }
    
    n_cycles++;
    if (n_cycles==n_samples){
        n_cycles=0;
            
        if((slaveBuffer[0] & 0b00000001)==1){
            TEMP_avg=value_TEMP/n_samples;
            slaveBuffer[3]=TEMP_avg>>8;
            slaveBuffer[4]=TEMP_avg & 0XFF;
            value_TEMP=0;
        }

        if((slaveBuffer[0] & 0b00000010)==2){
            LDR_avg=value_mv_LDR/n_samples;
            slaveBuffer[5]=LDR_avg>>8;
            slaveBuffer[6]=LDR_avg & 0XFF;
            value_mv_LDR=0;
        }
        
        n_samples= (slaveBuffer[0] & 0b00111100)>>2;
        Timer_ISR_WritePeriod(slaveBuffer[1]);
        
        if((1000/(50*n_samples)<slaveBuffer[1])){
            Timer_ISR_WritePeriod(1000/(50*n_samples));
        }
       
        
    }
    
 }


/* [] END OF FILE */
