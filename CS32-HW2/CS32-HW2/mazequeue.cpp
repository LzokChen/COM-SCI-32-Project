//mazequeue.cpp
#include <iostream>
#include <queue>
#include <string>
using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X....X...X",
		"X.XX.XX..X",
		"XXX....X.X",
		"X.XXX.XXXX",
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
	
	/*cout << endl;
	for (int k = 0; k < 10; k++)
	{
		cout << maze[k] << endl;
	}*/
}

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	queue<Coord> coordQueue;	//declare a queue of Coords
	Coord start(sr, sc);		//start point
	Coord end(er, ec);			//end point

	coordQueue.push(start);
	maze[sr].replace(sc, 1, "@"); //mark the start point has been encountered

	while (!coordQueue.empty())
	{
		Coord cur = coordQueue.front();
		coordQueue.pop();
		if (cur.r() == end.r() && cur.c() == end.c())
			return true;	//have solution

		//if we can move south (down)
		if (cur.r() + 1 <= nRows)
		{
			Coord temp(cur.r() + 1, cur.c());
			if (maze[temp.r()].substr(temp.c(), 1) != "X" && maze[temp.r()].substr(temp.c(), 1) != "@")
			{
				coordQueue.push(temp);						//push the corrdinate (r+1,c) on to the queue
				maze[temp.r()].replace(temp.c(), 1, "@");	//mark the point has been encountered
			}
		}

		//if we can move west (left)
		if (cur.c() - 1 >= 0)
		{
			Coord temp(cur.r(), cur.c() - 1);
			if (maze[temp.r()].substr(temp.c(), 1) != "X" && maze[temp.r()].substr(temp.c(), 1) != "@")
			{
				coordQueue.push(temp);						//push the corrdinate (r,c-1) on to the queue
				maze[temp.r()].replace(temp.c(), 1, "@");	//mark the point has been encountered
			}
		}

		//if we can move north (up)
		if (cur.r() - 1 >= 0)
		{
			Coord temp(cur.r() - 1, cur.c());
			if (maze[temp.r()].substr(temp.c(), 1) != "X" && maze[temp.r()].substr(temp.c(), 1) != "@")
			{
				coordQueue.push(temp);						//push the corrdinate (r-1,c) on to the queue
				maze[temp.r()].replace(temp.c(), 1, "@");	//mark the point has been encountered
			}
		}

		//if we can move east (right)
		if (cur.c() + 1 <= nCols)
		{
			Coord temp(cur.r(), cur.c() + 1);
			if (maze[temp.r()].substr(temp.c(), 1) != "X" && maze[temp.r()].substr(temp.c(), 1) != "@")
			{
				coordQueue.push(temp);						//push the corrdinate (r,c+1) on to the queue
				maze[temp.r()].replace(temp.c(), 1, "@");	//mark the point has been encountered
			}
		}
	}
	return false;	//no solution
}