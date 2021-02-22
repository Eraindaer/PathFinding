#include "GridManager.h"

GridManager::GridManager(HWND& hWnd, int width_in, int height_in)
	:
	hWnd(hWnd)
{
	width = width_in;
	height = height_in;
}

GridManager::~GridManager()
{
	Vide();
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
	nochemin = false;
	time = GetTickCount();
	Vide();
	Cases courant = depart;
	bool arrtrouv = false;

	//Etablissement des chemins possibles
	while (!arrtrouv && !nochemin) {
		int g = courant.G;
		AddO(courant.x - 1, courant.y, 1, g + 11);
		AddO(courant.x, courant.y - 1, 2, g + 11);
		AddO(courant.x + 1, courant.y, 3, g + 11);
		AddO(courant.x, courant.y + 1, 4, g + 11);
		AddO(courant.x - 1, courant.y - 1, 5, g + 20);
		AddO(courant.x + 1, courant.y - 1, 6, g + 20);
		AddO(courant.x - 1, courant.y + 1, 7, g + 20);
		AddO(courant.x + 1, courant.y + 1, 8, g + 20);

		courant = ouvert[0];

		for (int i = 1; i < nbouvert; i++) {
			if (ouvert[i].F <= courant.F)
				courant = ouvert[i];
		}
		AddF(courant);
		SupO(courant);
		if (courant.x == arrivee.x && courant.y == arrivee.y) {
			arrtrouv = true;
		}
		if (nbouvert == 0) {
			nochemin = true;
		}
	}
	//Tracer du chemin en partant de l'arrivée
	if (arrtrouv) {
		Cases temp = arrivee;
		while (temp.x != depart.x || temp.y != depart.y) {
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
			for (int i = 0; i < nbferme; i++) {
				if (temp.x == ferme[i].x && temp.y == ferme[i].y) {
					chemin[nbchemin] = ferme[i];
					nbchemin++;
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
	for (int a = 0; a < nbnonfranchissable; a++)
		gfx.DrawRect(nonfranchissable[a].x * TX, nonfranchissable[a].y * TY, TX, TY);

	// le chemin en bleu avec les directions en cyan
	for (int a = 0; a < nbchemin; a++)
	{
		glColor3f(0, 0, 1);
		gfx.DrawRect(chemin[a].x * TX, chemin[a].y * TY, TX, TY);
		glColor3f(0, 1, 1);
		gfx.DrawDir(chemin[a].x, chemin[a].y, dir[chemin[a].x][chemin[a].y]);
	}

	// le départ en vert
	glColor3f(0, 1, 0);
	gfx.DrawRect(depart.x * TX, depart.y * TY, TX, TY);
	// l'arrivée en rouge
	glColor3f(1, 0, 0);
	gfx.DrawRect(arrivee.x * TX, arrivee.y * TY, TX, TY);

	// le texte
	glColor3f(1, 1, 1);
	glRasterPos2i(0, 10);
	gfx.glPrint("(<h> pour aide)  © 2004 Cadé David", base);
	if (nochemin)
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
	p.x = (int)((float)p.x / (float)width * 640.);
	p.y = (int)((float)p.y / (float)height * 480.);

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
	x = (int)((float)x / (float)width * 640.);
	y = (int)((float)y / (float)height * 480.);

	x /= TX; y /= TY;
	if (nbchemin > 0) Vide();

	for (a = 0; a < nbnonfranchissable; a++)
		if (nonfranchissable[a].x == x && nonfranchissable[a].y == y)
		{
			PopNF(a);
			AStar();
			return;
		}
	nonfranchissable[nbnonfranchissable].x = x;
	nonfranchissable[nbnonfranchissable].y = y;
	nbnonfranchissable++;
	AStar();
}

void GridManager::Keyboard(unsigned char key, int x, int y)
{
	x = (int)((float)x / (float)width * 640.);
	y = (int)((float)y / (float)height * 480.);

	switch (key)
	{
	case 'D':
	case 'd':
		depart.x = x / TX;
		depart.y = y / TY;
		AStar();
		break;
	case 'A':
	case 'a':
		arrivee.x = x / TX;
		arrivee.y = y / TY;
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

void GridManager::Vide()
{
	nbouvert = 0;
	nbferme = 0;
	nbchemin = 0;
}

void GridManager::AddF(Cases c)
{
	ferme[nbferme] = c;
	nbferme++;
}

void GridManager::AddO(int x, int y, char direc, int g)
{
	int a;
	if (!(x < 0 || x >= BX || y < 0 || y >= BY))//bornes
	{
		for (a = 0; a < nbferme; a++)//case fermée?
			if (ferme[a].x == x && ferme[a].y == y)
				return;

		for (a = 0; a < nbnonfranchissable; a++)//case franchissable?
			if (nonfranchissable[a].x == x && nonfranchissable[a].y == y)
				return;

		for (a = 0; a < nbouvert; a++)//case ouverte?
			if (ouvert[a].x == x && ouvert[a].y == y)
			{
				if (ouvert[a].G > g)// on change la direction dans la case
				{
					ouvert[a].G = g;
					ouvert[a].F = g + ouvert[a].H;
					dir[x][y] = direc;
				}
				return;
			}
		// on rajoute la case
		ouvert[nbouvert].x = x;
		ouvert[nbouvert].y = y;
		ouvert[nbouvert].G = g;
		ouvert[nbouvert].H = 10 * (ABS(arrivee.x - x) + ABS(arrivee.y - y));
		ouvert[nbouvert].F = g + ouvert[nbouvert].H;
		dir[x][y] = direc;

		nbouvert++;
	}
}

void GridManager::SupO(Cases c)
{
	for (int a = 0; a < nbouvert; a++)

		if (c.x == ouvert[a].x && c.y == ouvert[a].y)
		{
			for (int i = a + 1; i < nbouvert; i++)
				ouvert[i - 1] = ouvert[i];
			nbouvert--;
		}
}

void GridManager::PopNF(int a)
{
	for (int i = a + 1; i < nbnonfranchissable; i++)
		nonfranchissable[i - 1] = nonfranchissable[i];
	nbnonfranchissable--;
}
