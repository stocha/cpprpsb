
class es{
private :
	std::set<int> d;
public :
	es& add(int);
	//es& operator += ( const es& e);
	es& operator += ( const int& e);
	friend std::ostream& operator<<(std::ostream& os, const es& e);
//	friend es operator + ( es el, const es& er);
	friend es operator + ( es el, const int& er);
};


void xoreqTest();
