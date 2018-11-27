#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <array>
#include <vector> 
#include <list>
#include <ctime>
#include "Aircraft.cpp"

#pragma warning(disable:4996)

using namespace std;

vector<Aircraft> Hit; 

// ATC data and functions handling
// scan airspace every 15 seconds

// maintain database (trackfile) log

// add into Hit list when aircraft in airspace
void addToLog(Aircraft airplane, int x, int y, int z) {

	int id = airplane.getId();
	int counter = 0;

	// if in those coordinates, then airplane in the airspace
	if ((x <= 50 & x >= -50) && (y <= 50 & y >= -50) && (z >= 15000 & z <= 40000)) {

		for (int i = 0; i < Hit.size(); i++)
		{
			int aircraftListId = Hit[i].getId();
			// Check if aircraft already inserted in the Hit list
			if (aircraftListId == id) 
				break;
			else 
				counter++;
		}

		// If the counter is equal to the size of the Hit list, 
		// it means it has looped through all of the list and that the aircraft doesn't exist in the list

		if (counter == Hit.size()) {
			Hit.push_back(airplane);
		}
	}
}


// when aircraft leaves airspace
void deleteFromLog(Aircraft airplane, int x, int y, int z) {

	int id = airplane.getId();

	if ((x > 50 | x < -50) || (y > 50 | y < -50) || (z > 40000 | z < 15000)) {

		for (int i = 0; i < Hit.size(); i++)
		{
			int aircraftListId = Hit[i].getId();

			if (aircraftListId == id) {
				Hit.erase(Hit.begin() + i);
				break; // break from the loop when aircraft found in the list
			}
		}
	}
}

void collisionAvoidance() {

}

// executes every 15 seconds to scan airplanes from the airfield
void trackerFile(Aircraft airplane) {

	// airspace dimensions are 100 x 100 miles (x,y) --> coordinates starts at origin from the middle, therefore -50 to 50 for x and y
	// height is 25000 ft and start at 15000 ft (above sea level) --> from 15000 to 40000 feet

	array<int, 3> coordinates = airplane.getPosition();
	int & x = coordinates[0];
	int & y = coordinates[1];
	int & z = coordinates[2];

	addToLog(airplane, x, y, z);
	deleteFromLog(airplane, x, y, z);
	collisionAvoidance();
}

void aircraftMovement() {

}

// store full hit list in history log every 60 seconds
void storeAirspaceStateInLog() {

}

// display all aircraft from the airspace
// every 5 seconds to see the changes in the airspace
void displayAirspace(vector<Aircraft> hitList) { 

	for (int i = 0; i < hitList.size(); i++)
	{
		int id = hitList[i].getId(); 
		array<int, 3> arrayPosition = hitList[i].getPosition();

		// display aircraft_id
		cout << id << " (";

		// display position
		for (int i = 0; i < arrayPosition.size(); i++)
		{
			if (i != 2)
				cout << arrayPosition[i] << ",";
			else
				cout << arrayPosition[i] << ")";
		}
		cout << endl;
	}
}


// Communications
void communicationsHandler() { // messages in queues

}

void send() {

}

void broadcast() {

}

void detectLostObjects() {

}

// detect or handle any failures including missed deadlines and failure of an aircraft to respond to an operator command
void failures() {

}

// will handle the sporadic and periodic jobs/processes
// Every operator sporadic inputs must be completed within 2 seconds
void scheduler() {

}

void timer_start(std::function<void(void)> func, unsigned int interval)
{
	std::thread([func, interval]() {
		while (true)
		{
			func();
			std::this_thread::sleep_for(std::chrono::seconds(interval));
		}
	}).detach();
}


void do_something() // only to test the clock
{
	std::cout << "I am doing something" << std::endl;
}

int main() {

	 // current date/time based on current system (testing only)
	auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());
	cout << "The local date and time is: " << ctime(&timenow) << endl;
	
	timer_start(do_something, 1);

	while (true);

	return 0;
}