#include <vector>
#include <iostream>
#include "screen.h"
#include <bitset>
#include "bitmap.h"
using namespace std;
using namespace physim;

const int matsz=128*2;
const int nblayer=1;
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

		u&=(~input).xxd();
		d&=(~input).xxu();
		l&=(~input).xxr();
		r&=(~input).xxl();
	
//		u&=(~input).xxd().xxd();
//		d&=(~input).xxu().xxu();
//		l&=(~input).xxr().xxr();
//		r&=(~input).xxl().xxl();

		auto xu=(u & input).xxu();
		auto xd=(d & input).xxd();
		auto xl=(l & input).xxl();
		auto xr=(r & input).xxr();

		auto coli= (xu&xd) | (xu & xr) | (xu & xl);
		coli|= (xd&xr) | (xd&xl);
		coli|= (xl&xr);
		coli=~coli;
	
		u&=coli.xxd();
		d&=coli.xxu();
		l&=coli.xxr();
		r&=coli.xxl();


		auto imm=input & (~u) & (~d) & (~l) & (~r) ;
		

		xu=(u & input).xxu();
		xd=(d & input).xxd();
		xl=(l & input).xxl();
		xr=(r & input).xxr();




		auto res=xu;
		res|=xd;
		res|=xr;
		res|=xl;
		res|=imm;
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
		int col= ((raw+(nbLevel/2))*mult);
		if(col<255) return col/2|(col<<8)|(col/2<<16);
		//if(raw<nbLevel) return ((raw)*mult) << 16;
		//if(raw<nbLevel*2) return( (raw-nbLevel)*mult)<< 8;
		//if(raw<nbLevel*3) return ((raw-nbLevel)*mult) ;
		return 0x7FFF7F;
	}
	

};

	
class calcsimple{
	const int zoom=2;
	
	order4 order;
	bitmap<matsz,matsz> bm,bbm;
	bitmap<matsz,matsz> floc,yfloc,nfloc;
	bitmap<matsz,matsz> difus,inhib;
	//loglay log;

	bitmap<matsz,matsz> scramble(bitmap<matsz,matsz> input){
		return input|input.xxu()|input.xxd()|input.xxr()|input.xxl();
		
	}
public :
	int szx(){return matsz*zoom;}
	int szy(){return matsz*zoom;}
	unsigned int col(int px, int py){

		int x=px/zoom;
		int y=py/zoom;
		int val=bm.get(x,y);
		if(val!=0) return 0x00|(0x00<<8)|(0x7F<<16);

		val=nfloc.get(x,y);
		if(val!=0) return 0x70|(0xFF<<8)|(0x00<<16);
	
		val=floc.get(x,y);
		if(val!=0) return 0xFF|(0xFF<<8)|(0xFF<<16);
		val=yfloc.get(x,y);
		if(val!=0) return 0x70|(0x70<<8)|(0x70<<16);
		val=bbm.get(x,y);
		if(val!=0) return 0x00|(0x00<<8)|(0xFF<<16);

		val=inhib.get(x,y);
		if(val!=0) return 0x40|(0x00<<8)|(0x00<<16);


		return 0;
	}
		
	calcsimple(){
		bm^=bm;
//		bm=bm.flip();
		bm&=order.rand();
		bm&=order.rand();
//		bm&=order.rand();

		floc^=floc;
		floc=~floc;
		floc&=order.rand();
		floc&=order.rand();
		floc&=order.rand();
		floc&=order.rand();

		floc&=order.rand();
		floc&=order.rand();
		floc&=order.rand();
		floc&=order.rand();

		floc&=order.rand();
		floc&=order.rand();
		floc&=order.rand();
		floc&=order.rand();
		//floc.set(matsz/2,matsz/2,1);

		yfloc^=yfloc;
		yfloc=~yfloc;
		yfloc&=order.rand();
		yfloc&=order.rand();
		yfloc&=order.rand();
		yfloc&=order.rand();

		yfloc&=order.rand();
		yfloc&=order.rand();
		yfloc&=order.rand();
		yfloc&=order.rand();

		yfloc&=order.rand();
		yfloc&=order.rand();
		yfloc&=order.rand();
		yfloc&=order.rand();

		difus^=difus;
		difus=~difus;
		difus&=order.rand();
		difus&=order.rand();
		difus&=order.rand();
		difus&=order.rand();

		difus&=order.rand();
		difus&=order.rand();
		difus&=order.rand();
		difus&=order.rand();

		difus&=order.rand();
		difus&=order.rand();
		difus&=order.rand();
		difus&=order.rand();
		
		nfloc^=nfloc;
	}

	void doit(){
		//bm.set(matsz/3,matsz/5,1);
		bm|=difus&(~floc)&(~yfloc);
		bm=order.doit(bm)&(~floc)&(~yfloc);	
		
		auto oldfloc=floc;
		auto sc=scramble(floc);
		floc|=sc&bm;
		oldfloc^=floc;
		
		nfloc|=oldfloc;
		nfloc&=~(scramble(oldfloc)& ~oldfloc);
		
		bm&=~floc;

		bbm|=scramble(nfloc)&(~floc)&(~yfloc)&order.rand()&order.rand();//&order.rand()&order.rand();
		bbm=order.doit(bbm)&(~floc)&(~bm);

		inhib|=scramble(floc&(~nfloc))&(~floc)&(~yfloc)&order.rand();//&order.rand();//&order.rand();//&order.rand();
		inhib=order.doit(inhib)&(~floc)&(~yfloc)&(~bm);

		auto tmp=bbm;
		bbm&=~inhib;
		inhib&=~tmp;

		sc=scramble(yfloc);
		yfloc|=sc&bbm;
		bbm&=~yfloc;


		//log.push(bm | floc);
	}

	void debug(){cout << endl;};
};
	


int main(int argc, char* argv[]){
	int bouc=0;
	int done=0;
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
		const int nbNormal=1;
		const int nbTotla=-1;//matsz;//3500;//30000;
		const int cycleSlow=5000;
		if(((++bouc) % nbNormal==0 ) || (bouc %cycleSlow<durRealTime)){
			cout << " " << bouc << "      " ;
			rs.debug();
			dis.paint();
			++done;
			if(nbTotla > -1 && bouc>nbTotla) break;
//			if(done > 1500) break;
		}
	};


	return 0;
}

	
