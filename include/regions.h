#ifndef __REGIONS_H__
#define __REGIONS_H__

#include "helper.h"

struct BOXRGN
{
    int x;
    int y;
    int w;
    int h;
    std::wstring text;
};

BOXRGN prompt = {0, 0, 0, 0, L""};
BOXRGN op1 = {0, 0, 0, 0, L""};
BOXRGN op2 = {0, 0, 0, 0, L""};
BOXRGN op3 = {0, 0, 0, 0, L""};
BOXRGN op4 = {0, 0, 0, 0, L""};

bool inside(BOXRGN *rgn, int x, int y)
{
    if(!(x > rgn->x)) return false;
    if(!(x < rgn->x + rgn->w)) return false;
    if(!(y > rgn->y)) return false;
    if(!(y < rgn->y + rgn->h)) return false;
    return true;
}

#endif // __REGIONS_H__
