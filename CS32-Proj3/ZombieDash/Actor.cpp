#include "Actor.h"
#include "GameConstants.h"
#include "StudentWorld.h"
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

///////   Actor  //////
//constructor - Actor
Actor::Actor(int imageID, double startX, double StartY, int StartDirection, int depth, StudentWorld *sw,
	bool is_Block, bool flammable, bool  infectable): GraphObject(imageID, startX, StartY, StartDirection, depth)
{
	m_existence = true;
	m_isBlock = is_Block;
	m_flammable = flammable;
	m_infectable = infectable;
	SW = sw;
}

bool Actor::getExistance() const
{
	return m_existence;
};

bool Actor::isBlock() const
{
	return m_isBlock;
}

bool Actor::flammable() const
{
	return m_flammable;
}

bool Actor::infectable() const
{
	return m_infectable;
}

void Actor::changeExistance()
{
	m_existence = !m_existence;
}

StudentWorld * Actor::getSW() const
{
	return SW;
}

Actor:: ~Actor() {};
///////////////////////

///////   Wall   //////
//constructor - Wall
Wall::Wall(double startX, double StartY, StudentWorld *sw)
	:Actor(IID_WALL, SPRITE_WIDTH * startX, SPRITE_HEIGHT * StartY, right, 0, sw, true, false, false) {}

int Wall::doSomething() { return 0; } //do nothing, return 0: defult

string Wall::getType() const
{
	return "Wall";
}

; //do nothing
///////////////////////

///////   Exit   //////
Exit::Exit(double startX, double StartY, StudentWorld *sw)
	:Actor(IID_EXIT, SPRITE_WIDTH * startX, SPRITE_HEIGHT * StartY, right, 1, sw, false, false, false) {}

int Exit::doSomething() {

	for (list<Actor*>::iterator lt = getSW()->GetList().begin(); lt != getSW()->GetList().end(); lt++)
	{
		if ((*lt)->getType() == "Citizen")	//if the actor is a "Citizen"
		{
			if (getSW()->overlap(*this, *(*lt)))	//if "Citizen" and "Exit" are overlap
			{
				getSW()->increaseScore(500);	//add 500 pt
				(*lt)->changeExistance();		//changeExistance to dead
				getSW()->changeNumCitizen(-1);	//deduct number of citizen by 1;
				getSW()->playSound(SOUND_CITIZEN_SAVED); //play the sound effect
			}
		}
	}

	if (getSW()->overlap(*this, *(getSW()->getPlayer())) && getSW()->getNumCitizen() == 0)
	{
		return 1; //return 1: next level
	}
	return 0;
}
string Exit::getType() const
{
	return "Exit";
}

///////////////////////

///////  Human   //////
//constructor - Human
Human::Human(double startX, double StartY, int imageID, StudentWorld *sw)
	:Actor(imageID, SPRITE_WIDTH * startX, SPRITE_HEIGHT * StartY, right, 0, sw, true, true, true), 
		infectionCount(0), infectionStatus(false) {}

bool Human::getInfectionStatus() const
{
	return infectionStatus;
}

int Human::getInfectionCount() const
{
	return infectionCount;
}

void Human::changeInfectionStatus()
{
	infectionStatus = !infectionStatus;
}

void Human::increaseInfectionCount(int i)
{
	infectionCount += i;
}

int Human::doSomething()
{
	//check existance status
	if (!getExistance())
	{
		return -1;			//Human is dead, return -1
	}
	//check infection status
	if (getInfectionStatus())
	{
		increaseInfectionCount(1);
		if (getInfectionCount() == 500) //Hunman's infection level reached max	
		{
			changeExistance();
			return -1;		//Human is dead, return -1	
		}
	}
}

Human::~Human() {};
///////////////////////

////// Penelope  //////
//constructor - Penelope
Penelope::Penelope(double startX, double StartY, StudentWorld *sw)
	:Human(startX, StartY, IID_PLAYER, sw), numLandmine(0), numFCharge(0), numVaccine(0){}

int Penelope::doSomething()
{
	Human::doSomething();
	if (Human::doSomething() == -1) return -1;	//Human is dead => Penelope is dead, return -1	
	
	//check key pressed
	int key;
	if (getSW()->getKey(key))
	{
		switch (key)
		{
		case KEY_PRESS_SPACE:

			break;
		case KEY_PRESS_TAB:

			break;
		case KEY_PRESS_ENTER:

			break;
		case KEY_PRESS_LEFT:
			setDirection(left);
			if(getSW()->accessible(getX() - 4, getY()))
			moveTo(getX() - 4, getY());
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if (getSW()->accessible(getX() + 4, getY()))
			moveTo(getX() + 4, getY());
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			if (getSW()->accessible(getX(), getY() + 4))
			moveTo(getX(), getY() + 4);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			if (getSW()->accessible(getX(), getY() - 4))
			moveTo(getX(), getY() - 4);
			break;
		default:
			break;
		}
	}
	return 0;	//return 0: defult
}

string Penelope::getType() const
{
	return "Penelope";
}

////// Citizen  //////
//constructor - Citizen
Citizen::Citizen(double startX, double StartY, StudentWorld *sw)
	:Human(startX, StartY, IID_CITIZEN, sw){}

int Citizen::doSomething()
{
	
	return 0;		//return 0: defult
}

string Citizen::getType() const
{
	return "Citizen";
}
