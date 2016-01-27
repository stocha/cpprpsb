#include <vector>
#include <iostream>
#include "screen.h"
#include <bitset>
#include "bitmap.h"
using namespace std;
using namespace physim;

const int matsz=128;
// application entry point

class calcsimple{
	const int dir=4;
	const int zoom=4;
	bitstack<matsz,matsz> dat;
	bitmap<matsz,matsz> bm;
public :
	int szx(){return matsz*zoom;}
	int szy(){return matsz*zoom;}
	unsigned int col(int px, int py){
		int x=px/zoom;
		int y=py/zoom;
		int val=dat.get(x,y);
		if(dir!=4) val=bm.get(x,y);
		int r=(val&1) * 255;
		int v= ((1&(val>>1)) * 255);
		int b= ((1&(val>>2)) * 255);
			
		//if(dir==3 ) {if(val==0) return 255; else return 255<<8;}
		if(dir!=4) return r|(v<<8)|(b<<16);
		return val;
	}
		
	calcsimple(): dat(24){
		dat.clear();
		bm^=bm;
		if(dir==0) bm.set(matsz/2,matsz-1,1); // up
		if(dir==1) bm.set(matsz/2,0,1); // down 
		if(dir==2) bm.set(matsz-1,matsz/2,1); // left
		if(dir==3) bm.set(0,matsz/2,1); // right 
		if(dir==4) dat.set(matsz/2,matsz/2,1);
	}

	void doit(){
		if(dir==0){
			auto u=bm.shu();
			bm=u.copyLine(matsz-1,bm.rule30x(),matsz-2);
		}
		if(dir==1){
			auto u=bm.shd();
			bm=u.copyLine(0,bm.rule30x(),1);
		}

		if(dir==2){
			auto u=bm.shr();
			bm=u.copyCol(matsz-1,bm.rule30y(),matsz-2);
		}
		if(dir==3){
			auto u=bm.shl();
			bm=u.copyCol(0,bm.rule30y(),1);
		}
		if(dir==4){
			dat.randomize();
		}

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

	
