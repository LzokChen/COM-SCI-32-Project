#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
//#include "Actor.h"
#include <string>
#include <list>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Penelope;

class StudentWorld : public GameWorld
{
public:
	//from GameWorld
	//int getLevel() const;
	//int getLives() const;
	//void decLives();
	//void incLives();
	//int getScore() const;
	//void increaseScore(int howMuch);
	//void setGameStatText(string text);
	//string assetPath() const;
	//bool getKey(int& value);
	//void playSound(int soundID);

    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	bool accessible(Actor *A, double x, double Y) const;	//check if (X,Y) is accessible; 
											//Non-accessible: (X,Y) is occupied by wall's,
											//citizen's, zombie's or Penelope's bounding box

	bool overlap(const double Ax, const double Ay, const double Bx, const double By) const;
	bool ActorOverlap(const Actor &A, const Actor &B) const;

	bool flameable(const double X, const double Y) const;

    int damage(Actor* source);

	list<Actor*>& getList();
	Penelope* getPlayer() const;
	int getNumCitizen() const;
	void changeNumCitizen(int k);
	~StudentWorld();
private:
	list<Actor*> ActorList;
	Penelope* Player;
	int numCitizen;

	bool gameWon;
	
};

#endif // STUDENTWORLD_H_
