
#include <iostream>
#include <sstream>
#include <ctime>
using namespace std;

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
}; 
ostream& operator<<(ostream& o, const BenchTime& ts){
	double t=ts.tsec(); 
	o<< t << "seconds ";
	return o;
}

inline unsigned long long selectbit(const unsigned long long m){
	unsigned long long i=m-1;
	i=~i&m;
	return i;	
}

inline long long delbit(const unsigned long long m, const unsigned long long bit){
	return m & ~bit;
}

inline unsigned long long packOrdered4(const unsigned long long m){
	unsigned long long n=m&1ULL;
	for(int i=0;i<12;i++){
		
		n=n|((m>>(3+3*i))&(1ULL<<(1+i)));
	}
	return n;
}



inline unsigned long long nextperm(unsigned long long m){
	unsigned long long v=m; // current permutation of bits 
	unsigned long long w; // next permutation of bits

	unsigned long long t = v | (v - 1); // t gets v's least significant 0 bits set to 1
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
string cardString(const long long b){
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

class Cards{
	unsigned long long m;
public :
	Cards(const unsigned long long m): m(m){};

	Cards() : Cards(0){};

	const Cards lastPerm() const{
		int nb=__builtin_popcountll(m);	
		unsigned long long n=(1ULL << 52)| ((1ULL << (nb-1)) -1ULL);		
		return Cards(n);
	};
	const Cards nextPerm() const{
		unsigned long long n=m;
		Cards res=Cards(nextperm(n));
		return res;
	}

	bool operator ==(const Cards &b) const{
		return m==b.m;
	}

	bool operator <(const Cards &b) const{
		return m<b.m;
	}

	
friend class ColorCards;
friend ostream& operator<<(ostream& o, const Cards& c);
};
 

const Cards firstPerm(const int nbCards){
	unsigned long long n=(1ULL<<nbCards) -1;
	return Cards(n);
}

class ColorCards{
	unsigned long long m;
public :
	ColorCards(const Cards c,int coul){
		unsigned long long n=c.m;
		unsigned long long mask=((0ULL -1 )/15)<<coul;
		m=mask&c.m;
		m=packOrdered4(m>>coul);

	}
	

friend ostream& operator<<(ostream& o, const ColorCards& c);
};

ostream& operator<<(ostream& o, const ColorCards& c){
	unsigned long long m = c.m;
	while(m!=0){
		long long i=selectbit(m);
		//o << cardString(i) << " ";		
		int v= __builtin_ctzll(i);
		o << cardHighChar(v) << " ";
		m=delbit(m,i);				

	}
	return o;
};




ostream& operator<<(ostream& o, const Cards& c){
	unsigned long long m = c.m;
	while(m!=0){
		long long i=selectbit(m);
		o << cardString(i) << " ";		
		m=delbit(m,i);				

	}
	return o;
};

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
		if(sec%3000 ==0)
			cout << i << " --- " << s << " / " << h << " / " << d << " / " << c  << endl;

		sec++;
		//if(sec >3000) break;
	}
	t.stop();
	cout << " il y  a " << sec << " combinaisons "<< endl;
	cout << t << endl;
	return 0;
}
