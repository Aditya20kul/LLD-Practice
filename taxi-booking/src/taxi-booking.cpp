//============================================================================
// Name        : taxi-booking.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include<iostream>
#include<vector>
#include<string>

using namespace std;
enum RideStatus {IDLE, CREATED, WITHDRAWN, COMPLETED};


class Ride{
public:
	static const int AMT_PER_KM = 20;
	Ride();
	int calculateFare(bool);
	int getDestination() const;
	void setDestination(int destination);
	int getId() const;
	void setId(int id);
	int getOrigin() const;
	void setOrigin(int origin);
	RideStatus getRideStatus() const;
	void setRideStatus(RideStatus rideStatus);
	int getSeats() const;
	void setSeats(int seats);

private:
	int id;
	int origin, destination;
	int seats;
	RideStatus rideStatus;
};

class Person{
public:
	string name;
	
};

class Driver: private Person{
public:
	Driver(string);
};

class Rider: private Person{
private:
	vector<Ride>completedRides;
	Ride currentRide;
public:
	Rider(string);
	void createRide(int, int, int, int);
	void updateRide(int, int, int, int);
	void withdrawRide(int);
	int closeRide();
};

int Ride :: calculateFare(bool isPriorityRider){
	int distance = destination - origin;
	if(seats < 2){
		return distance * AMT_PER_KM * (isPriorityRider ? 0.75 : 1);
	}
	return distance * seats * AMT_PER_KM * (isPriorityRider ? 0.75 : 1);
}

Ride :: Ride()
{
	id = origin = destination = seats = 0;
	rideStatus = RideStatus::IDLE;
}

int Ride::getDestination() const {
	return destination;
}

void Ride::setDestination(int destination) {
	this->destination = destination;
}

int Ride::getId() const {
	return id;
}

void Ride::setId(int id) {
	this->id = id;
}

int Ride::getOrigin() const {
	return origin;
}

void Ride::setOrigin(int origin) {
	this->origin = origin;
}

RideStatus Ride::getRideStatus() const {
	return rideStatus;
}

void Ride::setRideStatus(RideStatus rideStatus) {
	this->rideStatus = rideStatus;
}

int Ride::getSeats() const {
	return seats;
}

void Ride::setSeats(int seats) {
	this->seats = seats;
}

Driver :: Driver(string name){
	this->name = name;
}

Rider :: Rider(string name){
	this->name = name;
}

void Rider :: createRide(int id, int origin, int destination, int seats){

	if(origin>destination){
		cout<<"!!! Wrong values of Origin and destination entered !!!\n";
		return;
	}
	currentRide.setId(id);
	currentRide.setOrigin(origin);
	currentRide.setDestination(destination);
	currentRide.setSeats(seats);
	currentRide.setRideStatus(RideStatus::CREATED);
}

void Rider :: updateRide(int id, int origin, int destination, int seats){

	if(currentRide.getRideStatus() == RideStatus::WITHDRAWN){
		cout<<"!!! Can't Update Ride, Ride was Withdrawn !!!\n";
		return;
	}
	if(currentRide.getRideStatus() == RideStatus::COMPLETED){
			cout<<"!!! Can't Update Ride, Ride was Completed !!!\n";
			return;
	}
	createRide(id, origin, destination, seats);
}

void Rider :: withdrawRide(int id){

	if(currentRide.getId() != id){
		cout<<"!!! Wrong Id provided !!!\n";
		return;
	}
	if(currentRide.getRideStatus() == RideStatus::COMPLETED){
			cout<<"!!! Can't withdraw Ride, Ride was Completed !!!\n";
			return;
	}
	currentRide.setRideStatus(RideStatus::WITHDRAWN);
}

int Rider :: closeRide(){
	if(currentRide.getRideStatus() != RideStatus::CREATED){
		cout<<"Ride wasn't in progress!";
		return 0;
	}
	currentRide.setRideStatus(RideStatus::COMPLETED);
	completedRides.push_back(currentRide);
	return currentRide.calculateFare(completedRides.size()>=10);
}

int main() {
	Rider rider("Sam");
	Driver driver("Peter");
	rider.createRide(1, 20, 50, 2);
	rider.updateRide(1, 20, 60, 2);
	rider.withdrawRide(2);
	cout<<rider.closeRide();
	return 0;
}




















