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

	void positionDirectionChange(int x, int y, int z) { // by the ATC
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

		int speedValue = pow(x_speed, 2) + pow(y_speed, 2) + pow(z_speed, 2);

		cout << endl << "Aircraft current speed: " << pow(speedValue,0.5) << endl;
	}

	/*void aircraftPositionProjection() {
	
	}*/

	int getId() {
		return aircraft_id;
	}

	array<int, 3> getPosition() {
		return position;
	}

	int getXSpeed() {
		return x_speed;
	}

	int getYSpeed() {
		return y_speed;
	}

	int getZSpeed() {
		return z_speed;
	}

	void setXSpeed(int new_x_speed) {
		x_speed = new_x_speed;
	}

	void setYSpeed(int new_y_speed) {
		y_speed = new_y_speed;
	}

	void setZSpeed(int new_z_speed) {
		z_speed = new_z_speed;
	}


private:
	int aircraft_id;
	array<int, 3> position; // (x,y,z) coordinates
	int x_speed;
	int y_speed;
	int z_speed;
};