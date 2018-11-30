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

// Communications
void communicationsHandler(int aircraftID, string msg) { // messages in queues



}

void send(int aircraftID, string msg) {

	cout << "To the aircraft with ID: " + aircraftID << endl;
	cout << "Please " + msg << endl;

}

void broadcast(string msg) {
	cout << endl << "To all airplanes, " + msg << endl;
}

void detectLostObjects(int aircraftID) {
	if (aircraftID < 0) {
		string msg = "Identify yourselves";
		broadcast(msg);
	
	}
}

// add into Hit list when aircraft in airspace
void addToLog(Aircraft& airplane, int x, int y, int z) {

	int id = airplane.getId();
	int counter = 0;

	// if in those coordinates, then airplane in the airspace
	if ((x <= 100000 & x >= 0) && (y <= 100000 & y >= 0) && (z >= 0 & z <= 25000)) {

		for (int i = 0; i < Hit.size(); i++)
		{
			int aircraftListId = Hit[i].getId();
			
			// Check if aircraft already inserted in the Hit list

			int x_hit = airplane.getX_coord();
			int y_hit = airplane.getY_coord();
			int z_hit = airplane.getZ_coord();
			
			if ((id < 0) && (x_hit != x) && (y_hit != y) && (z_hit != z)) { // means that lost object, still add into hit list because id can be the same
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
void deleteFromLog(Aircraft& airplane, int x, int y, int z) {

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

void updateLog(Aircraft& airplane) {

	for (int i = 0; i < Hit.size(); i++)
	{
		int aircraftListId = Hit[i].getId();
		int id = airplane.getId();

		if (aircraftListId == id) {
			Hit[i].setX_coord(airplane.getX_coord());
			Hit[i].setY_coord(airplane.getY_coord());
			Hit[i].setZ_coord(airplane.getZ_coord());
		}
	}
}

void collisionAvoidance() {

}

void aircraftMovement(Aircraft& airplane) { // pass by reference in order to update and save the new position values

	int current_speed_x = airplane.getXSpeed();
	int current_speed_y = airplane.getYSpeed();
	int current_speed_z = airplane.getZSpeed();

	int current_pos_x = airplane.getX_coord();
	int current_pos_y = airplane.getY_coord();
	int current_pos_z = airplane.getZ_coord();

	// The position of the aircraft will be updated every second
	// New coordinates after each seconds
	airplane.setX_coord(current_pos_x + current_speed_x);
	airplane.setY_coord(current_pos_y + current_speed_y);
	airplane.setZ_coord(current_pos_z + current_speed_z);

	updateLog(airplane);
}

// store full hit list in history log every 60 seconds
void historyLog(vector<Aircraft> hitList, ostream &file) {

	
	for (int i = 0; i < hitList.size(); i++)
	{
		file << "ID: " << hitList[i].getId() << " " << "X position: " << hitList[i].getX_coord() << " " << "Y position: " << hitList[i].getY_coord()
			<< " " << "Z position: " << hitList[i].getZ_coord() << " " << "Entry time: " << hitList[i].getEntryTime() << " " << "X velocity: " << hitList[i].getXSpeed()
			<< " " << "Y Velocity: " << hitList[i].getYSpeed() << " " << "Z Velocity: " << hitList[i].getZSpeed() << endl;
	}
	
}

// display all aircraft from the airspace
// every 5 seconds to see the changes in the airspace
void displayAirspace() { 

	cout << endl;
	for (int i = 0; i < Hit.size(); i++)
	{
		int id = Hit[i].getId(); 
		int x = Hit[i].getX_coord();
		int y = Hit[i].getY_coord();
		int z = Hit[i].getZ_coord();

		int x_speed = Hit[i].getXSpeed();
		int y_speed = Hit[i].getYSpeed();
		int z_speed = Hit[i].getZSpeed();

		if ((x > 100000 | x < 0) || (y > 100000 | y < 0) || (z > 25000 | z < 0)) { 
			// don't display if out of airspace... do nothing
		}
		else {
			cout << "aircraft --> " << id << ", (" << x << "," << y << "," << z << "), ";

			float speedValue = pow(x_speed, 2) + pow(y_speed, 2) + pow(z_speed, 2);

			cout << "current speed: " << pow(speedValue, 0.5) << endl;
			cout << endl;
		}
	}
	cout << "--------------------------------------------------" << endl;
}

// detect or handle any failures including missed deadlines and failure of an aircraft to respond to an operator command
void failures() {

}

// scan airspace every 15 seconds
// maintain database (trackfile) log
// executes every 15 seconds to scan airplanes from the airfield

void trackerFile(vector<Aircraft>& listOfAircraft) {

	for (int i = 0; i < listOfAircraft.size(); i++)
	{
		// airspace dimensions are 100 x 100 miles (x,y) --> coordinates starts at origin from a corner, therefore 0 to 100000 in x and y
		// height is 25000 ft --> from 0 to 25000 feet
		int x = listOfAircraft[i].getX_coord();
		int y = listOfAircraft[i].getY_coord();
		int z = listOfAircraft[i].getZ_coord();

		addToLog(listOfAircraft[i], x, y, z);
		deleteFromLog(listOfAircraft[i], x, y, z);
		aircraftMovement(listOfAircraft[i]);
	}
}

// create thread for tracker file (radar) 
void start_timer_tracker(function<void(vector<Aircraft>&)> func, vector<Aircraft>& listOfAircraft , unsigned int interval)
{
	thread([func, &listOfAircraft, interval]() {
		while (true)
		{
			func(listOfAircraft);
			this_thread::sleep_for(chrono::seconds(interval));
		}
	}).detach();
}

// Create thread for the display function
void start_timer_display(function<void(void)> func, unsigned int interval)
{
	thread([func, interval]() {
		while (true)
		{
			func();
			this_thread::sleep_for(chrono::seconds(interval));
		}
	}).detach();
}

// Create thread for the movement function
void start_timer_movement(function<void(Aircraft&)> func, Aircraft& airplane, unsigned int interval)
{
	thread([func, &airplane, interval]() {
		while (true)
		{
			func(airplane);
			this_thread::sleep_for(chrono::seconds(interval));
		}
	}).detach();
}

/*void do_something() // only to test the clock
{
	std::cout << "I am doing something" << std::endl;
}*/


// will handle the sporadic and periodic jobs/processes
// Every operator sporadic inputs must be completed within 2 seconds

void scheduler(vector<Aircraft> listOfAircraft) {

	_sleep(1000);
	start_timer_tracker(trackerFile, listOfAircraft, 2);
	start_timer_display(displayAirspace, 2);
	while (true);

}

int main() {

	// current date/time based on current system (testing only)
	auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());
	cout << "The local date and time is: " << ctime(&timenow) << endl;

	//timer_start(do_something, 1);


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
	

	Aircraft aTest1(2018, 10000, 150, 100, 0, 0, 0, 2);
	Aircraft aTest2(2222, 100, 200, 400, 500, 250, 0, 4);
	Aircraft aTest3(3333, 1000, 0, 1, 0, 0, 0, 4);

	vector<Aircraft> listOfAircraft;
	listOfAircraft.push_back(a20);
	listOfAircraft.push_back(a13);
	listOfAircraft.push_back(a7);

	scheduler(listOfAircraft);

	//addToLog(aTest1, 641, 283, 500);
	//addToLog(aTest2, 223, 630, 526);

	//displayAirspace();

	system("pause");

	ofstream file;
	file.open("historyLog.txt");

	historyLog(Hit, file);

	file.close();


	return 0;
}