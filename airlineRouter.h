#pragma once
#include <string>
#include "airportTable.h"


struct flightNode{
	std::string flight;
	std::string destinationCode;
	std::string originCode;
	int destination;
	int origin;
	int departs;
	int lands;
	flightNode *next;
};


class airlineRouter{
	public:
	airlineRouter();
	~airlineRouter();
	bool readAirports(std::string);
	bool readFlights(std::string);
	void addFlight(std::string, std::string, int, int, std::string);
	airportTable airportData;
	void makeGraph();
	void printFlights();
	void findFlights(std::string, std::string, int);
	flightNode nullFlight;
	bool checkAirportCode(std::string);
	void showGraphStats();
	
	
	private:
	int vertexCount;
	int flightCount;
	int *times;
	flightNode *prev;
	flightNode **graphList;
	void printPath(const std::string, const std::string);
	void showPath(const int) const;
};

