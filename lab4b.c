  #include "mraa.h"
#include <math.h>
#include <time.h> 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>

const int B = 4275;
const int R0 = 100;
const int button_pin = 3; 
int fp;
int make_reports=1; 
int celcius=1; // default: celcius; alt: F 

void shutdown()
{
  // log time and SHUTDOWN 
  time_t curtime;
  curtime = time (NULL);
  struct tm *tm_struct = localtime (&curtime);
  int hour = tm_struct -> tm_hour;
  int min = tm_struct -> tm_min;
  int sec = tm_struct -> tm_sec;

  fprintf(fp, "%d:%d:%d SHUTDOWN\n",hour, min, sec);
  exit(0); 
}

int main()
{
  mraa_aio_context adc_a0;
  mraa_gpio_context gpio;
  uint16_t adcValue = 0;
  float adc_value_float = 0.0;
  int period = 1;
  char* filename = "log.txt"; 
  adc_a0 = mraa_aio_init(0);

  if (adc_a0 == NULL) {
    return 1;
  }

  char* buffer;
  size_t bufsize = 32;
  size_t characters; 
  buffer = (char *)malloc(bufsize * sizeof(char));

  gpio = mraa_gpio_init(button_pin); 
  mraa_gpio_dir(gpio, MRAA_GPIO_IN);

  time_t curtime; 
  
  time_t start, end;
  double elapsed;

  struct pollfd fds;
  int ret; 
  fds.fd = 0; /* this is STDIN */
  fds.events = POLLIN;

  fp = open("log.txt", O_CREAT | O_WRONLY | O_NONBLOCK);

  while(!mraa_gpio_read(gpio)){ // while button is not pressed 

    /* poll for input */ 
    ret = poll(&fds, 1, 0);
    if(ret == 1)
      printf("Yep\n");
    else if(ret == 0)
      printf("No\n");
    else
      printf("Error\n");

    /* read input if there */
    if(fds.revents & POLLIN)
      {
	characters = getline(&buffer,&bufsize,stdin);
	//	printf("You typed: %s \n",buffer);

	if(strcmp(buffer, "OFF\n") == 0)
	  {
	    fprintf(stderr, "...OFF\n");
       	    dprintf(fp, "OFF\n");
	    shutdown();
	  }        
	else if(strcmp(buffer, "STOP\n") == 0)
	  {
	    make_reports = 0;
	    dprintf(fp,"STOP\n");
	    fprintf(stderr, "...STOP\n");
	  }
	else if(strcmp("START\n", buffer) == 0)
	  {
	    make_reports = 1 ;
	    fprintf(stderr, "...START\n"); 
	  }
	else if(strcmp(buffer, "SCALE=F\n") == 0)
	  {
	    celcius=0;
	    fprintf(stderr, "...SCALE=F\n"); 
	    dprintf(fp, "SCALE=F\n");
	  }
	else if(strcmp(buffer, "SCALE=C\n") == 0)
	  {
	    celcius=1;
	    fprintf(stderr, "...SCALE=C\n");
	    dprintf(fp, "SCALE=C\n");
	  }
	else if( strstr(buffer, "PERIOD=") != NULL ) 
	  {
	    //int index = strchr(buffer,"=")-buffer;

	    period = atoi(buffer+7);
	    fprintf(stderr, "...PERIOD=%d\n", period);
       	    dprintf(fp, "...PERIOD=\n");
	  }
	else
	  {
	    fprintf(stderr, "Error: input option not valid\n");
	    exit(1);
	  }

      } // end of poll if 

    /* Calculate temperature reading */ 
    adcValue = mraa_aio_read(adc_a0);
    float R;
    R = 1023.0/((float)adcValue)-1.0;
    R = 100000.0*R;
    float temp  = 1.0/(log(R/100000.0)/B+1/298.15)-273.15;

    /* Farenheit */ 
    if(celcius == 0)
      {
	temp = temp*(9.0/5.0) + 32; 
      }
    
    /* button reading */ 
    int button_value = mraa_gpio_read(gpio); 
    
    /* Local Time */ 
    
    curtime = time (NULL);
    struct tm *tm_struct = localtime (&curtime);
    int hour = tm_struct -> tm_hour; 
    int min = tm_struct -> tm_min; 
    int sec = tm_struct -> tm_sec; 
    

    /* print logs  */

    fprintf (stdout, "%0.2f\n", temp);
    //    fprintf(stdout, "Gpio is %d\n", button_value);
    fprintf(stdout, "%d:%d:%d \n",hour, min, sec); 
    
    dprintf (fp, "%0.2f, temp");
    //    fprintf(fp, "Gpio is %d\n", button_value);
    dprintf(fp, "%d:%d:%d \n",hour, min, sec);

    /* Delay time */ 
    time(&start);
    do {
      time(&end);
      elapsed = difftime(end, start);
    } while(elapsed < period);
  
  } // end of infinite for-loop 

  mraa_aio_close(adc_a0);

  close(fp);

  shutdown(); 

  return MRAA_SUCCESS;
}
