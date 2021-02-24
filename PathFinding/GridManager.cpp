#include "GridManager.h"

GridManager::GridManager(HWND& hWnd, int& width_in, int& height_in)
	:
	hWnd(hWnd),
	width(width_in),
	height(height_in)
{
	for (int j = 0; j < BY; j++) {
		for (int i = 0; i < BX; i++) {
			if (i == 31)
				int test;
			grid[j * BX + i] = { i , j };
		}
	}
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
	while(!finished && !nopath){
		int g = current.G;
		AddO(current.x - 1, current.y, LEFT, g + 10);
		AddO(current.x, current.y - 1, UP, g + 10);
		AddO(current.x + 1, current.y, RIGHT, g + 10);
		AddO(current.x, current.y + 1, DOWN, g + 10);
		AddO(current.x - 1, current.y - 1, UPLEFT, g + 19);
		AddO(current.x + 1, current.y - 1, UPRIGHT, g + 19);
		AddO(current.x - 1, current.y + 1, DOWNLEFT, g + 19);
		AddO(current.x + 1, current.y + 1, DOWNRIGHT, g + 19);

		for (auto& tile : grid) {
			if (tile.state == Tiles::State::Opened) {
				nopath = false;
				current = tile;
				break;
			}
			else {
				nopath = true;
			}
		}
		for (auto& tile : grid) {
			switch (tile.state) {
			case Tiles::State::Opened:
				if (tile.F <= current.F)
					current = tile;
				break;
			default:
				break;
			}
		}

		AddF(grid[current.y * BX + current.x]);

		finished = current.x == end.x && current.y == end.y;
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
			auto& tile = grid[temp.y * BX + temp.x];
			tile.state = Tiles::State::Path;
		}
	}
}

void GridManager::Draw(const GraphicsManager& gfx, GLuint base) const
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto& tile : grid)
	{
		switch (tile.state) {
#if _DEBUG
		case Tiles::State::Opened:
			glColor3f(0.0f, 1.0f, 0.0f);
			gfx.DrawRect(tile.x * TX, tile.y * TY, TX, TY);
			break;
		case Tiles::State::Closed:
			glColor3f(1.0f, 0.0f, 0.0f);
			gfx.DrawRect(tile.x * TX, tile.y * TY, TX, TY);
			break;
#endif
		//Obstacles en gris
		case Tiles::State::Obstacle:
			glColor3f(.4f, .4f, .4f);
			gfx.DrawRect(tile.x * TX, tile.y * TY, TX, TY);
			break;
		//Chemin en cyan
		case Tiles::State::Path:
			glColor3f(0, 0, 1);
			gfx.DrawRect(tile.x * TX, tile.y * TY, TX, TY);
			glColor3f(0, 1, 1);
			gfx.DrawDir(tile.x, tile.y, dir[tile.x][tile.y]);
			break;
		default:
			break;
		}
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
	x = (int)((float)x / (float)width * 640.0f) / TX;
	y = (int)((float)y / (float)height * 480.0f) / TY;

	auto& tile = grid[y * BX + x];
	tile.state = tile.state == Tiles::State::Obstacle ? Tiles::State::None : Tiles::State::Obstacle;

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
	for (auto& tile : grid) {
		switch (tile.state) {
		case Tiles::State::Closed:
		case Tiles::State::Opened:
		case Tiles::State::Path:
			tile.state = Tiles::State::None;
			break;
		default:
			break;
		}
	}
}

void GridManager::AddF(Tiles& c)
{
	c.state = Tiles::State::Closed;
}

void GridManager::AddO(int x, int y, char direc, int g)
{
	if (!(x < 0 || x >= BX || y < 0 || y >= BY))//bornes
	{
		auto& tile = grid[y * BX + x];
		switch (tile.state)
		{
		case Tiles::State::Opened:
			if (tile.G >= g) {
				tile.G = g;
				tile.F = g + tile.H;
				dir[x][y] = direc;
			}
			break;
		case Tiles::State::None:
			tile.state = Tiles::State::Opened;
			tile.G = g;
			tile.H = 10 * (ABS(end.x - x) + ABS(end.y - y));
			tile.F = g + tile.H;
			dir[x][y] = direc;
		default:
			break;
		}
	}
}