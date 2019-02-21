#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	bool accessible(double x, double Y);	//check if (X,Y) is accessible; 
											//Non-accessible: (X,Y) is occupied by wall's,
											//citizen's, zombie's or Penelope's bounding box

	bool overlap(const Actor &A, const Actor &B) const;

	list<Actor*>& GetList();
	Actor* getPlayer() const;
	int getNumCitizen() const;
	void changeNumCitizen(int k);
	~StudentWorld();
private:
	list<Actor*> ActorList;
	Actor* Player;
	int numCitizen;
	
};

#endif // STUDENTWORLD_H_
