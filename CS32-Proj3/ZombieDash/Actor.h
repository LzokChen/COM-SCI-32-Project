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
		bool is_Block, bool block_Flame, bool damageable, bool  infectable);

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
	bool getInfectionStatus() const;
	void setInfectionStatus(bool k);

	virtual bool isHuman() const = 0;
	virtual bool isZombie() const = 0;

	virtual void getDamage();
	void setExistance(bool existance);
	StudentWorld * getSW() const;
	virtual int doSomething() = 0;
	//return  0: defult
	//return -1: Actor "Died"
	//return  1: next level

	virtual ~Actor();

private:

	bool m_existence;
	bool m_isBlock;
	bool m_blockFlame;
	bool m_damageable;
	bool m_infectable;
	bool m_infectionStatus;
	
	StudentWorld * SW;
};

class Inanimate : public Actor
{
public:
	Inanimate(int imageID, double StartX, double StartY, int depth, StudentWorld *sw,
		bool is_Block, bool block_Flame, bool damageable);
	virtual bool isHuman() const;
	virtual bool isZombie() const;
	virtual ~Inanimate();
};

class Wall : public Inanimate
{
public:
	Wall(double StartX, double startY, StudentWorld *sw);
	virtual int doSomething();
};

class Exit : public Inanimate
{
public:
	Exit(double StartX, double startY, StudentWorld *sw);
	virtual int doSomething();
};

class Pit : public Inanimate
{
public:
    Pit(double StartX, double startY, StudentWorld*sw);
    virtual int doSomething();
};


class Human : public Actor
{
public:
	Human(double StartX, double StartY, int imageID, StudentWorld *sw);
	virtual int doSomething();
	int getInfectionCount() const;
	virtual bool isHuman() const;
	virtual bool isZombie() const;
	virtual ~Human();
	

private:
	int infectionCount;
	
};

class Penelope : public Human
{
public:
	Penelope(double StartX, double StartY, StudentWorld *sw);
	virtual int doSomething();

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
	virtual int doSomething();
	virtual void getDamage();
};

class Projectile: public Inanimate
{
public:
    Projectile(double StartX, double StartY, int imageID, StudentWorld *sw);
    virtual int doSomething();
	virtual ~Projectile();
private:
    int tickcount;
};

class Flame : public Projectile
{
public:
    Flame(double StartX, double StartY, StudentWorld *sw);
    virtual int doSomething();
};

class Vomit : public Projectile
{
public:
    Vomit(double StartX, double StartY, StudentWorld *sw);
    virtual int doSomething();
};

class Goodie : public Inanimate
{
public:
	Goodie(double StartX, double StartY, int imageID, StudentWorld *sw);
	virtual int doSomething();
	virtual ~Goodie();
};

class Vaccine_Goodie :public Goodie
{
public:
	Vaccine_Goodie(double StartX, double StartY, StudentWorld *sw);
	virtual int doSomething();
};

class GasCan_Goodie :public Goodie
{
public:
	GasCan_Goodie(double StartX, double StartY, StudentWorld *sw);
	virtual int doSomething();
};

class Landmine_Goodie :public Goodie
{
public:
	Landmine_Goodie(double StartX, double StartY, StudentWorld *sw);
	virtual int doSomething();
};

class Landmine: public Inanimate
{
public:
    Landmine(double StartX, double StartY, StudentWorld *sw);
    virtual int doSomething();
	virtual void getDamage();
private:
    int tickcount;
    bool activeStatus;
	void trigger();
};

class Zombie : public Actor
{
public:
	Zombie(double StartX, double StartY, int imageID, StudentWorld *sw);
	virtual int doSomething();
	virtual bool isHuman() const;
	virtual bool isZombie() const;
	virtual void getDamage();
	virtual ~Zombie();


private:
	int infectionCount;

};
#endif // ACTOR_H_
