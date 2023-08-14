#include "regions.h"
#include "questions.h"
#include "startprompt.h"
#include "img_data.h"

HINSTANCE gInstance;
HWND hwnd;
int winx = 800, winy = 600;
int imgw = 0, imgh = 0;
std::wstring imagePath;


void setupImage()
{
    wchar_t path[500];
    GetEnvironmentVariableW(L"localappdata", path, 500);
    imagePath = path;
    imagePath += L"\\aq_box.png";
    std::wofstream outfile(imagePath.c_str(), std::ios_base::binary);
    outfile.write(rawImageData, sizeof(rawImageData) / sizeof(wchar_t));
    outfile.close();
}

void setMetrics()
{
    prompt = {0, winy - (((winx/2) * imgh / imgw) * 4), winx, (winx) * imgh / imgw, L""};

    int opw = winx/2;
    int oph = (winx/2) * imgh / imgw;
    op1 = {0, winy - ((winx/2) * imgh / imgw) * 2, opw, oph, L""};
    op3 = {0, winy - ((winx/2) * imgh / imgw), opw, oph, L""};
    op2 = {opw, winy - ((winx/2) * imgh / imgw) * 2, opw, oph, L""};
    op4 = {opw, winy - ((winx/2) * imgh / imgw), opw, oph, L""};
}

void display()
{
    if(!hwnd) return;

    Graphics graphics(GetDC(hwnd));
    graphics.Clear(Color(100,0,170));
    Image abox(imagePath.c_str());

    imgw = abox.GetWidth();
    imgh = abox.GetHeight();

    setMetrics();

    prompt.text = question[currentQ].question;
    op1.text = question[currentQ].a1;
    op2.text = question[currentQ].a2;
    op3.text = question[currentQ].a3;
    op4.text = question[currentQ].a4;

    // Draw answer boxes
    graphics.DrawImage(&abox, prompt.x, prompt.y, prompt.w, prompt.h);
    graphics.DrawImage(&abox, op1.x, op1.y, op1.w, op1.h);
    graphics.DrawImage(&abox, op2.x, op2.y, op2.w, op2.h);
    graphics.DrawImage(&abox, op3.x, op3.y, op3.w, op3.h);
    graphics.DrawImage(&abox, op4.x, op4.y, op4.w, op4.h);

    // Font setup
    Font fnt(L"Segoe UI", op1.h * 4/10, FontStyleRegular, UnitPixel);
    StringFormat sf;
    sf.SetAlignment(StringAlignmentCenter);
    sf.SetLineAlignment(StringAlignmentCenter);
    SolidBrush brush(Color(255, 255, 255));

    // Draw HUD
    std::wstring tmp;
    tmp = L"POINTS: " + std::to_wstring(points) + \
          L"\nLIVES: " + std::to_wstring(lives) + \
          L"\nQUESTION: " + std::to_wstring(currentQ + 1) + L"/" + std::to_wstring(maxQ) + \
          L"\nPress Esc to exit";
    graphics.DrawString(tmp.c_str(), -1, &fnt, RectF(0, 0, winx, winy/2), &sf, &brush);

    // Draw question
    graphics.DrawString(prompt.text.c_str(), -1, &fnt, RectF(prompt.x, prompt.y, prompt.w, prompt.h), &sf, &brush);
    sf.SetAlignment(StringAlignmentNear);
    int offset = op1.w * 1/10;
    // Draw options
    graphics.DrawString(op1.text.c_str(), -1, &fnt, RectF(op1.x + offset, op1.y, op1.w, op1.h), &sf, &brush);
    graphics.DrawString(op2.text.c_str(), -1, &fnt, RectF(op2.x + offset, op2.y, op2.w, op2.h), &sf, &brush);
    graphics.DrawString(op3.text.c_str(), -1, &fnt, RectF(op3.x + offset, op3.y, op3.w, op3.h), &sf, &brush);
    graphics.DrawString(op4.text.c_str(), -1, &fnt, RectF(op4.x + offset, op4.y, op4.w, op4.h), &sf, &brush);

    graphics.Flush();

    glutSwapBuffers();
}

void key(unsigned char k, int x, int y)
{
    std::string tmp;
    switch(k)
    {
    case 27:
    case 'q':
    case 'Q':
        ShowWindow(hwnd, SW_HIDE);
        tmp = StartPromptMain();
        if(tmp == "") glutLeaveMainLoop();
        else loadquestions(tmp);
        ShowWindow(hwnd, SW_SHOW);
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

void idle()
{
    if(!hwnd)
    {
        hwnd = FindWindow(NULL, "FreeGLUT Sample");
        gInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
        glutPostRedisplay();
        glutReshapeWindow(winx + 2, winy);
    }
}

void reshape(int x, int y)
{
    if(x < 640) glutReshapeWindow(640, y);
    if(y < 480) glutReshapeWindow(x, 480);

    winx = x;
    winy = y;

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    // button values: 0 -> left mouse button; 1 -> middle mouse button; 2 -> right mouse button
    // state values: 0 -> pressed; 1 -> released

    BOXRGN answer = {op1.x, op1.y, op1.w + op4.w, op1.h + op4.h, L""};
    std::wstring tmp;
    if(state == 0)
    {
        if(!inside(&answer, x, y)) return;

        if(inside(&op1, x, y)) tmp = op1.text;
        if(inside(&op2, x, y)) tmp = op2.text;
        if(inside(&op3, x, y)) tmp = op3.text;
        if(inside(&op4, x, y)) tmp = op4.text;
        if(tmp == question[currentQ].cr)
        {
            showcorrect(hwnd, winx, winy);
            if(currentQ == maxQ)
            {
                MessageBox(NULL, "Game completed!", "KBC", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
                key(27, 0, 0); // simulate Esc key
            }
        }
        else
        {
            showwrong(hwnd, winx, winy);
            if(lives < 0)
            {
                MessageBox(NULL, "Game failed!", "KBC", MB_OK | MB_ICONERROR | MB_APPLMODAL);
                key(27, 0, 0); // simulate Esc key
            }
        }
    }
}

int main(int argc, char** argv)
{
#ifdef _RELEASE
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif // _RELEASE

    // GDI+ Setup
    GdiplusStartupInput gsInput;
    ULONG_PTR gToken;
    GdiplusStartup(&gToken, &gsInput, NULL);

    // Misc setup
    setupImage();

    // Load questions
    std::string tmp = StartPromptMain();
    if(tmp == "") goto death;
    else loadquestions(tmp);


    // FreeGLUT setup
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(winx, winy);
    glutCreateWindow("FreeGLUT Sample");

    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);

    // Main loop
    glutMainLoop();

death:
    ;
    DeleteFileW(imagePath.c_str());
    GdiplusShutdown(gToken);

    return 0;
}
