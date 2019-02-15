/*
Source: BeagleBone Robotics.pdf

There are three GPIO subsystems in the AM334x processor in 
your BeagleNone black, and they are all controlled with one
GPIO driver.
Opening the Robotics Cape header pin table again, you will find that the
green led is connected to header pin 8. This corresponds to gpio2[3]
which mean gpio subsystem number 2 and channel 3 on that subsystem. This
information can be found in the header pin table. The GPIO driver designates
a single number to that pin for simplicity.
Since there are 32 pin per GPIO subsystem and 4 subsystem ( 0, 1, 2, 3)
we multiply the subsystem number by 32 and add the channel number. 
For this pin we have 2*32 + 3 = 67

The command line to the gpio driver directory and export the pin to enable it
 - root@beaglebone:~# cd /sys/class/gpio
 -root@beaglebone:/sys/class/gpio# echo 67 > export

Lets configure it for output and turn on the pin.
 - root@beaglebone:/sys/class/gpio/gpio67# echo out > direction
 -root@beaglebone:/sys/class/gpio/gpio67# echo 1 > value

Lets configure the gpio for input and read the value file
root@beaglebone:/sys/class/gpio/gpio67# echo in > direction
root@beaglebone:/sys/class/gpio/gpio67# cat value

*/

/*
* Controlling GPIO with C code
*/

/* 
* illuminates an LED connected to a gpio pin with only file GPIO
* use expansion header P8, pin 8
* GPIO2_3 designated as gpio 67
*/

#include <stdio.h>
#include <stddef.h>
#include <time.h>

#define GPIO_SUBSYSTEM 32

#define GPIO_PORT      2
#define GPIO_PIN       3

#define GPIO_CONTROLLED (GPIO_PORT*GPIO_SUBSYSTEM) + GPIO_PIN

int main()
{
	/* file handles */
	FILE *ofp_export;
	FILE *ofp_gpio67_value;
	FILE *ofp_gpio67_direction;

	/* export gpio pin for use */
	ofp_export = fopen("/sys/class/gpio/export", "w");
	if(ofp_export == NULL)
	{
		printf("[ERROR] Unable to open export\n");
		return -1;
	}
	fseek(ofp_export, 0, SEEK_SET); /* seek tot eh beginning of file */
	fprintf(ofp_export, "%d", GPIO_PIN); /* write the pin number to export */
	fflush(ofp_export); /* finish writing file */

	/* configure gpio for writing */
	ofp_gpio67_direction = fopen("/sys/class/gpio/gpio67/direction", "w");
	if(ofp_gpio67_direction == NULL)
	{
		printf("[ERROR] Unable to open gpio67_directon.\n");
		return -1;
	}

	fseek(ofp_gpio67_direction, 0, SEEK_SET); /* seek to beginning of file */
	fprintf(ofp_gpio67_direction, "out");     /* configure as output pin */
	fflush(ofp_gpio67_direction);             /* write file */

	/* open file pointer for writing the value */
	ofp_gpio67_value = fopen("/sys/class/gpio/gpio67/value", "w");
	if(ofp_gpio67_value == NULL)
	{
		printf("[ERROR] Unable to open gpio67_value.\n");
		return -1;
	}

	fseek(ofp_gpio67_value, 0, SEEK_SET);

	printf("Blinking LED\n");

	int count = 0;
	while(count < 10)
	{
		/* turn pin on */
		fprintf(ofp_gpio67_value, "%d", 1);
		fflush(ofp_gpio67_value);
		printf("ON\n");
		sleep(1);

		/* turn pin off */
		fprintf(ofp_gpio67_value, "%ds", 0);
		fflush(ofp_gpio67_value);
		printf("OFF\n");
		sleep(1);

		/* increase counter */
		count ++;
	}

	/*close all files */
	fclose(ofp_export);
	fclose(ofp_gpio67_value);
	fclose(ofp_gpio67_direction);

	return 0;
}

/*
* To compile, you can use GCC as follows 
*
* gcc gpio_file_io.c -o gpio
* Run the file by command: ./gpio
*/