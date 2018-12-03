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
#include <cmath>

#pragma warning(disable:4996)

vector<Aircraft> Hit;

vector<Aircraft> listOfAircraftAdded;

using namespace std;

// ATC data and functions handling

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

	for (int i = 0; i < listOfAircraftAdded.size(); i++)
	{
		int aircraftListId = listOfAircraftAdded[i].getId();
		int id = airplane.getId();

		if (aircraftListId == id) {
			listOfAircraftAdded[i].setX_coord(airplane.getX_coord());
			listOfAircraftAdded[i].setY_coord(airplane.getY_coord());
			listOfAircraftAdded[i].setZ_coord(airplane.getZ_coord());
		}
	}
}

// Communications
void communicationsHandler(int aircraftID, int aNumber, int sign) { // messages in queues

	int z_coord = 0;
	int counter = 0;

	// change altitude

	for (int i = 0; i < Hit.size(); i++)
	{
		if (aircraftID == Hit[i].getId()) {

			z_coord = Hit[i].getZ_coord();

			if (sign == 1) {

				z_coord += (aNumber*1000);
				Hit[i].setZ_coord(z_coord);
				updateLog(Hit[i]);

				cout << "Aircraft with ID: " << Hit[i].getId() << " --> Climb to " << z_coord << " feet." << endl;
			}
			else if (sign == 0)
			{
				z_coord -= (aNumber*1000);
				Hit[i].setZ_coord(z_coord);
				updateLog(Hit[i]);

				cout << "Aircraft with ID: " << Hit[i].getId() << " --> Descend to " << z_coord << " feet." << endl;
			}

			break;
		}
		else
			counter++;
	}

	if (counter == Hit.size()) {
		cout << "Aircraft with ID: " << aircraftID << " not found in airspace." << endl;
	}

}

void send(int aircraftID, string msg) {

	cout << "To the aircraft with ID: " << aircraftID << endl;
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

void communicationSpeedHandler(int aircraftID, int speedValue_x, int speedValue_y, int speedValue_z) {

	// change speed

	int counter = 0;

	for (int i = 0; i < Hit.size(); i++)
	{
		if (aircraftID == Hit[i].getId()) {

			Hit[i].setXSpeed(speedValue_x);
			Hit[i].setYSpeed(speedValue_y);
			Hit[i].setZSpeed(speedValue_z);

			updateLog(Hit[i]);
			break;
		}
		else
			counter++;
	}

	if (counter == Hit.size()) {
		cout << "Aircraft with ID: " << aircraftID << " not found in airspace." << endl;
	}
}

void userInput() {

	// ATC controls and communicates will only aircraft or lost objects from the airspace

	string input;
	while (true) {
		getline(cin, input);

		if (input == "send") {
			cout << "To which aircraft by ID from 0 to 20?";

			string message, id;

			getline(cin, id);

			cout << "Enter the message: ";
			getline(cin, message);
			send(atoi(id.c_str()), message);
		}

		if (input == "broadcast") {
			cout << "Enter the message: ";

			string message;
			getline(cin, message);
			broadcast(message);
		}

		if (input == "altitude change") {

			string id_string, upOrDown, n;
			int counter = 0;

			cout << "Aircraft ID to change altitude: ";
			getline(cin, id_string);
			int id_int = atoi(id_string.c_str());

			cout << "Enter 1 for climb, 0 for descend: ";
			getline(cin, upOrDown);
			int upOrDown_bool = atoi(upOrDown.c_str());

			cout << "Enter n amount of altitude change (n*1000): ";
			getline(cin, n);
			int n_int = atoi(n.c_str());

			communicationsHandler(id_int, n_int, upOrDown_bool);
		}

		if (input == "speed change") {
			
			string message, id_string, speedValue_x, speedValue_y, speedValue_z;

			cout << "Aircraft ID to change speed: ";
			getline(cin, id_string);
			int id_int = atoi(id_string.c_str());

			cout << "Change speed in x, y and z:  " << endl;
			cout << "New speed in x: ";
			getline(cin, speedValue_x);
			int speed_x = atoi(speedValue_x.c_str());

			cout << "New speed in y: ";
			getline(cin, speedValue_y);
			int speed_y = atoi(speedValue_y.c_str());

			cout << "New speed in z: ";
			getline(cin, speedValue_z);
			int speed_z = atoi(speedValue_z.c_str());


			communicationSpeedHandler(id_int, speed_x, speed_y, speed_z);
		}
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
				detectLostObjects(airplane.getId());
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
void deleteFromLog (Aircraft& airplane, int x, int y, int z) {

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

	// Compare each aircraft together from the hit least to detect collisions

	int size = Hit.size();
	bool collision = false;

	for (int i = 0; i < size-1; i++)
	{
		int x_a1 = Hit[i].getX_coord();
		int y_a1 = Hit[i].getY_coord();    
		int z_a1 = Hit[i].getZ_coord();

		for (int j = size-1; j > i; j--)
		{
			int x_a2 = Hit[j].getX_coord();
			int y_a2 = Hit[j].getY_coord();
			int z_a2 = Hit[j].getZ_coord();

			int x_difference = abs(x_a1 - x_a2); // absolute value of the difference between both aircraft distance
			int y_difference = abs(y_a1 - y_a2);
			int z_difference = abs(z_a1 - z_a2);

			if ((z_difference <= 1000) && (x_difference <= 3) && (y_difference <= 3)) 
			{	
				int speed_x = Hit[j].getXSpeed();
				int speed_y = Hit[j].getYSpeed();
				
				collision = true;
				

				// Make the plane go to the opposite direction to avoid collision

				if (speed_x != 0) {
					Hit[j].setXSpeed((speed_x)*-1);
					updateLog(Hit[j]);
				}
				else {
					Hit[j].setYSpeed((speed_y)*-1);
					updateLog(Hit[j]);
				}
			}

			if (collision)
				cout << "Collision warning!" << endl;
		}
	}
	
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
void historyLog(ostream &file) {

	// current date/time based on current system 
	auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());
	cout << "History log updated at: " << ctime(&timenow) << endl;

	file << "Airspace status at: " << ctime(&timenow) << endl;
	if (Hit.empty()) 
	{
		file << "     **The airspace is currently empty at this time**" << endl;
	}
	else 
	{
		for (int i = 0; i < Hit.size(); i++)
		{
			file << "     ID: " << Hit[i].getId() << " --> " << "X position: " << Hit[i].getX_coord() << " / " << "Y position: " << Hit[i].getY_coord()
				<< " / " << "Z position: " << Hit[i].getZ_coord() << " / " << "Entry time: " << Hit[i].getEntryTime() << " / " << "X velocity: " << Hit[i].getXSpeed()
				<< " / " << "Y Velocity: " << Hit[i].getYSpeed() << " / " << "Z Velocity: " << Hit[i].getZSpeed() << endl;
		}
	}
	file << endl << "---------------------------------------------------------------------------------" << endl;
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

		int entryTime = Hit[i].getEntryTime();

		if ((x > 100000 | x < 0) || (y > 100000 | y < 0) || (z > 25000 | z < 0)) { 
			// don't display if out of airspace... do nothing
		}
		else {
			cout << "aircraft --> " << id << ", (" << x << "," << y << "," << z << "), entry time: " << entryTime << "s. (";
			cout << "X velocity: " << Hit[i].getXSpeed() << " / " << "Y Velocity: " << Hit[i].getYSpeed() << " / " << "Z Velocity: " << Hit[i].getZSpeed() << ")" << endl << endl;
		}
	}
	cout << "----------------------------------------------------------------------------------------------------" << endl;
}

// scan airspace every 15 seconds
// maintain database (trackfile) log
// executes every 15 seconds to scan airplanes from the airfield

void trackerFile() {

	for (int i = 0; i < listOfAircraftAdded.size(); i++)
	{
		// airspace dimensions are 100 x 100 miles (x,y) --> coordinates starts at origin from a corner, therefore 0 to 100000 in x and y
		// height is 25000 ft --> from 0 to 25000 feet
		int x = listOfAircraftAdded[i].getX_coord();
		int y = listOfAircraftAdded[i].getY_coord();
		int z = listOfAircraftAdded[i].getZ_coord();

		addToLog(listOfAircraftAdded[i], x, y, z);
		deleteFromLog(listOfAircraftAdded[i], x, y, z);
		aircraftMovement(listOfAircraftAdded[i]);
	}
}

// Multithreading of timers

// create thread for tracker file (radar) 
void start_timer_tracker(function<void()> func, unsigned int interval)
{
	thread([func, interval]() {
		while (true)
		{
			func();
			this_thread::sleep_for(chrono::seconds(interval));
		}
	}).detach();
}

// Create thread for the display function
void start_timer_display(function<void()> func, unsigned int interval)
{
	thread([func, interval]() {
		while (true)
		{
			func();
			this_thread::sleep_for(chrono::seconds(interval));
		}
	}).detach();
}

// Create thread for the clock / release time configuration
void start_timer_clock(function<void(vector<Aircraft>& , int&)> func, vector<Aircraft>& airplane_list, int &counter, unsigned int interval)
{
	thread([func, &airplane_list,&counter, interval]() {
		while (true)
		{
			func(airplane_list, counter);
			this_thread::sleep_for(chrono::seconds(interval));
		}
	}).detach();
}

//Create thread for history log file
// Create new thread to not conflict with other processes
void start_timer_history(function<void(ofstream&)> func, ofstream& file, unsigned int interval) {

	thread([func, &file, interval]() {
		while (true)
		{
			func(file);
			this_thread::sleep_for(chrono::seconds(interval));
		}
	}).detach();
}

// Create thread for collision detection process
void start_timer_collision(function<void()> func, unsigned int interval) {

	thread([func, interval]() {
		while (true)
		{
			func();
			this_thread::sleep_for(chrono::seconds(interval));
		}
	}).detach();
}

void entryTime_counter(vector<Aircraft>& airplane_list, int& counter) {
	
	counter++;

	// compare the counter to the release time of the aircraft
	// If the counter is equal to the release time, then add to the list of aircraft 

	for (int i = 0; i < airplane_list.size(); i++)
	{
		Aircraft airplane = airplane_list[i];  
		int entryTime = airplane.getEntryTime();

		if (entryTime == counter) {
			listOfAircraftAdded.push_back(airplane);
		}
	}
}

void thread_reader(function<void()> func) {

	thread([func]() {
		while (true)
		{
			func();
		}
	}).detach();
}


// will handle the sporadic and periodic jobs/processes
// Every operator sporadic inputs must be completed within 2 seconds

void scheduler(vector<Aircraft>& airplane_list, int& counter, ofstream& file) {

	// sporadic operator / user inputs processes
	thread_reader(userInput);

	// Handling of aircrafts entry time
	start_timer_clock(entryTime_counter, airplane_list, counter, 1);

	// Update history log every 60 seconds as specified in requirements
	start_timer_history(historyLog, file, 60);

	// Scan airspace for aircrafts
	start_timer_tracker(trackerFile, 2);

	// Check for collisions every second
	start_timer_collision(collisionAvoidance, 1);

	// Display airspace every 5 seconds
	start_timer_display(displayAirspace, 5);
}

int main() {
	int counter = 0;
	ofstream file;
	file.open("historyLog.txt");

	// current date/time based on current system 
	auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());
	cout << "The local date and time is: " << ctime(&timenow) << endl;

	// each line represents an aircraft (ID, speed_x, speed_y, speed_z, x, y, z, entry time)

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
	
	vector<Aircraft> airplane_list = {a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20};


	//Aircraft at1(17, -1, -1, 40, 40000, 40000, 14000, 5);
	//Aircraft at2(18, -1, -1, 40, 40000, 40000, 14000, 6);
	//Aircraft at3(19, 194, 184, 598, 35000, 0, 2000, 8);

	//vector<Aircraft> airplane_list = { at1, at2 };

	scheduler(airplane_list, counter, file); 
	while (true);

	file.close();

	return 0;
}