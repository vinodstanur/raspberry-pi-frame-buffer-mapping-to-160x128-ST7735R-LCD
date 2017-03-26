#include "gpio.h"
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>


volatile unsigned int *gpio = MAP_FAILED;


void gpio_mode_output(int pin)
{
				int x;

				int p = pin;
				while(p > 9) {
								p -= 10;
				}
				
				gpio[GPIO_GPFSEL0 + pin/10] |=  1 << (p*3); //make as output
				//gpio[GPIO_GPFSEL0 + pin/10] &= ~( 1 << (p*3) ); //use this to make input. 
}


void gpio_set_pin(int pin)
{
				gpio[GPIO_GPSET0 + pin/32] |= 1 << (pin - (pin/32)*32);
}

void gpio_clear_pin(int pin)
{	
			gpio[GPIO_GPCLR0 + pin/32] |= 1 << (pin - (pin/32)*32);
}

void gpio_init(void)
{
				int memfd = -1;

				if((memfd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
								fprintf(stderr, "/dev/mem open error\n");
								return;
				}

				gpio = mmap(NULL, 4096, (PROT_READ | PROT_WRITE), MAP_SHARED, memfd, 0x20200000);
				if(gpio == MAP_FAILED) {
								fprintf(stderr, "mmap failed\n");
								return;
				}
				printf("gpio mmap value = %x\n", gpio);



//				gpio = (unsigned int *)GPIO_BASE;
}
