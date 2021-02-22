#include "GraphicsManager.h"

void GraphicsManager::DrawRect(int x, int y, int w, int h)
{
	glBegin(GL_QUADS);
	glVertex2i(x, y);
	glVertex2i(x, y + h);
	glVertex2i(x + w, y + h);
	glVertex2i(x + w, y);
	glEnd();
}

void GraphicsManager::DrawRectBorder(int x, int y, int w, int h)
{
	glBegin(GL_LINE_LOOP);
	glVertex2i(x, y);
	glVertex2i(x, y + h);
	glVertex2i(x + w, y + h);
	glVertex2i(x + w, y);
	glEnd();
}

void GraphicsManager::DrawDir(int x, int y, int direc)
{
	glBegin(GL_LINES);
	switch (direc) {
	case 1:
		glVertex2i(x * TX + (TX * 3) / 4, y * TY + TY / 2);
		glVertex2i(x * TX + TX / 4, y * TY + TY / 2);
		glVertex2i(x * TX + TX / 4, y * TY + TY / 2);
		glVertex2i(x * TX + TX / 4 + 2, y * TY + TY / 2 - 2);
		glVertex2i(x * TX + TX / 4, y * TY + TY / 2);
		glVertex2i(x * TX + TX / 4 + 2, y * TY + TY / 2 + 2);
		break;
	case 2:
		glVertex2i(x * TX + TX / 2, y * TY + (TY * 3) / 4);
		glVertex2i(x * TX + TX / 2, y * TY + TY / 4);
		glVertex2i(x * TX + TX / 2, y * TY + TY / 4);
		glVertex2i(x * TX + TX / 2 - 2, y * TY + TY / 4 + 2);
		glVertex2i(x * TX + TX / 2, y * TY + TY / 4);
		glVertex2i(x * TX + TX / 2 + 2, y * TY + TY / 4 + 2);
		break;
	case 3:
		glVertex2i(x * TX + (TX * 3) / 4, y * TY + TY / 2);
		glVertex2i(x * TX + TX / 4, y * TY + TY / 2);
		glVertex2i(x * TX + (TX * 3) / 4, y * TY + TY / 2);
		glVertex2i(x * TX + (TX * 3) / 4 - 2, y * TY + TY / 2 - 2);
		glVertex2i(x * TX + (TX * 3) / 4, y * TY + TY / 2);
		glVertex2i(x * TX + (TX * 3) / 4 - 2, y * TY + TY / 2 + 2);
		break;
	case 4:
		glVertex2i(x * TX + TX / 2, y * TY + (TY * 3) / 4);
		glVertex2i(x * TX + TX / 2, y * TY + TY / 4);
		glVertex2i(x * TX + TX / 2, y * TY + (TY * 3) / 4);
		glVertex2i(x * TX + TX / 2 - 2, y * TY + (TY * 3) / 4 - 2);
		glVertex2i(x * TX + TX / 2, y * TY + (TY * 3) / 4);
		glVertex2i(x * TX + TX / 2 + 2, y * TY + (TY * 3) / 4 - 2);
		break;
	case 5:
		glVertex2i(x * TX + (TX * 3) / 4, y * TY + (TY * 3) / 4);
		glVertex2i(x * TX + TX / 4, y * TY + TY / 4);
		glVertex2i(x * TX + TX / 4, y * TY + TY / 4);
		glVertex2i(x * TX + TX / 4, y * TY + TY / 4 + 3);
		glVertex2i(x * TX + TX / 4, y * TY + TY / 4);
		glVertex2i(x * TX + TX / 4 + 3, y * TY + TY / 4);
		break;
	case 6:
		glVertex2i(x * TX + TX / 4, y * TY + (TY * 3) / 4);
		glVertex2i(x * TX + (TX * 3) / 4, y * TY + TY / 4);
		glVertex2i(x * TX + (TX * 3) / 4, y * TY + TY / 4);
		glVertex2i(x * TX + (TX * 3) / 4, y * TY + TY / 4 + 3);
		glVertex2i(x * TX + (TX * 3) / 4, y * TY + TY / 4);
		glVertex2i(x * TX + (TX * 3) / 4 - 3, y * TY + TY / 4);
		break;
	case 7:
		glVertex2i(x * TX + TX / 4, y * TY + (TY * 3) / 4);
		glVertex2i(x * TX + (TX * 3) / 4, y * TY + TY / 4);
		glVertex2i(x * TX + TX / 4, y * TY + (TY * 3) / 4);
		glVertex2i(x * TX + TX / 4, y * TY + (TY * 3) / 4 - 3);
		glVertex2i(x * TX + TX / 4, y * TY + (TY * 3) / 4);
		glVertex2i(x * TX + TX / 4 + 3, y * TY + (TY * 3) / 4);
		break;
	case 8:
		glVertex2i(x * TX + (TX * 3) / 4, y * TY + (TY * 3) / 4);
		glVertex2i(x * TX + TX / 4, y * TY + TY / 4);
		glVertex2i(x * TX + (TX * 3) / 4, y * TY + (TY * 3) / 4);
		glVertex2i(x * TX + (TX * 3) / 4, y * TY + (TY * 3) / 4 - 3);
		glVertex2i(x * TX + (TX * 3) / 4, y * TY + (TY * 3) / 4);
		glVertex2i(x * TX + (TX * 3) / 4 - 3, y * TY + (TY * 3) / 4);
		break;
	default:
		break;
	}
	glEnd();
}

GLvoid GraphicsManager::glPrint(const char* fmt, GLuint base, ...)
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
