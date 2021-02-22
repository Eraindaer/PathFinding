#pragma once
#include "GraphicsManager.h"
#include "GridManager.h"
#include <memory>

class WinManager
{
public:
	WinManager() = default;
	WinManager(HWND& hWnd_in, HINSTANCE hInstance, WNDCLASS& wc_in, GridManager& grid, int& width, int& height);
	~WinManager();

	void Display();

private:
	void Init();
	void EnableOpenGL();
	void DisableOpenGL() const;

private:
	int&						width;
	int&					    height;
	GridManager&				grid;
	HWND&						hWnd;
	GraphicsManager				gfx;
	WNDCLASS					wc;
	GLuint						base;
	HGLRC						hRC;
	HDC							hDC;
};

