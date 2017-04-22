all:	Di

Di: main.o
	g++ main.o -o Di

main.o: main.cpp
	g++ -c main.cpp -std=c++11

clean:
	rm -f *.o Di
