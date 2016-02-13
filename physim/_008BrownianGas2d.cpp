#include <vector>
#include <iostream>
#include "screen.h"
#include <bitset>
#include "bitmap.h"
using namespace std;
using namespace physim;

const int matsz=32*4;
const int nblayer=64*2;
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

		const int x=0;
		u&=(~input).xxd(x);
		d&=(~input).xxu(x);
		l&=(~input).xxr(x);
		r&=(~input).xxl(x);
	

		auto xu=(u & input).xxu(x);
		auto xd=(d & input).xxd(x);
		auto xl=(l & input).xxl(x);
		auto xr=(r & input).xxr(x);

		auto coli= (xu&xd) | (xu & xr) | (xu & xl);
		coli|= (xd&xr) | (xd&xl);
		coli|= (xl&xr);
		coli=~coli;
	
		u&=coli.xxd(x);
		d&=coli.xxu(x);
		l&=coli.xxr(x);
		r&=coli.xxl(x);


		auto imm=input & (~u) & (~d) & (~l) & (~r) ;
		

		xu=(u & input).xxu(x);
		xd=(d & input).xxd(x);
		xl=(l & input).xxl(x);
		xr=(r & input).xxr(x);



		auto bord=input;
		bord^=bord;
		bord=~bord;
		bord=bord.shl().shl();
		bord=bord.shr();
		bord=bord.shu().shu();
		bord=bord.shd();	
		

		auto res=xu;
		res|=xd;
		res|=xr;
		res|=xl;
		res|=imm;

		if(x!=1) res&=bord;
		return res;
	}

	const bitmap<matsz,matsz> rand(){
		dat.randomize();
		return dat.getmap(0);
	}
};
class loglay{
	bitstack<matsz,matsz> dat;
public :
	loglay() : dat(nblayer){dat.clear();};
	void push(bitmap<matsz,matsz> arg){
		dat.push(arg);	
	}
	unsigned int get(int px, int py){
		int raw=dat.getsum(px,py);
		const int nbLevel=8;
		const int mult=(255/(nbLevel*2));
		if(raw==0) return 0;
	//	return 0xFFFFFF;
		int col= ((raw+(nbLevel/2))*mult);
		if(col<255) return col/2|(col<<8)|(col/2<<16);
		//if(raw<nbLevel) return ((raw)*mult) << 16;
		//if(raw<nbLevel*2) return( (raw-nbLevel)*mult)<< 8;
		//if(raw<nbLevel*3) return ((raw-nbLevel)*mult) ;
		return 0x7FFF7F;
	}
	

};

	
class calcsimple{
	const int zoom=1;
	
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
		//bm.set(matsz/2,matsz/2,1);
		//bm=bm.flip();
		bm&=order.rand();
		bm&=order.rand();
		bm&=order.rand();
		bm&=order.rand();
	}

	void doit(){

		bm.set(matsz/2,matsz/2,1);
		//bm.set(matsz/2+1,matsz/2,1);
		//bm.set(matsz/2-1,matsz/2,1);
		//bm.set(matsz/2,matsz/2+1,1);
		//bm.set(matsz/2,matsz/2-1,1);
		bm=order.doit(bm);	

		log.push(bm);
	}

	void debug(){cout << endl;};
};
	


int main(int argc, char* argv[]){
	int bouc=0;
	calcsimple rs;
	display<calcsimple,rawScreen> dis(rs);
//	display<calcsimple,rawPpm> dis(rs);
	int initphase=nblayer;
	for(int i=0;i<initphase;++i){
		rs.doit();
	}
	while(true) { 
		rs.doit();
		const int durRealTime=0;
		const int nbNormal=4;
		const int nbTotla=-1;//matsz;//3500;//30000;
		const int cycleSlow=5000;
		if(((++bouc) % nbNormal==0 ) || (bouc %cycleSlow<durRealTime)){
			cout << " " << bouc << "      " ;
			rs.debug();
			dis.paint();
			if(nbTotla > -1 && bouc>nbTotla) break;
		}
	};


	return 0;
}

	
