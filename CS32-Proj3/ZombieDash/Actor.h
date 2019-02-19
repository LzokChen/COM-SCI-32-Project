#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject
{
public:
	//constructor
	Actor(int imageID, double startX, double StartY, int StartDirection, int depth, bool is_Block, StudentWorld *sw);

	//from base class GraphObject
	//double getX() const; // in pixels (0-255)
	//double getY() const; // in pixels (0-255)
	//virtual void moveTo(double x, double y); // in pixels (0-255)
	//int getDirection() const; // in degrees (0-359)
	//void setDirection(Direction d); // {up, down, left, right}

	
	bool getExistance();
	bool isBlock();

	void changeExistance();
	StudentWorld * getSW();
	virtual bool doSomething() = 0;

	virtual ~Actor();

private:
	bool m_existence;
	bool m_isBlock;
	StudentWorld * SW;
};

class Wall : public Actor
{
public:
	Wall(double startX, double startY, StudentWorld *sw);
	virtual bool doSomething();
};

class Human : public Actor
{
public:
	Human(double startX, double StartY, int imageID, StudentWorld *sw);
	bool getInfectionStatus();
	int getInfectionCount();
	void changeInfectionStatus();
	void increaseInfectionCount(int i);
	virtual ~Human();

private:
	int infectionCount;
	bool infectionStatus;
};

class Penelope : public Human
{
public:
	Penelope(double startX, double StartY, StudentWorld *sw);
	virtual bool doSomething();

private:
	int numLandmine, numFCharge, numVaccine;
};
#endif // ACTOR_H_
