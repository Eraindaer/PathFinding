#pragma once
#include "GraphicsManager.h"
#include <memory>

#define LEFT		1
#define UP  		2
#define RIGHT		3
#define DOWN		4
#define UPLEFT  	5
#define UPRIGHT 	6
#define DOWNLEFT	7
#define DOWNRIGHT	8

#define ABS(x)	((x)<0?(-(x)):(x))	

class GridManager
{
private:
	struct Tiles {
		enum class State {
			None,
			Obstacle,
			Closed,
			Opened,
			Path
		};

		int x;
		int y;
		int F, G, H;
		State state = State::None;
	};

public:
	GridManager() = default;
	GridManager(HWND& hWnd, int& width_in, int& height_in);
	~GridManager();
	GridManager(GridManager&);
	GridManager& operator= (const GridManager&);

	void AStar();
	void Draw(const GraphicsManager& gfx, GLuint base) const;
	void MouseClick(int x, int y);
	void Keyboard(unsigned char key, int x, int y);

private:
	void Clear();
	void AddF(Tiles& c);
	void AddO(int x, int y, char direc, int g);
	void DelO(Tiles& c);

private:
	DWORD  time;
	HWND&  hWnd;
	int&   width;
	int&   height;


	/*Tiles  obstacle[BX * BY] = {};
	int	   nObstacle = 0;
	Tiles  opened[BX * BY] = {};
	Tiles  closed[BX * BY] = {};
	int    nClosed = 0;
	Tiles  path[BX * BY] = {};
	int    nPath;*/

	int	   nOpened = 0;
	char   openCoordX[BX * BY] = {};
	char   openCoordY[BX * BY] = {};
	Tiles  grid[BX * BY] = {};
	Tiles  start = { 0,0 };
	Tiles  end = { BX - 1,BY - 1 };
	char   dir[BX][BY];
	bool   nopath = false;// pour savoir si il y a un chemin
	bool   viewhelp = false;// pour afficher l'aide (H)
};