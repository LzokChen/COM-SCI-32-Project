#include "Actor.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include <string>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

///////   Actor  //////
//constructor - Actor
Actor::Actor(int imageID, int Type, double StartX, double StartY, int StartDirection,
	int depth, StudentWorld *sw, bool is_Block, bool damageable, bool  infectable)
	: GraphObject(imageID, StartX, StartY, StartDirection, depth)
{
    m_type = Type;
	m_existence = true;
	m_isBlock = is_Block;
	m_damageable = damageable;
	m_infectable = infectable;
	m_infectionStatus = false;
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

bool Actor::damageable() const
{
	return m_damageable;
}

bool Actor::infectable() const
{
	return m_infectable;
}

bool Actor::getInfectionStatus() const
{
	return m_infectionStatus;
}

void Actor::setInfectionStatus(bool k)
{
	if (m_infectable)
		m_infectionStatus = k;

}


void Actor::changeExistance()
{
	m_existence = !m_existence;
}

StudentWorld * Actor::getSW() const
{
	return SW;
}

int Actor::getType() const
{
    return m_type;
}

Actor:: ~Actor() {};
///////////////////////
////////////////////////////////////////

///////   Wall   //////
//constructor - Wall
Wall::Wall(double StartX, double StartY, StudentWorld *sw)
	:Actor(IID_WALL, IID_WALL, SPRITE_WIDTH * StartX, SPRITE_HEIGHT * StartY, right, 0, sw, true, false, false) {}

int Wall::doSomething() { return 0; } //do nothing, return 0: defult

///////////////////////
////////////////////////////////////////

///////   Exit   //////
Exit::Exit(double StartX, double StartY, StudentWorld *sw)
	:Actor(IID_EXIT, IID_EXIT, SPRITE_WIDTH * StartX, SPRITE_HEIGHT * StartY, right, 1, sw, false, false, false) {}

int Exit::doSomething() {

	for (list<Actor*>::iterator lt = getSW()->GetList().begin(); lt != getSW()->GetList().end(); lt++)
	{
		if ((*lt)->getExistance() && (*lt)->getType() == IID_CITIZEN)	
			//if the actor is an alive "Citizen"
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
///////////////////////
////////////////////////////////////////

///////   Pit    //////
//constructor - Pit
Pit::Pit(double StartX, double StartY, StudentWorld *sw)
:Actor(IID_PIT, IID_PIT, SPRITE_WIDTH * StartX, SPRITE_HEIGHT * StartY, right, 0, sw, false, false, false) {}

int Pit::doSomething()
{
    if ( getSW()->damage(this) == -1) //if the Pit do damage and player die
    {
        return -1; //return -1: player died
    }
    
    return 0;
}
///////////////////////
////////////////////////////////////////

///////  Human   //////
//constructor - Human
Human::Human(double StartX, double StartY, int imageID, int Type, StudentWorld *sw)
	:Actor(imageID, Type, SPRITE_WIDTH * StartX, SPRITE_HEIGHT * StartY, right, 0, sw, true, true, true),
		infectionCount(0) {}



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
		infectionCount += 1;
		if (infectionCount >= 500) //Hunman's infection level reached max	
		{
			changeExistance();
			return -1;		//Human is dead, return -1	
		}
	}
	else infectionCount = 0;

    return 0;   //return 0: defalut
}

int Human::getInfectionCount() const
{
	return infectionCount;
}

Human::~Human()
{
}

///////////////////////

////// Penelope  //////
//constructor - Penelope
Penelope::Penelope(double StartX, double StartY, StudentWorld *sw)
	:Human(StartX, StartY, IID_PLAYER, IID_PLAYER, sw), numLandmine(0), numFCharge(0), numVaccine(0){}

int Penelope::doSomething()
{
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
			if (numVaccine > 0)
			{
				setInfectionStatus(false);
				incNumVaccine(-1);
			}
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

int Penelope::getNumLandmine() const
{
	return numLandmine;
}

int Penelope::getNumFCharge() const
{
	return numFCharge;
}

int Penelope::getNmVaccine() const
{
	return numVaccine;
}

void Penelope::incNumLandmine(int i)
{
	numLandmine += i;
}

void Penelope::incNumFCharge(int i)
{
	numFCharge += i;
}

void Penelope::incNumVaccine(int i)
{
	numVaccine += i;
}

////// Citizen  //////
//constructor - Citizen
Citizen::Citizen(double StartX, double StartY, StudentWorld *sw)
	:Human(StartX, StartY, IID_CITIZEN, IID_CITIZEN, sw){}

int Citizen::doSomething()
{
	
	return 0;		//return 0: defult
}
///////////////////////
////////////////////////////////////////

////// Projectile /////
//constructor - Projectile
Projectile::Projectile(double StartX, double StartY, int imageID, int Type, StudentWorld *sw)
    :Actor(imageID, Type, SPRITE_WIDTH * StartX, SPRITE_HEIGHT * StartY, right, 0, sw, false, false, false),tickcount(0) {}

int Projectile::doSomething()
{
    if(!getExistance()) return -1;  //projectile is "dead"
    
    if(tickcount>2)
        changeExistance();
    else tickcount++;
    
    return 0;   //return 0: defult
}
Projectile::~Projectile()
{
}
///////////////////////

/////// Flame /////////
//constructor - Flame
Flame::Flame(double StartX, double StartY, StudentWorld *sw)
:Projectile(StartX, StartY, IID_FLAME, IID_FLAME, sw){}

int Flame::doSomething()
{
    if(Projectile::doSomething()== 0)   //if projectile is still alive
    {
        if ( getSW()->damage(this) == -1) //if projectile do damage and player die
        {
            return -1; //return -1: player died
        }
    }
    return 0;   //return 0: defult
}
///////////////////////

/////// Vomit /////////
//constructor - Vomit
Vomit::Vomit(double StartX, double StartY, StudentWorld *sw)
:Projectile(StartX, StartY, IID_VOMIT, IID_VOMIT, sw){}

int Vomit::doSomething()
{
    if(Projectile::doSomething()== 0)
    {
        for (list<Actor*>::iterator lt = getSW()->GetList().begin(); lt != getSW()->GetList().end(); lt++)
        {
            if ((*lt)->getExistance() && (*lt)->infectable() && getSW()->overlap(*this, *(*lt)))    
				//if lt is an alive infectable actor and overlaps with Vomit
            {	
				if (!((*lt)->getInfectionStatus()))
				{
					(*lt)->setInfectionStatus(true);
					getSW()->playSound(SOUND_CITIZEN_INFECTED);
				}
            }
        }

		if (getSW()->overlap(*this, *(getSW()->getPlayer())))//player got infect
		{
			if (!(getSW()->getPlayer()->getInfectionStatus()))	//if player is not infected
				getSW()->getPlayer()->setInfectionStatus(true);
		}
    }
    return 0;   //return 0: defult
}
///////////////////////
////////////////////////////////////////

/////    Goodie   /////
//constructor - Goodie
Goodie::Goodie(double StartX, double StartY, int imageID, int Type, StudentWorld *sw)
	:Actor(imageID, Type, SPRITE_WIDTH * StartX, SPRITE_HEIGHT * StartY, right, 1, sw, false, true, false) {}

int Goodie::doSomething()
{
	if (!getExistance()) return -1;  //Gooide is "dead"
		
	if (getSW()->overlap(*this, *(getSW()->getPlayer())))	//player pickup the Goodie
	{
		changeExistance();						//set to "dead"
		getSW()->increaseScore(50);				//earn 50 pt
		getSW()->playSound(SOUND_GOT_GOODIE);
		return 1;	//return 1: pickup Goodie
	}
		
	return 0;   //return 0: defult
}
Goodie::~Goodie()
{
}
///////////////////////

//  Vaccine_Goodie   //
//constructor - Vaccine_Goodie
Vaccine_Goodie::Vaccine_Goodie(double StartX, double StartY, StudentWorld *sw)
	:Goodie(StartX, StartY, IID_VACCINE_GOODIE, IID_VACCINE_GOODIE, sw) {}

int Vaccine_Goodie::doSomething()
{
	if (Goodie::doSomething() == 1)   //return 1: pickup Goodie
	{
		getSW()->getPlayer()->incNumVaccine(1);	//num of Vaccine inc by 1
	}
	return 0;   //return 0: defult
}
///////////////////////

//// GasCan_Goodie ////
//constructor - GasCan_Goodi
GasCan_Goodie::GasCan_Goodie(double StartX, double StartY, StudentWorld *sw)
	:Goodie(StartX, StartY, IID_GAS_CAN_GOODIE, IID_GAS_CAN_GOODIE, sw) {}

int GasCan_Goodie::doSomething()
{
	if (Goodie::doSomething() == 1)   //return 1: pickup Goodie
	{
		getSW()->getPlayer()->incNumFCharge(5);	//num of FireCharge inc by 5
	}
	return 0;   //return 0: defult
}
///////////////////////

//  Landmine_Goodie  //
//constructor - Landmine_Goodie
Landmine_Goodie::Landmine_Goodie(double StartX, double StartY, StudentWorld *sw)
	:Goodie(StartX, StartY, IID_LANDMINE_GOODIE, IID_LANDMINE_GOODIE, sw) {}

int Landmine_Goodie::doSomething()
{
	if (Goodie::doSomething() == 1)   //return 1: pickup Goodie
	{
		getSW()->getPlayer()->incNumLandmine(2);	//num of Landmine inc by 2
	}
	return 0;   //return 0: defult
}
///////////////////////