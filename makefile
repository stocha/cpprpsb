
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

