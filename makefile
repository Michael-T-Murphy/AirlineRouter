
CC		= g++ -Wall -Wextra -pedantic -g -std=c++11

all: travel

travel.o: travel.cpp airlineRouter.h airlineRouter.cpp priorityQueue.h airportTable.h airportTable.cpp
	$(CC) -c travel.cpp

airlineRouter.o: airlineRouter.h airlineRouter.cpp priorityQueue.h airportTable.h airportTable.cpp
	$(CC) -c airlineRouter.cpp

airportTable.o: airportTable.h airportTable.cpp
	$(CC) -c airportTable.cpp
	
travel: travel.o airportTable.o airlineRouter.o priorityQueue.h
	$(CC) travel.o airportTable.o airlineRouter.o -o travel
