#include <vector>
#include <iostream>
#include "screen.h"
#include <bitset>
using namespace std;
using namespace physim;

const int matsz=800;
// application entry point
;

template<int szx,int szy>
class bitmap{
	
	bitset<szx> bits[szy];	
public :
	int getszx() { return szx;}
	int getszy() { return szy;}
	
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

	const bitmap flip() const{
		bitmap res;
		for(int i=0;i<szy;i++){
			res.bits[i]=~bits[i];
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
		for(int i=v-1;i>=0;--i){
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
	const bitmap rou(int v=1) const{
		return shu(v) | shd(szx-v);	
	}	
	const bitmap rod(int v=1) const{
		return shd(v) | shu(szx-v);	
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
	const bitmap rule13_7_3x() const{
		// p xor (q or r)
		return (*this).ror(13) ^ ((*this).ror(3) ^ (*this).ror(7));	
	}

	const bitmap rule30y() const{
		// p xor (q or r)
		return (*this).rod() ^ ((*this) | (*this).rou());	
	}
	const bitmap copyLine(int dst,const bitmap& srcb,int src)const {
		bitmap res;
		res=(*this);
		res.bits[dst]=srcb.bits[src];
		return res;
	}
	const bitmap copyCol(int dst,const bitmap& srcb,int src) const{
		bitmap res;
		res=(*this);
		for(int i=0;i<szy;i++){
			bool v=srcb.bits[i][src];
			res.bits[i][dst]=v;
		}
		return res;
	}

};

template<int szx,int szy>
class bitstack{
	vector<bitmap<szx,szy>> d;
public :
	bitstack(int sz) : d(sz){}
	void clear(){
		for(int i=0;i<d.size();++i){
			d[i]^=d[i];
		}
	}
	void randomize(){
		int sz=d.size();
		for(int i=1;i<sz;++i){
			d[i]=d[i-1].rule13_7_3x()^d[i-1].rule30y();
		}
		d[0]=d[d.size()-1].rule13_7_3x()^d[d.size()-1].rule30y();
	}	

	unsigned long get(int x, int y) const{
		int sz=d.size();
		unsigned long res=0;
		for(int i=sz-1;i>=0;--i){
			res=(res<<1)+d[i].get(x,y);	
		}
		return res;
	}
	void set(int x,int y, unsigned int value){
		unsigned int v=value;
		for(int i=0;i<d.size();++i){
			if(v&1) d[i].set(x,y);
			v=v>>1;
		}

	}
};
class calcsimple{
	const int dir=4;
	const int zoom=1;
	bitstack<matsz,matsz> dat;
	bitmap<matsz,matsz> bm;
public :
	int szx(){return matsz*zoom;}
	int szy(){return matsz*zoom;}
	unsigned int col(int px, int py){
		int x=px/zoom;
		int y=py/zoom;
		int val=dat.get(x,y);
		int r=(val&1) * 255;
		int v= ((1&(val>>1)) * 255);
		int b= ((1&(val>>2)) * 255);
			
		//return r|(v<<8)|(b<<16);
		return val;
	}
		
	calcsimple(): dat(24){
		dat.clear();
		bm^=bm;
		if(dir==0) bm.set(matsz/2,matsz-1); // up
		if(dir==1) bm.set(matsz/2,0); // down 
		if(dir==2) bm.set(matsz-1,matsz/2); // left
		if(dir==3) bm.set(0,matsz/2); // right 
		if(dir==4) dat.set(matsz/2,matsz/2,1);
	}

	void doit(){
		if(dir==0){
			auto u=bm.shu();
			bm=u.copyLine(matsz-1,bm.rule30x(),matsz-2);
		}
		if(dir==1){
			//auto u=bm.shd();
			//bm=u.copyLine(0,bm.rule30x(),1);
		}

		if(dir==2){
			//auto u=bm.shr();
			//bm=u.copyCol(matsz-1,bm.rule30y(),matsz-2);
		}
		if(dir==3){
			//auto u=bm.shl();
			//bm=u.copyCol(0,bm.rule30y(),1);
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
		const int nbNormal=10;
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

	
