/*
* This example use the Robotics Cape library includes simple GPio command for exporting
*  reading and writing GPIO
*/

/*
* gpio_library_example.c
* illuminates an LED connected to a gpio pin with gpio library funcion
* use expansion header P8 pin 8
* GPIO2_3 degisnates as gpio 67
*/

#include <robotics_cape.h>

#define GPIO_PIN 67

int main(int argc, char const *argv[])
{
	/* export gpio pin for use */
	if(gpio_export(GPIO_PIN))
	{
		printf("Unable to open export.\n");
		return -1;
	}

	/* set pin for output */
	if(gpio_set_dir(GPIO_PIN, OUTPUT_PIN))
	{
		printf("Unable to open gpio67_direction.\n");
		return -1;
	}

	/* start blinking loop */

	int count = 0;
	while(count < 10)
	{
		/* turn on */
		gpio_set_value(GPIO_PIN, 1);
		printf("ON\n");
		sleep(1);

		/* turn off*/
		gpio_set_value(GPIO_PIN, 0);
		printf("OFF\n");
		sleep(1);

		/* increment counter */
		i++;
	}

	return 0;
}

/*
* To compile, you need to have the Robotics Cape package install and tell
* GCC linker to include the robotics cape library.
*
* gcc gpio_library_example.c -lrobotics_cape -o gpio
*
*/