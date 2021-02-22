#include "WinManager.h"
#include <memory>

int width;
int height;
HWND hWnd;
GridManager grid(hWnd, width, height);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static bool cl = false;

    POINT p;
    switch (message)
    {
    case WM_CREATE:
        return 0;
    case WM_CLOSE:
        PostQuitMessage(0);
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
        GetCursorPos(&p);
        ScreenToClient(hWnd, &p);
        //grid.Keyboard(wParam, p.x, p.y);
        break;
    case WM_LBUTTONDOWN:
        cl = true;
        break;
    case WM_LBUTTONUP:
        if (cl)
        {
            GetCursorPos(&p);
            ScreenToClient(hWnd, &p);
            //grid.MouseClick(p.x, p.y);
        }
        cl = false;
        break;

    case WM_SIZE:
        WinManager::Resize(LOWORD(lParam), HIWORD(lParam));
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

int WINAPI 
WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
    /* register window class */
    WNDCLASS wc;
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"GLSample";
    RegisterClass(&wc);
    std::unique_ptr<WinManager> wnd = std::make_unique<WinManager>(hWnd, hInstance, wc, grid, width, height);
    MSG msg;
    BOOL bQuit = FALSE;

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
            wnd->Display();
        }
    }
    return msg.wParam;
}
