//////////////////////////////////////////////////////////////////////
/*
 * main.c
 * http://blog.vinu.co.in
 * <mail@vinu.co.in>
 *brief: Mapping /dev/fb0 to 160x128 16 bit TFT st7735r from user space.
 *Cpu usage: 4.5 to 5% APROX at 20fbs
//////////////////////////////////////////////////////////////////////
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>*/
///////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <inttypes.h>
#include <stdlib.h>
#include "st7735.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "spi.h"
#include "gpio.h"
#include <endian.h>


//#define debug


uint16_t fb2[128 * 160];

int
main ()
{
  int fbfd = 0;

  struct fb_var_screeninfo vinfo;
  long int screensize = 0;
  uint8_t *fbp = 0;
  int i;
  uint16_t *p = fb2;
  int xx = 0;
  int yy = 0;

  fbfd = open ("/dev/fb0", O_RDWR);

  ST7735_Init ();

  ST7735_Orientation (1);

  memset (fb2, 0, sizeof (fb2));

  //clear frame buffer
  CS_L ();
  ST7735_AddrSet (0, 0, 160, 128);
  A0_H ();
  for (i = 0; i < 10; i++)
    {
      spi_write (0, (char *) p, 4096);
      p += 4096 / 2;
    }


  while (1)
    {
      usleep (50000);

      ioctl (fbfd, FBIOGET_VSCREENINFO, &vinfo);
#ifdef debug
      printf ("X = %d, Y = %d\n", vinfo.xres, vinfo.yres);
#endif
      CS_L ();

      ST7735_AddrSet (0, 4, 160, 124);

      A0_H ();

      int xoffset = vinfo.xres / 160;
      int yoffset = vinfo.xres * (vinfo.yres / 120 - 1);
#ifdef debug
      printf ("xoffset = %d, yoffset = %d\n", xoffset, yoffset);
#endif

      screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
#ifdef debug
      printf ("screensize = %d\n", screensize);
#endif
      const int PADDING = 4096;
      int mmapsize = (screensize + PADDING - 1) & ~(PADDING - 1);
#ifdef debug
      printf ("mapsize = %d\n", mmapsize);
#endif
      fbp =
	(char *) mmap (0, mmapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd,
		       0);

      uint16_t *p = fb2;
      uint16_t *x = (uint16_t *) fbp;
      for (xx = 0; xx < 120; xx++)
	{
	  for (yy = 0; yy < 160; yy++)
	    {
	      *p++ = htons (*x);
	      x += xoffset;
	    }
	  x += yoffset;
	}
      munmap (fbp, screensize);

      p = fb2;

      for (i = 0; i < 9; i++)
	{
	  spi_write (0, (char *) p, 4096);
	  p += 4096 / 2;
	}

      spi_write (0, (char *) p, 120 * 160 * 2 - 4096 * 9);
    }

  CS_H ();
  printf ("done\n");

  close (fbfd);

  return 0;
}
