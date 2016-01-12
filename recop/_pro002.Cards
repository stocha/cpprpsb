
#include <iostream>
#include <sstream>
#include <ctime>
using namespace std;
using u64 = unsigned long long;

// refclass
// BenchTime : pour chronometrer
// Cards : container cartes
// MonoCards : sans couleurs raw
// ColorCards : sans cardinality same color
// ValueCards : combinaisons
 
inline u64 normalizeBool(u64 i){
	if(i!=0) return 1;
	return 0;
}
inline u64 normalizeSup(u64 a, u64 b){
	if(a>b) return 1;
	else return 0;
}

class BenchTime{
	clock_t dstart;
	clock_t dstop;
public:
	void start(){
		dstart=clock();
	}
	void stop(){
		dstop=clock();
	};
	const double tsec() const{
		
		double t= (dstop-dstart)/(double)CLOCKS_PER_SEC;
		return t;
	}

friend ostream& operator<<(ostream& o, const BenchTime& t);
};// fin BenchTime 

ostream& operator<<(ostream& o, const BenchTime& ts){
	double t=ts.tsec(); 
	o<< t << "seconds ";
	return o;
}

inline u64 selectbit(const u64 m){
	u64 i=m-1;
	i=~i&m;
	return i;	
}

inline u64 delbit(const u64 m, const u64 bit){
	return m & ~bit;
}

inline u64 packOrdered4(const u64 m){
	u64 n=m&1ULL;
	for(int i=0;i<12;i++){
		
		n=n|((m>>(3+3*i))&(1ULL<<(1+i)));
	}
	return n;
}



inline u64 nextperm(u64 m){
	u64 v=m; // current permutation of bits 
	u64 w; // next permutation of bits

	u64 t = v | (v - 1); // t gets v's least significant 0 bits set to 1
	// Next set to 1 the most significant bit to change, 
	// set to 0 the least significant ones, and add the necessary 1 bits.
	w = (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctzll(v) + 1)); 	
	return w;
}

char cardHighChar(const int v){
	char sv='2'+v;
	if(v==8) sv='T';
	if(v==9) sv='J';
	if(v==10) sv='Q';
	if(v==11) sv='K';
	if(v==12) sv='A';
	if(v==13) sv='!';
	return sv;
}
string cardString(const u64 b){
	ostringstream res;

	int i=__builtin_ctzll(b);
	int v=i/4; 
	int c=i%4;
	char sc='E';
	if(c==0) sc='s';
	if(c==1) sc='h';
	if(c==2) sc='d';
	if(c==3) sc='c';
	res << cardHighChar(v)<< sc ;	

	if(v>13) return "??";
	return res.str();
}
class ValueCards{
	u64 m=0;
public:
	bool hasValue(){ return ((-1ULL<<53)&m)!=0 ;}
	void setAsMax(ValueCards v){
		if(v.m>m) m=v.m;
	}
	void setQuinteFlush(u64 cond){
		const u64 mask=cond<<62;
		m|=mask;
	}
	void setFour(){
		const u64 mask=1ULL<<61;
		m|=mask;
	}
	void setFull(){
		const u64 mask=1ULL<<60;
		m|=mask;
	}
	void setColor(u64 cond){
		const u64 mask=cond<<59;
		m|=mask;
	}
	void setQuinte(){
		const u64 mask=1ULL<<58;
		m|=mask;
	}
	void setThree(){
		const u64 mask=1ULL<<57;
		m|=mask;
	}
	void setTwoPairs(){
		const u64 mask=1ULL<<56;
		m|=mask;
	}
	void setOnePair(){
		const u64 mask=1ULL<<55;
		m|=mask;
	}
	void setCard(){
		const u64 mask=1ULL<<54;
		m|=mask;
	}
	void set4Desc(u64 d){
		m|=d<<(13*3);
	}
	void set3Desc(u64 d){
		m|=d<<(13*2);
	}
	void set2Desc(u64 d){
		m|=d<<(13*1);
	}
	void set1Desc(u64 d){
		m|=d;
	}

friend ostream& operator<<(ostream& o,const ValueCards& v);	
};

void outputHigh(ostream& o,const u64 v){
	u64 m=v&((1ULL << 14)-1);

	while(m!=0){
		u64 b=selectbit(m);
		m^= b;
		int hv=__builtin_ctzll(b); 
		
		o<< cardHighChar(hv) << " ";
	}
}	
ostream& operator<<(ostream& o,const ValueCards& v){
	string disp[]={ ""
		,"SC" // index 1
		,"4 "
		,"32"
		,"Co"
		,"S "
		,"3 "
		,"22"
		,"2 "
		,"1 "
		,"ERROR 0"
		,"ERROR 1"
		
	};	
	for(int i=54;i<63;i++){
	//for(int i=0;i<63;i++){
		int ind=62-i+1;	
		u64 mask=(1ULL<<i);
		//o << (v.m&mask); 
		if((v.m&mask) !=0){
			o << disp[ind] << " ";
		}
	
	}
	for(int i=3;i>=0;--i){
		outputHigh(o,v.m>>(i*13));
		o << '|';
	}	
	
	return o;
}

class Cards{
	u64 m;
public :
	Cards(const u64 m): m(m){};

	Cards() : Cards(0){};

	const Cards lastPerm() const{
		int nb=__builtin_popcountll(m);	
		u64 n=(1ULL << 52)| ((1ULL << (nb-1)) -1ULL);		
		return Cards(n);
	};
	const Cards nextPerm() const{
		u64 n=m;
		Cards res=Cards(nextperm(n));
		return res;
	}

	bool operator ==(const Cards &b) const{
		return m==b.m;
	}

	bool operator <(const Cards &b) const{
		return m<b.m;
	}

friend class MonoCards;	
friend class ColorCards;
friend ostream& operator<<(ostream& o, const Cards& c);
};
 

const Cards firstPerm(const int nbCards){
	u64 n=(1ULL<<nbCards) -1;
	return Cards(n);
}


const u64 maskcolor=((0ULL -1 )/15);

class ColorCards{
	u64 m;
public :
	ColorCards(const Cards c,int coul){
		u64 n=c.m;
		u64 mask=((0ULL -1 )/15)<<coul;
		m=mask&c.m;
		m=packOrdered4(m>>coul);

	}
	
	ColorCards(const u64 v){
		m=v;
	}
	
	ValueCards calcValue() const{
		u64 nb=	__builtin_popcountll(m);
		nb=normalizeSup(nb,4);	
		ValueCards res;
		res.setColor(nb);	
		u64 n=m&(m>>1)&(m<<1);
	        u64 n2=n&(n>>1)&(n<<1);	
		u64 isQFlush=normalizeBool(n2);
		res.setQuinteFlush(isQFlush);
		res.set1Desc(m);	
		
		return res;
	}	

friend ostream& operator<<(ostream& o, const ColorCards& c);
};

class MonoCards{
	u64 m;
	u64 extractcol(u64 dat,int coul){
		u64 mask=maskcolor<<coul;
		u64 n=mask&dat;
		return n>>coul;		
	}
public :
	
	MonoCards(const Cards c){
		u64 n=extractcol(c.m,0);
		n+=extractcol(c.m,1);
		n+=extractcol(c.m,2);
		n+=extractcol(c.m,3);
		m=n;
	}
	

friend ostream& operator<<(ostream& o, const MonoCards& c);
};

ostream& operator<<(ostream& o, const MonoCards& c){
	u64 m = c.m;
	for(int i=0;i<13;i++){
		u64 v=(c.m>>(i<<2))&15;
		if(v>0) o<< v <<'.'<< cardHighChar(i)<< ' ';
	}
	return o;
};



ostream& operator<<(ostream& o, const ColorCards& c){
	u64 m = c.m;
	while(m!=0){
		u64 i=selectbit(m);
		//o << cardString(i) << " ";		
		int v= __builtin_ctzll(i);
		o << cardHighChar(v) << " ";
		m=delbit(m,i);				

	}
	return o;
};




ostream& operator<<(ostream& o, const Cards& c){
	u64 m = c.m;
	while(m!=0){
		u64 i=selectbit(m);
		o << cardString(i) << " ";		
		m=delbit(m,i);				

	}
	return o;
};

ValueCards extractValue(Cards i){
	ValueCards vc;
	ColorCards s(i,0);
	ColorCards h(i,1);
	ColorCards d(i,2);
	ColorCards c(i,3);
	MonoCards mc(i);
	
	vc.setAsMax(s.calcValue());
	vc.setAsMax(h.calcValue());
	vc.setAsMax(s.calcValue());
	vc.setAsMax(s.calcValue());
	return vc;
	
}

int main()
{
	Cards c(-1);
	cout << c << endl;
	cout << "firstPerm" << " " << firstPerm(7) << endl;
	cout << "lastPerm" << " " << firstPerm(7).lastPerm() << endl;
		
	int sec=0;
	
	BenchTime t;
	t.start();
	for(Cards i=firstPerm(7);i<firstPerm(7).lastPerm();i=i.nextPerm()){
		//	cout << i << endl;
			ColorCards s(i,0);
			ColorCards h(i,1);
			ColorCards d(i,2);
			ColorCards c(i,3);
			MonoCards mc(i);

			ValueCards vc=extractValue(i);
		if(sec%3000 ==0 || vc.hasValue())
			cout << i <<"--" << vc << " --- " << s << " / " << h << " / " << d << " / " << c << " --- " << mc << endl;

		sec++;
		if(sec >100000) break;
	}
	t.stop();
	cout << " il y  a " << sec << " combinaisons "<< endl;
	cout << t << endl;
	return 0;
}
