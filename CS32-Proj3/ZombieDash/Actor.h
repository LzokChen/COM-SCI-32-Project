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
	Actor(int imageID, double StartX, double StartY, int StartDirection, int depth, StudentWorld *sw,
		bool is_Block, bool block_Flame, bool damageable, bool  infectable, bool infectionSorce, bool m_triggersActiveLanmines);

	//from base class GraphObject
	//double getX() const; // in pixels (0-255)
	//double getY() const; // in pixels (0-255)
	//virtual void moveTo(double x, double y); // in pixels (0-255)
	//int getDirection() const; // in degrees (0-359)
	//void setDirection(Direction d); // {up, down, left, right}

	
	bool getExistance() const;
	bool isBlock() const;
	bool blockFlame() const;
	bool damageable() const;
	bool infectable() const;
	bool infectionSorce()	const;
	bool triggersActiveLandmines() const;

	bool getInfectionStatus() const;
	void setInfectionStatus(bool k);

	virtual void useExit();
	virtual void getDamage();
	void setExistance(bool existance);
	StudentWorld * getSW() const;
	virtual void doSomething() = 0;
	//return  0: defult
	//return -1: Actor "Died"
	//return  1: next level

	virtual ~Actor();

private:

	bool m_existence;
	bool m_infectionStatus;

	bool m_isBlock;
	bool m_blockFlame;
	bool m_damageable;
	bool m_infectable;
	bool m_infectionSorce;
	bool m_triggersActiveLanmines;

	
	StudentWorld * SW;
};

class Inanimate : public Actor
{
public:
	Inanimate(int imageID, double StartX, double StartY, int StartDirection, int depth, StudentWorld *sw,
		bool is_Block, bool block_Flame, bool damageable);
	virtual ~Inanimate();
};

class Wall : public Inanimate
{
public:
	Wall(double StartX, double startY, StudentWorld *sw);
	virtual void doSomething();
};

class Exit : public Inanimate
{
public:
	Exit(double StartX, double startY, StudentWorld *sw);
	virtual void doSomething();
};

class Pit : public Inanimate
{
public:
    Pit(double StartX, double startY, StudentWorld*sw);
    virtual void doSomething();
};

class Projectile : public Inanimate
{
public:
	Projectile(double StartX, double StartY, int StartDirection, int imageID, StudentWorld *sw);
	virtual void doSomething();
	virtual void action() = 0;
	virtual ~Projectile();
private:
	int tickcount;
};

class Flame : public Projectile
{
public:
	Flame(double StartX, double StartY, int StartDirection, StudentWorld *sw);
	virtual void action();
};

class Vomit : public Projectile
{
public:
	Vomit(double StartX, double StartY, int StartDirection, StudentWorld *sw);
	virtual void action();
};

class Goodie : public Inanimate
{
public:
	Goodie(double StartX, double StartY, int imageID, StudentWorld *sw);
	virtual void doSomething();
	virtual void pickup() = 0;
	virtual ~Goodie();
};

class Vaccine_Goodie :public Goodie
{
public:
	Vaccine_Goodie(double StartX, double StartY, StudentWorld *sw);
	virtual void pickup();
};

class GasCan_Goodie :public Goodie
{
public:
	GasCan_Goodie(double StartX, double StartY, StudentWorld *sw);
	virtual void pickup();
};

class Landmine_Goodie :public Goodie
{
public:
	Landmine_Goodie(double StartX, double StartY, StudentWorld *sw);
	virtual void pickup();
};

class Landmine : public Inanimate
{
public:
	Landmine(double StartX, double StartY, StudentWorld *sw);
	virtual void doSomething();
	virtual void getDamage();
private:
	int tickcount;
	bool activeStatus;
	void trigger();
};

class Agent : public Actor
{
public:
	Agent(double StartX, double StartY, int imageID, StudentWorld *sw, bool infectable, bool infectionSorce);
	virtual void towardTo(Actor *Target);
	bool tryToMoveForward(double numPixels);
};

class Human : public Agent
{
public:
	Human(double StartX, double StartY, int imageID, StudentWorld *sw);
	virtual void doSomething();
    virtual void action() = 0;
	int getInfectionCount() const;
	virtual ~Human();
	

private:
	int infectionCount;
	
};

class Penelope : public Human
{
public:
	Penelope(double StartX, double StartY, StudentWorld *sw);
    virtual void action();
	virtual void getDamage();
	virtual void useExit();
	//virtual bool useExit(Actor *exit);


	int getNumLandmine() const;
	int getNumFCharge() const;
	int getNmVaccine() const;

	void incNumLandmine(int i);
	void incNumFCharge(int i);
	void incNumVaccine(int i);
	

private:
	int numLandmine, numFCharge, numVaccine;
	void fire() const;
};

class Citizen : public Human
{
public:
	Citizen(double StartX, double StartY, StudentWorld *sw);
    virtual void action();
	virtual void getDamage();
	virtual void useExit();

private:
	int tickcount;
	bool moveTowardToPenelope();
	bool moveAwayFromZombie(double original_dist_z);
};

class Zombie : public Agent
{
public:
	Zombie(double StartX, double StartY, StudentWorld *sw);
	virtual void doSomething();
	virtual void getDamage();
	virtual void selectDirection() = 0;
	void randomDirection();
	bool checkFrontandVomit();
	virtual ~Zombie();

private:
	int moveDistance;
	int tickcount;
};

class Dumb_Zombie :public Zombie
{
public:
	Dumb_Zombie(double StartX, double StartY, StudentWorld *sw);
	virtual void getDamage();
	virtual void selectDirection();

private:
	bool carVaccine;
	void dropVaccineGooide();
};

class Smart_Zombie :public Zombie
{
public:
	Smart_Zombie(double StartX, double StartY, StudentWorld *sw);
	virtual void getDamage();
	virtual void selectDirection();
};
#endif // ACTOR_H_
