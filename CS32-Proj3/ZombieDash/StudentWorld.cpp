#include "StudentWorld.h"
#include "GameConstants.h"
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
: GameWorld(assetPath), numCitizen(0), Player(nullptr), gameWon(false){}

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
		gameWon = true;
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
				//
				break;

			case Level::smart_zombie:
				//
				break;
			
			case Level::citizen:
				ActorList.push_back(new Citizen(x, y, this));
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
	Player->doSomething();
	if (Player->doSomething() == -1)	//if player is dead (by player's doSomething, die due to infection)
	{
		decLives();
		playSound(SOUND_PLAYER_DIE);
		return GWSTATUS_PLAYER_DIED;
	}

	//doSomething for alive actor
	for (list<Actor*>::iterator lt = ActorList.begin(); lt != ActorList.end(); lt++)
	{
		if ((*lt)->getExistance())
		{
			int result = (*lt)->doSomething();
			switch (result)
			{
			case 1:
				playSound(SOUND_LEVEL_FINISHED);
				return GWSTATUS_FINISHED_LEVEL;
				break;
			case -1:
				decLives();
				playSound(SOUND_PLAYER_DIE);
				return GWSTATUS_PLAYER_DIED; 
			}
		}
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

	GameStat << "Score: " << getScore() << "  ";
	GameStat << "Level: "  << getLevel() << "  ";
	GameStat << "Lives: "  << getLives() << "  ";
	GameStat << "Vaccines: "  << Player->getNmVaccine() << "  ";
	GameStat << "Flames: "  << Player->getNumFCharge() << "  ";
	GameStat << "Mines: "  << Player->getNumLandmine() << "  ";
	GameStat << "Infected: "  << Player->getInfectionCount() << "  ";

	setGameStatText(GameStat.str());




	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
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

bool StudentWorld::accessible(double X, double Y) const
{
	//determinate the vertexs of the bounding Box of the actor move to (X,Y)
	double vertex[4][2] = { {X, Y}, {X, Y + SPRITE_HEIGHT - 1 },
	{X + SPRITE_WIDTH - 1, Y + SPRITE_HEIGHT - 1}, {X + SPRITE_WIDTH - 1,Y} };

	for (list<Actor*>::const_iterator lt = ActorList.begin(); lt != ActorList.end(); lt++)
	{
		if ((*lt)->getExistance() && (*lt)->isBlock())	//if the actor is alive and is a "blocklike" object
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
	return true;

}

bool StudentWorld::overlap(const double Ax, const double Ay, const double Bx, const double By) const
{
	double dx = pow(Ax - Bx, 2);
	double dY = pow(Ay - By, 2);

	if ((dx + dY) <= 100)
		return true;
	else return false;
}

bool StudentWorld::ActorOverlap(const Actor &A, const Actor &B) const
{
	return overlap(A.getX(), A.getY(), B.getX(), B.getY());
}

bool StudentWorld::flameable(const double X, const double Y) const
{
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

int StudentWorld::damage(Actor *source)
{
    for (list<Actor*>::iterator lt = ActorList.begin(); lt != ActorList.end(); lt++)
    {
        if ((*lt)->getExistance() && (*lt)->damageable() && ActorOverlap(*source, *(*lt))) 
		{
			(*lt)->getDamage();
			//if lt is an alive damageable actor and overlaps with damage source

            if((*lt)->isZombie())
            {
                increaseScore(1000);    //increase 1000 pt
                playSound(SOUND_ZOMBIE_DIE);
            }
            (*lt)->setExistance(false);        //setExistance to dead
        }
    }
    
    if (ActorOverlap(*source, *getPlayer())) //player got damage
    {
        return -1; //return -1: player dead
    }
    
    
    return 0; // defult
}

list<Actor*>&  StudentWorld::getList()
{
	return ActorList;
}

Penelope * StudentWorld::getPlayer() const
{
	return Player;
}

int StudentWorld::getNumCitizen() const
{
	return numCitizen;
}

void StudentWorld::changeNumCitizen(int k)
{
	numCitizen += k;
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}

