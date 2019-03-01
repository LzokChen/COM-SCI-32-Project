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
		bool is_Block, bool block_Flame, bool damageable, bool  infectable, bool infectionSource, bool m_triggersActiveLanmines);

	//from base class GraphObject
	//double getX() const; // in pixels (0-255)
	//double getY() const; // in pixels (0-255)
	//virtual void moveTo(double x, double y); // in pixels (0-255)
	//int getDirection() const; // in degrees (0-359)
	//void setDirection(Direction d); // {up, down, left, right}

	
	bool getExistance() const;				//get the existance status of actor
	void setExistance(bool existance);		//change the existance status of actor
	bool isBlock() const;					//determinate if the actor is an blocklike object
	bool blockFlame() const;				//determinate if the actor block the flame
	bool damageable() const;				//determinate if the actor is damageable
	bool infectable() const;				//determinate if the actor is infectable
	bool infectionSource()	const;			//determinate if the actor is an infection source
	bool triggersActiveLandmines() const;	//determinate if th actor can trigger an active landmine

	bool getInfectionStatus() const;		//get the infection status of actor
	void setInfectionStatus(bool k);		//change the existance status of actor (if it is infectable)

	virtual void useExit();					//if actor overlap with the Exit object
	virtual void getDamage();				//if the actor get damage
	StudentWorld * getSW() const;			//get the studentworld that actor belongs to
	virtual void doSomething() = 0;

	virtual ~Actor();						//destructor

private:

	bool m_existence;
	bool m_infectionStatus;

	bool m_isBlock;
	bool m_blockFlame;
	bool m_damageable;
	bool m_infectable;
	bool m_infectionSource;
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
	virtual void doSomething();	//do nothing
};

class Exit : public Inanimate
{
public:
	Exit(double StartX, double startY, StudentWorld *sw);
	virtual void doSomething();	//check if any alive actor overlap with the exit
};

class Pit : public Inanimate
{
public:
    Pit(double StartX, double startY, StudentWorld*sw);
    virtual void doSomething();	//Let the pit do damage to the alive damageable actor who overlap with it 
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
	virtual void action();		//Let the Flame do damage to the alive damageable actor who overlap with it 
};

class Vomit : public Projectile
{
public:
	Vomit(double StartX, double StartY, int StartDirection, StudentWorld *sw);
	virtual void action();		//infect the infectable actor (human) overlap with the vomit
};

class Goodie : public Inanimate
{
public:
	Goodie(double StartX, double StartY, int imageID, StudentWorld *sw);
	virtual void doSomething();
	virtual void pickup() = 0;	//do the pickup action: increase the number of certain item player have
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
	virtual void getDamage();	//if the landmine get damage (from flame)
private:
	int tickcount;
	bool activeStatus;
	void trigger();				//trigger the landmine, introduce flames and pit
};

class Agent : public Actor
{
public:
	Agent(double StartX, double StartY, int imageID, StudentWorld *sw, bool infectable, bool infectionSource);
	void towardTo(Actor *Target);				//set the agnet direction toward to the target actor
	bool tryToMoveForward(double numPixels);	//try to move agent forword certain pixels in current direction
};

class Human : public Agent
{
public:
	Human(double StartX, double StartY, int imageID, StudentWorld *sw);
	virtual void doSomething();
    virtual void action() = 0;
	int getInfectionCount() const;				//get the infection cout of human
	virtual ~Human();
	

private:
	int infectionCount;
	
};

class Penelope : public Human
{
public:
	Penelope(double StartX, double StartY, StudentWorld *sw);
    virtual void action();					//get user's input and to the certian action
	virtual void getDamage();				//if Penelope get damage
	virtual void useExit();					//if Penelope overlap with the exit


	int getNumLandmine() const;				//get the number of landmine Penelope have
	int getNumFCharge() const;				//get the number of Flame Charge Penelope have
	int getNmVaccine() const;				//get the number of Vaccine Penelope have

	void incNumLandmine(int i);				//increase the number of landmine Penelope have by i
	void incNumFCharge(int i);				//increase the number of Flame charge Penelope have by i
	void incNumVaccine(int i);				//increase the number of vaccine Penelope have by i
	

private:
	int numLandmine, numFCharge, numVaccine;
	void fire() const;						//Penelope fire the flames
};

class Citizen : public Human
{
public:
	Citizen(double StartX, double StartY, StudentWorld *sw);
    virtual void action();					//determinate if citizen need to move toward to Penelope or move away from zombies
	virtual void getDamage();				//if citizen get damage
	virtual void useExit();					//if citizen overlap with exit

private:
	int tickcount;
	bool moveTowardToPenelope();						//try to move citizon toward to penelope
	bool moveAwayFromZombie(double original_dist_z);	//try to move away from the nearest zombie
};

class Zombie : public Agent
{
public:
	Zombie(double StartX, double StartY, StudentWorld *sw);
	virtual void doSomething();
	virtual void getDamage();				//if Zombie get damage
	virtual void selectDirection() = 0;
	void randomDirection();					//randomly select directon for zombie
	bool checkFrontandVomit();				//check front if it is an human and the vomit
	virtual ~Zombie();

private:
	int moveDistance;
	int tickcount;
};

class Dumb_Zombie :public Zombie
{
public:
	Dumb_Zombie(double StartX, double StartY, StudentWorld *sw);
	virtual void getDamage();				//if dumb zombie get damage
	virtual void selectDirection();			//select distance for the dumb zombie

private:
	bool carVaccine;
	void dropVaccineGooide();				//throw the vaccine goodie
};

class Smart_Zombie :public Zombie
{
public:
	Smart_Zombie(double StartX, double StartY, StudentWorld *sw);
	virtual void getDamage();				//if smart zombie get damage
	virtual void selectDirection();			//select distance for the smart zombie to chase the human
};
#endif // ACTOR_H_
