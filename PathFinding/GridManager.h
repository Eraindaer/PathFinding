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
	struct Cases {
		int x;
		int y;
		int F, G, H;
	};

public:
	GridManager() = default;
	GridManager(HWND& hWnd, int width_in, int height_in);
	~GridManager();
	GridManager(GridManager&);
	GridManager& operator= (const GridManager&);

	void AStar();
	void Draw(GraphicsManager& gfx, GLuint base);
	void MouseClick(int x, int y);
	void Keyboard(unsigned char key, int x, int y);

private:
	void Vide();
	void AddF(Cases c);
	void AddO(int x, int y, char direc, int g);
	void SupO(Cases c);
	void PopNF(int a);

private:
	HWND&  hWnd;
	DWORD  time;
	int    width;
	int    height;
	Cases  nonfranchissable[BX * BY] = {};
	int	   nbnonfranchissable = 0;
	Cases  ouvert[BX * BY] = {};
	int	   nbouvert = 0;
	Cases  ferme[BX * BY] = {};
	int    nbferme = 0;
	Cases  chemin[BX * BY] = {};
	int    nbchemin;
	Cases  depart = { 0,0 };
	Cases  arrivee = { BX - 1,BY - 1 };
	char   dir[BX][BY];
	bool   nochemin = false;// pour savoir si il y a un chemin
	bool   viewhelp = false;// pour afficher l'aide (H)
};