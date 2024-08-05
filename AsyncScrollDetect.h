#pragma ones

#include <windows.h>
//#include <ctime>
#include <time.h>

HHOOK mh;
int delta = 0;

LRESULT CALLBACK Mouse(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
	{
		return CallNextHookEx(mh, nCode, wParam, lParam);
	}
	
    MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam;

    if (pMouseStruct != NULL)
    {
        if (wParam == WM_MOUSEWHEEL)
        {
            if (HIWORD(pMouseStruct->mouseData) == 120)
            {
            	delta = 1;
			}
			else
			{
				delta = -1;
			}
        }
        else
        {
        	delta = 0;
		}
    }
    
    return CallNextHookEx(mh, nCode, wParam, lParam);
}

int GetScrollDelta()
{
	delta = 0;
	if (!(mh = SetWindowsHookExA(WH_MOUSE_LL, Mouse, NULL, 0)))
	{
		delta = -404;
	}
	
	MSG message;
	int peek = 1;
	long tm = time(0);
	
	while (peek)
	{
		PeekMessage(&message, NULL, 0, 0, PM_REMOVE);
		if (!delta == 0 || tm < time(0))
		{
			peek = 0;
		}
	}
	
	UnhookWindowsHookEx(mh);
	
	return delta;
}
