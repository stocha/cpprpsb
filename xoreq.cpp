#include <iostream>
#include <set>
#include "xoreq.h"
es& es::add(int){
	std::cout << "add";
}

void xoreqTest(){
	es e;
	e.add(4);
	e.add(5);
}
