#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), numCitizen(0), Player(nullptr)
{
}

int StudentWorld::init()
{
	//get the level file name
	ostringstream oss;
	oss.fill('0');
	oss << "level" << setw(2) << this->getLevel() << ".txt";

	Level lev(assetPath());
	string levelFile = oss.str();
	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found)
		cerr << "Cannot find level01.txt data file" << endl;
	else if (result == Level::load_fail_bad_format)
		cerr << "Your level was improperly formatted" << endl;
	else if (result == Level::load_success)
	{
		cerr << "Successfully loaded "<< levelFile<< endl;
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
				//
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
				//
				break;

			case Level::wall:
				ActorList.push_back( new Wall(x, y, this));
				break;

			case Level::pit:
				//
				break;

			case Level::vaccine_goodie:
				//
				break;

			case Level::gas_can_goodie:
				//
				break;

			case Level::landmine_goodie:
				//
				break;
			}
		}
	}
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    //decLives();
    //return GWSTATUS_PLAYER_DIED;

	if (!Player->doSomething())
	{
		playSound(SOUND_PLAYER_DIE);
		return GWSTATUS_PLAYER_DIED;
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	numCitizen = 0;
	delete Player;
	for (list<Actor*>::iterator lt = ActorList.begin(); lt != ActorList.end(); lt++)
		delete *lt;
}

bool StudentWorld::accessible(double X, double Y)
{
	//determinate the vertexs of the bounding Box of the actor move to (X,Y)
	double vertex[4][2] = { {X, Y}, {X, Y + SPRITE_HEIGHT - 1 },
	{X + SPRITE_WIDTH - 1,Y + SPRITE_HEIGHT - 1}, {X + SPRITE_WIDTH - 1,Y} };

	
	
	for (list<Actor*>::iterator lt = ActorList.begin(); lt != ActorList.end(); lt++)
	{
		if ((*lt)->isBlock())	//if the actor is a "blocklike" object
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

StudentWorld::~StudentWorld()
{
	cleanUp();
}
