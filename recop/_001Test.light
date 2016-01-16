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
//const int nbRay=4096*64;
const int nbRay=120;
const int dist=360;
const bool loop=true;
// application entry point
class ray{
	private :
		int vx=0;
		int vy=0;
		int toLive=0;
		const int sz=0;
	public :
		ray(int sz) : sz(sz){};
		void reset(int startx,int starty,int startLive){
			vx=startx;vy=starty;toLive=startLive;
		}	
		int x(){return vx;};
		int y(){return vy;};
		void doit(){
			int sx=-1;
			int sy=-1;
			if(rand()&1) sx=1;
			if(rand()&1) sy=1;
			vx+=sx;
			vy+=sy;
			--toLive;
			if(toLive<0) toLive=0;
			if(vx>sz-1) toLive=0;
			if(vx<1) toLive=0;
			if(vy>sz-1) toLive=0;
			if(vy<1) toLive=0;

		}
		bool dead(){return toLive==0;};
};

class calcSimple{
	private :
		const int sz;
		int nbRayAlive=0;
	public :
		vector<unsigned long long> dat;
		vector<ray> rays;
		calcSimple(int sz) : sz(sz),dat(sz*sz){

			for(int i=0;i<sz*sz;i++){
				dat[i]=0;
			}
		};	

		void decStar(){
			for(int i=0;i<sz*sz;i++){
				if(dat[i]>0)
					--dat[i];
			}
		}

		unsigned long long get(int x,int y){

			return dat[y*sz + x];
		}
		void doit(){
			if(nbRayAlive<nbRay){ 
				ray r(sz); 
				r.reset(sz/2,sz/2,dist);				
				rays.push_back(r);

			}	
			for(ray& r : rays){
				if(r.dead()){
					r.reset(sz/2,sz/2,dist);
					decStar();
				}
				++dat[r.x()+r.y()*sz];
				r.doit();

			}
		}	
		void mdo(int nb){
			for(int i=0;i<nb;++i){
				doit();
			}
		}
		void debug(){
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
void plotOnePict(char* fbp, struct fb_fix_screeninfo& finfo){
	// draw...
	int x, y;
	unsigned int pix_offset;

	int sz=900;
	calcSimple cs(sz);
	cs.doit();
	cs.mdo(nbRay);
	//	cs.debug();
	for (y = 0; y < sz; y++) {
		for (x = 0; x < sz; x++) {

			// calculate the pixel's byte offset inside the buffer
			// see the image above in the blog...
			pix_offset = 1524+x * 4+ y * finfo.line_length ;

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

	//sleep(0);

}
void drawit(char* fbp, struct fb_fix_screeninfo& finfo){
	do{
		plotOnePict(fbp,finfo);
	}while(loop);
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
		drawit(fbp,finfo);
	}

	// cleanup
	//munmap(fbp, screensize);
	//if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_vinfo)) {
	//  printf("Error re-setting variable information.\n");
	// }
	close(fbfd);

	return 0;

}
