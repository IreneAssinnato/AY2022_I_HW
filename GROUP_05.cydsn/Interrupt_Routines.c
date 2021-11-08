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
uint32 value_digit_TEMP;
uint32 value_digit_LDR;
uint16 value_mv_TEMP; //FLOAT???
uint16 value_mv_LDR;//FLOAT???
int8 n_samples=0;
uint16 TEMP_avg;
uint16 LDR_avg;
char Transmission_Flag;


 
CY_ISR(Custom_ISR_ADC)
{
    /*if(Transmission_Flag==1){
       //INVIA I2C
        
        Transmission_Flag=0;
    }
    */
    
    //else{
            if(n_samples<5){
                Timer_ISR_ReadStatusRegister();
    
                if((slaveBuffer[0] & 0b00000001)==1){
                    AMux_Select(0);
                    value_digit_TEMP=ADC_DelSig_Read32();
                    //if (value_digit_TEMP<0)  value_digit_TEMP=0;
                    //if (value_digit_TEMP>65535)  value_digit_TEMP=65535;
                    value_mv_TEMP+=ADC_DelSig_CountsTo_mVolts(value_digit_TEMP);
                    //AGGIUNGERE CONVERSIONE DA MV A R E DA R A TEMPERATURA
                }
    
                if((slaveBuffer[0] & 0b00000010)==2){
                    AMux_Select(1);
                    value_digit_LDR=ADC_DelSig_Read32();
                    if (value_digit_LDR<0)  value_digit_LDR=0;
                    if (value_digit_LDR>65535)  value_digit_LDR=65535;
                    value_mv_LDR+=ADC_DelSig_CountsTo_mVolts(value_digit_LDR);
                    //AGGIUNGERE CONVERSIONE DA MV A R E DA R A INTENSITà LUCE
                }
    
                n_samples++;
                if (n_samples==5){
                    n_samples=0;
                    
                    TEMP_avg=value_mv_TEMP/5;
                    slaveBuffer[3]=TEMP_avg>>8;
                    slaveBuffer[4]=TEMP_avg & 0XFF;

                    LDR_avg=value_mv_LDR/5;
                    slaveBuffer[5]=LDR_avg>>8;
                    slaveBuffer[6]=LDR_avg & 0XFF;
                    
                    value_mv_TEMP=0;
                    value_mv_LDR=0;
                    //Transmission_Flag=1;
                }
            }
        //}

}

/* [] END OF FILE */
