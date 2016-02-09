// Include dependency
//#include <vector>
//#include <bitset>

template<int szx,int szy>
class bitmap{
	
	std::bitset<szx> bits[szy];	
public :
	int getszx() { return szx;}
	int getszy() { return szy;}
	
	bitmap& set(int x, int y,unsigned int v) {
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
	
	const bitmap rol(const int v=1) const{
		return shl(v) | shr(szx-v);	
	}	
	const bitmap ror(const int v=1) const{
		return shr(v) | shl(szx-v);	
	}
	const bitmap rou(const int v=1) const{
		return shu(v) | shd(szx-v);	
	}	
	const bitmap rod(const int v=1) const{
		return shd(v) | shu(szx-v);	
	}


	const bitmap xxl(const int loop=1, const int v=1) const{
		return loop?rol(v):shl(v);
	}
	
	const bitmap xxr(const int loop=1, const int v=1) const{
		return loop?ror(v):shr(v);
	}
	
	const bitmap xxu(const int loop=1, const int v=1) const{
		return loop?rou(v):shu(v);
	}
	
	const bitmap xxd(const int loop=1, const int v=1) const{
		return loop?rod(v):shd(v);
	}

	const bitmap operator~() const{
		return bitmap(*this).flip();
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
	std::vector<bitmap<szx,szy>> d;
public :
	bitstack(int sz) : d(sz){}
	bitmap<szx,szy> getmap(int i) { return d[i]; }
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
	unsigned long getsum(int x,int y) const{
		int sz=d.size();
		unsigned long res=0;
		for(int i=sz-1;i>=0;--i){
			res=res+d[i].get(x,y);	
		}
		return res;
	}
	void set(int x,int y, unsigned int value){
		unsigned int v=value;
		for(int i=0;i<d.size();++i){
			if(v&1) d[i].set(x,y,1);
			v=v>>1;
		}
	}
	void shy0(int v=1){
		int sz=d.size();
		for(int i=0;i<sz-v;++i){
			d[i]=d[i+v];
		}
		for(int i=sz-v;i<sz;++i){
			d[i]^=d[i];
		}
	}
	void setat(bitmap<szx,szy> arg, int at){
		d[at]=arg;
	}
	int size() { return d.size();}
	void push(bitmap<szx,szy> arg){
		shy0();
		setat(arg,d.size()-1);	
		
	}
};

