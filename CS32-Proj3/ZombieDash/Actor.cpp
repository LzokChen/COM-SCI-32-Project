#include "Actor.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include <string>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

////////////////////////////////////////
///////   Actor  //////
//constructor - Actor
Actor::Actor(int imageID, double StartX, double StartY, int StartDirection,
	int depth, StudentWorld *sw, bool is_Block, bool block_Flame, bool damageable, bool  infectable)
	: GraphObject(imageID, StartX, StartY, StartDirection, depth)
{
	m_existence = true;
	m_isBlock = is_Block;
	m_blockFlame = block_Flame;
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

bool Actor::blockFlame() const
{
	return m_blockFlame;
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

void Actor::getDamage()
{
	if(damageable())
		m_existence = false;        //setExistance to dead
}

void Actor::setExistance(bool existance)
{
	m_existence = existance;
}

StudentWorld * Actor::getSW() const
{
	return SW;
}

Actor:: ~Actor() {};
///////////////////////
////////////////////////////////////////

////////////////////////////////////////
////// Inanimate //////
//constructor - Inanimate
Inanimate::Inanimate(int imageID, double StartX, double StartY, int depth, StudentWorld *sw,bool is_Block, bool block_Flame, bool damageable)
	:Actor::Actor(imageID, SPRITE_WIDTH * StartX, SPRITE_HEIGHT * StartY, right, depth, sw, is_Block, block_Flame, damageable, false) {}

bool Inanimate::isHuman() const
{
	return false;
}

bool Inanimate::isZombie() const
{
	return false;
}

Inanimate::~Inanimate()
{
}

///////   Wall   //////
//constructor - Wall
Wall::Wall(double StartX, double StartY, StudentWorld *sw)
	:Inanimate(IID_WALL, StartX, StartY, 0, sw, true, true, false) {}

int Wall::doSomething() { return 0; } //do nothing, return 0: defult


///////////////////////
////////////////////////////////////////

///////   Exit   //////
Exit::Exit(double StartX, double StartY, StudentWorld *sw)
	:Inanimate(IID_EXIT, StartX, StartY, 1, sw, false, false, false){}

int Exit::doSomething() {

	for (list<Actor*>::iterator lt = getSW()->getList().begin(); lt != getSW()->getList().end(); lt++)
	{
		if ((*lt)->getExistance() && (*lt)-> isHuman())	
			//if the actor is an alive "Human" - which is Citizen
		{
			if (getSW()->ActorOverlap(*this, *(*lt)))	//if "Citizen" and "Exit" are ActorOverlap
			{
				getSW()->increaseScore(500);	//add 500 pt
				(*lt)->setExistance(false);		//setExistance to dead
				getSW()->changeNumCitizen(-1);	//deduct number of citizen by 1;
				getSW()->playSound(SOUND_CITIZEN_SAVED); //play the sound effect
			}
		}
	}

	if (getSW()->ActorOverlap(*this, *(getSW()->getPlayer())) && getSW()->getNumCitizen() == 0)
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
	:Inanimate(IID_PIT, StartX, StartY, 0, sw, false, false, false) {}

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
Human::Human(double StartX, double StartY, int imageID, StudentWorld *sw)
	:Actor(imageID, SPRITE_WIDTH * StartX, SPRITE_HEIGHT * StartY, right, 0, sw, true, false, true, true),
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
			return 1;		//Human is dead(fully infected), return 1	
		}
	}
	else infectionCount = 0;

    return 0;   //return 0: defalut
}

int Human::getInfectionCount() const
{
	return infectionCount;
}

bool Human::isHuman() const
{
	return true;
}

bool Human::isZombie() const
{
	return false;
}

Human::~Human()
{
}

///////////////////////

////// Penelope  //////
//constructor - Penelope
Penelope::Penelope(double StartX, double StartY, StudentWorld *sw)
	:Human(StartX, StartY, IID_PLAYER, sw), numLandmine(110), numFCharge(110), numVaccine(110){}

int Penelope::doSomething()
{
	if (Human::doSomething() != 0)	return -1;	//Human is dead => Penelope is dead, return -1	
	
	//check key pressed
	int key;
	if (getSW()->getKey(key))
	{
		switch (key)
		{
		case KEY_PRESS_SPACE:
			if (numFCharge > 0)
			{
				fire();
				incNumFCharge(-1);
			}
			break;
		case KEY_PRESS_TAB:
            if (numLandmine > 0)
            {
                getSW()->getList().push_back(new Landmine(getX(), getY(), getSW()));
                incNumLandmine(-1);
            }
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



void Penelope::fire() const
{
	getSW()->playSound(SOUND_PLAYER_FIRE);
	
	for (int i = 1; i <= 3; i++)
	{
		switch (getDirection())
		{
		case up:
			if(getSW()->flameable(getX(), getY() + i * SPRITE_HEIGHT))
				getSW()->getList().push_back(new Flame(getX() / SPRITE_WIDTH, getY() / SPRITE_HEIGHT + i, getSW()));
			else return;
			break;

		case down:
			if (getSW()->flameable(getX(), getY() - i * SPRITE_HEIGHT))
				getSW()->getList().push_back(new Flame(getX() / SPRITE_WIDTH, getY() / SPRITE_HEIGHT - i, getSW()));
			else return;
			break;

		case left:
			if (getSW()->flameable(getX() - i * SPRITE_WIDTH, getY()))
				getSW()->getList().push_back(new Flame(getX() / SPRITE_WIDTH - i, getY() / SPRITE_HEIGHT, getSW()));
			else return;
			break;

		case right:
			if (getSW()->flameable(getX() + (i * SPRITE_WIDTH), getY()))
				getSW()->getList().push_back(new Flame(getX() / SPRITE_WIDTH + i, getY() / SPRITE_HEIGHT, getSW()));
			else return;
			break;
		}
	}

		

}

////// Citizen  //////
//constructor - Citizen
Citizen::Citizen(double StartX, double StartY, StudentWorld *sw)
	:Human(StartX, StartY, IID_CITIZEN, sw){}

int Citizen::doSomething()
{
	return 0;		//return 0: defult
}

void Citizen::getDamage()
{
	getSW()->increaseScore(-1000);			//lose 1000 pt
	getSW()->changeNumCitizen(-1);			//deduct number of citizen by 1;
	getSW()->playSound(SOUND_CITIZEN_DIE);	//sound effect
	Actor::getDamage();
}

///////////////////////
////////////////////////////////////////

////// Projectile /////
//constructor - Projectile
Projectile::Projectile(double StartX, double StartY, int imageID, StudentWorld *sw)
	:Inanimate(imageID, StartX, StartY, 0, sw, false, false, false), tickcount(2) {}

int Projectile::doSomething()
{
    if(!getExistance()) return -1;  //projectile is "dead"
    
	if (tickcount > 0)
	{
		
		tickcount--;
		return 0;
	}
	else
		setExistance(false);
    return 0;   //return 0: defult
}
Projectile::~Projectile()
{
}
///////////////////////

/////// Flame /////////
//constructor - Flame
Flame::Flame(double StartX, double StartY, StudentWorld *sw)
	:Projectile(StartX, StartY, IID_FLAME, sw) {
	//cerr << "flame at: (" << getX() << ", " << getY() <<")"<< endl;
}

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
:Projectile(StartX, StartY, IID_VOMIT, sw){}

int Vomit::doSomething()
{
    if(Projectile::doSomething()== 0)
    {
        for (list<Actor*>::iterator lt = getSW()->getList().begin(); lt != getSW()->getList().end(); lt++)
        {
            if ((*lt)->getExistance() && (*lt)->infectable() && getSW()->ActorOverlap(*this, *(*lt)))    
				//if lt is an alive infectable actor and overlaps with Vomit
            {	
				if (!((*lt)->getInfectionStatus()))
				{
					(*lt)->setInfectionStatus(true);
					getSW()->playSound(SOUND_CITIZEN_INFECTED);
				}
            }
        }

		if (getSW()->ActorOverlap(*this, *(getSW()->getPlayer())))//player got infect
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
Goodie::Goodie(double StartX, double StartY, int imageID, StudentWorld *sw)
	:Inanimate(imageID, StartX, StartY, 1, sw, false, false, true){}

int Goodie::doSomething()
{
	if (!getExistance()) return -1;  //Gooide is "dead"
		
	if (getSW()->ActorOverlap(*this, *(getSW()->getPlayer())))	//player pickup the Goodie
	{
		setExistance(false);						//set to "dead"
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
	:Goodie(StartX, StartY,  IID_VACCINE_GOODIE, sw) {}

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
	:Goodie(StartX, StartY, IID_GAS_CAN_GOODIE, sw) {}

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
	:Goodie(StartX, StartY, IID_LANDMINE_GOODIE, sw) {}

int Landmine_Goodie::doSomething()
{
	if (Goodie::doSomething() == 1)   //return 1: pickup Goodie
	{
		getSW()->getPlayer()->incNumLandmine(2);	//num of Landmine inc by 2
	}
	return 0;   //return 0: defult
}
///////////////////////

////////////////////////////////////////

/////    Landmine   /////
//constructor - Landmine
Landmine::Landmine(double StartX, double StartY, StudentWorld *sw)
	:Inanimate(IID_LANDMINE, StartX / SPRITE_WIDTH, StartY / SPRITE_HEIGHT, 1, sw, false, false, true), tickcount(60), activeStatus(false) 
{
	cerr << "place a landmine at (" << getX() << ", " << getY() << ")." << endl;
}

int Landmine::doSomething()
{
    if (!getExistance()) return 0; //the Landmine is not alive, ruturn 0 by defult;
    
    if(activeStatus)    //active
    {
		for (list<Actor*>::iterator lt = getSW()->getList().begin(); lt != getSW()->getList().end(); lt++)
		{
			if ((*lt)->getExistance() && getSW()->ActorOverlap(*this, *(*lt)) && ((*lt)->isHuman() || (*lt)->isZombie()))
			{
				cerr << "trigger from dosomething" << endl;
				trigger();              
				//if a alive damagable overlaps with(step on) an active landmine, trigger the landmine
			}
		}

		if (getSW()->getPlayer()->getExistance() && getSW()->ActorOverlap(*this, *getSW()->getPlayer()))
			trigger();
    }
	else //not active
	{
		tickcount -= 1;
		if (tickcount == 0) activeStatus = true;   //the safety tickcount is 0, the landmaine is active
	}
    
    
        
    
    return 0; //return 0: defult
}

void Landmine::getDamage()
{
	trigger();
}

void Landmine::trigger()
{
	if (!getExistance()) return;
	cerr << "trigger the lanmine: (" << getX() << ", " << getY() << ")." << endl;
    setExistance(false);							//set eixstance to "dead"
	getSW()->playSound(SOUND_LANDMINE_EXPLODE);		//play sound effect
	//double x = getX() / SPRITE_WIDTH;
	//double y = getY() / SPRITE_HEIGHT;
	
	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			if (getSW()->flameable(getX() + x * SPRITE_WIDTH, getY() + y * SPRITE_HEIGHT))
			{
				getSW()->getList().push_back(new Flame(getX() / SPRITE_WIDTH + x, getY() / SPRITE_HEIGHT + y, getSW()));
			}
		}
	}
	getSW()->getList().push_back(new Pit(getX() / SPRITE_WIDTH, getY() / SPRITE_HEIGHT, getSW()));				//introduce a pit at (x,y)
	
}
///////////////////////
