#include <iostream>
using namespace std;

using u64 = unsigned long long;

inline u64 nextperm(u64 m){
	u64 v=m; // current permutation of bits 
	u64 w; // next permutation of bits

	u64 t = v | (v - 1); // t gets v's least significant 0 bits set to 1
	// Next set to 1 the most significant bit to change, 
	// set to 0 the least significant ones, and add the necessary 1 bits.
	w = (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctzll(v) + 1)); 	
	return w;
}

int main(){
	u64 countComb=0;
	for(u64 i=(1ULL<<7)-1;i<((1ULL<<52));i=nextperm(i)){
		countComb++;

		}
	cout << "nbcomb " << countComb << endl;
return 0;
}
