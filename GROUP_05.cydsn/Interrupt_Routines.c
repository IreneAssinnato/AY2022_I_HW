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

int16 value_digit_TEMP;
int16 value_digit_LDR;
int16 value_mv_TEMP; //FLOAT???
int16 value_mv_LDR;//FLOAT???
int8 n_samples;
float TEMP_avg;
float LDR_avg;

 
CY_ISR(Custom_ISR_ADC)
{
    Timer_ISR_ReadStatusRegister();
    
    AMux_Select(0);
    value_digit_TEMP=ADC_DelSig_Read16();
    if (value_digit_TEMP<0)  value_digit_TEMP=0;
    if (value_digit_TEMP>255)  value_digit_TEMP=255;
    value_mv_TEMP+=ADC_DelSig_CountsTo_mVolts(value_digit_TEMP);
    //AGGIUNGERE CONVERSIONE DA MV A R E DA R A TEMPERATURA
    
    AMux_Select(1);
    value_digit_LDR=ADC_DelSig_Read16();
    if (value_digit_LDR<0)  value_digit_LDR=0;
    if (value_digit_LDR>255)  value_digit_LDR=255;
    value_mv_LDR+=ADC_DelSig_CountsTo_mVolts(value_digit_LDR);
    //AGGIUNGERE CONVERSIONE DA MV A R E DA R A INTENSITà LUCE
    
    n_samples++;
    if (n_samples==5){
        n_samples=0;
        TEMP_avg=value_mv_TEMP/5;
        LDR_avg=value_mv_LDR/5;
        value_mv_TEMP=0;
        value_mv_LDR=0;
    }
    sprintf(DataBuffer, "Sample: %ld mV\r\n", value_mv);
    PacketReadyFlag=1;
}

/* [] END OF FILE */
