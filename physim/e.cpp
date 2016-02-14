#include <vector>
#include <iostream>
#include "screen.h"
#include <bitset>
#include "bitmap.h"
using namespace std;
using namespace physim;

const int matsz=32*2;
const int nblayer=4;
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
	unsigned int pix(int r,int v, int b){ return r<<16 | v<<8 | b ;}
public :
	loglay() : dat(nblayer){dat.clear();};
	void push(bitmap<matsz,matsz> arg){
		dat.push(arg);	
	}
	unsigned int get(int px, int py){
		int raw=dat.getsum(px,py);
		//return getstcol(raw);
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
	unsigned int getstcol(const int raw){
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
		
			default :
				if(colind<8   +3){	
					intens=raw - 3*256;
					return pix(255,255,intens>>3);
				}else
		//		if(colind<16  +8+3){	
		//			intens=raw - (8+3)*256;
		//			return pix(255-(intens>>5),255-(intens>>5),intens>>4);
		//		}
			//	else
	
				{
					return pix(255,255,255);
				}
				
		}	
		return 0x0;
	}


};

	
class calcsimple{
	const int zoom=2;
	
	order4 order;
	bitmap<matsz,matsz> bm;
	bitmap<matsz,matsz> bmneg;
	bitmap<matsz,matsz> froz,sig1,sig0a,sig0b;

	//loglay log;

public :
	int szx(){return matsz*zoom;}
	int szy(){return matsz*zoom;}
	unsigned int col(int px, int py, bitmap<matsz,matsz> bm){
		int x=px/zoom;
		int y=py/zoom;
			

		const bool f4=sig1.get(x,y)!=0;
		const bool f1=bmneg.get(x,y)!=0;
		const bool f2=bm.get(x,y)!=0;
		const bool f3=froz.get(x,y)!=0;

		if(f4) return 0xFFFFFF;else
		if(f1) return 0x00FF00;else
		if(f2) return 0x7F00FF;else
		if(f3) return 0xFF0000;else
		
		return 0x000000;



		//return log.get(x,y);
		return 0x00;
	}

	unsigned int col(int px, int py){
		int res=col(px,py,bm);	
		int res2=col(px,py,bmneg);

		return res|res2;
	}
		
	calcsimple(){
		bm^=bm;
		//bm.set(matsz/2,matsz/2,1);
		//bm=bm.flip();
		bm&=order.rand();
		bm&=order.rand();
		bm&=order.rand();
		bm&=order.rand();

		bmneg^=bmneg;
		froz^=froz;
	}
	bitmap<matsz,matsz> scramble(bitmap<matsz,matsz> x){
		return x|x.shl()|x.shr()|x.shu()|x.shd();
		}

	void doit(){



		auto src0=bm;
		src0^=src0;
		src0.set(matsz/2-matsz/3,matsz/2-matsz/6,1);
		auto src1=bmneg;
		src1^=src1;
		src1.set(matsz/2+matsz/5,matsz/2+matsz/4,1);
		
		bm|=src0;
		bmneg|=src1;
		bm=order.doit(bm);	
		bmneg=order.doit(bmneg);	

		auto both=bm&bmneg;
		froz|=both;
		both=scramble(froz);
		both&=bm|bmneg;
		froz|=both;
		both=froz;

		auto r=order.rand();
		r&=order.rand();
		r&=order.rand();
		r&=order.rand();
		r&=order.rand();
		r&=order.rand();
		r&=order.rand();

		//froz=froz& ~r;

		bm=bm& ~both;
		bmneg=bmneg& ~both;

		auto sc0=sig0b;
		auto push=(src0|src1|sig1);//&froz;//(~sc0)&;
		push=scramble(push)&froz&(~sc0);
		sig0b=sig0a;
		sig0a=sig1;
		sig1=push;	

	//	log.push(bm);
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
		const int nbNormal=1;//2*50;
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

	
