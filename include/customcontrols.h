#ifndef __CUSTOMCONTROLS_H__
#define __CUSTOMCONTROLS_H__

#include <windows.h>
#include <commctrl.h>

#define HOVER 0x0001
#define LBDOWN 0x0002
#define LBUP 0x0003

BOOL g_fMouseTracking = FALSE;

LRESULT CALLBACK OwnerDrawButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    LRESULT lr = 0;
    switch (uMsg)
    {
    case WM_LBUTTONDBLCLK:
        return TRUE;
        break;

    case  WM_MOUSEMOVE:
        if (!g_fMouseTracking)
        {
            TRACKMOUSEEVENT tme;
            tme.cbSize = sizeof(TRACKMOUSEEVENT);
            tme.dwFlags = TME_HOVER | TME_LEAVE;
            tme.hwndTrack = hWnd;
            tme.dwHoverTime = 50;
            g_fMouseTracking = TrackMouseEvent(&tme);
        }
        break;

    case  WM_MOUSEHOVER:
    {
        g_fMouseTracking = FALSE;
        SendMessage(hWnd, WM_PAINT, MAKEWPARAM(HOVER, 0), (LPARAM) NULL);
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = hWnd;
        g_fMouseTracking = TrackMouseEvent(&tme);
    }
    return 0;
    break;

    case WM_MOUSELEAVE:
        g_fMouseTracking = FALSE;
        SendMessage(hWnd, WM_PAINT, MAKEWPARAM(0, 0), (LPARAM) NULL);
        return 0;
        break;

    case WM_LBUTTONDOWN:
        lr = DefSubclassProc(hWnd, uMsg, wParam, lParam);
        SendMessage(hWnd, WM_PAINT, MAKEWPARAM(LBDOWN, 0), (LPARAM) NULL);
        break;

    case WM_LBUTTONUP:
        lr = DefSubclassProc(hWnd, uMsg, wParam, lParam);
        SendMessage(hWnd, WM_PAINT, MAKEWPARAM(HOVER, 0), (LPARAM) NULL);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = GetDC(hWnd);
        BeginPaint(hWnd, &ps);
        HBRUSH blue = CreateSolidBrush(RGB(150,0,220));
        HBRUSH ltblue = CreateSolidBrush(RGB(130,0,220));
        HBRUSH dkblue = CreateSolidBrush(RGB(170,0,170));

        RECT rc;
        GetClientRect(hWnd, &rc);

        HRGN hrgn = CreateRoundRectRgn(0, 0, rc.right, rc.bottom, 3, 3);

        switch(LOWORD(wParam))
        {
        case HOVER:
            SetTextColor(hdc, RGB(255, 255, 255));
            FillRgn(hdc, hrgn, ltblue);
            break;

        case LBDOWN:

            FillRgn(hdc, hrgn, dkblue);
            break;

        default:
            SetTextColor(hdc, RGB(0, 0, 0));
            SelectObject(hdc, blue);
            PaintRgn(hdc, hrgn);
            break;
        }

        CHAR text[500];
        GetWindowText(hWnd, text, sizeof(text));
        SetTextColor(hdc, RGB(255, 255, 255));
        HFONT font = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Segoe UI");
        SelectObject(hdc, font);
        SetBkMode(hdc, TRANSPARENT);
        DrawText(hdc, text, -1, &rc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

        EndPaint(hWnd, &ps);
    }
    break;

    default:
        return DefSubclassProc(hWnd, uMsg, wParam, lParam);
    }

    return lr;
}

#endif // __CUSTOMCONTROLS_H__
