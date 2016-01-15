#include <iostream>
#include <sstream>
#include <ctime>
using namespace std;
using u64 = unsigned long long int;
using u16 = unsigned short;
const int u64_sz = sizeof(u64)*8;

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
// BenchTime<<
ostream& operator<<(ostream& o, const BenchTime& ts){
	double t=ts.tsec(); 
	o<< t << "seconds ";
	return o;
}
// asm_fun
inline u64 selectHbit(const u64 m){
	if(m==0) return 0;
	u64 highB= 63- __builtin_clzll(m);
	u64 highM= 1ULL<< highB;
	return highM;

}

// asm_fun
inline u64 selectbit(const u64 m){
	u64 i=m-1;
	i=~i&m;
	return i;	
}

// asm_fun
inline u64 nextperm(const u64 m){
	u64 v=m; // current permutation of bits 
	u64 w; // next permutation of bits

	u64 t = v | (v - 1); // t gets v's least significant 0 bits set to 1
	// Next set to 1 the most significant bit to change, 
	// set to 0 the least significant ones, and add the necessary 1 bits.
	w = (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctzll(v) + 1)); 	
	return w;
}

// asm_fun
inline u64 at(const int i){
	return 1ULL << i;
}


// debug_fun
void debug64(u64 par, ostream& o, bool (*f)(int)){
	for(int i=0;i<sizeof(u64)*8;i++)
	{
		o<< ((par>>(u64_sz-1-i))&1);
		if(f(i)) o << " ";
	}

}

//Affichage sous forme de bits
class Bt{
private :
	u64 m;
public :
	Bt(u64 m) : m(m){};
friend ostream& operator<< (ostream& o, Bt& v);
};
// Bt<<
ostream& operator<< (ostream& o, Bt& v){
	debug64(v.m,o,[](int i){ return i%8==7;});
	return o;	
}
// Affichage des valeurs
namespace Values{
	const u64 qf=at(62);
	const u64 sq=at(61); 
	const u64 fu=at(60);
	const u64 co=at(59);
	const u64 qu=at(58);
	const u64 tr=at(57);
	const u64 tp=at(56);
	const u64 op=at(55);
	const u64 sc=at(54); 
}
// debug_fun
char cardHighChar(const int v){
	char sv='2'+v;
	if(v==8) sv='T';
	if(v==9) sv='J';
	if(v==10) sv='Q';
	if(v==11) sv='K';
	if(v==12) sv='A';
	if(v==13) sv='!';
	if(v>13) sv='?';
	return sv;
}
// debug_fun
char cardColorChar(const int c){
	ostringstream res;

	char sc='E';
	if(c==0) sc='s';
	if(c==1) sc='h';
	if(c==2) sc='d';
	if(c==3) sc='c';

	if(c>3) return '?';
	return sc; 
}

void test00(){
	u64 m =(-1ULL) / ( at(8)-1);
	Bt v(m);
	cout << v << endl;
	BenchTime bench;
	long count=0;
	bench.start();
	for(u64 co=at(7)-1;co<at(52);co=nextperm(co)){
		count++;
	}
	bench.stop();
	cout << " il y a " << count << " combinaisons " << endl;
	double nbPerSec=count/bench.tsec();
	cout << bench << " nous fait " << nbPerSec << endl;	
}

int main(){
	test00();
	return 0;
}
