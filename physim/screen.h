namespace physim{
	class rawScreen{
		rawScreen();
		~rawScreen();
		void plot(int x,int y,unsigned int col);
		void paint();	
	};	
// T : szx() szy() [size]  x() y() col() [x, y , couleur ]
	template <class T,class G>
		class display{
			public :
				T& src;
				G g;
				display(T& param) : src(param) {} 

				void paint(){
					for(int i=0;i<src.szx();++i) for(int j=0;j<src.szy();++j){
						g.plot(src.x(),src.y(),src.col());
					}
					g.paint();	
				}	
		};
} // namespace physim
