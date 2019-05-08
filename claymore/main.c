#include <stdio.h>
#include <stdlib.h>
#include <BBBiolib.h>
#include <signal.h>

void sig_handler(int signo);
void initialize_pwm();
void motion();
void manual();

void main()
{
    if (signal(SIGINT, sig_handler) == SIG_ERR)
	    printf("\ncan't catch SIGINT\n");
    else
	    printf("Successfully set up SIGINT handler\n");
    
    iolib_init();
	initialize_pwm();
    
    iolib_setdir(9,15, BBBIO_DIR_OUT);
    iolib_setdir(9,12, BBBIO_DIR_IN);
    
    while (1)
    {
        FILE *fp;
    	int buff;
       	fp = fopen("/var/lib/cloud9/claymore/Method.txt", "r");
	    fscanf(fp, "%d", &buff);
		fclose(fp);
		int method = buff;
        if (method == 0)
        {
            manual();
        }
        else if (method == 1)
        {
            motion();
        }
        usleep(1000);
    }
}


void sig_handler(int signo)
{
	system("echo 0 > /sys/class/pwm/pwmchip2/pwm0/duty_cycle");
	pin_low(9,15);
	iolib_free();
	exit(0);
}


void motion()
{
    int count = 0;
    int even = 1;
    
    if (is_high(9,12))
    {
        while (count < 200)
        {
            if (even == 1)
            {
                pin_high(9,15);
                system("echo 0 > /sys/class/pwm/pwmchip2/pwm0/duty_cycle");
                even = 0;
            }
            else if (even == 0)
            {
                pin_low(9,15);
                system("echo 350000 > /sys/class/pwm/pwmchip2/pwm0/duty_cycle");
                even = 1;
            }
            count ++;
            usleep(50000);
            pin_low(9,15);
            system("echo 0 > /sys/class/pwm/pwmchip2/pwm0/duty_cycle");
        }
        FILE *fp;
    	int buff;
       	fp = fopen("/var/lib/cloud9/claymore/Method.txt", "w");
        fputc('0', fp);
    	fclose(fp);
    }
}


void manual()
{
    FILE *fp;
	int buff;
   	fp = fopen("/var/lib/cloud9/claymore/Trigger.txt", "r");
    fscanf(fp, "%d", &buff);
	fclose(fp);
	
	int boomboom = buff;
    int count = 0;
    int even = 1;
	
	if (boomboom == 1)
	{
        while (count < 200)
        {
            if (even == 1)
            {
                pin_high(9,15);
                system("echo 0 > /sys/class/pwm/pwmchip2/pwm0/duty_cycle");
                even = 0;
            }
            else if (even == 0)
            {
                pin_low(9,15);
                system("echo 350000 > /sys/class/pwm/pwmchip2/pwm0/duty_cycle");
                even = 1;
            }
            count ++;
            usleep(50000);
            pin_low(9,15);
            system("echo 0 > /sys/class/pwm/pwmchip2/pwm0/duty_cycle");
        }
	}
   	fp = fopen("/var/lib/cloud9/claymore/Trigger.txt", "w");
    fputc('0', fp);
	fclose(fp);
}


void initialize_pwm()
{
   FILE *fp;
   char *mode = "r+b";

   // Configure P9_14 as a PWM pin
   system("config-pin P9.14 pwm");

   // Wait for 10ms to allow changes to take effect
   usleep(10000);

   // Check to see if the PWM0 directory has been created by attempting
   // to open the pwm0/enable file.  If the file exists, then you should
   // not echo 0 to the export file.  If it does not exist, then you need
   // to echo 0 to the export file to create the pwm0 directory
   fp = fopen("/sys/class/pwm/pwmchip2/pwm0/enable", mode);

   if (fp == NULL)
   {
      printf("Creating PWM0 for the first time\n");
      system("echo 0 > /sys/class/pwm/pwmchip2/export");
      // Wait for 10ms to allow changes to take effect
      usleep(10000);
   }
   else
   {
      fclose(fp);
   }

   // Turn the pwm on
          system("echo 1 > /sys/class/pwm/pwmchip2/pwm0/enable");
   // Wait for 10ms to allow changes to take effect
   usleep(10000);

   // Set the PWM period to 500000
   system("echo 500000 > /sys/class/pwm/pwmchip2/pwm0/period");
   // Wait for 10ms to allow changes to take effect
   usleep(10000);
}