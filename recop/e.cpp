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
const int sz=500;
const int nbRay=10;
const int nbStepFrame=100000;
const int dist=30*3*10;
class ray{
	private :
		int vx=0;
		int vy=0;
		int toLive=0;
		const int sz=0;
		int vintens=1;
		vector<int> xt;
		vector<int> yt;
	public :
		ray(int sz) : sz(sz){};
		void reset(int startx,int starty,int startLive,int startIntens){
			vx=startx;vy=starty;toLive=startLive;vintens=startIntens;
			if(startIntens<0) toLive=toLive*2;

			{ xt.clear();yt.clear();};
		}	
		int x(){return vx;};
		int y(){return vy;};
		int intens() {return vintens;}
		int size(){return xt.size();}
		int xat(int at){return xt[at];}
		int yat(int at){return yt[at];}
		void doit(){
			int sx=-1;
			int sy=-1;
			int r=rand()&3;
			if(r==0) {sx=1;sy=0;}
			if(r==1) {sx=0;sy=1;}
			if(r==2) {sx=-1;sy=0;}
			if(r==3) {sx=0;sy=-1;}
			xt.push_back(vx);
			yt.push_back(vy);
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
		int currRay=0;
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
					dat[i]=dat[i]*1000/1001;
			}
		}

		unsigned long long get(int x,int y){

			return dat[y*sz + x];
		}
		void doit(){
			if(nbRayAlive<nbRay){ 
				int dir=1;
				ray r(sz); 
				r.reset(sz/2,sz/2,dist,dir);				
				rays.push_back(r);

				++nbRayAlive;
			}	
			++currRay;
			if(currRay>=nbRayAlive){currRay=0;};
			ray& r=rays[currRay];
				if(r.dead()){
					int dstx=sz/2+30*2;
					int dsty=sz/2+10*2;
					bool cond=false;
					for(int i=0;i<r.size();++i){
					
						if(r.xat(i)==sz/2 +30*2 && r.yat(i)==sz/2 - 20*2) cond=true; 
						if(r.xat(i)==sz/2 -30*2 && r.yat(i)==sz/2 - 20*2) cond=true; 

					}
					if(cond==true) 	for(int i=0;i<r.size();i++){
								int ind=r.xat(i)+r.yat(i)*sz;
								if((dat[ind] >0) || (r.intens() >0))
									dat[ind]+=r.intens();
							}
					int dir=1;
					r.reset(sz/2,sz/2,dist,dir);
				}

				r.doit();
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
unsigned int liss(unsigned int src2){
//	return src2&((1<<24)-1);
	int src=src2;
//	if(src&1==1) src=0xFFFFFF;
//	if(src&2==2) src=0xFFFFFF;
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
}



	calcSimple cs(sz);
void plotOnePict(char* fbp, struct fb_fix_screeninfo& finfo){
	// draw...
	int x, y;
	unsigned int pix_offset;

	cs.mdo(nbStepFrame);
	//	cs.debug();
	for (y = 0; y < sz; y++) {
		for (x = 0; x < sz; x++) {

			// calculate the pixel's byte offset inside the buffer
			// see the image above in the blog...
			pix_offset = (sz)*4+x * 4+ y * finfo.line_length ;

			// now this is about the same as fbp[pix_offset] = value
			unsigned int rv=cs.get(x,y);
			*((int*)(fbp + pix_offset)) =liss(rv);

		}
	}


}


void drawit(char* fbp, struct fb_fix_screeninfo& finfo){
	do{
		plotOnePict(fbp,finfo);
	}while(true);
}
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


