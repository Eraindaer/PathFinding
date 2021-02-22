#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <gl/gl.h>
#include <gl/glu.h>

#define BX	40
#define BY	30
#define TX	16
#define TY	16

#ifndef CALLBACK
#define CALLBACK
#endif

class GraphicsManager
{
public:
	GraphicsManager() = default;
	~GraphicsManager();

	void DrawRect(int x, int y, int w, int h);
	void DrawRectBorder(int x, int y, int w, int h);
	void DrawDir(int x, int y, int direc);
	GLvoid glPrint(const char* fmt, GLuint base, ...);
};

