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

using namespace std;

// ATC data and functions handling

void updateLog(Aircraft& airplane, vector<Aircraft>& Hit) {

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

// Communications
void communicationsHandler(vector<Aircraft>& Hit, int& aircraftID, int& aNumber, int sign) { // messages in queues

	int result = 0;
	int z_cood = 0;
	int counter = 0;

	// change altitude

	for (int i = 0; i < Hit.size(); i++)
	{
		Aircraft airplane = Hit[i];

		if (aircraftID == airplane.getId()) {

			z_cood = airplane.getZ_coord();

			if (sign == 1) {

				result = aNumber * 1000;
				z_cood += result;

				cout << "Aircraft with ID: " << airplane.getId() << " --> Climb to " << z_cood << " feet." << endl;
			}
			else if (sign == 0)
			{
				result = aNumber * 1000;
				z_cood -= result;

				cout << "Aircraft with ID: " << airplane.getId() << " --> Descend to " << z_cood << " feet." << endl;
			}
			airplane.setZ_coord(z_cood);
			updateLog(airplane, Hit);

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

void userInput(vector<Aircraft>& Hit) {

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

			communicationsHandler(Hit, id_int, n_int, upOrDown_bool);

		}
	}
}

// add into Hit list when aircraft in airspace
void addToLog(vector<Aircraft>& Hit, Aircraft& airplane, int x, int y, int z) {

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
void deleteFromLog(vector<Aircraft>& Hit, Aircraft& airplane, int x, int y, int z) {

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

void collisionAvoidance(vector<Aircraft>& Hit) {

	// Compare each aircraft together from the hit least to detect collisions

	int size = Hit.size();

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
				
				// Make the plane go to the opposite direction to avoid collision

				if (speed_x != 0)
					Hit[j].setXSpeed((speed_x)*-1);
				else
					Hit[j].setYSpeed((speed_y)*-1);
			}
		}
	}
	
}

void aircraftMovement(Aircraft& airplane, vector<Aircraft>& Hit) { // pass by reference in order to update and save the new position values

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

	updateLog(airplane, Hit);
}

// store full hit list in history log every 60 seconds
void historyLog(ostream &file, vector<Aircraft>& Hit) {

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
void displayAirspace(vector<Aircraft>& Hit) {

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

// detect or handle any failures including missed deadlines and failure of an aircraft to respond to an operator command
void failures() {

}

// scan airspace every 15 seconds
// maintain database (trackfile) log
// executes every 15 seconds to scan airplanes from the airfield

void trackerFile(vector<Aircraft>& Hit, vector<Aircraft>& listOfAircraft) {

	for (int i = 0; i < listOfAircraft.size(); i++)
	{
		// airspace dimensions are 100 x 100 miles (x,y) --> coordinates starts at origin from a corner, therefore 0 to 100000 in x and y
		// height is 25000 ft --> from 0 to 25000 feet
		int x = listOfAircraft[i].getX_coord();
		int y = listOfAircraft[i].getY_coord();
		int z = listOfAircraft[i].getZ_coord();

		addToLog(Hit, listOfAircraft[i], x, y, z);
		deleteFromLog(Hit, listOfAircraft[i], x, y, z);
		aircraftMovement(listOfAircraft[i], Hit);
		collisionAvoidance(Hit);
	}
}

// Multithreading of timers

// create thread for tracker file (radar) 
void start_timer_tracker(function<void(vector<Aircraft>&, vector<Aircraft>&)> func, vector<Aircraft>& Hit, vector<Aircraft>& listOfAircraft , unsigned int interval)
{
	thread([func, &Hit, &listOfAircraft, interval]() {
		while (true)
		{
			func(Hit, listOfAircraft);
			this_thread::sleep_for(chrono::seconds(interval));
		}
	}).detach();
}

// Create thread for the display function
void start_timer_display(function<void(vector<Aircraft>&)> func, vector<Aircraft>& Hit, unsigned int interval)
{
	thread([func, &Hit, interval]() {
		while (true)
		{
			func(Hit);
			this_thread::sleep_for(chrono::seconds(interval));
		}
	}).detach();
}

// Create thread for the clock / release time configuration
void start_timer_clock(function<void(vector<Aircraft>&, vector<Aircraft>& , int&)> func, vector<Aircraft>& airplane_list, vector<Aircraft>& listOfAircraft, int &counter, unsigned int interval)
{
	thread([func, &airplane_list, &listOfAircraft,&counter, interval]() {
		while (true)
		{
			func(airplane_list, listOfAircraft, counter);
			this_thread::sleep_for(chrono::seconds(interval));
		}
	}).detach();
}

//Create thread for history log file
// Create new thread to not conflict with other processes
void start_timer_history(function<void(ofstream&, vector<Aircraft>&)> func, ofstream& file, vector<Aircraft>& Hit, unsigned int interval) {

	thread([func, &file, &Hit, interval]() {
		while (true)
		{
			func(file, Hit);
			this_thread::sleep_for(chrono::seconds(interval));
		}
	}).detach();
}

// Create thread for collision detection process
void start_timer_collision(function<void(vector<Aircraft>&)> func, vector<Aircraft>& Hit, unsigned int interval) {

	thread([func, &Hit, interval]() {
		while (true)
		{
			func(Hit);
			this_thread::sleep_for(chrono::seconds(interval));
		}
	}).detach();
}

void entryTime_counter(vector<Aircraft>& airplane_list, vector<Aircraft>& listOfAircraftAdded, int& counter) {
	
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

void thread_reader(function<void(vector<Aircraft>&)> func, vector<Aircraft>& Hit) {

	thread([func, &Hit]() {
		while (true)
		{
			func(Hit);
		}
	}).detach();
}


// will handle the sporadic and periodic jobs/processes
// Every operator sporadic inputs must be completed within 2 seconds

void scheduler(vector<Aircraft>& Hit, vector<Aircraft>& listOfAircraftAdded, vector<Aircraft>& airplane_list, int& counter, ofstream& file) {

	// sporadic operator / user inputs processes
	thread_reader(userInput, Hit);

	// Handling of aircrafts entry time
	start_timer_clock(entryTime_counter, airplane_list, listOfAircraftAdded, counter, 1);

	// Update history log every 60 seconds as specified in requirements
	start_timer_history(historyLog, file, Hit, 60);

	// Scan airspace for aircrafts
	start_timer_tracker(trackerFile, Hit, listOfAircraftAdded, 4);

	// Check for collisions every second
	start_timer_collision(collisionAvoidance, Hit, 1);

	// Display airspace every 5 seconds
	start_timer_display(displayAirspace, Hit, 5);
}

int main() {

	//LIST OF ALL PLANES CURRENTLY IN THE SPACE
	vector<Aircraft> Hit;

	vector<Aircraft> listOfAircraftAdded;
	int counter = 0;
	ofstream file;
	file.open("historyLog.txt");

	// current date/time based on current system 
	auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());
	cout << "The local date and time is: " << ctime(&timenow) << endl;

	// each line represents an aircraft (ID, speed_x, speed_y, speed_z, x, y, z, entry time)
	int airplane_schedule[160] = 
	{ 
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
	
	vector<Aircraft> airplane_list = {a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20};

	scheduler(Hit, listOfAircraftAdded, airplane_list, counter, file); 

	// for sporadic processes
	//thread reader(userInput, Hit);
	//reader.detach();


	while (true);

	file.close();

	return 0;
}