// CS 302
// Final Project, airline routing

// Note, must use C++11 compiler option
//	g++ -Wall -Wpedantic -g -std=c++11

#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>

#include "airlineRouter.h"

using namespace std;

// *****************************************************************

int main(int argc, char *argv[])
{

// ------------------------------------------------------------------
//  Declarations, headers...

	string	stars, bars, dashes;
	string	fName;
	stars.append(65, '*');
	bars.append(65, '=');
	dashes.append(65,'-');
	const char* bold   = "\033[1m";
	const char* unbold   = "\033[0m";

	cout << stars << endl << bold << "CS 302 - Assignment #11" << endl;
	cout << "Final Project - Air Travel Routing Program" << unbold << endl;
	cout << endl;

// ------------------------------------------------------------------
//  Check argument
//	requires formatted airline routes file name.

	if (argc == 1) {
		cout << "usage: <airportsFileName> <routesFileName>" << endl;
		return 0;
	}

	if (argc != 3) {
		cout << "Error, invalid command line arguments." << endl;
		exit(1);
	}

// ------------------------------------------------------------------
//  Get data and build graph.

	string		routesFile;
	string		airportsFile;
	airlineRouter	myTravelAgent;
	bool		allDone = false, gotAnswer = false;
	string		org, dst;
	int		startTime;
	string		userResponse;

	airportsFile = string(argv[1]);
	routesFile = string(argv[2]);

	// read airports data
	if (!myTravelAgent.readAirports(airportsFile)) {
		cout << "main: Error reading airport data from: " <<
			airportsFile << "." << endl;
		exit(1);
	}

	// read flight data
	if (!myTravelAgent.readFlights(routesFile)) {
		cout << "main: Error reading flight data from: " <<
			routesFile << "." << endl;
		exit(1);
	}

	// show graph stats, for reference
	myTravelAgent.showGraphStats();


// ------------------------------------------------------------------
//  Main processing loop.
//	get travel info
//	find flights, show itinerary
//	ask if user wants to get another

	while (!allDone) {

		// get user input
		while (true) {
			cout << bars << endl;
			cout << bold << "Travel Agent" << unbold << endl;

			cout << "  Origin Airport Code (3 letters): ";
			cin >> org;
			transform(org.begin(), org.end(), org.begin(), ::toupper);
			if (!myTravelAgent.checkAirportCode(org)) {
				cout << endl << "Error, invalid airport code, " <<
					"please try again." << endl << endl;
				continue;
			}

			cout << "  Destination Airport Code (3 letters): ";
			cin >> dst;
			transform(dst.begin(), dst.end(), dst.begin(), ::toupper);
			if (!myTravelAgent.checkAirportCode(dst)) {
				cout << endl << "Error, invalid airport code, " <<
					"please try again." << endl << endl;
				continue;
			}


			if (org == dst) {
				cout << endl << "Error, origin and destination " <<
					" airports are the same, " <<
					"please try again." << endl << endl;
				continue;
			}

			cout << "  Earliest Departure Time: ";
			cin >> startTime;
			if (startTime < 0 || startTime/100 > 24 || startTime%100 >= 60) {
				cout << endl << "Error, invalid daparture time, " <<
					"please try again." << endl << endl;
				continue;
			}
			break;
		}

		// get and show itinerary
		myTravelAgent.findFlights(org, dst, startTime);

		// do another?
		gotAnswer = false;
		while (!gotAnswer) {
			cout << "Another (Y/y/N/n): ";
			cin >> userResponse;

			if (userResponse == "Y" || userResponse == "y")
				gotAnswer = true;
			if (userResponse == "N" || userResponse == "n") {
				allDone = true;
				gotAnswer = true;
			}
			if (!gotAnswer)
				cout << "Error, invalid input, please try again." << endl;
		}
	}


// ------------------------------------------------------------------
//  All done.

	cout << stars << endl;
	cout << "Game over, thanks for playing." << endl;

	return 0;
}

