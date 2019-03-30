run : main.o myTrans.o
	g++ -o run main.o myTrans.o -lm -lGL -lGLU -lglut

myTrans.o : myTrans.cpp
	g++ -c myTrans.cpp

main.o : main.cpp
	g++ -c main.cpp -lm -lGL -lGLU -lglut

