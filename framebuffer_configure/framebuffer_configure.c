#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "mxcfb.h" // see ref  mx50_evk_rd3_linux.pdf from freescale in L2.6.35_11.04.01_ER_docs.tar.gz

typedef struct  {
	int fbfd;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	// mapped frame buffer address
	unsigned short int *fb; // 16 bit ie two pixels should prob change
	size_t fb_size; 
} fbdev;


int main()
{
	
	fbdev *fb;
	fb = malloc(sizeof(*fb));

	// Open the file for reading and writing
	if ((fb->fbfd = open("/dev/fb0", O_RDWR)) == -1) {
		printf("Error opening fb device");
		return -1;
	}
	
	// Get fixed screen information
	ioctl(fb->fbfd, FBIOGET_FSCREENINFO, &fb->finfo);


	ioctl(fb->fbfd, FBIOGET_VSCREENINFO, &fb->vinfo);


	// Configure Screen
	fb->vinfo.bits_per_pixel = 8;
	fb->vinfo.grayscale = 1; // 0 is landscape right handed, 3 is portrait
	fb->vinfo.rotate = 3;
    if (ioctl(fb->fbfd, FBIOPUT_VSCREENINFO, &fb->vinfo) == -1) {
		printf("Error configuring display");
		return -1;
	}
	return 0;
}
