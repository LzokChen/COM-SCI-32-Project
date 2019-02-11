//maze.cpp
///*
#include <iostream>
#include <string>

using namespace std;
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X....X...X",
		"X.X.XXX..X",
		"XXX....X.X",
		"X.X.XXXXXX",
		"X.X...X..X",
		"X...X.X..X",
		"XXXXX.X.XX",
		"X........X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 3, 5, 8, 8))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}
//*/


bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	if (sr == er && sc == ec) return true;

	maze[sr].replace(sc, 1, "@"); //mark the start point has been encountered


	//if we can move south (down)
	if (sr + 1 <= nRows)
	{
		if (maze[sr + 1].substr(sc, 1) == ".")
			if (pathExists(maze, nRows, nCols, sr + 1, sc, er, ec)) return true;
	}

	//if we can move west (left)
	if (sc - 1 >= 0)
	{
		if (maze[sr].substr(sc - 1, 1) == ".")
			if (pathExists(maze, nRows, nCols, sr, sc - 1, er, ec)) return true;
	}


	//if we can move north (up)
	if (sr - 1 >= 0)
	{
		if (maze[sr - 1].substr(sc, 1) == ".")
			if (pathExists(maze, nRows, nCols, sr - 1, sc, er, ec)) return true;
	}

	//if we can move east (right)
	if (sc + 1 <= nCols)
	{
		if (maze[sr].substr(sc + 1, 1) == ".")
			if (pathExists(maze, nRows, nCols, sr, sc + 1, er, ec)) return true;
	}
	
	return false;
}