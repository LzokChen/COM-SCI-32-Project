//Game.h

#ifndef Game_H
#define Game_H

class Arena;

class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nZombies);
	~Game();

	// Mutators
	void play();

private:
	Arena * m_arena;
};

#endif // !Game_H

