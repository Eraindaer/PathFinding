#define _CRT_SECURE_NO_WARNINGS
/**************************
 * Includes
 *
 **************************/

#include <windows.h>
#include <stdio.h>
#include <gl/gl.h>
#include <gl/glu.h>


/**************************
 * Function Declarations
 *
 **************************/

#ifndef CALLBACK
#define CALLBACK
#endif

#define GAUCHE	1
#define HAUT	2
#define DROITE	3
#define BAS		4
#define GAUCHEHAUT	5
#define DROITEHAUT	6
#define GAUCHEBAS	7
#define DROITEBAS	8

#define BX	40
#define BY	30
#define TX	16
#define TY	16

#define ABS(x)	((x)<0?(-(x)):(x))	

HWND hWnd;
HDC hDC;
GLuint base;
int width;//taille de la fenêtre
int height;
DWORD time;//temps de calcul

// la structure pour une case
struct cases
{
	int x;
	int y;
	int F,G,H;
};

cases nonfranchissable[BX*BY]={};
int nbnonfranchissable=0;

cases ouvert[BX*BY]={};
int nbouvert=0;

cases ferme[BX*BY]={};
int nbferme=0;	

cases chemin[BX*BY]={};
int nbchemin;

cases depart={0,0};
cases arrivee={BX-1,BY-1};

char dir[BX][BY];

bool nochemin=false;// pour savoir si il y a un chemin
bool viewhelp=false;// pour afficher l'aide (H)

void display(void);//affichage
void FullRectangle (int x,int y,int w,int h);//Dessine un rectangle plein
void glPrint (const char*fmt,...);//Ecrit le texte 

void AddF (cases c)//Ajoute une case aux fermés
{
	ferme[nbferme]=c;
	nbferme++;
}

void AddO (int x,int y,char direc,int g)//on ajoute la case en ouvert si case valide et on calcule F,G,H
{
	int a;
	if (!(x<0 || x>=BX || y<0 || y>=BY))//bornes
	{
		for (a=0;a<nbferme;a++)//case fermée?
			if (ferme[a].x==x && ferme[a].y==y)
				return;
				
		for (a=0;a<nbnonfranchissable;a++)//case franchissable?
			if (nonfranchissable[a].x==x && nonfranchissable[a].y==y)
				return;
				
		for (a=0;a<nbouvert;a++)//case ouverte?
			if (ouvert[a].x==x && ouvert[a].y==y)
			{
				if (ouvert[a].G>g)// on change la direction dans la case
				{
    				ouvert[a].G=g;
    				ouvert[a].F=g+ouvert[a].H;
    				dir[x][y]=direc;
				}
				return;
			}
		// on rajoute la case
		ouvert[nbouvert].x=x;
		ouvert[nbouvert].y=y;
		ouvert[nbouvert].G=g;
		ouvert[nbouvert].H=10*(ABS(arrivee.x-x)+ABS(arrivee.y-y));
		ouvert[nbouvert].F=g+ouvert[nbouvert].H;
		dir[x][y]=direc;

		nbouvert++;
	}
}

void SupO (cases c)//On supprime la case des ouverts
{
	for (int a=0;a<nbouvert;a++)
	
		if (c.x==ouvert[a].x && c.y==ouvert[a].y)	
		{
			for (int i=a+1;i<nbouvert;i++)
				ouvert[i-1]=ouvert[i];
			nbouvert--;
		}
}

void vide (void)// on vide tout
{
	nbouvert=0;
 	nbferme=0;
  	nbchemin=0;
}				
void DrawDir(int x,int y,int direc)// on dessine les directions dans les cases (le truc bleu clair)
{
	glBegin (GL_LINES);
	if (direc==HAUT||direc==BAS)
	{
		glVertex2i(x*TX+TX/2,y*TY+(TY*3)/4);
		glVertex2i(x*TX+TX/2,y*TY+TY/4);
	}
	if (direc==GAUCHE||direc==DROITE)
	{
		glVertex2i(x*TX+(TX*3)/4,y*TY+TY/2);
		glVertex2i(x*TX+TX/4,y*TY+TY/2);
	}	
	if (direc==GAUCHEBAS||direc==DROITEHAUT)
	{
		glVertex2i(x*TX+TX/4,y*TY+(TY*3)/4);
		glVertex2i(x*TX+(TX*3)/4,y*TY+TY/4);
	}
	if (direc==GAUCHEHAUT||direc==DROITEBAS)
	{
		glVertex2i(x*TX+(TX*3)/4,y*TY+(TY*3)/4);
		glVertex2i(x*TX+TX/4,y*TY+TY/4);
	}	
	if (direc==HAUT)
	{
		glVertex2i (x*TX+TX/2,y*TY+TY/4);
		glVertex2i (x*TX+TX/2-2,y*TY+TY/4+2);
		glVertex2i (x*TX+TX/2,y*TY+TY/4);
		glVertex2i (x*TX+TX/2+2,y*TY+TY/4+2);
	}	
	else if (direc==BAS)
 	{
 		glVertex2i(x*TX+TX/2,y*TY+(TY*3)/4);
   		glVertex2i(x*TX+TX/2-2,y*TY+(TY*3)/4-2);
   		glVertex2i(x*TX+TX/2,y*TY+(TY*3)/4);
		glVertex2i(x*TX+TX/2+2,y*TY+(TY*3)/4-2);
    }		
    else if (direc==GAUCHE)
    {
		glVertex2i(x*TX+TX/4,y*TY+TY/2);
		glVertex2i(x*TX+TX/4+2,y*TY+TY/2-2);
		glVertex2i(x*TX+TX/4,y*TY+TY/2);
		glVertex2i(x*TX+TX/4+2,y*TY+TY/2+2);
	}  
 	else if (direc==DROITE)
  	{
		glVertex2i(x*TX+(TX*3)/4,y*TY+TY/2);
		glVertex2i(x*TX+(TX*3)/4-2,y*TY+TY/2-2);
		glVertex2i(x*TX+(TX*3)/4,y*TY+TY/2);
		glVertex2i(x*TX+(TX*3)/4-2,y*TY+TY/2+2);
    }  
    else if (direc==GAUCHEHAUT)
    {
	    glVertex2i(x*TX+TX/4,y*TY+TY/4);
		glVertex2i(x*TX+TX/4,y*TY+TY/4+3);
		glVertex2i(x*TX+TX/4,y*TY+TY/4);
		glVertex2i(x*TX+TX/4+3,y*TY+TY/4);
	}    	
	else if (direc==DROITEBAS)
	{
		glVertex2i(x*TX+(TX*3)/4,y*TY+(TY*3)/4);
		glVertex2i(x*TX+(TX*3)/4,y*TY+(TY*3)/4-3);
		glVertex2i(x*TX+(TX*3)/4,y*TY+(TY*3)/4);
		glVertex2i(x*TX+(TX*3)/4-3,y*TY+(TY*3)/4);
	}
	else if (direc==GAUCHEBAS)
	{
		glVertex2i(x*TX+TX/4,y*TY+(TY*3)/4);
		glVertex2i(x*TX+TX/4,y*TY+(TY*3)/4-3);
		glVertex2i(x*TX+TX/4,y*TY+(TY*3)/4);
		glVertex2i(x*TX+TX/4+3,y*TY+(TY*3)/4);
	}		
	else if (direc==DROITEHAUT)
	{
		glVertex2i(x*TX+(TX*3)/4,y*TY+TY/4);
		glVertex2i(x*TX+(TX*3)/4,y*TY+TY/4+3);
		glVertex2i(x*TX+(TX*3)/4,y*TY+TY/4);
		glVertex2i(x*TX+(TX*3)/4-3,y*TY+TY/4);
	}	
	glEnd();
}		
		
void astar (void)// L'algorithme A*...
{
	nochemin=false;
	time=GetTickCount();
	vide();
	cases courant=depart;
	bool arrtrouv=false;

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
		cases temp = arrivee;
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
	
	
void init (void)
{
   // Piqué chez Nehe***************
	HFONT	font;						// Windows Font ID
	HFONT	oldfont;					// Used For Good House Keeping

	base = glGenLists(256);					// Storage For 256 Characters ( NEW )
	font = CreateFont(	-12,				// Height Of Font ( NEW )
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
				FF_DONTCARE|DEFAULT_PITCH,	// Family And Pitch
				L"Tahoma");			// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);		// Selects The Font We Want
	wglUseFontBitmaps(hDC, 0, 256, base);			// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);				// Selects The Font We Want
	DeleteObject(font);					// Delete The Font
   // *********************************
	astar();
	glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_BLEND);
}
//Piqué chez Nehe *********************
GLvoid glPrint(const char *fmt, ...)				// Custom GL "Print" Routine
{
	char		text[256];				// Holds Our String
	va_list		ap;					// Pointer To List Of Arguments
	if (fmt == NULL)					// If There's No Text
		return;						// Do Nothing
	va_start(ap, fmt);					// Parses The String For Variables
	    vsprintf(text, fmt, ap);				// And Converts Symbols To Actual Numbers
	va_end(ap);						// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);				// Pushes The Display List Bits		( NEW )
	glListBase(base);					// Sets The Base Character to 32	( NEW )

	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text	( NEW )
	glPopAttrib();						// Pops The Display List Bits	( NEW )
}
//*************************************

//dessine un rectangle plein
void FullRectangle (int x,int y,int w,int h)
{
	glBegin (GL_QUADS);
	glVertex2i (x,y);
	glVertex2i (x,y+h);
	glVertex2i (x+w,y+h);
	glVertex2i (x+w,y);
	glEnd();
}
//dessine un rectangle vide (inutilisé)
void ERectangle (int x,int y,int w,int h)
{
	glBegin (GL_LINE_LOOP);
	glVertex2i (x,y);
	glVertex2i (x,y+h);
	glVertex2i (x+w,y+h);
	glVertex2i (x+w,y);
	glEnd();
}	

// procédure d'affichage
void display (void)
{
	glClear (GL_COLOR_BUFFER_BIT);

// grille
/*	glColor3f (.5,.5,.5);
	for (int a=0;a<640;a+=16)
		for (int b=0;b<480;b+=16)
			ERectangle (a,b,16,16);*/
			
	// les murs en gris
	glColor3f (.4f,.4f,.4f); 
	for (int a=0;a<nbnonfranchissable;a++)
		FullRectangle(nonfranchissable[a].x*TX,nonfranchissable[a].y*TY,TX,TY);

    // le chemin en bleu avec les directions en cyan
	for (int a=0;a<nbchemin;a++)
	{
		glColor3f(0,0,1);		
		FullRectangle(chemin[a].x*TX,chemin[a].y*TY,TX,TY);
		glColor3f(0,1,1);
		DrawDir (chemin[a].x,chemin[a].y,dir[chemin[a].x][chemin[a].y]);
	}

	// le départ en vert
	glColor3f (0,1,0);
	FullRectangle(depart.x*TX,depart.y*TY,TX,TY);
	// l'arrivée en rouge
	glColor3f (1,0,0);
	FullRectangle(arrivee.x*TX,arrivee.y*TY,TX,TY);

	// le texte
	glColor3f(1,1,1);
	glRasterPos2i (0,10);
	glPrint("(<h> pour aide)  © 2004 Cadé David");
	if (nochemin)
	{
 		glRasterPos2i (0,20);
 		glPrint("Pas de chemin");
	}

	// l'aide
	if (viewhelp)
	{
 		glColor3f (1,0,0);
 		glRasterPos2i (20,50);
 		glPrint ("AIDE");
 		glColor3f(1,1,1);
 		glRasterPos2i (20,70);
 		glPrint ("<D> met le point de départ (case verte) sous le curseur");
 		glRasterPos2i (20,80);
 		glPrint ("<A> met le point d'arrivée (case rouge) sous le curseur");
 		glRasterPos2i (20,90); 
 		glPrint ("Un clic met ou enlève le mur (cases grises) sous le curseur");
 		glRasterPos2i (20,100);
 		glPrint ("Le curseur est constitué d'un indicateur (plein) pour voir la case courante");
	
 	}	

	// le curseur
	POINT p;
	GetCursorPos(&p);
	ScreenToClient (hWnd,&p);
	p.x=(int) ((float)p.x/(float)width*640.);
	p.y=(int) ((float)p.y/(float)height*480.);
	
	glColor4f (.5f,1.0f,.5f,.4f);	
	FullRectangle(p.x/TX*TX+2,p.y/TY*TY+2,TX-4,TY-4);

//	ERectangle (p.x-TX/2,p.y-TY/2,TX,TY);
	glBegin (GL_TRIANGLE_FAN);
		glColor4f(0,0,0,.5);
		glVertex2i (p.x+4,p.y+2);
		glVertex2i (p.x+4,p.y+15);
		glVertex2i (p.x+8,p.y+12);
		glVertex2i (p.x+14,p.y+15);
    	
	glEnd();
	glBegin (GL_TRIANGLE_FAN);
		glColor4f(1.0f,.5f,1.0f,.8f);
		glVertex2i (p.x,p.y);
		glColor4f(1.0f,1.0f,.5f,.8f);
		glVertex2i (p.x,p.y+13);
		glColor4f(1.0f,.5f,.5f,.8f);
		glVertex2i (p.x+4,p.y+10);
  		glColor4f(1.0f,0.0f,0.0f,.8f);
		glVertex2i (p.x+10,p.y+13);
    	
	glEnd();

	glFlush();
}

void reshape (int w,int h)
{
	glViewport (0,0,(GLsizei) w,(GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho (0,640,480,0,-1,1);  //projection 640*480 (comme d'hab'...)
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	width=w;
	height=h;
}

void keyboard(unsigned char key, int x,int y)
{
	x=(int) ((float)x/(float)width*640.);
	y=(int) ((float)y/(float)height*480.);

	switch (key)
 	{
 		 case 'D':
 		 case 'd':
 		 	depart.x=x/TX;
 		 	depart.y=y/TY;
 		 	astar();
 		 	break;
 	     case 'A':
 	     case 'a':
 	     	arrivee.x=x/TX;
 	     	arrivee.y=y/TY;
 	     	astar();
 	     	break;
    	 case 'H':
    	 case 'h':
    	 	viewhelp=!viewhelp;
    	 	break;
    	 	
        case 27:
        	exit(0);
            break;
        default:
        	break;
    }
}

void PopNF(int a)// on enleve le mur a
{
	for (int i=a+1;i<nbnonfranchissable;i++)
		nonfranchissable[i-1]=nonfranchissable[i];
	nbnonfranchissable--;
}

void mouseClick(int x,int y)// en cas de click, on rajoute ou on enlève un mur
{
	int a;
	x=(int) ((float)x/(float)width*640.);
	y=(int) ((float)y/(float)height*480.);
	
	x/=TX;y/=TY;
	if (nbchemin>0) vide();
	
	for (a=0;a<nbnonfranchissable;a++)
		if (nonfranchissable[a].x==x && nonfranchissable[a].y==y)
		{
			PopNF (a);
			astar();
			return;
		}
	nonfranchissable[nbnonfranchissable].x=x;
	nonfranchissable[nbnonfranchissable].y=y;
	nbnonfranchissable++;
	astar();
}

// code OpenGL produit par Dev-C++
  
LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
WPARAM wParam, LPARAM lParam);
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);

/**************************
 * WinMain
 *
 **************************/

int WINAPI 
WinMain (HINSTANCE hInstance,
         HINSTANCE hPrevInstance,
         LPSTR lpCmdLine,
         int iCmdShow)
{
    WNDCLASS wc;

    HGLRC hRC;        
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"GLSample";
    RegisterClass (&wc);

    /* create main window */
    hWnd = CreateWindow (
      L"GLSample", L"A* Test", 
      WS_OVERLAPPEDWINDOW,
      0, 0, 640, 480,
      NULL, NULL, hInstance, NULL);

    /* enable OpenGL for the window */
    EnableOpenGL (hWnd, &hDC, &hRC);
    init();
    
    ShowWindow (hWnd,SW_SHOW);
    ShowCursor (FALSE);
    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */
            display();
             
            SwapBuffers (hDC);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL (hWnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow (hWnd);

    return msg.wParam;
}

/********************
 * Window Procedure
 *
 ********************/

// Callback modifiée pour prendre les messages WM_CHAR->keyboard, WM_LBUTTON(DOWN|UP)->mouseclick,
// et WM_SIZE->reshape	
LRESULT CALLBACK 
WndProc (HWND hWnd, UINT message,
         WPARAM wParam, LPARAM lParam)
{
	static bool cl=false;

	POINT p;
    switch (message)
    {
    case WM_CREATE:
        return 0;
    case WM_CLOSE:
        PostQuitMessage (0);
        return 0;

    case WM_DESTROY:
        return 0;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;
        }
        return 0;
    case WM_CHAR:
    	GetCursorPos (&p);
     	ScreenToClient(hWnd,&p);
    	keyboard (wParam,p.x,p.y);
    	break;
   	case WM_LBUTTONDOWN:
   		cl=true;
   		break;
   	case WM_LBUTTONUP:
    	if (cl)
     	{
          	GetCursorPos (&p);
          	ScreenToClient(hWnd,&p);
          	mouseClick (p.x,p.y);
     	}
     	cl=false;
     	break;
    			
   	case WM_SIZE:
   		reshape (LOWORD (lParam),HIWORD (lParam));
    default:
        return DefWindowProc (hWnd, message, wParam, lParam);
    }
}

/*******************
 * Enable OpenGL
 *
 *******************/

void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC (hWnd);

    /* set the pixel format for the DC */
    ZeroMemory (&pfd, sizeof (pfd));
    pfd.nSize = sizeof (pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
      PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat (*hDC, &pfd);
    SetPixelFormat (*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext( *hDC );
    wglMakeCurrent( *hDC, *hRC );
}

/******************
 * Disable OpenGL
 *
 ******************/

void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext (hRC);
    ReleaseDC (hWnd, hDC);
}