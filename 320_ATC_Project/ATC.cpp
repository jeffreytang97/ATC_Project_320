#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <array>
#include <vector> 
#include <list>
#include <ctime>
#include "Aircraft.cpp"
#include <fstream>

#pragma warning(disable:4996)

using namespace std;

//LIST OF ALL PLANES CURRENTLY IN THE SPACE
vector<Aircraft> Hit; 

// ATC data and functions handling
// scan airspace every 15 seconds

// maintain database (trackfile) log

// add into Hit list when aircraft in airspace
void addToLog(Aircraft airplane, int x, int y, int z) {

	int id = airplane.getId();
	int counter = 0;

	// if in those coordinates, then airplane in the airspace
	if ((x <= 100000 & x >= 0) && (y <= 100000 & y >= 0) && (z >= 0 & z <= 25000)) {

		for (int i = 0; i < Hit.size(); i++)
		{
			int aircraftListId = Hit[i].getId();
			
			// Check if aircraft already inserted in the Hit list
			
			if (id < 0) { // means that lost object, still add into hit list because id can be the same
				Hit.push_back(airplane);
				break;
			}
			else if (aircraftListId == id) 
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

	if ((x > 100000 | x < 0) || (y > 100000 | y < 0) || (z > 25000 | z < 0)) {

		for (int i = 0; i < Hit.size(); i++)
		{
			int aircraftListId = Hit[i].getId();

			int x_hit = airplane.getX_coord();
			int y_hit = airplane.getY_coord();
			int z_hit = airplane.getZ_coord();

			if ((aircraftListId == id) && (x_hit == x) && (y_hit == y) && (z_hit == z)) {
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

	// airspace dimensions are 100 x 100 miles (x,y) --> coordinates starts at origin from a corner, therefore 0 to 100000 in x and y
	// height is 25000 ft --> from 0 to 25000 feet
	int x = airplane.getX_coord();
	int y = airplane.getY_coord();
	int z = airplane.getZ_coord();

	addToLog(airplane, x, y, z);
	deleteFromLog(airplane, x, y, z);
	collisionAvoidance();
}

void aircraftMovement(Aircraft airplane) {

	int x = airplane.getX_coord();
	int y = airplane.getY_coord();
	int z = airplane.getZ_coord();

	int current_speed_x = airplane.getXSpeed();
	int current_speed_y = airplane.getYSpeed();
	int current_speed_z = airplane.getZSpeed();

	// The position of the aircraft will be updated every second
	x += current_speed_x;
	y += current_speed_y;
	z += current_speed_z;

	// New coordinates after each seconds
	airplane.positionChange(x, y, z);
}

// store full hit list in history log every 60 seconds
void historyLog(vector<Aircraft> hitList, ostream& file) {
	
	for (int i = 0; i < hitList.size(); i++)
	{
		
	}

}

// display all aircraft from the airspace
// every 5 seconds to see the changes in the airspace
void displayAirspace(vector<Aircraft> hitList) { 

	for (int i = 0; i < hitList.size(); i++)
	{
		int id = hitList[i].getId(); 
		int x = hitList[i].getX_coord();
		int y = hitList[i].getY_coord();
		int z = hitList[i].getZ_coord();

		cout << "aircraft --> " << id << ", (" << x << "," << y << "," << z << ")" << endl;;

		int speedValue = pow(x, 2) + pow(y, 2) + pow(z, 2);

		cout << endl << "Aircraft " << id << " current speed: " << pow(speedValue, 0.5) << endl;
		cout << endl;
	}
}


// Communications
void communicationsHandler(int aircraftID, string msg) { // messages in queues

	

}

void send(int aircraftID, string msg) {

	cout << "To the aircraft with ID: " + aircraftID << endl;
	cout << "Please " + msg << endl;

}

void broadcast(string msg) {
	cout << "To all airplanes, " + msg << endl;
}

void detectLostObjects(int aircraftID) {
	if (aircraftID < 0) {
		string msg = "Identify yourselves";
		broadcast(msg);
	}
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

	int airplane_schedule[160] = { // each line represents an aircraft (ID, speed_x, speed_y, speed_z, x, y, z, entry time)

		0, -641, 283, 500, 95000, 101589, 10000, 13, // a1
		1, -223, -630, -526, 71000, 100000, 13000, 16, // a2
		-1, -180, -446, -186, 41000, 100000, 6000, 31, // a3
		3, 474, -239, 428, 38000, 0, 14000, 44, // a4
		-1, -535, 520, 360, 95000, 100000, 1000, 49, // a5 
		-1, -164, -593, -501, 83000, 100000, 12000, 67, // a6
		6, 512, 614, 520, 86000, -1571, 9000, 87, // a7
		7, -275, 153, -399, 47000, 100000, 12000, 103, // a8
		8, -129, 525, -300, 92000, 100000, 1000, 123, // a9
		9, 437, 574, 339, 32000, 0, 8000, 129, // a10
		10, 568, 538, 192, 50000, 0, 4000, 133, // a11
		11, 347, 529, -599, 83000, -1799, 10000, 140, // a12
		12, -512, -482, 578, 35000, 100000, 4000, 142, // a13
		13, 619, -280, 194, 74000, 0, 10000, 157, // a14
		14, -141, 427, -321, 41000, 102251, 11000, 165, // a15
		15, -199, 242, -205, 56000, 100000, 4000, 172, // a16
		16, 315, -197, -365, 77000, 0, 1000, 187, // a17
		17, -138, 455, 602, 23000, 102290, 14000, 199, // a18
		18, -150, 557, -356, 38000, 100000, 1000, 204, // a19
		19, 194, 184, 598, 35000, 0, 2000, 221, // a20
	};

	Aircraft a1(0, -641, 283, 500, 95000, 101589, 10000, 13);
	Aircraft a2(1, -223, -630, -526, 71000, 100000, 13000, 16);
	Aircraft a3(-1, -180, -446, -186, 41000, 100000, 6000, 31);
	Aircraft a4(3, 474, -239, 428, 38000, 0, 14000, 44);
	Aircraft a5(-1, -535, 520, 360, 95000, 100000, 1000, 49);
	Aircraft a6(-1, -164, -593, -501, 83000, 100000, 12000, 67);
	Aircraft a7(6, 512, 614, 520, 86000, -1571, 9000, 87);
	Aircraft a8(7, -275, 153, -399, 47000, 100000, 12000, 103);
	Aircraft a9(8, -129, 525, -300, 92000, 100000, 1000, 123);
	Aircraft a10(9, 437, 574, 339, 32000, 0, 8000, 129);
	Aircraft a11(10, 568, 538, 192, 50000, 0, 4000, 133);
	Aircraft a12(11, 347, 529, -599, 83000, -1799, 10000, 140);
	Aircraft a13(12, -512, -482, 578, 35000, 100000, 4000, 142);
	Aircraft a14(13, 619, -280, 194, 74000, 0, 10000, 157);
	Aircraft a15(14, -141, 427, -321, 41000, 102251, 11000, 165);
	Aircraft a16(15, -199, 242, -205, 56000, 100000, 4000, 172);
	Aircraft a17(16, 315, -197, -365, 77000, 0, 1000, 187);
	Aircraft a18(17, -138, 455, 602, 23000, 102290, 14000, 199);
	Aircraft a19(18, -150, 557, -356, 38000, 100000, 1000, 204);
	Aircraft a20(19, 194, 184, 598, 35000, 0, 2000, 221);

	return 0;
}