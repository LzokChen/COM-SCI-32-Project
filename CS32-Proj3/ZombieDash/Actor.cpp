#include "Actor.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include <string>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

////////////////////////////////////////
///////   Actor  //////
//constructor - Actor
Actor::Actor(int imageID, double StartX, double StartY, int StartDirection, int depth, StudentWorld *sw ,
	bool is_Block, bool block_Flame, bool damageable, bool  infectable, bool infectionSource, bool triggersActiveLanmines)
	: GraphObject(imageID, StartX, StartY, StartDirection, depth)
{
	SW = sw;
	m_isBlock = is_Block;
	m_blockFlame = block_Flame;
	m_damageable = damageable;
	m_infectable = infectable;
	m_infectionSource = infectionSource;
	m_triggersActiveLanmines = triggersActiveLanmines;
	m_existence = true;
	m_infectionStatus = false;
	
}

bool Actor::getExistance() const			//get the existance status of actor
{
	return m_existence;
};

void Actor::setExistance(bool existance)	//change the existance status of actor
{
	m_existence = existance;
}

bool Actor::getInfectionStatus() const		//get the infection status of actor
{
	return m_infectionStatus;
}
	
void Actor::setInfectionStatus(bool k)		//change the existance status of actor (if it is infectable)
{
	if (m_infectable)
		m_infectionStatus = k;
}

void Actor::useExit()						//if actor overlap with the Exit object
{
}

void Actor::getDamage()						//if the actor get damage
{
	if (m_damageable)
		m_existence = false;
}

StudentWorld * Actor::getSW() const			//get the studentworld that actor belongs to
{
	return SW;
}

bool Actor::isBlock() const					//determinate if the actor is an blocklike object
{
	return m_isBlock;
}

bool Actor::blockFlame() const				//determinate if the actor block the flame
{
	return m_blockFlame;
}

bool Actor::damageable() const				//determinate if the actor is damageable
{
	return m_damageable;
}

bool Actor::infectable() const				//determinate if the actor is infectable
{
	return  m_infectable;
}

bool Actor::infectionSource() const			//determinate if the actor is an infection source
{
	return m_infectionSource;
}

bool Actor::triggersActiveLandmines() const	//determinate if th actor can trigger an active landmine
{
	return m_triggersActiveLanmines;
}

Actor:: ~Actor() {};						//destructor
///////////////////////
////////////////////////////////////////

////////////////////////////////////////
////// Inanimate //////
//constructor - Inanimate
Inanimate::Inanimate(int imageID, double StartX, double StartY, int StartDirection, int depth, StudentWorld *sw, bool is_Block, bool block_Flame, bool damageable)
	:Actor::Actor(imageID, SPRITE_WIDTH * StartX, SPRITE_HEIGHT * StartY, StartDirection, depth, sw, is_Block, block_Flame, damageable, false, false, false) {}

Inanimate::~Inanimate()
{
}

///////   Wall   //////
//constructor - Wall
Wall::Wall(double StartX, double StartY, StudentWorld *sw)
	:Inanimate(IID_WALL, StartX, StartY, right, 0, sw, true, true, false) {}


void Wall::doSomething() {} //do nothing


///////////////////////
////////////////////////////////////////

///////   Exit   //////
Exit::Exit(double StartX, double StartY, StudentWorld *sw)
	:Inanimate(IID_EXIT, StartX, StartY, right, 1, sw, false, true, false){}

void Exit::doSomething() {
	//check if any alive actor overlap with the exit
	for (list<Actor*>::iterator lt = getSW()->getList().begin(); lt != getSW()->getList().end(); lt++)
	{
		if ((*lt)->getExistance() && getSW()->ActorOverlap(*this, *(*lt)))
			(*lt)->useExit();							//if the an alive actor steps on exist 		
	}
	
	if (getSW()->getPlayer()->getExistance() && getSW()->ActorOverlap(*this, *getSW()->getPlayer()))	
		getSW()->getPlayer()->useExit();				//if the an alive player steps on exist 
}
///////////////////////
////////////////////////////////////////

///////   Pit    //////
//constructor - Pit
Pit::Pit(double StartX, double StartY, StudentWorld *sw)
	:Inanimate(IID_PIT, StartX, StartY, 0, right, sw, false, false, false) {}

void Pit::doSomething()
{	//Let the pit do damage to the alive damageable actor who overlap with it 
    getSW()->doDamage(this);
}
///////////////////////
////////////////////////////////////////

////// Projectile /////
//constructor - Projectile
Projectile::Projectile(double StartX, double StartY, int StartDirection, int imageID, StudentWorld *sw)
	:Inanimate(imageID, StartX, StartY, StartDirection, 0, sw, false, false, false), tickcount(2) {}

void Projectile::doSomething()
{
	if (!getExistance()) return;	//projectile is "dead"
	
	if (tickcount <= 0)				//check the tickcout
	{
		setExistance(false);
		return;
	}
	else
		tickcount--;

	action();						//do the certain action
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

void Flame::action()				
{
	getSW()->doDamage(this);		//Let the Flame do damage to the alive damageable actor who overlap with it 
}
///////////////////////

/////// Vomit /////////
//constructor - Vomit
Vomit::Vomit(double StartX, double StartY, int StartDirection, StudentWorld *sw)
	:Projectile(StartX, StartY, StartDirection, IID_VOMIT, sw) {}

void Vomit::action()	//infect the infectable actor (human) overlap with the vomit
{
	for (list<Actor*>::iterator lt = getSW()->getList().begin(); lt != getSW()->getList().end(); lt++)
	{
		if ((*lt) != this && (*lt)->getExistance() && (*lt)->infectable() && getSW()->ActorOverlap(*this, *(*lt)))
			//if lt is an alive infectable actor and overlaps with Vomit
		{
			if (!((*lt)->getInfectionStatus()))		//if actor is not infected
				(*lt)->setInfectionStatus(true);	//change the infection status to be true
		}
	}

	if (getSW()->ActorOverlap(*this, *(getSW()->getPlayer())))	//player got infect
	{
		if (!(getSW()->getPlayer()->getInfectionStatus()))		//if player is not infected
			getSW()->getPlayer()->setInfectionStatus(true);		//change the infection status to be true
	}
}
///////////////////////
////////////////////////////////////////

/////    Goodie   /////
//constructor - Goodie
Goodie::Goodie(double StartX, double StartY, int imageID, StudentWorld *sw)
	:Inanimate(imageID, StartX, StartY, right, 1, sw, false, false, true) {}

void Goodie::doSomething()
{
	if (!getExistance()) return;  //Gooide is "dead"

	if (getSW()->ActorOverlap(*this, *(getSW()->getPlayer())))	//player pickup the Goodie
	{
		pickup();									//do the pickup action: increase the number of certain item player have
		setExistance(false);						//set to "dead"
		getSW()->increaseScore(50);					//player earn 50 pt
		getSW()->playSound(SOUND_GOT_GOODIE);		//sound effect
	}
}

Goodie::~Goodie()
{
}
///////////////////////

//  Vaccine_Goodie   //
//constructor - Vaccine_Goodie
Vaccine_Goodie::Vaccine_Goodie(double StartX, double StartY, StudentWorld *sw)
	:Goodie(StartX, StartY, IID_VACCINE_GOODIE, sw) {}

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
	:Inanimate(IID_LANDMINE, StartX / SPRITE_WIDTH, StartY / SPRITE_HEIGHT, right, 1, sw, false, false, true), tickcount(30), activeStatus(false)
{
	cerr << "plants a landmine at (" << getX() << ", " << getY() << ")." << endl;
}

void Landmine::doSomething()
{
	if (!getExistance()) return; //the Landmine is not alive, ruturn 0 by defult;

	if (activeStatus)    //if the landmine is active
	{
		for (list<Actor*>::iterator lt = getSW()->getList().begin(); lt != getSW()->getList().end(); lt++)
		{
			//if a alive damagable overlaps with(step on) an active landmine, trigger the landmine
			if ((*lt)->getExistance() && getSW()->ActorOverlap(*this, *(*lt)) && (*lt)->triggersActiveLandmines())
			{
				trigger();
			}
		}
		
		//if player overlaps with(step on) an active landmine, trigger the landmine
		if (getSW()->getPlayer()->getExistance() && getSW()->ActorOverlap(*this, *getSW()->getPlayer()))
			trigger();
	}
	else //not active
	{
		tickcount -= 1;
		if (tickcount == 0) activeStatus = true;   //the safety tickcount is 0, the landmaine is active
	}
}

void Landmine::getDamage()	//if the landmine get damage (from flame), trigger the landmine
{
	trigger();
}

void Landmine::trigger()
{
	if (!getExistance()) return;					//if the landmine is "dead", return directly
	cerr << "trigger the lanmine: (" << getX() << ", " << getY() << ")." << endl;
	setExistance(false);							//set eixstance to "dead"
	getSW()->playSound(SOUND_LANDMINE_EXPLODE);		//play sound effect

	//introduce flames round it
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
//////////////////////////////////

///////  Agent   //////
//constructor - Agent
Agent::Agent(double StartX, double StartY, int imageID, StudentWorld * sw, bool infectable, bool infectionSource)
	: Actor(imageID, SPRITE_WIDTH * StartX, SPRITE_HEIGHT * StartY, right, 0, sw, true, false, true, infectable, infectionSource, true)
{}

//set the agnet direction toward to the target actor
void Agent::towardTo(Actor * Target)
{
	if (getX() == Target->getX())		//if same col
	{
		if (getY() < Target->getY())
			setDirection(up);
		else setDirection(down);

	}
	else if (getY() == Target->getY())	//if same row
	{
		if (getX() < Target->getX())
			setDirection(right);
		else setDirection(left);
	}
	else if (randInt(1, 2) == 1)			//randomly choose a direction : Vertically
	{
		if (getY() < Target->getY())		//if agent is below the target
			setDirection(up);
		else setDirection(down);
	}
	else									//	randomly choose a direction : Horizontally
	{
		if (getX() < Target->getX())		//if agent is on the left side of  the target
			setDirection(right);
		else setDirection(left);
	}
}

//try to move agent forword certain pixels in current direction
bool Agent::tryToMoveForward(double numPixels)
{
	switch (getDirection())
	{
		//move Up
	case up:
		if (getSW()->accessible(this, getX(), getY() + numPixels))
		{
			moveTo(getX(), getY() + numPixels);
			return true;
		}
		break;
		//move Down
	case down:
		if (getSW()->accessible(this, getX(), getY() - numPixels))
			moveTo(getX(), getY() - numPixels);
		break;
		//move left
	case left:
		if (getSW()->accessible(this, getX() - numPixels, getY()))
		{
			moveTo(getX() - numPixels, getY());
			return true;
		}
		break;
		//move Right
	case right:
		if (getSW()->accessible(this, getX() + numPixels, getY()))
		{
			moveTo(getX() + numPixels, getY());
			return true;
		}
		break;
	}
	return false;
}
//////////////////////////////////

///////  Human   //////
//constructor - Human
Human::Human(double StartX, double StartY, int imageID, StudentWorld *sw)
	:Agent(StartX, StartY, imageID, sw, true, false), infectionCount(0) {}

void Human::doSomething()
{
	//check existance status
	if (!getExistance())
		return;			//Human is dead, return

	//check infection status
	if (getInfectionStatus())
	{
		infectionCount += 1;
		if (infectionCount >= 500)		//Hunman's infection level reached max
			setExistance(false);		//Human is dead(fully infected), set existance to false
        
	}else infectionCount = 0;

    action();
   
}

int Human::getInfectionCount() const	//get the infection cout of human
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
	:Human(StartX, StartY, IID_PLAYER, sw), numLandmine(0), numFCharge(0), numVaccine(0){}

//get user's input and to the certian action
void Penelope::action()
{
    if (!getExistance())
        return;
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
}

//if Penelope get Damage
void Penelope::getDamage()
{
	getSW()->decLives();					//deduct live by 1
	getSW()->playSound(SOUND_PLAYER_DIE);	//sound effect
	setExistance(false);					//set existance to false
}

//if Penelope overlap with the exit
void Penelope::useExit()
{
	if(getSW()->getNumCitizen() == 0)
		getSW()->goNextLevel();
}

int Penelope::getNumLandmine() const		//get the number of landmine Penelope have
{
	return numLandmine;
}

int Penelope::getNumFCharge() const			//get the number of Flame Charge Penelope have
{
	return numFCharge;
}

int Penelope::getNmVaccine() const			//get the number of Vaccine Penelope have
{
	return numVaccine;
}

void Penelope::incNumLandmine(int i)		//increase the number of landmine Penelope have by i
{
	numLandmine += i;
}

void Penelope::incNumFCharge(int i)			//increase the number of Flame charge Penelope have by i
{
	numFCharge += i;
}

void Penelope::incNumVaccine(int i)			//increase the number of vaccine Penelope have by i
{
	numVaccine += i;
}
	
void Penelope::fire() const					//Penelope fire the flames
{
	getSW()->playSound(SOUND_PLAYER_FIRE);
	
	for (int i = 1; i <= 3; i++)
	{
		//determinate destination coordinate
		double dest_X = 0.0, dest_Y = 0.0;
		getSW()->determineDestination(this, i, dest_X, dest_Y);
		//check if we can introduce a flame at that destination corrdiante
		if (getSW()->flameable(dest_X, dest_Y))
			getSW()->getList().push_back(new Flame(dest_X / SPRITE_WIDTH, dest_Y / SPRITE_HEIGHT, getDirection(), getSW()));
		else return;
	}

}

////// Citizen  //////
//constructor - Citizen
Citizen::Citizen(double StartX, double StartY, StudentWorld *sw)
	:Human(StartX, StartY, IID_CITIZEN, sw), tickcount(0){}

void Citizen::action()
{
	
	if (!getExistance())	//if citizen is dead (due to infection)
	{
		cerr << "A Citizen got fully infective, ";
		getSW()->playSound(SOUND_ZOMBIE_BORN);
		getSW()->increaseScore(-1000);
		getSW()->changeNumCitizen(-1);

		//introduce a smart zombie or dumb zombie randomly
		int i = randInt(1, 10);
		if (i <= 3)
		{
			getSW()->getList().push_back(new Smart_Zombie(getX() / SPRITE_WIDTH, getY() / SPRITE_HEIGHT, getSW()));
			cerr << "an new Smart Zombie born" << endl;
		}
		else
		{
			getSW()->getList().push_back(new Dumb_Zombie(getX() / SPRITE_WIDTH, getY() / SPRITE_HEIGHT, getSW()));
			cerr << "an new Dumb Zombie born" << endl;
		}
		return;
	}
    
	//play the sound effect when the citizen get infected at frist time
    if(getInfectionCount() == 1)
        getSW()->playSound(SOUND_CITIZEN_INFECTED);

	tickcount++;
	if (tickcount % 2 == 0)	return;	//in parlyzed Tick

	double dist_p = getSW()->getDistance(getX(), getY(), getSW()->getPlayer());	//get the distance to the player
	double dist_z = 0.0;
	Actor * nearestZombie = getSW()->nearestZombie(getX(), getY(), dist_z);		//get the distance to the nearest zombie

	//citizen is much more closer to penelope (<=80) than zombies, or there is no zombie
	if ((dist_p < dist_z || nearestZombie == nullptr) && dist_p <= 80)
		if (moveTowardToPenelope()) return;

	//no penelope near by or zombie is much more closer(<=80)
	if (nearestZombie != nullptr && dist_z <= 80)
		if (moveAwayFromZombie(dist_z)) return;
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

//try to move citizon toward to penelope
bool Citizen::moveTowardToPenelope()
{
	towardTo(getSW()->getPlayer());
	if (tryToMoveForward(2))
		return true;			//move succeeded
	else
	{
		//citizen is not in the same row or column as penelope and unable to move forward
		if (getX() != getSW()->getPlayer()->getX() && getY() != getSW()->getPlayer()->getY())
		{
			//if fail to move left or right in the previous try
			if (getDirection() == left || getDirection() == right)
			{
				if (getY() < getSW()->getPlayer()->getY())
					setDirection(up);
				else setDirection(down);
			}
			else	//if fail to move up or dwon in the previous try	
			{
				if (getX() < getSW()->getPlayer()->getX())
					setDirection(right);
				else setDirection(left);
			}
			if (tryToMoveForward(2))
				return true;	//move succeeded
		}
	}

	return false;	//fail to move toward to penelope
}

//check if citaizen can mvove away extend the distance from the nearest zombie
bool Citizen::moveAwayFromZombie(double original_dist_z)
{
	double dest_X = 0.0, dest_Y = 0.0;
	double shortestDistance = original_dist_z;

	if (getSW()->accessible(this, getX(), getY() + 2)) //check up
	{
		double tempDistance;
		getSW()->nearestZombie(getX(), getY() + 2, tempDistance);	//get the new distance from the nearestZombie if citizen move up 2 pixels
		if (tempDistance > shortestDistance)
		{
			shortestDistance = tempDistance;
			dest_X = getX();
			dest_Y = getY() + 2;
			setDirection(up);
		}
	}
	if (getSW()->accessible(this, getX(), getY() - 2)) //check down
	{
		double tempDistance;
		getSW()->nearestZombie(getX(), getY() - 2, tempDistance);	//get the new distance from the nearestZombie if citizen move down 2 pixels
		if (tempDistance > shortestDistance)
		{
			shortestDistance = tempDistance;
			dest_X = getX();
			dest_Y = getY() - 2;
			setDirection(down);
		}
	}
	if (getSW()->accessible(this, getX() + 2, getY())) //check right
	{
		double tempDistance;
		getSW()->nearestZombie(getX() + 2, getY(), tempDistance);	//get the new distance from the nearestZombie if citizen move right 2 pixels
		if (tempDistance > shortestDistance)
		{
			shortestDistance = tempDistance;
			dest_X = getX() + 2;
			dest_Y = getY();
			setDirection(right);
		}
	}
	if (getSW()->accessible(this, getX() - 2, getY())) //check left
	{
		double tempDistance;
		getSW()->nearestZombie(getX() - 2, getY(), tempDistance);	//get the new distance from the nearestZombie if citizen move left 2 pixels
		if (tempDistance > shortestDistance)
		{
			shortestDistance = tempDistance;
			dest_X = getX() - 2;
			dest_Y = getY();
			setDirection(left);
		}
	}
	//if it does get a further distance to zombie than original
	if (shortestDistance > original_dist_z)
	{
		moveTo(dest_X, dest_Y);
		return true;
	}
	return false;
}

///////////////////////
////////////////////////////////////////

////////////////////////////////////////

/////     Zombie    /////
//constructor - Zombie
Zombie::Zombie(double StartX, double StartY, StudentWorld * sw)
	:Agent(StartX, StartY, IID_ZOMBIE, sw, false, true), moveDistance(0), tickcount(0) {}

void Zombie::doSomething()
{
	if (!getExistance()) return;	//the Zombie is dead

	tickcount++;
	if (tickcount % 2 == 0)	return; //in parlyzed Tick
		

	if (checkFrontandVomit()) return;	//check if there is an infectable actor at front of Zombie
										//and try to vomit
	if (moveDistance == 0)
	{
		moveDistance += randInt(3, 10);	//randomly select the moveDistance from 3~10
		selectDirection();				//call derive class zombie's selectDirection
	}

	if (tryToMoveForward(1))
		moveDistance -= 1;
	else
		moveDistance = 0;
	
}

//if Zombie get damage
void Zombie::getDamage()
{
	getSW()->playSound(SOUND_ZOMBIE_DIE);	//sound effect
	setExistance(false);					//set existance to dead
}

//randomly select directon for zombie
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

bool Zombie::checkFrontandVomit()	//check if there is an infectable actor at front of Zombie, if it does vomit 
{
	//determinate destination coordinate
	double dest_X =0.0, dest_Y = 0.0;
	getSW()->determineDestination(this, 1, dest_X, dest_Y);

	bool findPerson = false;

	//check if there is an citizen at the front of the zombie
	for (list<Actor*>::iterator lt = getSW()->getList().begin(); lt != getSW()->getList().end(); lt++)
	{
		if ((*lt)->getExistance() && (*lt)->infectable() && getSW()->overlap(dest_X, dest_Y, (*lt)->getX(), (*lt)->getY()))
		{
			findPerson = true;
			break;
		}
	}
	//check if the palyer at the front of the zombie
	if (getSW()->overlap(dest_X, dest_Y, getSW()->getPlayer()->getX(), getSW()->getPlayer()->getY()))
	{
		findPerson = true;
	}
	//ramdomly decide to vomit or not
	if (findPerson && randInt(1, 3) == 1)
	{
		getSW()->getList().push_back(new Vomit(dest_X / SPRITE_WIDTH, dest_Y / SPRITE_HEIGHT, getDirection(), getSW()));
		getSW()->playSound(SOUND_ZOMBIE_VOMIT);
		return true;
	}
	return false;
}

Zombie::~Zombie()
{
}

///////////////////////

/////  Dumb_Zombie /////
//constructor - Dumb_Zombie
Dumb_Zombie::Dumb_Zombie(double StartX, double StartY, StudentWorld * sw)
	:Zombie(StartX, StartY, sw)
{
	if (randInt(1, 10) == 1)		//randomly determinate if dumb zombie carries a vaccine goodie
		carVaccine = true;
	else carVaccine = false;
}

//if dumb zombie get damage
void Dumb_Zombie::getDamage()		
{
	Zombie::getDamage();
	getSW()->increaseScore(1000);
	if (carVaccine)
		dropVaccineGooide();
}

//select distance for the dumb zombie
void Dumb_Zombie::selectDirection()
{
	randomDirection();
}

//throw the vaccine goodie
void Dumb_Zombie::dropVaccineGooide()
{
	//select random direction
	randomDirection();		

	//determinate dest_X and dest_Y
	double dest_X = 0.0, dest_Y = 0.0;
	getSW()->determineDestination(this, 1, dest_X, dest_Y);


	//check if other objcet in the game would overlap with an object created at those coordinate
	for (list<Actor*>::iterator lt = getSW()->getList().begin(); lt != getSW()->getList().end(); lt++)
	{
		if ((*lt)->getExistance() && getSW()->overlap(dest_X, dest_Y, (*lt)->getX(), (*lt)->getY()))
		{
			return;
		}

	}

	//check if player would overlap with an object created at those coordinate
	if (getSW()->overlap(dest_X, dest_Y, getSW()->getPlayer()->getX(), getSW()->getPlayer()->getY()))
	{
		return;
	}
	
	//introduce an new Vaccine Gooide object
	getSW()->getList().push_back(new Vaccine_Goodie(dest_X / SPRITE_WIDTH, dest_Y/ SPRITE_HEIGHT, getSW()));
	cerr << "dumb zombie dropped a vaccine goodie at (" << dest_X << ", " << dest_Y << ")" << endl;

}


Smart_Zombie::Smart_Zombie(double StartX, double StartY, StudentWorld * sw)
	:Zombie(StartX, StartY, sw)
{
}

//if smart zombie get damage
void Smart_Zombie::getDamage()
{
	Zombie::getDamage();
	getSW()->increaseScore(2000);
}

//select distance for the smart zombie to chase the human
void Smart_Zombie::selectDirection()
{
	double distance = 0.0;
	Actor * nearestHuman = getSW()->nearestHuman(this, distance);

	if (distance > 80)
		randomDirection();
	else
		towardTo(nearestHuman);
}


