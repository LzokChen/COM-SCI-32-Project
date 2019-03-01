#include "StudentWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <sstream>
#include <iomanip>
#include<math.h>
using namespace std;



GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), numCitizen(0), Player(nullptr), nextLevel(false){}

int StudentWorld::init()
{
	//get the level file name
	ostringstream oss;
	oss.fill('0');
	oss << "level" << setw(2) << this->getLevel() << ".txt";

	Level lev(assetPath());
	string LevelFile = oss.str();
	Level::LoadResult result = lev.loadLevel(LevelFile);

	//if the game just pass level (the next level is 100) or we cannot find the level file
	if (this->getLevel() == 100 || result == Level::load_fail_file_not_found)
	{
		cerr << "Cannot find "<< LevelFile << " data file" << endl;
		return GWSTATUS_PLAYER_WON;
	}
	//the level file is not in the proper format
	else if (result == Level::load_fail_bad_format)
	{
		cerr << "Your level was improperly formatted" << endl;
		return GWSTATUS_LEVEL_ERROR;
	}
	else if (result == Level::load_success)
	{
		cerr << "Successfully loaded "<< LevelFile << endl;
	}

	for (int x = 0; x < LEVEL_WIDTH; x++)
	{
		for (int y = 0; y < LEVEL_HEIGHT; y++)
		{
			Level::MazeEntry ge = lev.getContentsOf(x, y);
			switch (ge)
			{
			case Level::empty:
				break;

			case Level::exit:
				ActorList.push_back(new Exit(x, y, this));
				break;

			case Level::player:
				Player = new Penelope(x, y, this);
				break;

			case Level::dumb_zombie:
				ActorList.push_back(new Dumb_Zombie(x, y, this));
				break;

			case Level::smart_zombie:
				ActorList.push_back(new Smart_Zombie(x, y, this));
				break;
			
			case Level::citizen:
				ActorList.push_back(new Citizen(x, y, this));
				changeNumCitizen(1);	
				break;

			case Level::wall:
				ActorList.push_back( new Wall(x, y, this));
				break;

			case Level::pit:
                ActorList.push_back(new Pit(x, y, this));
				break;

			case Level::vaccine_goodie:
				ActorList.push_back(new Vaccine_Goodie(x, y, this));
				break;

			case Level::gas_can_goodie:
				ActorList.push_back(new GasCan_Goodie(x, y, this));
				break;

			case Level::landmine_goodie:
				ActorList.push_back(new Landmine_Goodie(x, y, this));
				break;
			}
		}
	}
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{

	//check if Player is alive
    if (!Player->getExistance())    //if player is dead
        return GWSTATUS_PLAYER_DIED;
    
	Player->doSomething();
	
	//doSomething for alive actor
	for (list<Actor*>::iterator lt = ActorList.begin(); lt != ActorList.end(); lt++)
	{
		if ((*lt)->getExistance())
			(*lt)->doSomething();
	}

	//delete died actor
	for (list<Actor*>::iterator lt = ActorList.begin(); lt != ActorList.end();)
	{
		if (!(*lt)->getExistance())
		{
			if (*lt != nullptr)
			{
				delete (*lt);
				*lt = nullptr;
			}
			(ActorList).erase(lt++);
		}
		else lt++;
	}

	//update GameStatText;
	ostringstream GameStat;
	//Score: 004500 Level: 27 Lives: 3 Vaccines: 2 Flames: 16 Mines: 1 Infected: 0

	GameStat.fill('0');
	if (getScore() >= 0)
		GameStat << "Score: " << setw(6) << getScore() << "  ";
	else
		GameStat << "Score: -" << setw(5) << abs(getScore()) << "  ";
	GameStat << "Level: "  << getLevel() << "  ";
	GameStat << "Lives: "  << getLives() << "  ";
	GameStat << "Vaccines: "  << Player->getNmVaccine() << "  ";
	GameStat << "Flames: "  << Player->getNumFCharge() << "  ";
	GameStat << "Mines: "  << Player->getNumLandmine() << "  ";
	GameStat << "Infected: "  << Player->getInfectionCount() << "  ";

	setGameStatText(GameStat.str());

	//if the current level finished
    if(nextLevel)
    {
        playSound(SOUND_LEVEL_FINISHED);
        return GWSTATUS_FINISHED_LEVEL;
    }
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    nextLevel = false;
	numCitizen = 0;
	for (list<Actor*>::iterator lt = ActorList.begin(); lt != ActorList.end();)
	{
		if (*lt != nullptr)
		{
			delete (*lt);
			*lt = nullptr;
		}
		(ActorList).erase(lt++);
		
	}
	if (Player != nullptr)
	{
		delete Player;
		Player = nullptr;
	}

}
//check if (X,Y) is accessible
bool StudentWorld::accessible(Actor *A, double X, double Y) const
{
	//determinate the vertexs of the bounding Box of the actor move to (X,Y)
	double vertex[4][2] = { {X, Y}, {X, Y + SPRITE_HEIGHT - 1 },
	{X + SPRITE_WIDTH - 1, Y + SPRITE_HEIGHT - 1}, {X + SPRITE_WIDTH - 1,Y} };

	for (list<Actor*>::const_iterator lt = ActorList.begin(); lt != ActorList.end(); lt++)
	{
		if ((*lt) != A && (*lt)->getExistance() && (*lt)->isBlock())	//if the actor is alive and is a "blocklike" object
		{
			//determinate the boundaries of the bounding box of the "blocklike" actor
			double upperBound	= (*lt)->getY() + SPRITE_HEIGHT - 1;
			double lowerBound	= (*lt)->getY();
			double leftBound	= (*lt)->getX();
			double rightBound	= (*lt)->getX() + SPRITE_WIDTH - 1;

			//check if one of the vertex inside the boundaries, 
			//if it is, it implies that there are intersections btn two bounding boxs
			for (int i = 0; i < 4; i++)
			{
				if (vertex[i][0] <= rightBound && vertex[i][0] >= leftBound
					&& vertex[i][1]<=upperBound && vertex[i][1] >=lowerBound)
					return false;
			}
		}
	}

	//check if (X,Y) is occupied by player
	if (A != Player)
	{
		double upperBound = Player->getY() + SPRITE_HEIGHT - 1;
		double lowerBound = Player->getY();
		double leftBound = Player->getX();
		double rightBound = Player->getX() + SPRITE_WIDTH - 1;

		for (int i = 0; i < 4; i++)
		{
			if (vertex[i][0] <= rightBound && vertex[i][0] >= leftBound
				&& vertex[i][1] <= upperBound && vertex[i][1] >= lowerBound)
				return false;
		}
	}
	return true;
}

//check if Actor A whose coordinate is(Ax, Ay) is overlap with Actor B whose coordiante is (Bx, By)
bool StudentWorld::overlap(const double Ax, const double Ay, const double Bx, const double By) const
{
	double dX = pow(Ax - Bx, 2);
	double dY = pow(Ay - By, 2);

	if ((dX + dY) <= 100)
		return true;
	else return false;
}

//check if Actor A and Actor B are overlap
bool StudentWorld::ActorOverlap(const Actor &A, const Actor &B) const
{
	return overlap(A.getX(), A.getY(), B.getX(), B.getY());
}

//check if we can introduce a flame at (X, Y)
bool StudentWorld::flameable(const double X, const double Y) const
{

	////determinate the vertexs of the bounding Box of the actor move to (X,Y)
	//double vertex[4][2] = { {X, Y}, {X, Y + SPRITE_HEIGHT - 1 },
	//{X + SPRITE_WIDTH - 1, Y + SPRITE_HEIGHT - 1}, {X + SPRITE_WIDTH - 1,Y} };

	//for (list<Actor*>::const_iterator lt = ActorList.begin(); lt != ActorList.end(); lt++)
	//{
	//	if ((*lt)->getExistance() && (*lt)->blockFlame())	//if the actor is alive and is a "blockFlame" object
	//	{
	//		//determinate the boundaries of the bounding box of the "blocklike" actor
	//		double upperBound = (*lt)->getY() + SPRITE_HEIGHT - 1;
	//		double lowerBound = (*lt)->getY();
	//		double leftBound = (*lt)->getX();
	//		double rightBound = (*lt)->getX() + SPRITE_WIDTH - 1;

	//		//check if one of the vertex inside the boundaries, 
	//		//if it is, it implies that there are intersections btn two bounding boxs
	//		for (int i = 0; i < 4; i++)
	//		{
	//			if (vertex[i][0] <= rightBound && vertex[i][0] >= leftBound
	//				&& vertex[i][1] <= upperBound && vertex[i][1] >= lowerBound)
	//				return false;
	//		}
	//	}
	//}
	//return true;

	for (list<Actor*>::const_iterator lt = ActorList.begin(); lt != ActorList.end(); lt++)
	{
		//if an Actor overlap with up comming fire
		if ((*lt)->getExistance() && (*lt)->blockFlame() && overlap((*lt)->getX(), (*lt)->getY(), X, Y))
		{
			//cerr << "(" << X << ", " << Y << ") is not flameable" << endl;
			//if the actor overlap with up coming fire will block the flame, return false(block the fire)
			return false;

		}
	}
	//cerr << "(" << X << ", " << Y << ") is flameable" << endl;
	return true;
}

//let the actor source to do the damage to all the alive damageable actors who are overlap with the damage source
void StudentWorld::doDamage(Actor *source)
{
    for (list<Actor*>::iterator lt = ActorList.begin(); lt != ActorList.end(); lt++)
    {
        if ((*lt) != source && (*lt)->getExistance() && (*lt)->damageable() && ActorOverlap(*source, *(*lt)))
			(*lt)->getDamage();
			//if lt is an alive damageable actor and overlaps with damage source
    }
    
    if (getPlayer()->getExistance() && ActorOverlap(*source, *getPlayer())) //player got damage
    {
		getPlayer()->getDamage();
    }

    return; // defult
}

//determinate the destination corrdinate (X,Y) of actor A if we move actor A certain block in current direction
void StudentWorld::determineDestination(const Actor * A, double block, double & destX, double &destY) const
{
	switch (A -> getDirection())
	{
	case 90:
		destX = A -> getX();
		destY = A -> getY() + block * SPRITE_HEIGHT;
		break;

	case 270:
		destX = A->getX();
		destY = A->getY() - block * SPRITE_HEIGHT;
		break;

	case 180:
		destX = A->getX() - block * SPRITE_WIDTH;
		destY = A->getY();
		break;

	case 0:
		destX = A->getX() + block * SPRITE_WIDTH;
		destY = A->getY();
		break;
	}
}

//get the distance between corrdinate (Ax, Ay) and actor B
double StudentWorld::getDistance(double Ax, double Ay, Actor *B) const
{
	double dX = pow(Ax - B->getX(), 2);
	double dY = pow(Ay - B->getY(), 2);

	return sqrt(dX + dY);
}

//find the nearest Human for Actor A and get the distance
Actor* StudentWorld::nearestHuman(Actor * A, double &Distance) const
{
	Actor *NearestHuman = Player;		//assume that penelope is the nearest human
	Distance = getDistance(A->getX(), A->getY(), NearestHuman);

	for (list<Actor*>::const_iterator lt = ActorList.begin(); lt != ActorList.end(); lt++)
	{
		if ((*lt) != A && (*lt)->getExistance() && (*lt)->infectable())
		{
			if (getDistance(A->getX(), A->getY(), *lt) < Distance)
			{
				NearestHuman = *lt;
				Distance = getDistance(A->getX(), A->getY(), NearestHuman);
			}
		}
	}

	return NearestHuman;
}

//find the nearest Zombie from coordinate (Ax, Ay) and get the distance
Actor * StudentWorld::nearestZombie(double Ax, double Ay, double & Distance) const
{
	Actor *NearestZombie = nullptr;
	Distance = sqrt(pow(VIEW_WIDTH, 2) + pow(VIEW_HEIGHT, 2));	//set the initital distance is the diagonal length of the playground

	for (list<Actor*>::const_iterator lt = ActorList.begin(); lt != ActorList.end(); lt++)
	{
		if ((*lt)->getExistance() && (*lt)->infectionSource())
		{
			if (getDistance(Ax, Ay, *lt) < Distance)
			{
				NearestZombie = *lt;
				Distance = getDistance(Ax, Ay, NearestZombie);
			}
		}
	}

	return NearestZombie;
}

void StudentWorld::goNextLevel()
{
    nextLevel = true;
}

//get the actor list
list<Actor*>&  StudentWorld::getList()
{
	return ActorList;
}

//get the player 
Penelope * StudentWorld::getPlayer() const
{
	return Player;
}

//get the number of alive citizen
int StudentWorld::getNumCitizen() const
{
	return numCitizen;
}

//change the number of alive citizen
void StudentWorld::changeNumCitizen(int k)
{
	numCitizen += k;
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}



