#include "WinManager.h"

WinManager::WinManager(HWND& hWnd_in, HINSTANCE hInstance, WNDCLASS& wc_in, GridManager& grid_in, int& width_in, int& height_in)
    :
    width(width_in),
    height(height_in),
    grid(grid_in),
    hWnd(hWnd_in)
{
    wc = wc_in;
    /* create main window */
    hWnd = CreateWindow(
        L"GLSample", L"A* Test",
        WS_OVERLAPPEDWINDOW,
        0, 0, 640, 480,
        NULL, NULL, hInstance, NULL);

    EnableOpenGL();
    Init();

    ShowWindow(hWnd, SW_SHOW);
    ShowCursor(FALSE);
}

WinManager::~WinManager()
{
    DisableOpenGL();
    /* destroy the window explicitly */
    DestroyWindow(hWnd);
}

void WinManager::Display()
{
    grid.Draw(gfx, base);
    SwapBuffers(hDC);
}

void WinManager::EnableOpenGL()
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    hDC = GetDC(hWnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);
}

void WinManager::DisableOpenGL() const
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
}

void WinManager::Init()
{
	// Piqué chez Nehe***************
	HFONT	font;						// Windows Font ID
	HFONT	oldfont;					// Used For Good House Keeping

	base = glGenLists(256);					// Storage For 256 Characters ( NEW )
	font = CreateFont(-12,				// Height Of Font ( NEW )
		0,				// Width Of Font
		0,				// Angle Of Escapement
		0,				// Orientation Angle
		FW_NORMAL,			// Font Weight
		FALSE,				// Italic
		FALSE,				// Underline
		FALSE,				// Strikeout
		ANSI_CHARSET,			// Character Set Identifier
		OUT_TT_PRECIS,			// Output Precision
		CLIP_DEFAULT_PRECIS,		// Clipping Precision
		ANTIALIASED_QUALITY,		// Output Quality
		FF_DONTCARE | DEFAULT_PITCH,	// Family And Pitch
		L"Tahoma");			// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);		// Selects The Font We Want
	wglUseFontBitmaps(hDC, 0, 256, base);			// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);				// Selects The Font We Want
	DeleteObject(font);					// Delete The Font
   // *********************************
	grid.AStar();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}