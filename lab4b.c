#include "mraa.h"
#include <math.h>
#include <time.h> 

const int B = 4275;
const int R0 = 100;
const int button_pin = 3; 

int main()
{
  mraa_aio_context adc_a0;
  mraa_gpio_context gpio;

  uint16_t adcValue = 0;
  float adc_value_float = 0.0;

  adc_a0 = mraa_aio_init(0);

  if (adc_a0 == NULL) {
    return 1;
  }

  gpio = mraa_gpio_init(button_pin); 
  mraa_gpio_dir(gpio, MRAA_GPIO_IN);

  time_t curtime;
  struct tm *loctime;

  for (;;) {

    /* Calculate temperature reading */ 
    adcValue = mraa_aio_read(adc_a0);
    float R;
    R = 1023.0/((float)adcValue)-1.0;
    R = 100000.0*R;
    float temp  = 1.0/(log(R/100000.0)/B+1/298.15)-273.15;

    /* button reading */ 
    int button_value = mraa_gpio_read(gpio); 

    /* Local Time */ 
    curtime = time (NULL);
    loctime = localtime (&curtime);


    /* print logs  */
    printf ("The temperature is %0.2f degree Celcius\n", temp);
    fprintf(stdout, "Gpio is %d\n", button_value);
    fprintf(stdout, "%s \n",(asctime (loctime))); 

  }
  mraa_aio_close(adc_a0);
  return MRAA_SUCCESS;
}
