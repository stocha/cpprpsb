#include <vector>
#include <iostream>
#include "screen.h"
#include <bitset>
#include "bitmap.h"
using namespace std;
using namespace physim;

const int matsz=128;
// application entry point
class order4{
	bitstack<matsz,matsz> dat;

public :
	order4() : dat(2){
		dat.clear();
		dat.set(matsz/2,matsz/2,1);
		for(int i=0;i<2048;i++){
			dat.randomize();
		}
	}

	const bitmap<matsz,matsz> doit(bitmap<matsz,matsz> input){
		dat.randomize();
		auto m0=dat.getmap(0);
		auto m1=dat.getmap(1);
		auto u=~m0&~m1;
		auto d=~m0&m1;
		auto r=m0&~m1;
		auto l=m0&m1;
	
		auto res=(u & input).rou();
		res|=(d & input).rod();
		res|=(r & input).rol();
		res|=(l & input).ror();
		//res|=(input & ~ u & ~d);
		return res;
	}

};

class calcsimple{
	const int zoom=4;
	
	order4 order;
	bitmap<matsz,matsz> bm;
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
			
		{if(val==0) return 255; else return 255<<8;}
		//if(dir!=4) return r|(v<<8)|(b<<16);
		//return val;
	}
		
	calcsimple(){
		bm^=bm;
		//bm.set(matsz/2,matsz/2,1);
		bm=bm.flip();
	}

	void doit(){
		bm=order.doit(bm);	

	}

	void debug(){cout << endl;};
};
	


int main(int argc, char* argv[]){
	int bouc=0;
	calcsimple rs;
	display<calcsimple,rawScreen> dis(rs);
//	display<calcsimple,rawPpm> dis(rs);
	while(true) { 
		rs.doit();
		const int durRealTime=50;
		const int nbNormal=1;
		const int nbTotla=-1;//matsz;//3500;//30000;
		const int cycleSlow=10000;
		if((++bouc) % nbNormal==0 || (((bouc/cycleSlow)%5==1) && (bouc %cycleSlow<durRealTime))){
			cout << " " << bouc << "      " ;
			rs.debug();
			dis.paint();
			if(nbTotla > -1 && bouc>nbTotla) break;
		}
	};


	return 0;
}

	
