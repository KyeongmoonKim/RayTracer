run : main.o vect.o quat.o
	g++ -o run main.o vect.o quat.o

main.o : main.cpp vect.o quat.o
	g++ -c main.cpp -lm

quat.o : quat.cpp vect.o
	g++ -c quat.cpp -lm

vect.o : vect.cpp
	g++ -c vect.cpp -lm

