all: clean
	g++ -c -o main.o main.cpp
	g++ main.o -o simpleComputer -lpthread
clean:
	rm -rf *.o simpleComputer
