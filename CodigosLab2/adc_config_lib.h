/* 
 * File:   adc_config_lib.h
 * Author: Christian Cruz 21759
 *
 * Created on July 18, 2023, 4:06 PM
 */

#ifndef ADC_CONFIG_LIB_H
#define	ADC_CONFIG_LIB_H

void adc_init(int channel);
int adc_read();
void adc_change_channel(int channel);
int adc_get_channel();

#endif	/* ADC_CONFIG_LIB_H */

