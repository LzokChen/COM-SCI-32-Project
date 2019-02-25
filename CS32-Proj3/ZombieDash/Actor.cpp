#include "Actor.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include <string>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

////////////////////////////////////////
///////   Actor  //////
//constructor - Actor
Actor::Actor(int imageID, double StartX, double StartY, int StartDirection, int depth, StudentWorld *sw ,
	bool is_Block, bool block_Flame, bool damageable, bool  infectable, bool triggersActiveLanmines)
	: GraphObject(imageID, StartX, StartY, StartDirection, depth)
{
	SW = sw;
	m_isBlock = is_Block;
	m_blockFlame = block_Flame;
	m_damageable = damageable;
	m_infectable = infectable;
	m_triggersActiveLanmines = triggersActiveLanmines;
	m_existence = true;
	m_infectionStatus = false;
	
}

bool Actor::getExistance() const
{
	return m_existence;
};

void Actor::setExistance(bool existance)
{
	m_existence = existance;
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

void Actor::useExit()
{
}

void Actor::getDamage()
{
	if (m_damageable)
		m_existence = false;
}

StudentWorld * Actor::getSW() const
{
	return SW;
}

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
	return  m_infectable;
}

bool Actor::triggersActiveLandmines() const
{
	return m_triggersActiveLanmines;
}






Actor:: ~Actor() {};
///////////////////////
////////////////////////////////////////

////////////////////////////////////////
////// Inanimate //////
//constructor - Inanimate
Inanimate::Inanimate(int imageID, double StartX, double StartY, int StartDirection, int depth, StudentWorld *sw, bool is_Block, bool block_Flame, bool damageable)
	:Actor::Actor(imageID, SPRITE_WIDTH * StartX, SPRITE_HEIGHT * StartY, StartDirection, depth, sw, is_Block, block_Flame, damageable, false, false) {}

Inanimate::~Inanimate()
{
}

///////   Wall   //////
//constructor - Wall
Wall::Wall(double StartX, double StartY, StudentWorld *sw)
	:Inanimate(IID_WALL, StartX, StartY, right, 0, sw, true, true, false) {}


int Wall::doSomething() { return 0; } //do nothing, return 0: defult


///////////////////////
////////////////////////////////////////

///////   Exit   //////
Exit::Exit(double StartX, double StartY, StudentWorld *sw)
	:Inanimate(IID_EXIT, StartX, StartY, right, 1, sw, false, true, false){}

int Exit::doSomething() {

	for (list<Actor*>::iterator lt = getSW()->getList().begin(); lt != getSW()->getList().end(); lt++)
	{
		if ((*lt)->getExistance() && getSW()->ActorOverlap(*this, *(*lt)))
			(*lt)->useExit();							//if the an alive actor steps on exist 		
	}
	
	if (getSW()->ActorOverlap(*this, *getSW()->getPlayer()) && getSW()->getNumCitizen() == 0)
		return 1;										//player useExit, return 1 next level
	
	return 0;
}
///////////////////////
////////////////////////////////////////

///////   Pit    //////
//constructor - Pit
Pit::Pit(double StartX, double StartY, StudentWorld *sw)
	:Inanimate(IID_PIT, StartX, StartY, 0, right, sw, false, false, false) {}

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
	:Actor(imageID, SPRITE_WIDTH * StartX, SPRITE_HEIGHT * StartY, right, 0, sw, true, false, true, true, true),
		infectionCount(0) {}

int Human::doSomething()
{
	bool fullInfected = false;
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
			setExistance(false);		//set existance to false
			return -1;					//Human is dead(fully infected), return -1	
		}
	}else infectionCount = 0;

	return 0;		//return 0: defalut
   
}


int Human::getInfectionCount() const
{
	return infectionCount;
}

bool Human::triggersZombieVomit() const
{
	return true;
}

bool Human::triggersLandmines() const
{
	return true;
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
	if (Human::doSomething() == -1)	//Penelope died due to infection
		return -1;	//Human is dead => Penelope is dead, return -1	


	//check key pressed
	int key;
	if (getSW()->getKey(key))
	{
		switch (key)
		{
			//fire
		case KEY_PRESS_SPACE:
			if (numFCharge > 0)
			{
				fire();
				incNumFCharge(-1);
			}
			break;
			//plant the landmine
		case KEY_PRESS_TAB:
            if (numLandmine > 0)
            {
                getSW()->getList().push_back(new Landmine(getX(), getY(), getSW()));
                incNumLandmine(-1);
            }
			break;
			//use Vaccine
		case KEY_PRESS_ENTER:
			if (numVaccine > 0)
			{
				setInfectionStatus(false);
				incNumVaccine(-1);
			}
			break;
			//move Left
		case KEY_PRESS_LEFT:
			setDirection(left);
			if(getSW()->accessible(this, getX() - 4, getY()))
				moveTo(getX() - 4, getY());
			break;
			//move Right
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if (getSW()->accessible(this, getX() + 4, getY()))
				moveTo(getX() + 4, getY());
			break;
			//move Up
		case KEY_PRESS_UP:
			setDirection(up);
			if (getSW()->accessible(this, getX(), getY() + 4))
				moveTo(getX(), getY() + 4);
			break;
			//move Down
		case KEY_PRESS_DOWN:
			setDirection(down);
			if (getSW()->accessible(this, getX(), getY() - 4))
				moveTo(getX(), getY() - 4);
			break;
		default:
			break;
		}
	}
	return 0;	//return 0: defult
}

void Penelope::getDamage()
{
	getSW()->decLives();					//deduct live by 1
	getSW()->playSound(SOUND_PLAYER_DIE);	//sound effect
	setExistance(false);					//set existance to false
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
		//determinate dir_X and Dir_Y
		double dir_X, dir_Y;
		switch (getDirection())
		{
		case up:
			dir_X = getX();
			dir_Y = getY() + i * SPRITE_HEIGHT;
			break;

		case down:
			dir_X = getX();
			dir_Y = getY() - i * SPRITE_HEIGHT;
			break;

		case left:
			dir_X = getX() - i * SPRITE_WIDTH;
			dir_Y = getY();
			break;

		case right:
			dir_X = getX() + i * SPRITE_WIDTH;
			dir_Y = getY();
			break;
		}

		if (getSW()->flameable(dir_X, dir_Y))
			getSW()->getList().push_back(new Flame(dir_X / SPRITE_WIDTH, dir_Y / SPRITE_HEIGHT, getDirection(), getSW()));
		else return;
	}

}

////// Citizen  //////
//constructor - Citizen
Citizen::Citizen(double StartX, double StartY, StudentWorld *sw)
	:Human(StartX, StartY, IID_CITIZEN, sw){}

int Citizen::doSomething()
{
	if (Human::doSomething() == -1)
		cerr << "Citizen got fully infective" << endl;
	if(getInfectionCount() == 1)
		getSW()->playSound(SOUND_CITIZEN_INFECTED);
	return 0;		//return 0: defult
}

void Citizen::getDamage()
{
	getSW()->increaseScore(-1000);			//player lose 1000 pt
	getSW()->changeNumCitizen(-1);			//deduct number of citizen by 1;
	getSW()->playSound(SOUND_CITIZEN_DIE);	//sound effect
	setExistance(false); 					//set existance to false
}

void Citizen::useExit()
{
	getSW()->increaseScore(500);				//add 500 pt
	setExistance(false);						//setExistance to dead
	getSW()->changeNumCitizen(-1);				//deduct number of citizen by 1;
	getSW()->playSound(SOUND_CITIZEN_SAVED);	//play the sound effect
}

///////////////////////
////////////////////////////////////////

////// Projectile /////
//constructor - Projectile
Projectile::Projectile(double StartX, double StartY, int StartDirection, int imageID, StudentWorld *sw)
	:Inanimate(imageID, StartX, StartY, StartDirection, 0, sw, false, false, false), tickcount(2) {}

int Projectile::doSomething()
{
    if(!getExistance()) return -1;  //projectile is "dead"
    
	if (tickcount <= 0)
	{
		setExistance(false);
		return -1;
	}
	else
		tickcount--;
    return 0;   //return 0: defult
}
Projectile::~Projectile()
{
}
///////////////////////

/////// Flame /////////
//constructor - Flame
Flame::Flame(double StartX, double StartY, int StartDirection, StudentWorld *sw)
	:Projectile(StartX, StartY, StartDirection, IID_FLAME, sw) {
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
Vomit::Vomit(double StartX, double StartY, int StartDirection, StudentWorld *sw)
:Projectile(StartX, StartY, StartDirection, IID_VOMIT, sw){}

int Vomit::doSomething()
{
    if(Projectile::doSomething()== 0)
    {
        for (list<Actor*>::iterator lt = getSW()->getList().begin(); lt != getSW()->getList().end(); lt++)
        {
            if ((*lt) != this && (*lt)->getExistance() && (*lt)->infectable() && getSW()->ActorOverlap(*this, *(*lt)))    
				//if lt is an alive infectable actor and overlaps with Vomit
            {	
				if (!((*lt)->getInfectionStatus()))
					(*lt)->setInfectionStatus(true);
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
	:Inanimate(imageID, StartX, StartY, right, 1, sw, false, false, true){}

int Goodie::doSomething()
{
	if (!getExistance()) return 0;  //Gooide is "dead"
		
	if (getSW()->ActorOverlap(*this, *(getSW()->getPlayer())))	//player pickup the Goodie
	{
		pickup();
		setExistance(false);						//set to "dead"
		getSW()->increaseScore(50);					//player earn 50 pt
		getSW()->playSound(SOUND_GOT_GOODIE);		//sound effect
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

void Vaccine_Goodie::pickup()
{
	getSW()->getPlayer()->incNumVaccine(1);	//num of Vaccine inc by 1
}

///////////////////////

//// GasCan_Goodie ////
//constructor - GasCan_Goodie
GasCan_Goodie::GasCan_Goodie(double StartX, double StartY, StudentWorld *sw)
	:Goodie(StartX, StartY, IID_GAS_CAN_GOODIE, sw) {}

void GasCan_Goodie::pickup()
{
	getSW()->getPlayer()->incNumFCharge(5);	//num of FireCharge inc by 5
}
///////////////////////

//  Landmine_Goodie  //
//constructor - Landmine_Goodie
Landmine_Goodie::Landmine_Goodie(double StartX, double StartY, StudentWorld *sw)
	:Goodie(StartX, StartY, IID_LANDMINE_GOODIE, sw) {}

void Landmine_Goodie::pickup()
{
	getSW()->getPlayer()->incNumLandmine(2);	//num of Landmine inc by 2
}
///////////////////////

////////////////////////////////////////

/////    Landmine   /////
//constructor - Landmine
Landmine::Landmine(double StartX, double StartY, StudentWorld *sw)
	:Inanimate(IID_LANDMINE, StartX / SPRITE_WIDTH, StartY / SPRITE_HEIGHT, right, 1, sw, false, false, true), tickcount(60), activeStatus(false) 
{
	cerr << "plants a landmine at (" << getX() << ", " << getY() << ")." << endl;
}

int Landmine::doSomething()
{
    if (!getExistance()) return 0; //the Landmine is not alive, ruturn 0 by defult;
    
    if(activeStatus)    //active
    {
		for (list<Actor*>::iterator lt = getSW()->getList().begin(); lt != getSW()->getList().end(); lt++)
		{
			if ((*lt)->getExistance() && getSW()->ActorOverlap(*this, *(*lt)) && (*lt)->triggersActiveLandmines())
			{
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
	
	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			if (getSW()->flameable(getX() + x * SPRITE_WIDTH, getY() + y * SPRITE_HEIGHT))
			{
				getSW()->getList().push_back(new Flame(getX() / SPRITE_WIDTH + x, getY() / SPRITE_HEIGHT + y, up, getSW()));
			}
		}
	}
	getSW()->getList().push_back(new Pit(getX() / SPRITE_WIDTH, getY() / SPRITE_HEIGHT, getSW()));				//introduce a pit at (x,y)
	
}
///////////////////////

////////////////////////////////////////

/////     Zombie    /////
//constructor - Zombie
Zombie::Zombie(double StartX, double StartY, StudentWorld * sw)
	:Actor(IID_ZOMBIE, SPRITE_WIDTH * StartX, SPRITE_HEIGHT * StartY, right, 0, sw, true, false, true, false, true)
	, moveDistance(0), tickcount(0) {}

int Zombie::doSomething()
{
	if (!getExistance()) return 0;	//the Zombie is dead

	tickcount++;
	if (tickcount % 2 != 0)	//not in parlyzed Tick
	{
		if (checkFrontandVomit()) return 0;

		if (moveDistance == 0)
		{
			moveDistance += randInt(3, 10);
			selectDirection();				//call derive class zombie's selectDirection
		}

		if (move())
			moveDistance -= 1;
		else
			moveDistance = 0;
	}
	return 0;
}

void Zombie::getDamage()
{
	getSW()->playSound(SOUND_ZOMBIE_DIE);	//sound effect
	setExistance(false);					//set existance to dead
}

void Zombie::randomDirection()
{
	int Dir = randInt(1, 4);
	switch (Dir)
	{
	case 1: setDirection(up);
		break;
	case 2:	setDirection(down);
		break;
	case 3: setDirection(left);
		break;
	case 4: setDirection(right);
		break;
	}
}

bool Zombie::checkFrontandVomit()
{
	double dir_X, dir_Y;
	switch (getDirection())
	{
	case up:
		dir_X = getX();
		dir_Y = getY() + SPRITE_HEIGHT;
		break;

	case down:
		dir_X = getX();
		dir_Y = getY() - SPRITE_HEIGHT;
		break;

	case left:
		dir_X = getX() - SPRITE_WIDTH;
		dir_Y = getY();
		break;

	case right:
		dir_X = getX() + SPRITE_WIDTH;
		dir_Y = getY();
		break;
	}

	bool findPerson = false;

	for (list<Actor*>::iterator lt = getSW()->getList().begin(); lt != getSW()->getList().end(); lt++)
	{
		if ((*lt)->getExistance() && (*lt)->infectable() && getSW()->overlap(dir_X, dir_Y, (*lt)->getX(), (*lt)->getY()))
		{
			findPerson = true;
			break;
		}
		
	}

	if (getSW()->overlap(dir_X, dir_Y, getSW()->getPlayer()->getX(), getSW()->getPlayer()->getY()))
	{
		findPerson = true;
	}

	if (findPerson && randInt(1, 3) == 1)
	{
		getSW()->getList().push_front(new Vomit(dir_X / SPRITE_WIDTH, dir_Y / SPRITE_HEIGHT, getDirection(), getSW()));
		getSW()->playSound(SOUND_ZOMBIE_VOMIT);
		return true;
	}
	return false;
}

Zombie::~Zombie()
{
}

bool Zombie::move()
{
	switch (getDirection())
	{
		//move Up
	case up:
		if (getSW()->accessible(this, getX(), getY() + 1))
		{
			moveTo(getX(), getY() + 1);
			return true;
		}
		break;
		//move Down
	case down:
		if (getSW()->accessible(this, getX(), getY() - 1))
			moveTo(getX(), getY() - 1);
		break;
		//move left
	case left:
		if (getSW()->accessible(this, getX() - 1, getY()))
		{
			moveTo(getX() - 1, getY());
			return true;
		}
		break;
		//move Right
	case right:
		if (getSW()->accessible(this, getX() + 1, getY()))
		{
			moveTo(getX() + 1, getY());
			return true;
		}
		break;
	}
	return false;
}
///////////////////////

/////  Dumb_Zombie /////
//constructor - Dumb_Zombie
Dumb_Zombie::Dumb_Zombie(double StartX, double StartY, StudentWorld * sw)
	:Zombie(StartX, StartY, sw)
{
	if (randInt(1, 10) == 1)
		carVaccine = true;
	else carVaccine = false;
}

void Dumb_Zombie::getDamage()
{
	Zombie::getDamage();
	getSW()->increaseScore(1000);
	if (carVaccine)
		dropVaccineGooide();
}

void Dumb_Zombie::selectDirection()
{
	randomDirection();
}

void Dumb_Zombie::dropVaccineGooide()
{
	//select random direction
	randomDirection();		

	//determinate dir_X and Dir_Y
	double dir_X, dir_Y;
	switch (getDirection())	
	{
	case up:
		dir_X = getX();
		dir_Y = getY() + SPRITE_HEIGHT;
		break;

	case down:
		dir_X = getX();
		dir_Y = getY() - SPRITE_HEIGHT;
		break;

	case left:
		dir_X = getX() - SPRITE_WIDTH;
		dir_Y = getY();
		break;

	case right:
		dir_X = getX() + SPRITE_WIDTH;
		dir_Y = getY();
		break;
	}


	//check if other objcet in the game would overlap with an object created at those coordinate
	for (list<Actor*>::iterator lt = getSW()->getList().begin(); lt != getSW()->getList().end(); lt++)
	{
		if ((*lt)->getExistance() && getSW()->overlap(dir_X, dir_Y, (*lt)->getX(), (*lt)->getY()))
		{
			return;
		}

	}

	//check if player would overlap with an object created at those coordinate
	if (getSW()->overlap(dir_X, dir_Y, getSW()->getPlayer()->getX(), getSW()->getPlayer()->getY()))
	{
		return;
	}
	
	//introduce an new Vaccine Gooide object
	getSW()->getList().push_back(new Vaccine_Goodie(dir_X / SPRITE_WIDTH, dir_Y / SPRITE_HEIGHT, getSW()));
	cerr << "dumb zombie dropped a vaccine goodie at (" << dir_X << ", " << dir_Y << ")" << endl;

}


Smart_Zombie::Smart_Zombie(double StartX, double StartY, StudentWorld * sw)
	:Zombie(StartX, StartY, sw)
{
}

void Smart_Zombie::getDamage()
{
	Zombie::getDamage();
	getSW()->increaseScore(2000);
}

void Smart_Zombie::selectDirection()
{
}
