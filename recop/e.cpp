#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <vector>
#include <iostream>
using namespace std;
// application entry point
class calcSimple{
private :
	int sz;
public :
	vector<unsigned long long> dat;
	calcSimple(int sz) : sz(sz),dat(sz*sz){
		for(int i=0;i<sz*sz;i++){
			dat[i]=0;
		}
	};	
	
	unsigned long long get(int x,int y){

		return dat[y*sz + x];
	}
	void doit(){
		int x=sz/2;
		int y=sz/2;	

		for(int i=0;i<sz*20;++i){
			++dat[x+y*sz];
			int sx=-1;
			int sy=-1;
			if(rand()&1) sx= 1;
			if(rand()&1) sy= 1;

			x+=sx;
			y+=sy;

			if(x>sz-1) break;
			if(x<1) break;
			if(y>sz-1) break;
			if(y<1) break;
		}
	
	}	
	void mdo(int nb){
		for(int i=0;i<nb;++i){
			doit();
		}
	}
	void debug(){
		for(int i=0;i<sz*sz;i++){
		}			
		for(int x=0;x<sz;x++) for(int y=0;y<sz;y++) {
			int v=get(x,y);
			if(v!=0) cout << v << endl;	
	}
	}

};
// pixel_asm
unsigned int pix(unsigned int r,unsigned int g,unsigned int b){
	
	r=r&255;
	g=g&255;
	b=b&255;
	//unsigned int rs=(r<<0);
	unsigned int rs=(r<<16)|(g<<8)|b;
	//unsigned int rs=(b<<8);
	//unsigned int rs=  -1;
	return rs;
}
unsigned int min (unsigned int a, unsigned int b){ if(a<b) return a; else return b;}
int main(int argc, char* argv[])
{
  int fbfd = 0;
  struct fb_var_screeninfo orig_vinfo;
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;
  long int screensize = 0;
  char *fbp = 0;


  // Open the file for reading and writing
  fbfd = open("/dev/fb0", O_RDWR);
  if (!fbfd) {
    printf("Error: cannot open framebuffer device.\n");
    return(1);
  }
  printf("The framebuffer device was opened successfully.\n");

  // Get variable screen information
  if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
    printf("Error reading variable information.\n");
  }
  printf("Original %dx%d, %dbpp\n", vinfo.xres, vinfo.yres, 
         vinfo.bits_per_pixel );

  // Store for reset (copy vinfo to vinfo_orig)
  memcpy(&orig_vinfo, &vinfo, sizeof(struct fb_var_screeninfo));

  // Change variable info
  vinfo.bits_per_pixel = 32;
 // vinfo.grayscale=1;
           // vinfo.red.offset    = 0;
            // vinfo.red.length    = 8;
 //            vinfo.green.offset  = 8;
 //            vinfo.green.length  = 8;
    //         vinfo.blue.offset   = 16;
 //            vinfo.blue.length   = 8;
 //            vinfo.transp.offset = 0;
 //            vinfo.transp.length = 0;
  if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo)) {
    printf("Error setting variable information.\n");
  }
  
  // Get fixed screen information
  if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
    printf("Error reading fixed information.\n");
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
  }
  else {
    // draw...
    int x, y;
    unsigned int pix_offset;

    int sz=900;
	calcSimple cs(sz);
	cs.doit();
	cs.mdo(1024);
//	cs.debug();
    for (y = 0; y < sz; y++) {
      for (x = 0; x < sz; x++) {

        // calculate the pixel's byte offset inside the buffer
        // see the image above in the blog...
        pix_offset = x * 4+ y * finfo.line_length ;

        // now this is about the same as fbp[pix_offset] = value
	unsigned long long rv=cs.get(x,y);
	unsigned int v=0;	

	if(rv<255) v=pix(rv,0,0);
	else if(rv<255*3) v= pix(255,(rv-255 )/2,0);	
	else if(rv<255*7) v= pix(255,255,(rv-255*3)/4);	
	//else if(rv<255*3) v= pix(rv-255*3,rv-255,rv-255*2);	
	else if(rv>=255*7) v= pix(255,255,255);
	
//	if(v>0) cout << v << " " << x << " " << y << endl;
	unsigned int gray=v ;
	//if(v>255 ) gray=255;
        *((int*)(fbp + pix_offset)) =v;

      }
    }

    sleep(1);
  }

  // cleanup
  munmap(fbp, screensize);
  if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_vinfo)) {
    printf("Error re-setting variable information.\n");
  }
  close(fbfd);

  return 0;
  
}
