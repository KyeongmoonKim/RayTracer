run : main.o vect.o quat.o
	g++ `Magick++-config --cppflags --cxxflags` -o run main.o vect.o quat.o `Magick++-config --ldflags --libs`

main.o : main.cpp vect.o quat.o
	g++ `Magick++-config --cppflags --cxxflags` -c main.cpp -lm `Magick++-config --ldflags --libs`

quat.o : quat.cpp vect.o
	g++ -c quat.cpp -lm

vect.o : vect.cpp
	g++ -c vect.cpp -lm

