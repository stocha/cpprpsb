
#include <iostream>
#include <vector>
using namespace std;

int toto(){
	int ov1[10];
	int ov2[10];
	ov2[0]=42;
	vector<int> vov;
	vov.push_back(2);
	vov.push_back(3);

	ov1[10]=5;
	cout << "ele 0 of sec buff" << "= "<< ov2[0] << endl;
}

int main(){
	std::cout << "overflow of a stack based tab" << std::endl;
	toto();

}


