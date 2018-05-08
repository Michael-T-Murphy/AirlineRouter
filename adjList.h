#include <iostream>
#include <limits>
#include <fstream>
#include <iomanip>
#include "priorityQueue.h"
#include "linkedStack.h"

using namespace std;

struct flightNode {
  unsigned int Departure;
  unsigned int Arrival;
  string flightName;
  unsigned int orgHash;
  unsigned int destHash;
  unsigned int orgListPos;
  unsigned int dstListPos;
};

class adjList {
  public:
    adjList();
    ~adjList();
    void newGraph(int);
    void addEdge(flightNode, int);
    bool dijkstraSP(unsigned int, unsigned int, unsigned int);
    void printPath(const unsigned int, const unsigned int, linkedStack<flightNode>*) ;
  private:
    int vertexCount, edgeCount;
    unsigned int *dist;
    flightNode *prev;
    flightNode nullFlight;
    linkedStack<flightNode> *graphList;
    void showPath(const int, linkedStack<flightNode>*) ;
    void destroyList();
};


adjList::adjList() {
	vertexCount = 0;
	edgeCount = 0;

	nullFlight.Departure = 0;
	nullFlight.Arrival = 0;
	nullFlight.destHash = 0;
	nullFlight.orgHash = 0;
	nullFlight.orgListPos = 0;
	nullFlight.dstListPos = 0;
	nullFlight.flightName = "";

	dist = NULL;
	prev = NULL;
	graphList = NULL;
}

adjList::~adjList() {
	if (graphList != NULL) {
		destroyList();
	}
}

void adjList::newGraph(int vertex) {
	if (graphList != NULL) {
		destroyList();
	}
	vertexCount = vertex;
	graphList = new linkedStack<flightNode> [vertexCount];
}

void adjList::addEdge(flightNode item, int pos) {
	graphList[pos].push(item);
	edgeCount++;
}

bool adjList::dijkstraSP(unsigned int sHash, unsigned int sNode, unsigned int earliest) {
	if (graphList[sNode].isEmpty()) {
		return false;
	}

  unsigned int  newDist, newDist1, u;
  unsigned long long currDistLL;
  flightNode current;
	dist = new unsigned int [vertexCount];
	prev = new flightNode [vertexCount];
	for (int i = 0; i < vertexCount; i++) {
		dist[i] = std::numeric_limits<int>::max();
	}
	for (int i = 0; i < vertexCount; i++) {
		prev[i] = nullFlight;
	}
	unsigned int earliestLower = (earliest % 100);
	earliest = (earliest / 100) ;
	if (earliestLower < 30) {
		earliestLower += 60;
		earliestLower -= 30;
		earliest--;
		earliest = (earliest*100) + earliestLower;
	}
	else {
		earliest = (earliestLower - 30) + (earliest*100);
	}
	priorityQueue<unsigned int> SPqueue;
	dist[sNode] = earliest;
	currDistLL = (unsigned long long) dist[sNode];
	SPqueue.insert(sNode, currDistLL);

	while(!SPqueue.isEmpty()) {
    SPqueue.deleteMin(u, currDistLL);

    //30 Minute Layover CalculationS
    newDist1 = (currDistLL / 100) * 100;
    newDist = (currDistLL %  100) + 30;
    if (newDist >= 60) {
      newDist -= 60;
      newDist1 += 100;
    }

    newDist += newDist1;

    //Iterate All Connections In the Linked List U
    for (int i = 0; i < graphList[u].getStackCount(); i++) {
      graphList[u].traversal(current);
      //Overnight Case
      if (current.Arrival <= current.Departure) {
        current.Arrival += 2400;
      }
	
    //Check if Earlier Than Current Flight
		if (newDist < current.Departure && current.Arrival < dist[current.dstListPos]) {
			  dist[current.dstListPos] = current.Arrival;

        prev[current.dstListPos].Arrival = current.Arrival;
			  prev[current.dstListPos].Departure = current.Departure;
			  prev[current.dstListPos].orgHash = current.orgHash;;
			  prev[current.dstListPos].destHash = current.destHash;
			  prev[current.dstListPos].orgListPos = current.orgListPos;
			  prev[current.dstListPos].dstListPos = current.dstListPos;
			  prev[current.dstListPos].flightName = current.flightName;
   
        SPqueue.insert(current.dstListPos, current.Arrival);
		}
	 }
	}
	return true;
}

void adjList::printPath(const unsigned int sNode, const unsigned int dNode, linkedStack<flightNode> *print) {
  if (prev[dNode].Arrival == 0) {  
	return;
  } 
  showPath(dNode, print);
}

//**************************************************************
//Private Implemntations
//**************************************************************
void adjList::destroyList() {
  if(dist != NULL) {
    delete [] dist;
    dist = NULL;
  }
  if (prev != NULL) {
    delete [] prev;
    prev = NULL;
  }
  delete [] graphList;
  graphList = NULL;
  vertexCount = 0;
}

void adjList::showPath(const int node, linkedStack<flightNode> *print ) {
  if (prev[node].Arrival == 0) {
    return;
  }
  else {
	 print->push(prev[node]);
    showPath(prev[node].orgListPos, print);
  }
}
