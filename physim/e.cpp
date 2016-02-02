#include <vector>
#include <iostream>
#include "screen.h"
#include <bitset>
#include "bitmap.h"
using namespace std;
using namespace physim;

const int matsz=32*2;
const int nblayer=16*64*8;
// application entry point
class order4{
	bitstack<matsz,matsz> dat;

public :
	order4() : dat(2){
		dat.clear();
		dat.set(matsz/2,matsz/2,1);
		for(int i=0;i<matsz;i++){
			dat.randomize();
		}
		cout << "Randomized" << endl;
	}

	const bitmap<matsz,matsz> doit(bitmap<matsz,matsz> input){
		dat.randomize();
		auto m0=dat.getmap(0);
		auto m1=dat.getmap(1);
		auto u=~m0&~m1;
		auto d=~m0&m1;
		auto r=m0&~m1;
		auto l=m0&m1;
	
		auto res=(u & input).shu();
		res|=(d & input).shd();
		res|=(r & input).shl();
		res|=(l & input).shr();
		//res|=(input & ~ u & ~d);
		return res;
	}

	const bitmap<matsz,matsz> rand(){
		dat.randomize();
		return dat.getmap(0);
	}
};
class loglay{
	bitstack<matsz,matsz> dat;
	unsigned int pix(int r,int v, int b){ return r<<16 | v<<8 | b ;}
public :
	loglay() : dat(nblayer){dat.clear();};
	void push(bitmap<matsz,matsz> arg){
		dat.push(arg);	
	}
	unsigned int get(int px, int py){
		const int raw=dat.getsum(px,py);
		const int nbLevel=255;
		const int subLevelFull=1;
		const int mult=1;//(255/(nbLevel*subLeveFull));
		if(raw==0) return 0;
		//return 0xFFFFFF;
		int col= ((raw+(nbLevel/subLevelFull))*mult);
		//if(col<255) return col/2|(col<<8)|(col/2<<16);
		int intens=0;
		int colind=0;
		for (int i=0;i<7;i++){
			if(raw<nbLevel*(i+1)){
				 intens= ((raw-nbLevel*i)*mult) ;
				 colind=i;
				 break;
			}
			
			++colind;
		}
		colind =raw/256;
		intens=raw - colind*256;
		switch(colind) {
			case 0 :
				return pix(intens,0,0);
			break;
			case 1 :
				return pix(255,intens>>1,0);
			break;
		
			case 2 :
				return pix(255,127+(intens>>1),0);
			break;
		
			case 3 :
				return pix(255,255,intens>>2);
			break;
			
			case 4 :
				return pix(255,255,(0xFF/4)+(intens>>2));
			break;
			
			case 5 :
				return pix(255,255,(2*0xFF/4)+(intens>>2));
			break;
			
			case 6 :
				return pix(255,255,(3*0xFF/4)+(intens>>2));
			break;


			default :
				return pix(0,0,255);
				
		}	
		return 0x0;
	}
	

};

	
class calcsimple{
	const int zoom=10;
	
	order4 order;
	bitmap<matsz,matsz> bm;
	loglay log;

public :
	int szx(){return matsz*zoom;}
	int szy(){return matsz*zoom;}
	unsigned int col(int px, int py){
		int x=px/zoom;
		int y=py/zoom;
		int val=bm.get(x,y);
		int r=(val&1) * 255;
		int v= ((1&(val>>1)) * 255);
		int b= ((1&(val>>2)) * 255);
			

		return log.get(x,y);
		{if(val==0) return 255; else return 255<<8;}
		//if(dir!=4) return r|(v<<8)|(b<<16);
		//return val;
	}
		
	calcsimple(){
		bm^=bm;
		bm.set(matsz/2,matsz/2,1);
		return;
		//bm=bm.flip();
		for(int i=0;i<1000;i++){
			bm=order.doit(bm);
		}
		bm&=order.rand();
		bm&=order.rand();
		bm&=order.rand();
	}

	void doit(){
		auto st=bm;
		auto acc=bm;
		for(int i=0;i<32*10;i++){
			st=order.doit(st);
			acc|=st;
		}

		log.push(acc);
	}

	void debug(){cout << endl;};
};
	


int main(int argc, char* argv[]){
	int bouc=0;
	calcsimple rs;
//	display<calcsimple,rawScreen> dis(rs);
	display<calcsimple,rawPpm> dis(rs);
	int initphase=nblayer;
	for(int i=0;i<initphase;++i){
		rs.doit();
	}
	int im=0;
	while(true) { 
		rs.doit();
		const int durRealTime=0;
		const int nbNormal=nblayer/5;
		const int nbTotla=-1;//matsz;//3500;//30000;
		const int cycleSlow=5000;
	
		if(((++bouc) % nbNormal==0 ) || (bouc %cycleSlow<durRealTime)){
			cout << " " << bouc << "      " ;
			rs.debug();
			dis.paint();
			if(nbTotla > -1 && bouc>nbTotla) break;
		
			++im;
		}
		if(im>2500) break;
	};


	return 0;
}

	
