#include "mraa/aio.h"
#include <math.h>

const int B = 4275;
const int R0 = 100;

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
    adcvalue = mraa_aio_read(adc_a0);
    float R;
    R = 1023.0/((float)adcValue)-1.0;    // Promote to float
    R = 100000.0*R;

    float temp  = 1.0/(log(R/100000.0)/B+1/298.15)-273.15;

    printf ("The temperature is %0.2f degree Celcius\n", temp);

  }
  mraa_aio_close(adc_a0);
  return MRAA_SUCCESS;
}
