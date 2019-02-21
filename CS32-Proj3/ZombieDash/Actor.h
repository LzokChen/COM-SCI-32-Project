#ifndef ACTOR_H_
#define ACTOR_H_

#include <list>
#include "GraphObject.h"
using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject
{
public:
	//constructor
	Actor(int imageID, double startX, double StartY, int StartDirection, int depth, StudentWorld *sw,
		bool is_Block, bool flammable, bool  infectable);

	//from base class GraphObject
	//double getX() const; // in pixels (0-255)
	//double getY() const; // in pixels (0-255)
	//virtual void moveTo(double x, double y); // in pixels (0-255)
	//int getDirection() const; // in degrees (0-359)
	//void setDirection(Direction d); // {up, down, left, right}

	
	bool getExistance() const;
	bool isBlock() const;
	bool flammable() const;
	bool infectable() const;

	void changeExistance();
	StudentWorld * getSW() const;
	virtual string getType() const = 0;
	virtual int doSomething() = 0;
	//return  0: defult
	//return -1: Actor "Died"
	//return  1: next level

	virtual ~Actor();

private:

	bool m_existence;
	bool m_isBlock;
	bool m_flammable;
	bool m_infectable;
	StudentWorld * SW;
};

class Wall : public Actor
{
public:
	Wall(double startX, double startY, StudentWorld *sw);
	virtual int doSomething();
	virtual string getType() const;
};

class Exit : public Actor
{
public:
	Exit(double startX, double startY, StudentWorld *sw);
	virtual int doSomething();
	virtual string getType() const;
};

class Human : public Actor
{
public:
	Human(double startX, double StartY, int imageID, StudentWorld *sw);
	bool getInfectionStatus() const;
	int getInfectionCount() const;
	void changeInfectionStatus();
	void increaseInfectionCount(int i);
	virtual int doSomething();
	virtual ~Human();

private:
	int infectionCount;
	bool infectionStatus;
};

class Penelope : public Human
{
public:
	Penelope(double startX, double StartY, StudentWorld *sw);
	virtual int doSomething();
	virtual string getType() const;

private:
	int numLandmine, numFCharge, numVaccine;
};

class Citizen : public Human
{
public:
	Citizen(double startX, double StartY, StudentWorld *sw);
	virtual int doSomething();
	virtual string getType() const;
};
#endif // ACTOR_H_
