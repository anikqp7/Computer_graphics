#include <graphics.h>
#include <cmath>
#include <algorithm>
using namespace std;

int main()
{
    int gd=DETECT, gm;
    initgraph(&gd,&gm,"");

    int x1=100,y1=100,x2=500,y2=300;
    int dx=x2-x1, dy=y2-y1;
    int s=max(abs(dx),abs(dy));

    float x=x1,y=y1;
    float xi=(float)dx/s, yi=(float)dy/s;

    for(int i=0;i<=s;i++)
    {
        putpixel(round(x),round(y),WHITE);
        x+=xi;
        y+=yi;
    }

    getch();
    closegraph();
}