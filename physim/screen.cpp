#include "screen.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <cstdlib>

using namespace physim;
namespace rawscreen{
	int fbfd = 0;
	struct fb_var_screeninfo orig_vinfo;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	long int screensize = 0;
	char *fbp = 0;
}
using namespace rawscreen;

void rawScreen::plot(int x, int y,int sx,int sy, unsigned int col){
				// calculate the pixel's byte offset inside the buffer
			// see the image above in the blog...
			unsigned int pix_offset = (x <<2)+ y * finfo.line_length ;

			// now this is about the same as fbp[pix_offset] = value
			*((int*)(fbp + pix_offset)) =col;


}
void rawScreen::paint(){}
rawScreen::rawScreen(){
	// Open the file for reading and writing
	fbfd = open("/dev/fb0", O_RDWR);
	if (!fbfd) {
		printf("Error: cannot open framebuffer device.\n");
		exit( 666);
	}
	printf("The framebuffer device was opened successfully.\n");

	// Get variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("Error reading variable information.\n");
		exit( 666);
	}
	printf("Original %dx%d, %dbpp\n", vinfo.xres, vinfo.yres, 
			vinfo.bits_per_pixel );

	// Store for reset (copy vinfo to vinfo_orig)
	memcpy(&orig_vinfo, &vinfo, sizeof(struct fb_var_screeninfo));

	// Change variable info
	vinfo.bits_per_pixel = 32;
	if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo)) {
		printf("Error setting variable information.\n");
		exit( 666);
	}

	// Get fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		printf("Error reading fixed information.\n");
		exit( 666);
	}

	// map fb to user mem 
	screensize = finfo.smem_len;
	fbp = (char*)mmap(0, 
			screensize, 
			PROT_READ | PROT_WRITE, 
			MAP_SHARED, 
			fbfd, 
			0);

	if ((int)fbp == -1) {
		printf("Failed to mmap.\n");
		exit (666);
	}
	
}
rawScreen::~rawScreen(){

	// cleanup
	munmap(fbp, screensize);
	if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_vinfo)) {
	  printf("Error re-setting variable information.\n");
	 }
	close(fbfd);


}
