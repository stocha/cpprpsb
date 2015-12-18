
all : hello
	
# HELLO TEST 001
hello : hello.o
	g++ -o hello hello.o
hello.o : hello.cpp 
	g++ -o hello.o -c hello.cpp
cleanhello : 
	rm hello.o hello

