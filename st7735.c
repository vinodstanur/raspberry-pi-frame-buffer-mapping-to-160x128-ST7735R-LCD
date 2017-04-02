/*
	http://blog.vinu.co.in
	basic driver for st7735r lcd.
	This is just to initialize the lcd and not for any rendering purpose.
	 
	 This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
		 but WITHOUT ANY WARRANTY; without even the implied warranty of
		 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
		 GNU General Public License for more details.

		 You should have received a copy of the GNU General Public License
		 along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include <stdio.h>
#include "st7735.h"
#include <inttypes.h>
#include <stdlib.h>
#include "spi.h"
#include "gpio.h"

uint16_t scr_width;
uint16_t scr_height;

void ST7735_write(uint8_t data)
{
	spi_write(0, &data, 1);
	//wiringPiSPIDataW(0, &data, 1) ;
}

void ST7735_cmd(uint8_t cmd)
{
	A0_L();
	ST7735_write(cmd);
}

void ST7735_data(uint8_t data)
{
	A0_H();
	ST7735_write(data);
}

uint16_t RGB565(uint8_t R, uint8_t G, uint8_t B)
{
	return ((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3);
}

void ST7735_Init(void)
{

	//init spi
	printf("ST7735_Init...\n");
	//wiringPiSetup ();
	gpio_init();
	spi_init(0, 30000000, 0);

	gpio_mode_output(CS_PIN);
	gpio_mode_output(A0_PIN);
	gpio_mode_output(RST_PIN);
	gpio_mode_output(23);
	gpio_mode_output(17);

	//toggle test for debugging
	/*while(1) {
//	   CS_H();
//	   CS_L();

//	   A0_H();
//	   A0_L();

	   RST_H();
		usleep(1000);
	   RST_L();
		usleep(1000);

	   }
*/
	 
	// Reset display
	CS_H();
	RST_H();
	Delay_US(5);
	RST_L();
	Delay_US(5);
	RST_H();
	CS_H();
	Delay_US(5);
	CS_L();

	ST7735_cmd(0x11);	// Sleep out, booster on
	Delay_US(20);

	ST7735_cmd(0xb1);	// In normal mode (full colors):
	A0_H();
	ST7735_write(0x05);	//   RTNA set 1-line period: RTNA2, RTNA0
	ST7735_write(0x3c);	//   Front porch: FPA5,FPA4,FPA3,FPA2
	ST7735_write(0x3c);	//   Back porch: BPA5,BPA4,BPA3,BPA2

	ST7735_cmd(0xb2);	// In idle mode (8-colors):
	A0_H();
	ST7735_write(0x05);	//   RTNB set 1-line period: RTNAB, RTNB0
	ST7735_write(0x3c);	//   Front porch: FPB5,FPB4,FPB3,FPB2
	ST7735_write(0x3c);	//   Back porch: BPB5,BPB4,BPB3,BPB2

	ST7735_cmd(0xb3);	// In partial mode + full colors:
	A0_H();
	ST7735_write(0x05);	//   RTNC set 1-line period: RTNC2, RTNC0
	ST7735_write(0x3c);	//   Front porch: FPC5,FPC4,FPC3,FPC2
	ST7735_write(0x3c);	//   Back porch: BPC5,BPC4,BPC3,BPC2
	ST7735_write(0x05);	//   RTND set 1-line period: RTND2, RTND0
	ST7735_write(0x3c);	//   Front porch: FPD5,FPD4,FPD3,FPD2
	ST7735_write(0x3c);	//   Back porch: BPD5,BPD4,BPD3,BPD2

	ST7735_cmd(0xB4);	// Display dot inversion control:
	ST7735_data(0x03);	//   NLB,NLC

	ST7735_cmd(0x3a);	// Interface pixel format
	//ST7735_data(0x03);    // 12-bit/pixel RGB 4-4-4 (4k colors)
	ST7735_data(0x05);	// 16-bit/pixel RGB 5-6-5 (65k colors)
	//ST7735_data(0x06);    // 18-bit/pixel RGB 6-6-6 (256k colors)

	ST7735_cmd(0x36);	// Memory data access control:
	// MY MX MV ML RGB MH - -
	//ST7735_data(0x08 + 2+4);    // Normal: Top to Bottom; Left to Right; RGB
	ST7735_data(0x8);	// Y-Mirror: Bottom to top; Left to Right; RGB
	//ST7735_data(0x40);    // X-Mirror: Top to Bottom; Right to Left; RGB
	//ST7735_data(0xc0);    // X-Mirror,Y-Mirror: Bottom to top; Right to left; RGB
	//ST7735_data(0x20);    // X-Y Exchange: X and Y changed positions
	//ST7735_data(0xA0);    // X-Y Exchange,Y-Mirror
	//ST7735_data(0x60);    // X-Y Exchange,X-Mirror
	//ST7735_data(0xE0);    // X-Y Exchange,X-Mirror,Y-Mirror

	ST7735_cmd(0x20);	// Display inversion off
	//ST7735_cmd(0x21);     // Display inversion on

	ST7735_cmd(0x13);	// Partial mode off

	ST7735_cmd(0x26);	// Gamma curve set:
	//ST7735_data(0x01);	// Gamma curve 1 (G2.2) or (G1.0)
	//ST7735_data(0x02);    // Gamma curve 2 (G1.8) or (G2.5)
	ST7735_data(0x04);    // Gamma curve 3 (G2.5) or (G2.2)
	//ST7735_data(0x08);    // Gamma curve 4 (G1.0) or (G1.8)

	ST7735_cmd(0x38);	// Display on
	ST7735_cmd(0x29);	// Display on

	CS_H();

	ST7735_Orientation(scr_normal);
	printf("ST7735_Init done!\n");
}

void ST7735_Orientation(uint8_t orientation)
{
	CS_L();
	ST7735_cmd(0x36);	// Memory data access control:
	switch (orientation) {
	case scr_CW:
		scr_width = scr_h;
		scr_height = scr_w;
		ST7735_data(0xa0);	// X-Y Exchange,Y-Mirror
		break;
	case scr_CCW:
		scr_width = scr_h;
		scr_height = scr_w;
		ST7735_data(0x60);	// X-Y Exchange,X-Mirror
		break;
	case scr_180:
		scr_width = scr_w;
		scr_height = scr_h;
		ST7735_data(0xc0);	// X-Mirror,Y-Mirror: Bottom to top; Right to left; RGB
		break;
	default:
		scr_width = scr_w;
		scr_height = scr_h;
		ST7735_data(0x00);	// Normal: Top to Bottom; Left to Right; RGB
		break;
	}
	CS_H();
}

void ST7735_AddrSet(uint16_t XS, uint16_t YS, uint16_t XE, uint16_t YE)
{
	ST7735_cmd(0x2a);	// Column address set
	A0_H();
	ST7735_write(XS >> 8);
	ST7735_write(XS);
	ST7735_write(XE >> 8);
	ST7735_write(XE);

	ST7735_cmd(0x2b);	// Row address set
	A0_H();
	ST7735_write(YS >> 8);
	ST7735_write(YS);
	ST7735_write(YE >> 8);
	ST7735_write(YE);

	ST7735_cmd(0x2c);	// Memory write
}

void ST7735_Clear(uint16_t color)
{
	uint16_t i;
	uint8_t CH, CL;

	CH = color >> 8;
	CL = (uint8_t) color;

	CS_L();
	ST7735_AddrSet(0, 0, scr_width - 1, scr_height - 1);
	A0_H();
	for (i = 0; i < scr_width * scr_height; i++) {
		ST7735_write(CH);
		ST7735_write(CL);
	}
	CS_H();
}
