#include <graphics.h>
#include <conio.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int x1=50,y1=100, x2=100,y2=50, x3=150,y3=100;

    // Original
    line(x1,y1,x2,y2);
    line(x2,y2,x3,y3);
    line(x3,y3,x1,y1);
    setfillstyle(SOLID_FILL, RED);
    floodfill(100,80,WHITE);

    // Scaling
    int sx=2, sy=2;
    line(x1*sx,y1*sy,x2*sx,y2*sy);
    line(x2*sx,y2*sy,x3*sx,y3*sy);
    line(x3*sx,y3*sy,x1*sx,y1*sy);
    setfillstyle(SOLID_FILL, GREEN);
    floodfill(200,150,WHITE);

    // Translation
    int tx=300, ty=0;
    line(x1+tx,y1+ty,x2+tx,y2+ty);
    line(x2+tx,y2+ty,x3+tx,y3+ty);
    line(x3+tx,y3+ty,x1+tx,y1+ty);
    setfillstyle(SOLID_FILL, BLUE);
    floodfill(400,80,WHITE);

    getch();
    closegraph();
}