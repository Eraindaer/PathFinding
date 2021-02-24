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

		int x = 0;
		int y = 0;
		int F = 0, G = 0, H = 0;
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

private:
	DWORD  time;
	HWND&  hWnd;
	int&   width;
	int&   height;

	Tiles  grid[BX * BY] = {};
	Tiles  start = { 0,0 };
	Tiles  end = { BX - 1,BY - 1 };
	char   dir[BX][BY];
	bool   nopath = false;// pour savoir si il y a un chemin
	bool   viewhelp = false;// pour afficher l'aide (H)
};