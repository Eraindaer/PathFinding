#pragma once
#include "GraphicsManager.h"
#include "GridManager.h"

class WinManager
{
public:
	WinManager() = default;
	WinManager(HWND& hWnd_in, HINSTANCE hInstance, WNDCLASS& wc_in, GridManager& grid, int width, int height);
	~WinManager();

	static void Resize(int w, int h);

	void Display();

private:
	void Init();
	void EnableOpenGL();
	void DisableOpenGL();

private:
	static int				    width;
	static int				    height;
	GridManager					grid;
	HWND&						hWnd;
	GraphicsManager				gfx;
	WNDCLASS					wc;
	GLuint						base;
	HGLRC						hRC;
	HDC							hDC;
};

