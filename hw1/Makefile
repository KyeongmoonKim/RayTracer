t : main.o myTrans.o
	g++ -o t main.o myTrans.o -lm -lGL -lGLU -lglut

myTrans.o : myTrans.cpp
	g++ -c myTrans.cpp

main.o : myTrans.h main.cpp
	g++ -c main.cpp
