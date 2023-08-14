#ifndef __QUESTIONS_H__
#define __QUESTIONS_H__

#include "helper.h"

struct QTN
{
    std::wstring question;
    std::wstring a1;
    std::wstring a2;
    std::wstring a3;
    std::wstring a4;
    std::wstring cr;
};

QTN question[1000];
int currentQ = 0;
int maxQ = 0;
int points = 0;
int lives = 3;

void loadquestions(std::string filename)
{
    maxQ = 0;
    points = 0;
    currentQ = 0;
    lives = 3;
    std::wifstream file(filename);
    while(!file.eof())
    {
        std::getline(file, question[maxQ].question);
        std::getline(file, question[maxQ].a1);
        std::getline(file, question[maxQ].a2);
        std::getline(file, question[maxQ].a3);
        std::getline(file, question[maxQ].a4);
        std::getline(file, question[maxQ].cr);
        maxQ++;
    }
    file.close();
}

void incrementQuestion()
{
    currentQ++;
}

void incrementPoints()
{
    points++;
}

void decrementLives()
{
    lives--;
}

void showcorrect(HWND hwnd, int winx, int winy)
{
    Graphics graphics(GetDC(hwnd));
    graphics.Clear(Color(100, 0, 170));

    Font fnt(L"Segoe UI", 100, FontStyleRegular, UnitPixel);
    StringFormat sf;
    sf.SetAlignment(StringAlignmentCenter);
    sf.SetLineAlignment(StringAlignmentCenter);
    SolidBrush brush(Color(255, 255, 255));

    graphics.DrawString(L"CORRECT!", -1, &fnt, RectF(0, 0, winx, winy), &sf, &brush);
    Sleep(1500);
    incrementQuestion();
    incrementPoints();

    if(currentQ == maxQ)
    {
        graphics.Clear(Color(100, 0, 170));
        Font fnt2(L"Segoe UI", 50, FontStyleRegular, UnitPixel);
        std::wstring prompt = L"CONGRATULATIONS!\nYou completed the quiz!\n";
                    prompt += L"\nTotal points: " + std::to_wstring(points) + L"/" + std::to_wstring(maxQ) + \
                              L"\nRemaining lives: " + std::to_wstring(lives) + \
                              L"\nTotal score: " + std::to_wstring((points * 100) / maxQ) + L"%";
        graphics.DrawString(prompt.c_str(), -1, &fnt2, RectF(0, 0, winx, winy), &sf, &brush);
        Sleep(2000);
    }
}

void showwrong(HWND hwnd, int winx, int winy)
{
    Graphics graphics(GetDC(hwnd));
    graphics.Clear(Color(100, 0, 170));

    Font fnt(L"Segoe UI", 100, FontStyleRegular, UnitPixel);
    StringFormat sf;
    sf.SetAlignment(StringAlignmentCenter);
    sf.SetLineAlignment(StringAlignmentCenter);
    SolidBrush brush(Color(255, 255, 255));

    graphics.DrawString(L"WRONG!", -1, &fnt, RectF(0, 0, winx, winy), &sf, &brush);
    Sleep(1500);
    decrementLives();

    if(lives < 0)
    {
        graphics.Clear(Color(100, 0, 170));
        Font fnt2(L"Segoe UI", 50, FontStyleRegular, UnitPixel);
        std::wstring prompt = L"SORRY!\nYou are out of lives!\n";
                    prompt += L"\nTotal points: " + std::to_wstring(points) + L"/" + std::to_wstring(maxQ) + \
                              L"\nRemaining lives: " + std::to_wstring(lives) + \
                              L"\nTotal score: " + std::to_wstring((points * 100) / maxQ) + L"%";
        graphics.DrawString(prompt.c_str(), -1, &fnt2, RectF(0, 0, winx, winy), &sf, &brush);
        Sleep(2000);
    }
}

#endif // __QUESTIONS_H__
