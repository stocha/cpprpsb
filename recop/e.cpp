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
const bool loop=true;
const int nbRayPerImage=20;
const int matsz=300;
const int decImage=1800-matsz;
// application entry point
;

class calcSimple{
	private :
		const int sz;
	public :
		vector<unsigned int> dat;
		vector<unsigned int> sec;
		calcSimple(int sz) : sz(sz),dat(sz*sz),sec(sz*sz){

			for(int i=0;i<sz*sz;i++){
				dat[i]=0;
			}
	//		set(sz/2+100,sz/2,1<<31);
	//		set(sz/2-100,sz/2,1<<31);
		//j	set(sz/2,sz/2-50,1<<31);

			doit();
		//	set(sz/2+100,sz/2,1<<31);
			//set(sz/2-100,sz/2,1<<31);
			//set(sz/2,sz/2-50,1<<31);

		};	

		void decStar(){
			for(int i=0;i<sz*sz;i++){
				if(dat[i]>0)
					dat[i]=dat[i]*1000/1001;
			}
		}

		unsigned int get(int x,int y){

			return dat[y*sz + x];
		}
		int getOffset(int x, int y) {return y*sz+x;}
		void set(int x,int y,unsigned int v){
			dat[y*sz + x]=v;
		}
		void doit(){
			for( int i=0;i<sz*sz;i++) sec[i]=dat[i]; 
			for(int x=1;x<sz-1;x++) for (int y=1;y<sz-1;y++){
				int i=getOffset(x,y);
				dat[i]=sec[i]&3;
					
				int up=getOffset(x,y+1);
				int down=getOffset(x,y-1);	
				int left=getOffset(x-1,y);
				int right=getOffset(x+1,y);

				dat[i]+=sec[up]>>2;
				dat[i]+=sec[down]>>2;
				dat[i]+=sec[left]>>2;
				dat[i]+=sec[right]>>2;
						
			}			
			set(sz/2+100,sz/2,1<<16);
			
			set(sz/2-100,sz/2,1<<16);
			set(sz/2,sz/2-50,1<<16);
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

unsigned int liss(unsigned int src){
	int ro=0;
	int ve=0;
	int bl=0;	
	int born=(1<<8);
	int pborn=0;
	if(src<born) { ro=src; return pix(ro,ve,bl);}	
	pborn=born;
	born=(1<<16);
	if(src<born) { 
		ve=src>>8;
		ro=pborn-ve;
		return pix(ro,ve,bl);
	}
	born=(1<<24);
	if(src<born) { 
		bl=src>>16;
		ve=pborn-bl;
		return pix(ro,ve,bl);	
	};
	born=(1<<30);
	if(src<born) { 
		//bl=src>>16;
		bl=255;
		ro=src>>24;
		ve=src>>24;
		return pix(ro,ve,bl);	
	};
	return pix(255,255,255);

		
			
			

	//if(rv<255) v=pix(rv,0,0);
//	else if(rv<255*3) v= pix(255,(rv-255 )/2,0);	
//	else if(rv<255*7) v= pix(255,255,(rv-255*3)/4);	
	//else if(rv<255*3) v= pix(rv-255*3,rv-255,rv-255*2);	
//	else if(rv>=255*7) v= pix(255,255,255);



}

int sz=matsz;
calcSimple cs(sz);
void plotOnePict(char* fbp, struct fb_fix_screeninfo& finfo){
	// draw...
	int x, y;
	unsigned int pix_offset;
	
	cs.mdo(nbRayPerImage);
	//	cs.debug();
	for (y = 0; y < sz; y++) {
		for (x = 0; x < sz; x++) {

			// calculate the pixel's byte offset inside the buffer
			// see the image above in the blog...
			pix_offset = decImage*4+x * 4+ y * finfo.line_length ;

			// now this is about the same as fbp[pix_offset] = value
			unsigned int rv=cs.get(x,y);
			unsigned int v=0;	

//			if(rv<255) v=pix(rv,0,0);
//			else if(rv<255*3) v= pix(255,(rv-255 )/2,0);	
//			else if(rv<255*7) v= pix(255,255,(rv-255*3)/4);	
//			//else if(rv<255*3) v= pix(rv-255*3,rv-255,rv-255*2);	
//			else if(rv>=255*7) v= pix(255,255,255);

			//	if(v>0) cout << v << " " << x << " " << y << endl;
//			unsigned int gray=v ;
			//if(v>255 ) gray=255;
//			unsigned int trv=(unsigned int)(rv^(rv>>1));
//			trv=(trv>>16 & 255 ) | (trv&(255<<8))| (trv<<16 )& ((255<<16));	
			*((int*)(fbp + pix_offset)) =liss(rv);

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
