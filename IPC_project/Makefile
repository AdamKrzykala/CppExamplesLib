__start__: main.out
	./main

main.out: main.o customQueue.o
	g++ -o main main.o customQueue.o -pthread

main.o: main.cpp customQueue.hpp
	g++ -c main.cpp customQueue.hpp

customQueue.o: customQueue.cpp customQueue.hpp
	g++ -c customQueue.cpp customQueue.hpp
