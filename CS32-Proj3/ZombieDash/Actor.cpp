#include "Actor.h"
#include "GameConstants.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

///////   Actor  //////
//constructor - Actor
Actor::Actor(int imageID, double startX, double StartY, int StartDirection, int depth, bool is_Block, StudentWorld *sw)
	: GraphObject(imageID, startX, StartY, StartDirection, depth), m_existence(true), m_isBlock(is_Block), SW(sw) {}

bool Actor::getExistance()
{
	return m_existence;
};

bool Actor::isBlock()
{
	return m_isBlock;
}

void Actor::changeExistance()
{
	m_existence = !m_existence;
}

StudentWorld * Actor::getSW()
{
	return SW;
}

Actor:: ~Actor() {};
///////////////////////

///////   Wall   //////
//constructor - Wall
Wall::Wall(double startX, double StartY, StudentWorld *sw)
	:Actor(IID_WALL, SPRITE_WIDTH * startX, SPRITE_HEIGHT * StartY, right, 0, true, sw) {}

bool Wall::doSomething() { return true; }; //do nothing
///////////////////////

///////  Human   //////
//constructor - Human
Human::Human(double startX, double StartY, int imageID, StudentWorld *sw)
	:Actor(imageID, SPRITE_WIDTH * startX, SPRITE_HEIGHT * StartY, right, 0, true, sw), 
		infectionCount(0), infectionStatus(false) {}

bool Human::getInfectionStatus()
{
	return infectionStatus;
}

int Human::getInfectionCount()
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

Human::~Human() {};
///////////////////////

////// Penelope  //////
//constructor - Penelope
Penelope::Penelope(double startX, double StartY, StudentWorld *sw)
	:Human(startX, StartY, IID_PLAYER, sw), numLandmine(0), numFCharge(0), numVaccine(0){}

bool Penelope::doSomething()
{
	//check existance status
	if (!getExistance()) return false;	//Penelope is not exist(alive)

	//check infection status
	if (getInfectionStatus())
	{
		increaseInfectionCount(1);
		if (getInfectionCount() == 500)
		{
			changeExistance();
			return false;				//Penelope is not alive
		}
	}
	
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
	return true;	//Penelope is still alive

}
