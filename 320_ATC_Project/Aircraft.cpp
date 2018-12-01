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
	 
	//Constructor
	Aircraft(int id, int speed_x, int speed_y, int speed_z, int x, int y, int z, int entryTime) {
		
		aircraft_id = id;
		x_speed = speed_x;
		y_speed = speed_y;
		z_speed = speed_z;
		x_coord = x;
		y_coord = y;
		z_coord = z;
		entry_time = entryTime;
	}

	int getId() const {
		return aircraft_id;
	}

	int getEntryTime() const {
		return entry_time;
	}

	void setX_coord(int x) { // for change altitude feature
		x_coord = x;
	}

	void setY_coord(int y) {
		y_coord = y;
	}

	void setZ_coord(int newAltitude) {
		z_coord = newAltitude;
	}

	int getX_coord() const {
		return x_coord;
	}

	int getY_coord() const {
		return y_coord;
	}

	int getZ_coord() const {
		return z_coord;
	}

	void positionChange(int x, int y, int z) { // by the ATC, for the aircraft movement function
		x_coord = x;
		y_coord = y;
		z_coord = z;
	}

	bool holdingPattern() {
	
	}

	void reportAircraftUpdate() {
		cout << "aircraft --> " << aircraft_id << ", (" << x_coord << "," << y_coord << "," << z_coord << ")" << endl;;
		
		int speedValue = pow(x_speed, 2) + pow(y_speed, 2) + pow(z_speed, 2);

		cout << endl << "Aircraft " << aircraft_id << " current speed: " << pow(speedValue,0.5) << endl << endl;
	}

	/*void aircraftPositionProjection() {
	
	}*/

	int getXSpeed() const {
		return x_speed;
	}

	int getYSpeed() const {
		return y_speed;
	}

	int getZSpeed() const {
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
	int x_coord, y_coord, z_coord;               
	int x_speed, y_speed, z_speed;
	int entry_time;
};