#include "mraa/aio.h"
#include <math.h>

const int B = 4275;
const int R0 = 100000;
const int pinTempSensor = A0; 


int main()
{
  mraa_aio_context adc_a0;
  uint16_t adc_value = 0;
  float adc_value_float = 0.0;
  adc_a0 = mraa_aio_init(0);
  if (adc_a0 == NULL) {
    return 1;
  }
  for (;;) {
    int a = analogRead(pinTempSensor);
    adc_value = mraa_aio_read(adc_a0);

    float R = 1023.0/adc_value-1.0;
    R = R0*R;
    float temp = 1.0/(log(R/R0)/B+1/298.15)-273.15;
    fprintf(stdout, "temperature = %d \n", temp); 
    //    adc_value_float = mraa_aio_read_float(adc_a0);
    //    fprintf(stdout, "ADC A0 read %X - %d\n", adc_value, adc_value);
    //    fprintf(stdout, "ADC A0 read float - %.5f\n", adc_value_float);
  }
  mraa_aio_close(adc_a0);
  return MRAA_SUCCESS;
}
