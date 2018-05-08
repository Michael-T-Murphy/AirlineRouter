#include "airlineRouter.h"
#include "airportTable.h"
#include "priorityQueue.h"
#include <limits>
#include <sstream>
#include <cmath>
#include <fstream>

const int CODE_LOCATION = 111;
int INF = std::numeric_limits<int>::max();

airlineRouter::airlineRouter(){
	vertexCount = 0;
	times = NULL;
	prev = NULL;
	graphList = NULL;
	flightCount = 0;
}

airlineRouter::~airlineRouter(){
	flightNode *current;
	flightNode *last;
	for (int i = 0; i < vertexCount; i++){
		current = graphList[i];
		while(current != NULL){
			last = current;
			current = current->next;
			delete last;
		}
	}
	delete [] graphList;
}

void airlineRouter::printFlights(){
	flightNode *current;
	for (int i = 0; i < vertexCount; i++){
		current = graphList[i];
		if(graphList[i] != NULL){
			std::cout << std::endl << std::endl <<"FLIGHTS FROM VERTEX " << i 
			<<"------------------------------------------------" << std::endl;
		}
		while(current != NULL){
			std::cout << current->flight << "|" << current->originCode << "|" 
			<< current->destinationCode << "|" << "|" << current->departs << "|" 
			<< current->lands << std::endl;
			current = current->next;
		}
	}
	std::cout << std::endl << "total flights = " << flightCount;
}

void airlineRouter::makeGraph(){
	graphList = new flightNode *[vertexCount];
	for (int i = 0; i < vertexCount; i++){
		graphList[i] = new flightNode;
		graphList[i] = NULL;
		
	}
	
}


bool airlineRouter::readAirports(std::string fileName){
	std::ifstream inFile;
	std::string str;
	std::string name;
	std::string city;
	std::string country;
	std::string code;
	int code_location = CODE_LOCATION;

	
	inFile.open(fileName.c_str());
	if(!inFile.is_open()){
		return false;
	}
	
	//get whole line, split at feilds
	getline(inFile, str);
	while(!inFile.fail()){
		
		name = str.substr(0, 55);
		//name = name.substr(0, name.find_last_not_of(' ')+1);
		city = str.substr(55, 28);
		city = city.substr(city.find_first_not_of(' '));
		city = city.substr(0, city.find_last_not_of(' ')+1);
		country = str.substr(83, 28);
		country = country.substr(0, country.find_last_not_of(' ')+1);
		country = country.substr(country.find_first_not_of(' '));
		
		code = str.substr(code_location, 3);
		while(code[0] == ' '){
			code = str.substr(++code_location, 3);
		}
		code_location = CODE_LOCATION;
		

		airportData.insert(code, name, city, country, ++vertexCount);
		
		getline(inFile, str);
	}
	makeGraph();
	
	return true;
}

bool airlineRouter::readFlights(std::string fileName){
	std::ifstream inFile;
	std::string str;
	std::string flight;
	std::string from;
	std::string depart;
	std::string to;
	std::string land;
	int departTime;
	int landTime;
	std::stringstream ss;
	
	
	inFile.open(fileName.c_str());
	if(!inFile.is_open()){
		return false;
	}
	
	//get whole line, split at feilds
	getline(inFile, str);
	while(!inFile.fail()){
		flight = str.substr(0, 6);
		from = str.substr(8, 3);
		depart = str.substr(12, 4);
		to = str.substr(19, 3);
		land = str.substr(23, 4);
		
		ss.clear();
		ss << depart;
		ss >> departTime;
		ss.clear();
		ss << land;
		ss >> landTime;
		
		addFlight(from, to, departTime, landTime, flight);
		flightCount++;
		
		getline(inFile, str);
	}
	
	return true;
}


void airlineRouter::addFlight(std::string from, std::string to, int depart, int land, std::string flightNum){
	flightNode *newNode = new flightNode;
	int fromVertex = airportData.vertexLookup(from);
	int toVertex = airportData.vertexLookup(to);
	//int num = 0;
	
	newNode->next = NULL;
	newNode->flight = flightNum;
	newNode->departs = depart;
	newNode->lands = land;
	newNode->destination = toVertex;
	newNode->destinationCode = to;
	newNode->originCode = from;
	newNode->origin = fromVertex;

	if(graphList[fromVertex] == NULL){
		graphList[fromVertex] = newNode;
	}else{
		newNode->next = graphList[fromVertex];
		graphList[fromVertex] = newNode;
	}

		
}

void airlineRouter::findFlights(std::string orig, std::string dest, int start){
	unsigned long long placeHolder;
	int u;
	flightNode *current;
	int newTime;

	
	
	times = new int [vertexCount];
	prev = new flightNode [vertexCount];
	
	if (vertexCount <= 0){
		std::cout << "No graph" << std::endl;
		return;
	}
	
	if(!airportData.search(orig)){
		std::cout << "Invalid Origin Airport." << std::endl << std::endl;
		return;
	}
	
	if(!airportData.search(dest)){
		std::cout << "Invalid Destination Airport." << std::endl << std::endl;
		return;
	}
	
	int origVertex = airportData.vertexLookup(orig);
	if(graphList[origVertex] == NULL){
		std::cout << std::endl << "No flights from " << orig << std::endl << std::endl;
		return;
	}
	for(int i = 0; i < vertexCount; i++){
		prev[i].origin = -1;
		times[i] = INF;
	}
	times[origVertex] = start - 30;
	if(times[origVertex] % 100 >= 60){
		times[origVertex] -= 40;
	}
	
	priorityQueue<int> pq;
	pq.insert(origVertex, 0);
	
	while(!pq.isEmpty()){
		pq.deleteMin(u, placeHolder);
		// find out if u connects to v and the time is greater than 30
		// traverse the linked list at u searching for vertex v
		current = graphList[u];
		while(current != NULL){
			int dest = current->destination;
			if(current->lands <= current->departs){
				current->lands += 2400;
			}

			newTime = times[u] + 30;
			if (newTime % 100 >= 60){
				newTime += 40;
			}
			if( current->departs > newTime ){
			//if the flight departure time is 30 min after arriving at current airport check current time for lowest
				if(current->lands < times[dest]){
					times[dest] 				= current->lands;
					prev[dest].departs 			= current->departs;
					prev[dest].lands   			= current->lands;
					prev[dest].destination 		= current->destination;
					prev[dest].origin 			= current->origin;
					prev[dest].flight 			= current->flight;
					prev[dest].destinationCode  = current->destinationCode;						
					prev[dest].originCode 		= current->originCode;
					
					pq.insertSwap(dest, current->lands);
				}
			}
			current = current->next;
		}
	}

	printPath(orig, dest);
	delete [] times;
	delete [] prev;
	
}


void airlineRouter::printPath(const std::string origin, const std::string destination){

	std::string fromAP;
	std::string toAP;
	int to = airportData.vertexLookup(destination);
	airportData.nameLookup(origin, fromAP);
	airportData.nameLookup(destination, toAP);
	

	std::cout << std::endl << "--------------------------------------------";
	std::cout << std::endl << "From / To" << std::endl << "  " << origin << " - ";
	std::cout << fromAP << std::endl << "  " << destination << " - ";
	std::cout << toAP;
	if(times[to] == INF){
		std::cout << std::endl << std::endl << "  " << destination 
		<< " is not reachable from " << origin << "." << std::endl << std::endl;
		return;
	}
	showPath(to);
	std::cout << std::endl << std::endl;
	
	
}

void airlineRouter::showPath(const int current) const{
	if(prev[current].origin == -1){
		return;
	}
	showPath(prev[current].origin);
	std::cout << std::endl << std::endl <<"Flight:" << std::endl << "  " << prev[current].originCode;
	std::cout << "-" << prev[current].destinationCode << " :: DeptTime: ";
	std::cout << prev[current].departs << "   ArrvTime: ";
	std::cout << prev[current].lands << "   " << std::endl << "  Flight Number: ";
	std::cout << prev[current].flight;
	return;
	
	
}

bool airlineRouter::checkAirportCode(std::string code){
	return airportData.search(code);
}

void airlineRouter::showGraphStats(){
	std::cout << "Graph Statistics:" << std::endl
	<< "   Nodes: " << vertexCount << std::endl
	<< "   Edges: " << flightCount << std::endl << std::endl;
	
}
