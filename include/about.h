#ifndef __ABOUT_H__
#define __ABOUT_H__

INT_PTR CALLBACK DlgAboutProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);
    switch(msg)
    {
    case WM_INITDIALOG:
    {
        HWND button = GetDlgItem(hwnd, IDOK);
        SetWindowSubclass(button, OwnerDrawButtonProc, 0, 0);
        SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG) GetStockObject(BLACK_BRUSH));
        return (INT_PTR)TRUE;
    }

    break;

    case WM_CTLCOLORSTATIC:
        SetTextColor((HDC) wparam, RGB(255, 255, 255));
        SetBkMode((HDC) wparam, TRANSPARENT);

    case WM_CTLCOLORDLG:
        return (INT_PTR) CreateSolidBrush(RGB(100, 0, 170));
        break;

    case WM_COMMAND:
        if(LOWORD(wparam) == IDOK)
            EndDialog(hwnd, (INT_PTR) NULL);
        break;

    case WM_CLOSE:
        EndDialog(hwnd, (INT_PTR) NULL);
        break;

    }

    return (INT_PTR)FALSE;
}

#endif // __ABOUT_H__
