namespace physim{
	class rawScreen{
		rawScreen();
		~rawScreen();
		void plot(int x,int y,unsigned int col);
		void paint();	
	};	
	template <class T>
		class screen {
			public :
				T& src;
				rawScreen g;
				screen(T& param) : src(param) {} 

				void paint(){
					for(int i=0;i<src.szx();++i) for(int j=0;j<src.szy();++j){
						g.plot(src.x(),src.y(),src.col());
					}
					g.paint();	
				}	
		};
} // namespace physim
