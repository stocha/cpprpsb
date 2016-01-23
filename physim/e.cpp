#include <vector>
#include <iostream>
#include "screen.h"
using namespace std;
using namespace physim;

const bool loop=true;
const int nbrayperimage=1;
const int matsz=300;
const int decimage=1800-matsz;
const bool donuts_shape=true;
const unsigned int maximage=0;//nbrayperimage*40;
// application entry point
;
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
	//return src2&((1<<24)-1);
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

class calcsimple{
	private :
		const int sz;
	public :
		int nbimage=0;
		vector<unsigned int> dat;
		vector<unsigned int> sec;
		void initstartdist(){
			int xmax=sz/3+sz/2;
		        int ymax=sz/3+sz/2;
			set(sz/5,sz/5,1<<26);
			set(xmax,ymax,1<<26);
	//		for(int i=sz/4;i<(sz/2+sz/4);i++)
	//			set(i,sz/2,1<<31);


		}
		calcsimple(int sz) : sz(sz),dat(sz*sz),sec(sz*sz){

			for(int i=0;i<sz*sz;i++){
				dat[i]=0;
			}
			initstartdist();
			doit();
			initstartdist();

		};	

		void decstar(){
			for(int i=0;i<sz*sz;i++){
				if(dat[i]>0)
					dat[i]=dat[i]*1000/1001;
			}
		}

		unsigned int get(int x,int y){

			return dat[y*sz + x];
		}
		int szy() { return sz;};
		int szx() { return sz;};

		unsigned int col(int x, int y){ return liss(get(x,y));}
		int getOffset(int x, int y) {return y*sz+x;}
		void set(int x,int y,unsigned int v){
			dat[y*sz + x]=v;
		}
		void donuts(){
			if(donuts_shape){
				for(int x=0;x<sz;x++){
					dat[getOffset(x,0)]=dat[getOffset(x,sz-2)];
					dat[getOffset(x,sz-1)]=dat[getOffset(x,1)];
					dat[getOffset(0,x)]=dat[getOffset(sz-2,x)];
					dat[getOffset(sz-1,x)]=dat[getOffset(1,x)];
			
				}

			}
		}
		void doLine(){
			int xMax=sz/5+sz/2;
			int yMax=sz/3+sz/2;
			int xMin=5;
			int yMin=5;
			for( int i=0;i<sz*sz;i++) sec[i]=dat[i]; 
			for(int x=1;x<sz-1;x++) for (int y=1;y<sz-1;y++){ 
				int i=getOffset(x,y);
					
				int up=getOffset(x,y-1);
				int down=getOffset(x,y+1);	
				int left=getOffset(x-1,y);
				int right=getOffset(x+1,y);
				
				int counVois=0;
				if(x+1<=xMax) counVois++;
				if(y+1<=yMax) counVois++;	
			//	if(x>=xMin) counVois++;
			//	if(y>=yMin) counVois++;
				if(counVois >0){	
					sec[i]=sec[i]-sec[i]/counVois;
				
				if(x+1<=xMax){		dat[right]+=sec[i]/counVois; sec[i]=sec[i]-sec[i]/counVois;}
				if(y+1<=yMax){			dat[down]+=sec[i]/counVois;sec[i]=sec[i]-sec[i]/counVois;}

				}		
			}			
			
			set(xMax,yMax,1<<26);
			set(xMin,yMin,1<<26);
			
		}
		void doit(){
			//doLine(); return;
			donuts();
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
			//set(sz/2+100,sz/2,1<<30);
			
			//set(sz/2-100,sz/2,1<<30);
			//for(int i=0;i<sz;i++){
			//	if(i%50!=0)
			//	set(sz/2,sz/2,0);
			//}
			//set(sz/2,sz/2-50,1<<30);
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


int main(int argc, char* argv[]){
	calcsimple rs(200);
	display<calcsimple,rawScreen> dis(rs);
	while(true) { 
rs.mdo(4);
dis.paint(); };

	return 0;
}

	
