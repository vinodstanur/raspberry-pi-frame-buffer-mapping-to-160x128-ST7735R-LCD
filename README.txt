This is a userspace code for mapping raspberry pi default frame buffer (/dev/fb0) to a tft with st7735 driver. CPU USAGE = 3.2 to 4.5% at 20fps.
FPS can be increased by modifying the usleep(50000) value in main.c and also modifying the spi speed in st7735.c. 


Usage:

	 enable spi for first time using "sudo raspi-config"
   
	 Then make this project
	 
	 make clean

	 make

	 sudo ./run


	 Now tty1 should be mapped to the LCD.. You can type on pi keyboad connected to usb/usb hub to see changes on display.

	 You can try ./video1.sh to play test1 video sample.
	 Similarly other examples (video2.sh, rtsp_test.sh etc)


	 You can try typing startx to start X GUI as well.


Visit http://blog.vinu.co.in for more details.

mail: m a i l @ v i n u . c o . i n 


THANKS 

:)
