#include "GridManager.h"

GridManager::GridManager(HWND& hWnd, int& width_in, int& height_in)
	:
	hWnd(hWnd),
	width(width_in),
	height(height_in)
{
}

GridManager::~GridManager()
{
	Clear();
}

GridManager::GridManager(GridManager& copy)
	:
	GridManager(copy.hWnd, copy.width, copy.height)
{
}

GridManager& GridManager::operator=(const GridManager& gridMan)
{
	hWnd = gridMan.hWnd;
	width = gridMan.width;
	height = gridMan.height;
	return *this;
}

void GridManager::AStar()
{
	nopath = false;
	time = GetTickCount();
	Clear();
	Tiles current = start;
	bool finished = false;

	//Etablissement des chemins possibles
	while (!finished && !nopath) {
		int g = current.G;
		AddO(current.x - 1, current.y, 1, g + 11);
		AddO(current.x, current.y - 1, 2, g + 11);
		AddO(current.x + 1, current.y, 3, g + 11);
		AddO(current.x, current.y + 1, 4, g + 11);
		AddO(current.x - 1, current.y - 1, 5, g + 20);
		AddO(current.x + 1, current.y - 1, 6, g + 20);
		AddO(current.x - 1, current.y + 1, 7, g + 20);
		AddO(current.x + 1, current.y + 1, 8, g + 20);

		current = opened[0];

		for (int i = 1; i < nOpened; i++) {
			if (opened[i].F <= current.F)
				current = opened[i];
		}
		AddF(current);
		DelO(current);
		if (current.x == end.x && current.y == end.y) {
			finished = true;
		}
		if (nOpened == 0) {
			nopath = true;
		}
	}
	//Tracer du chemin en partant de l'arrivée
	if (finished) {
		Tiles temp = end;
		while (temp.x != start.x || temp.y != start.y) {
			switch (dir[temp.x][temp.y]) {
			case 1:
				temp.x++;
				break;
			case 2:
				temp.y++;
				break;
			case 3:
				temp.x--;
				break;
			case 4:
				temp.y--;
				break;
			case 5:
				temp.x++;
				temp.y++;
				break;
			case 6:
				temp.x--;
				temp.y++;
				break;
			case 7:
				temp.x++;
				temp.y--;
				break;
			case 8:
				temp.x--;
				temp.y--;
				break;
			default:
				break;
			}
			for (int i = 0; i < nClosed; i++) {
				if (temp.x == closed[i].x && temp.y == closed[i].y) {
					path[nPath] = closed[i];
					nPath++;
					break;
				}
			}
		}
	}
}

void GridManager::Draw(GraphicsManager& gfx, GLuint base)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// grille
	/*	glColor3f (.5,.5,.5);
		for (int a=0;a<640;a+=16)
			for (int b=0;b<480;b+=16)
				ERectangle (a,b,16,16);*/

				// les murs en gris
	glColor3f(.4f, .4f, .4f);
	for (int a = 0; a < nObstacle; a++)
		gfx.DrawRect(obstacle[a].x * TX, obstacle[a].y * TY, TX, TY);

	// le chemin en bleu avec les directions en cyan
	for (int a = 0; a < nPath; a++)
	{
		glColor3f(0, 0, 1);
		gfx.DrawRect(path[a].x * TX, path[a].y * TY, TX, TY);
		glColor3f(0, 1, 1);
		gfx.DrawDir(path[a].x, path[a].y, dir[path[a].x][path[a].y]);
	}

	// le départ en vert
	glColor3f(0, 1, 0);
	gfx.DrawRect(start.x * TX, start.y * TY, TX, TY);
	// l'arrivée en rouge
	glColor3f(1, 0, 0);
	gfx.DrawRect(end.x * TX, end.y * TY, TX, TY);

	// le texte
	glColor3f(1, 1, 1);
	glRasterPos2i(0, 10);
	gfx.glPrint("(<h> pour aide)  © 2004 Cadé David", base);
	if (nopath)
	{
		glRasterPos2i(0, 20);
		gfx.glPrint("Pas de chemin", base);
	}

	// l'aide
	if (viewhelp)
	{
		glColor3f(1, 0, 0);
		glRasterPos2i(20, 50);
		gfx.glPrint("AIDE", base);
		glColor3f(1, 1, 1);
		glRasterPos2i(20, 70);
		gfx.glPrint("<D> met le point de départ (case verte) sous le curseur", base);
		glRasterPos2i(20, 80);
		gfx.glPrint("<A> met le point d'arrivée (case rouge) sous le curseur", base);
		glRasterPos2i(20, 90);
		gfx.glPrint("Un clic met ou enlève le mur (cases grises) sous le curseur", base);
		glRasterPos2i(20, 100);
		gfx.glPrint("Le curseur est constitué d'un indicateur (plein) pour voir la case courante", base);

	}

	// le curseur
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(hWnd, &p);
	p.x = (int)((float)p.x / (float)width * 640.0f);
	p.y = (int)((float)p.y / (float)height * 480.0f);

	glColor4f(.5f, 1.0f, .5f, .4f);
	gfx.DrawRect(p.x / TX * TX + 2, p.y / TY * TY + 2, TX - 4, TY - 4);

	//	ERectangle (p.x-TX/2,p.y-TY/2,TX,TY);
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(0, 0, 0, .5);
	glVertex2i(p.x + 4, p.y + 2);
	glVertex2i(p.x + 4, p.y + 15);
	glVertex2i(p.x + 8, p.y + 12);
	glVertex2i(p.x + 14, p.y + 15);

	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(1.0f, .5f, 1.0f, .8f);
	glVertex2i(p.x, p.y);
	glColor4f(1.0f, 1.0f, .5f, .8f);
	glVertex2i(p.x, p.y + 13);
	glColor4f(1.0f, .5f, .5f, .8f);
	glVertex2i(p.x + 4, p.y + 10);
	glColor4f(1.0f, 0.0f, 0.0f, .8f);
	glVertex2i(p.x + 10, p.y + 13);

	glEnd();

	glFlush();
}

void GridManager::MouseClick(int x, int y)
{
	int a;
	x = (int)((float)x / (float)width * 640.0f);
	y = (int)((float)y / (float)height * 480.0f);

	x /= TX; y /= TY;
	if (nPath > 0) Clear();

	for (a = 0; a < nObstacle; a++)
		if (obstacle[a].x == x && obstacle[a].y == y)
		{
			PopNF(a);
			AStar();
			return;
		}
	obstacle[nObstacle].x = x;
	obstacle[nObstacle].y = y;
	nObstacle++;
	AStar();
}

void GridManager::Keyboard(unsigned char key, int x, int y)
{
	x = (int)((float)x / (float)width * 640.0f);
	y = (int)((float)y / (float)height * 480.0f);

	switch (key)
	{
	case 'D':
	case 'd':
		start.x = x / TX;
		start.y = y / TY;
		AStar();
		break;
	case 'A':
	case 'a':
		end.x = x / TX;
		end.y = y / TY;
		AStar();
		break;
	case 'H':
	case 'h':
		viewhelp = !viewhelp;
		break;

	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void GridManager::Clear()
{
	nOpened = 0;
	nClosed = 0;
	nPath = 0;
}

void GridManager::AddF(Tiles c)
{
	closed[nClosed] = c;
	nClosed++;
}

void GridManager::AddO(int x, int y, char direc, int g)
{
	int a;
	if (!(x < 0 || x >= BX || y < 0 || y >= BY))//bornes
	{
		for (a = 0; a < nClosed; a++)//case fermée?
			if (closed[a].x == x && closed[a].y == y)
				return;

		for (a = 0; a < nObstacle; a++)//case franchissable?
			if (obstacle[a].x == x && obstacle[a].y == y)
				return;

		for (a = 0; a < nOpened; a++)//case ouverte?
			if (opened[a].x == x && opened[a].y == y)
			{
				if (opened[a].G > g)// on change la direction dans la case
				{
					opened[a].G = g;
					opened[a].F = g + opened[a].H;
					dir[x][y] = direc;
				}
				return;
			}
		// on rajoute la case
		opened[nOpened].x = x;
		opened[nOpened].y = y;
		opened[nOpened].G = g;
		opened[nOpened].H = 10 * (ABS(end.x - x) + ABS(end.y - y));
		opened[nOpened].F = g + opened[nOpened].H;
		dir[x][y] = direc;

		nOpened++;
	}
}

void GridManager::DelO(Tiles c)
{
	for (int a = 0; a < nOpened; a++)

		if (c.x == opened[a].x && c.y == opened[a].y)
		{
			for (int i = a + 1; i < nOpened; i++)
				opened[i - 1] = opened[i];
			nOpened--;
		}
}

void GridManager::PopNF(int a)
{
	for (int i = a + 1; i < nObstacle; i++)
		obstacle[i - 1] = obstacle[i];
	nObstacle--;
}
