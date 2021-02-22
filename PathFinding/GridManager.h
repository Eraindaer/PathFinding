#pragma once
#include "GraphicsManager.h"
#include <memory>

#define GAUCHE	1
#define HAUT	2
#define DROITE	3
#define BAS		4
#define GAUCHEHAUT	5
#define DROITEHAUT	6
#define GAUCHEBAS	7
#define DROITEBAS	8

#define ABS(x)	((x)<0?(-(x)):(x))	

class GridManager
{
private:
	struct Tiles {
		int x;
		int y;
		int F, G, H;
	};

public:
	GridManager() = default;
	GridManager(HWND& hWnd, int& width_in, int& height_in);
	~GridManager();
	GridManager(GridManager&);
	GridManager& operator= (const GridManager&);

	void AStar();
	void Draw(GraphicsManager& gfx, GLuint base);
	void MouseClick(int x, int y);
	void Keyboard(unsigned char key, int x, int y);

private:
	void Clear();
	void AddF(Tiles c);
	void AddO(int x, int y, char direc, int g);
	void DelO(Tiles c);
	void PopNF(int a);

private:
	HWND&  hWnd;
	DWORD  time;
	int&   width;
	int&   height;
	Tiles  obstacle[BX * BY] = {};
	int	   nObstacle = 0;
	Tiles  opened[BX * BY] = {};
	int	   nOpened = 0;
	Tiles  closed[BX * BY] = {};
	int    nClosed = 0;
	Tiles  path[BX * BY] = {};
	int    nPath;
	Tiles  start = { 0,0 };
	Tiles  end = { BX - 1,BY - 1 };
	char   dir[BX][BY];
	bool   nopath = false;// pour savoir si il y a un chemin
	bool   viewhelp = false;// pour afficher l'aide (H)
};