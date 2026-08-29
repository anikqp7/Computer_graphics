#include <graphics.h>
#include <cmath>
using namespace std;

int main()
{
    int gd=DETECT,gm;
    initgraph(&gd,&gm,"");

    int x1=100,y1=100,x2=500,y2=300;
    int dx=x2-x1,dy=y2-y1;
    int p=2*dy-dx;

    while(x1<=x2)
    {
        putpixel(x1,y1,RED);
        x1++;

        if(p<0)
            p+=2*dy;
        else
        {
            y1++;
            p+=2*(dy-dx);
        }
    }

    getch();
    closegraph();
}