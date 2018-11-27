#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <array>
#include <vector> 
#include <list>

using namespace std;

class Aircraft {          

public:
	void altitudeChange(int newAltitude) {
		position[2] = newAltitude;
	}

	void speedChange(int newSpeed) {
		speed = newSpeed;
	}

	void positionChange(int x, int y, int z) { // by the ATC
		position[0] = x;
		position[1] = y;
		position[2] = z;
	}

	bool holdingPattern() {
	
	}

	void reportAircraftUpdate() {
		cout << aircraft_id << endl << "(";
		for (int i = 0; i < position.size(); i++)
		{
			if (i != 2)
				cout << position[i] << ",";
			else
				cout << position[i] << ")";
		}

		cout << endl << "Aircraft current speed: " << speed << endl;
	}

	void aircraftPositionProjection() {
	
	}

	int getId() {
		return aircraft_id;
	}

	array<int, 3> getPosition() {
		return position;
	}

	int getSpeed() {
		return speed;
	}

private:
	int aircraft_id;
	array<int, 3> position; // (x,y,z) coordinates
	int speed;
};