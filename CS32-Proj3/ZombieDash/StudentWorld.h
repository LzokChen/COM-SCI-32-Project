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

	//check if Actor A whose coordinate is(Ax, Ay) is overlap with Actor B whose coordiante is (Bx, By)
	bool overlap(const double Ax, const double Ay, const double Bx, const double By) const;
	
	//check if Actor A and Actor B are overlap
	bool ActorOverlap(const Actor &A, const Actor &B) const;
	
	//check if we can introduce a flame at (X, Y)
	bool flameable(const double X, const double Y) const;
	
	//let the actor source to do the damage to all the alive damageable actors who are overlap with the damage source
    void doDamage(Actor* source);
	
	//determinate the destination corrdinate (X,Y) of actor A if we move actor A certain block in current direction
	void determineDestination(const Actor *A, double block, double &destX, double &destY) const;
	
	//get the distance between corrdinate (Ax, Ay) and actor B
	double getDistance(double Ax, double Ay, Actor *B) const;

	//find the nearest Human for Actor A and get the distance
	Actor* nearestHuman(Actor *A, double &Distance) const;

	//find the nearest Zombie from coordinate (Ax, Ay) and get the distance
	Actor* nearestZombie(double Ax, double Ay, double &Distance) const;

    void goNextLevel();
    
	list<Actor*>& getList();		//get the actor list
	Penelope* getPlayer() const;	//get the player 
	int getNumCitizen() const;		//get the number of alive citizen
	void changeNumCitizen(int k);	//change the number of alive citizen
	~StudentWorld();
private:
	//get the distance between Actor A and actor B

	list<Actor*> ActorList;
	Penelope* Player;
	int numCitizen;
	bool nextLevel;	//nextLevel;
	
};

#endif // STUDENTWORLD_H_
