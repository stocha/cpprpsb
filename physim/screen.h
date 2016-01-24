namespace physim{
	class rawPpm{
		public :
		rawPpm(int sx,int sy);
		~rawPpm();
		void plot(int x,int y,unsigned int col);
		void paint();
	};
	class rawScreen{
		public :
		rawScreen(int sz, int sy);
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
				display(T& param) : src(param),g(src.szx(),src.szy()) {} 

				void paint(){
					for(int i=0;i<src.szx();++i) for(int j=0;j<src.szy();++j){
						g.plot(i,j,src.col(i,j));
					}
					g.paint();	
				}	
		};
} // namespace physim
