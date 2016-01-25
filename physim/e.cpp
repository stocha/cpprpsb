#include <vector>
#include <iostream>
#include "screen.h"
#include <bitset>
using namespace std;
using namespace physim;

const bool loop=true;
const int nbrayperimage=1;
const int matsz=800;
const int decimage=1800-matsz;
const bool donuts_shape=false;//true;
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
unsigned int liss(int src2){
//	if(src2<0) return src2&((1<<24)-1);
	int src=src2;
//	if(src2<0) src=-src2;
	if(src2>0) src=-src2;
	if(src2>(1<<1)) src=0xFFFFFF;
	if(src2>(1<<2)) src=0x005F00;
	if(src2>(1<<3)) src=0xFF0000;
//	if(src&1==1) src=0xFFFFFF;
//	if(src&2==2) src=0xFF00FF;
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
template<int szx,int szy>
class bitmap{
	
	bitset<szx> bits[szy];	
public :
	
	bitmap& set(int x, int y) {
		bits[y][x]=true;
		return (*this);
	}
	int	get(int x,int y) const{
		return bits[y][x]?1:0;
	}

	bitmap& operator&=(const bitmap& it){
		for(int i=0;i<szy;i++){
			bits[i]&=it.bits[i];
		}	
		return (*this);
	}
	bitmap& operator|=(const bitmap& it){
		for(int i=0;i<szy;i++){
			bits[i]|=it.bits[i];
		}	
		return (*this);
	}
	bitmap& operator^=(const bitmap& it){
		for(int i=0;i<szy;i++){
			bits[i]^=it.bits[i];
		}	
		return (*this);
	}
	const bitmap shl(int v=1) const{
		bitmap res;
		for(int i=0;i<szy;i++) {
			res.bits[i]=bits[i]<<v;
		}
		return res;
	}

	const bitmap shr(int v=1) const{
		bitmap res;
		for(int i=0;i<szy;i++){
			res.bits[i]=bits[i]>>v;
		}
		return res;
	}	
	const bitmap shu(int v=1) const{
		bitmap res;
		for(int i=0;i<szy-v;++i){
			res.bits[i]=bits[i+v];
		}
		for(int i=szy-v;i<szy;++i){
			res.bits[i]=0;
		}
		return res;
	}
	const bitmap shd(int v=1) const{
		bitmap res;
		for(int i=szy-1;i>=v;--i){
			res.bits[i]=bits[i-v];
		}
		for(int i=szy-v;i<szy;++i){
			res.bits[i]=0;
		}
		return res;
	}
		
	const bitmap rol(int v=1) const{
		return shl(v) | shr(szx-v);	
	}	
	const bitmap ror(int v=1) const{
		return shr(v) | shl(szx-v);	
	}

			
	const bitmap operator&( const bitmap& b) const{
		return bitmap(*this)&=b;
	}

	const bitmap operator^( const bitmap& b) const{
		return bitmap(*this)^=b;
	}

	const bitmap operator|( const bitmap& b) const{
		return bitmap(*this)|=b;
	}
	const bitmap rule30x() const{
		// p xor (q or r)
		return (*this).ror() ^ ((*this) | (*this).rol());	
	}
	const bitmap copyLine(int dst,const bitmap& srcb,int src)const {
		bitmap res;
		res=(*this);
		res.bits[dst]=srcb.bits[src];
		return res;
	}

};
class calcsimple{
	const int zoom=1;
	bitmap<matsz,matsz> dat;
public :
	int szx(){return matsz*zoom;}
	int szy(){return matsz*zoom;}
	unsigned int col(int x, int y){return dat.get(x/zoom,y/zoom)*255;};
		
	calcsimple(){
		dat^=dat;
		dat.set(matsz/2,matsz-1);
	}

	void doit(){
		auto u=dat.shu();
		dat=u.copyLine(matsz-1,dat.rule30x(),matsz-2);
	};
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
		const int nbTotla=matsz;//3500;//30000;
		const int cycleSlow=1000;
		if((++bouc) % nbNormal==0 || (((bouc/cycleSlow)%5==1) && (bouc %cycleSlow<durRealTime))){
			cout << " " << bouc << "      " ;
			rs.debug();
			dis.paint();
			if(nbTotla > -1 && bouc>nbTotla) break;
		}
	};


	return 0;
}

	
