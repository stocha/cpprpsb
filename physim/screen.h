namespace physim{
	class rawScreen{
		public :
		rawScreen();
		~rawScreen();
		void plot(int x,int y,int sx,int sy,unsigned int col);
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
						g.plot(i,j,src.szx(),src.szy(),src.col(i,j));
					}
					g.paint();	
				}	
		};
} // namespace physim
