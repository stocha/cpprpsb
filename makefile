
all : hello
	
# HELLO TEST 001
hello : hello.o
	g++ -o hello hello.o
hello.o : hello.cpp 
	g++ -o hello.o -c hello.cpp
cleanhello : 
	rm hello.o hello

# STACKBUFF TEST 002
stackbuff : stackbuff.o
	g++ -o stackbuff stackbuff.o
stackbuff.o : stackbuff.cpp 
	g++ -o stackbuff.o -c stackbuff.cpp
cleanstackbuff : 
	rm stackbuff.o stackbuff


# KEYLEARN CODE 003 
keylearn : keylearn.o
	g++ -o keylearn keylearn.o keylearnmain.cpp
keylearn.o : keylearn.cpp 
	g++ -o keylearn.o -c keylearn.cpp
cleankeylearn : 
	rm keylearn.o keylearn


# XOREQ CODE 003 
xoreq : xoreq.o
	g++ -o xoreq xoreq.o xoreqmain.cpp
xoreq.o : xoreq.cpp 
	g++ -o xoreq.o -c xoreq.cpp
cleanxoreq : 
	rm xoreq.o xoreq


# simplereg CODE 003 
simplereg : simplereg.o
	g++ -o simplereg simplereg.o simpleregmain.cpp
simplereg.o : simplereg.cpp 
	g++ -o simplereg.o -c simplereg.cpp
cleansimplereg : 
	rm simplereg.o simplereg


