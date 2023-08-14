#ifndef __STARTPROMPT_H__
#define __STARTPROMPT_H__

#include "customcontrols.h"
#include "helper.h"
#include "resource.h"
#include "about.h"
#include <commctrl.h>
#include <commdlg.h>

std::string filename;
HWND load;
HWND quit;

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);
    OPENFILENAME ofn;
    char szFile[260];
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

    switch(msg)
    {
    case WM_INITDIALOG:
    {
        HWND load = GetDlgItem(hwnd, IDLOAD);
        HWND instructions = GetDlgItem(hwnd, IDINSTRUCTIONS);
        HWND about = GetDlgItem(hwnd, IDABOUT);
        HWND quit = GetDlgItem(hwnd, IDQUIT);

        SetWindowSubclass(load, OwnerDrawButtonProc, 0, 0);
        SetWindowSubclass(instructions, OwnerDrawButtonProc, 0, 0);
        SetWindowSubclass(about, OwnerDrawButtonProc, 0, 0);
        SetWindowSubclass(quit, OwnerDrawButtonProc, 0, 0);
    }
    break;

    case WM_CTLCOLORSTATIC:
        SetTextColor((HDC) wparam, RGB(255, 255, 255));
        SetBkMode((HDC) wparam, TRANSPARENT);

    case WM_CTLCOLORDLG:
        return (INT_PTR) CreateSolidBrush(RGB(100, 0, 170));
        break;


    case WM_CLOSE:
        EndDialog(hwnd, (INT_PTR) NULL);
        break;

    case WM_COMMAND:
        if(LOWORD(wparam) == IDLOAD)
        {
            if(GetOpenFileName(&ofn) == TRUE)
            {
                filename = ofn.lpstrFile;
                EndDialog(hwnd, (INT_PTR) NULL);
            }
        }

        if(LOWORD(wparam) == IDINSTRUCTIONS)
            DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_INSTRUCTIONS), hwnd, DlgAboutProc);

        if(LOWORD(wparam) == IDABOUT)
            DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hwnd, DlgAboutProc);

        if(LOWORD(wparam) == IDQUIT) EndDialog(hwnd, (INT_PTR) NULL);
        UpdateWindow(hwnd);
        break;
    }

    return (INT_PTR)FALSE;
}


std::string StartPromptMain()
{
    filename = "";
    InitCommonControls();
    DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
    return filename;
}

#endif // __STARTPROMPT_H__
