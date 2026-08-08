#include <graphics.h>
#include <conio.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

#define PI 3.14159265358979323846

const int W = 1200;
const int H = 700;

//GLOBAL ANIMATION VARIABLES 
float turbineAngle = 0;
float orbitAngle = 0;
float robotAngle = 0;

int cloudX1 = 60;
int cloudX2 = 650;
int birdX = 280;

int visitorX1 = 100;
int visitorX2 = 850;

bool nightMode = false;
bool running = true;

int selectedStall = 0;

// BASIC HELPERS

void filledRect(int x1, int y1, int x2, int y2, int color)
{
    setfillstyle(SOLID_FILL, color);
    bar(x1, y1, x2, y2);
}

void filledCircle(int x, int y, int r, int color)
{
    setcolor(color);
    setfillstyle(SOLID_FILL, color);
    fillellipse(x, y, r, r);
}

void textCenter(int x1, int x2, int y, const char *txt)
{
    int w = textwidth((char*)txt);
    outtextxy((x1 + x2 - w) / 2, y, (char*)txt);
}

// SKY

void drawSky()
{
    if (!nightMode)
    {
        // layered blue sky
        filledRect(0, 0, W, 80, COLOR(75, 160, 235));
        filledRect(0, 80, W, 160, COLOR(95, 180, 245));
        filledRect(0, 160, W, 260, COLOR(130, 205, 250));
        filledRect(0, 260, W, 360, COLOR(175, 225, 250));
    }
    else
    {
        filledRect(0, 0, W, 360, COLOR(8, 18, 55));

        // stars
        int stars[][2] =
        {
            {70,55},{140,110},{215,45},{300,95},{370,55},
            {450,125},{525,60},{610,105},{695,45},{780,125},
            {860,70},{950,115},{1040,55},{1120,140},
            {175,175},{570,165},{900,185},{1070,210}
        };

        setcolor(WHITE);

        for (int i = 0; i < 18; i++)
        {
            int x = stars[i][0];
            int y = stars[i][1];

            line(x - 2, y, x + 2, y);
            line(x, y - 2, x, y + 2);
        }
    }
}


// SUN / MOON


void drawSunMoon()
{
    if (!nightMode)
    {
        filledCircle(1080, 80, 32, YELLOW);

        setcolor(COLOR(255, 220, 40));

        for (int i = 0; i < 12; i++)
        {
            float a = i * 2 * PI / 12;

            int x1 = 1080 + 40 * cos(a);
            int y1 = 80 + 40 * sin(a);

            int x2 = 1080 + 52 * cos(a);
            int y2 = 80 + 52 * sin(a);

            line(x1, y1, x2, y2);
        }
    }
    else
    {
        filledCircle(1080, 75, 30, COLOR(255, 245, 190));
        filledCircle(1092, 65, 27, COLOR(8, 18, 55));
    }
}

// CLOUD

void drawCloud(int x, int y)
{
    int c = nightMode ? COLOR(100, 110, 135) : WHITE;

    filledCircle(x, y, 22, c);
    filledCircle(x + 25, y - 12, 28, c);
    filledCircle(x + 55, y, 24, c);
    filledCircle(x + 80, y + 3, 18, c);

    setfillstyle(SOLID_FILL, c);
    bar(x, y, x + 78, y + 18);
}

// BIRD

void drawBird(int x, int y)
{
    setcolor(nightMode ? LIGHTGRAY : BLACK);

    arc(x, y, 15, 170, 8);
    arc(x + 16, y, 10, 165, 8);
}

// GRASS & DISTANT GROUND

void drawGround()
{
    int grass = nightMode ? COLOR(25, 75, 50) : COLOR(80, 180, 75);

    filledRect(0, 330, W, 700, grass);

    // distant grass line
    setcolor(nightMode ? COLOR(20, 60, 40) : COLOR(40, 135, 50));

    for (int x = 0; x < W; x += 15)
    {
        line(x, 350, x + 4, 340);
        line(x + 4, 350, x + 8, 338);
    }
}

// TREES

void drawTree(int x, int y, float scale = 1.0)
{
    int trunk = COLOR(105, 65, 35);
    int leaves = nightMode ? COLOR(20, 80, 45) : COLOR(35, 145, 55);
    int leaves2 = nightMode ? COLOR(30, 95, 55) : COLOR(50, 170, 70);

    int trunkW = 18 * scale;
    int trunkH = 90 * scale;

    filledRect(x - trunkW / 2, y, x + trunkW / 2, y + trunkH, trunk);

    filledCircle(x, y - 15 * scale, 35 * scale, leaves);
    filledCircle(x - 30 * scale, y, 30 * scale, leaves);
    filledCircle(x + 30 * scale, y, 30 * scale, leaves2);
    filledCircle(x, y - 40 * scale, 34 * scale, leaves2);
}

// DECORATIVE FLAGS

void drawFlags()
{
    setcolor(nightMode ? LIGHTGRAY : BLACK);
    line(80, 185, 1120, 185);

    int colors[] = {RED, YELLOW, GREEN, CYAN, MAGENTA, LIGHTRED};

    for (int x = 100, i = 0; x < 1120; x += 55, i++)
    {
        int pts[] =
        {
            x, 186,
            x + 25, 186,
            x + 12, 205
        };

        setfillstyle(SOLID_FILL, colors[i % 6]);
        setcolor(colors[i % 6]);
        fillpoly(3, pts);
    }
}

// ENTRANCE / TITLE

void drawFairBanner()
{
    // shadow
    filledRect(356, 112, 844, 172, COLOR(50, 50, 70));

    // main banner
    filledRect(350, 106, 838, 166, COLOR(18, 65, 125));

    setcolor(YELLOW);
    rectangle(356, 112, 832, 160);

    setbkcolor(COLOR(18, 65, 125));

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(WHITE);
    textCenter(350, 838, 120, "SCIENCE & TECHNOLOGY FAIR");

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    setcolor(YELLOW);
    textCenter(350, 838, 145, "EXPLORE  -  DISCOVER  -  INNOVATE");

    setbkcolor(nightMode ? COLOR(8,18,55) : COLOR(95,180,245));
}

// STALL BODY

void drawStallBody(int x, int y, int w, int h,
                   int mainColor, int roofColor,
                   const char *title, bool selected)
{
    // stall shadow
    filledRect(x + 8, y + 8, x + w + 8, y + h + 8, COLOR(65, 70, 65));

    // body
    filledRect(x, y, x + w, y + h, mainColor);

    setcolor(selected ? YELLOW : WHITE);
    setlinestyle(SOLID_LINE, 0, selected ? 3 : 1);
    rectangle(x, y, x + w, y + h);

    setlinestyle(SOLID_LINE, 0, 1);

    // roof / canopy
    int roof[] =
    {
        x - 8, y,
        x + 15, y - 35,
        x + w - 15, y - 35,
        x + w + 8, y
    };

    setfillstyle(SOLID_FILL, roofColor);
    setcolor(roofColor);
    fillpoly(4, roof);

    // title board
    filledRect(x + 18, y - 28, x + w - 18, y - 5, COLOR(30, 40, 55));

    setbkcolor(COLOR(30, 40, 55));
    setcolor(WHITE);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    textCenter(x + 18, x + w - 18, y - 21, title);

    setbkcolor(mainColor);

    // canopy stripe
    for (int i = 0; i < w; i += 40)
    {
        int stripeColor = ((i / 40) % 2 == 0) ? WHITE : roofColor;

        int p[] =
        {
            x + i, y,
            x + i + 20, y,
            x + i + 15, y + 15,
            x + i - 5, y + 15
        };

        setcolor(stripeColor);
        setfillstyle(SOLID_FILL, stripeColor);
        fillpoly(4, p);
    }
}

// WIND TURBINE

void drawWindTurbine(int cx, int cy)
{
    setcolor(COLOR(210, 220, 225));

    // tower
    line(cx, cy, cx - 12, cy + 110);
    line(cx, cy, cx + 12, cy + 110);
    line(cx - 12, cy + 110, cx + 12, cy + 110);

    // blades
    for (int i = 0; i < 3; i++)
    {
        float a = turbineAngle + i * 2 * PI / 3;

        int x1 = cx + 10 * cos(a);
        int y1 = cy + 10 * sin(a);

        int x2 = cx + 58 * cos(a);
        int y2 = cy + 58 * sin(a);

        setcolor(WHITE);
        setlinestyle(SOLID_LINE, 0, 3);
        line(x1, y1, x2, y2);

        filledCircle(x2, y2, 4, LIGHTCYAN);
    }

    setlinestyle(SOLID_LINE, 0, 1);
    filledCircle(cx, cy, 9, YELLOW);
}

// SOLAR PANEL

void drawSolarPanel(int x, int y)
{
    int p[] =
    {
        x, y,
        x + 80, y - 12,
        x + 90, y + 38,
        x + 10, y + 48
    };

    setfillstyle(SOLID_FILL, COLOR(25, 90, 150));
    setcolor(LIGHTCYAN);
    fillpoly(4, p);

    // panel grid
    for (int i = 1; i < 4; i++)
    {
        int xx1 = x + i * 20;
        line(xx1, y - i * 3, xx1 + 10, y + 48 - i * 3);
    }

    for (int i = 1; i < 3; i++)
    {
        line(x + 4, y + i * 15,
             x + 84, y + i * 15 - 12);
    }

    // stand
    setcolor(DARKGRAY);
    line(x + 43, y + 45, x + 43, y + 70);
    line(x + 20, y + 70, x + 65, y + 70);
}

// ENERGY STALL

void drawEnergyStall()
{
    int x = 55;
    int y = 310;
    int w = 330;
    int h = 210;

    drawStallBody(
        x, y, w, h,
        COLOR(225, 245, 220),
        COLOR(25, 150, 70),
        "RENEWABLE ENERGY",
        selectedStall == 1
    );

    drawSolarPanel(x + 30, y + 85);
    drawWindTurbine(x + 235, y + 72);

    // exhibition desk
    filledRect(x + 22, y + 172, x + 135, y + 205, COLOR(65, 130, 80));

    setbkcolor(COLOR(65, 130, 80));
    setcolor(WHITE);
    outtextxy(x + 38, y + 182, (char*)"SOLAR POWER");

    setbkcolor(COLOR(225,245,220));

    // little plant
    filledRect(x + 285, y + 168, x + 290, y + 199, COLOR(90,55,30));
    filledCircle(x + 278, y + 165, 9, GREEN);
    filledCircle(x + 294, y + 160, 10, LIGHTGREEN);
}

// ROBOT

void drawRobot(int x, int y)
{
    // head
    filledRect(x - 20, y - 55, x + 20, y - 20, COLOR(180, 200, 210));

    setcolor(BLACK);
    rectangle(x - 20, y - 55, x + 20, y - 20);

    filledCircle(x - 9, y - 42, 4, CYAN);
    filledCircle(x + 9, y - 42, 4, CYAN);

    line(x - 8, y - 28, x + 8, y - 28);

    // antenna
    line(x, y - 55, x, y - 68);
    filledCircle(x, y - 70, 3, RED);

    // body
    filledRect(x - 25, y - 15, x + 25, y + 38, COLOR(110, 150, 190));

    setcolor(WHITE);
    rectangle(x - 25, y - 15, x + 25, y + 38);

    filledCircle(x, y, 5, GREEN);

    // arms
    setcolor(COLOR(190, 200, 210));
    setlinestyle(SOLID_LINE, 0, 4);

    line(x - 25, y, x - 45, y + 25);
    line(x + 25, y, x + 45, y + 25);

    // legs
    line(x - 12, y + 38, x - 17, y + 65);
    line(x + 12, y + 38, x + 17, y + 65);

    setlinestyle(SOLID_LINE, 0, 1);
}

// ROBOTIC ARM

void drawRoboticArm(int x, int y)
{
    // base
    filledRect(x - 25, y, x + 25, y + 15, DARKGRAY);
    filledCircle(x, y, 10, LIGHTGRAY);

    float a1 = -1.55 + 0.35 * sin(robotAngle);
    float a2 = a1 + 0.8 + 0.35 * sin(robotAngle * 1.4);

    int x2 = x + 48 * cos(a1);
    int y2 = y + 48 * sin(a1);

    int x3 = x2 + 45 * cos(a2);
    int y3 = y2 + 45 * sin(a2);

    setcolor(YELLOW);
    setlinestyle(SOLID_LINE, 0, 7);

    line(x, y, x2, y2);
    line(x2, y2, x3, y3);

    setlinestyle(SOLID_LINE, 0, 1);

    filledCircle(x2, y2, 7, RED);
    filledCircle(x3, y3, 6, LIGHTRED);

    // claw
    setcolor(WHITE);
    line(x3, y3, x3 - 9, y3 - 10);
    line(x3, y3, x3 + 9, y3 - 10);
}

// ROBOTICS STALL

void drawRoboticsStall()
{
    int x = 435;
    int y = 310;
    int w = 330;
    int h = 210;

    drawStallBody(
        x, y, w, h,
        COLOR(215, 235, 250),
        COLOR(35, 110, 205),
        "ROBOTICS LAB",
        selectedStall == 2
    );

    drawRobot(x + 80, y + 110);

    // table
    filledRect(x + 145, y + 155, x + 300, y + 205, COLOR(60, 75, 95));

    setbkcolor(COLOR(60,75,95));
    setcolor(LIGHTCYAN);
    outtextxy(x + 165, y + 185, (char*)"ROBOTIC ARM");

    drawRoboticArm(x + 225, y + 150);

    // monitor
    filledRect(x + 260, y + 55, x + 310, y + 95, BLACK);
    filledRect(x + 265, y + 60, x + 305, y + 88, COLOR(15,65,60));

    setcolor(LIGHTGREEN);
    line(x + 269, y + 75, x + 278, y + 68);
    line(x + 278, y + 68, x + 286, y + 81);
    line(x + 286, y + 81, x + 300, y + 65);

    setbkcolor(COLOR(215,235,250));
}

// ROCKET

void drawRocket(int x, int y)
{
    int body[] =
    {
        x, y - 55,
        x - 18, y - 20,
        x - 15, y + 30,
        x + 15, y + 30,
        x + 18, y - 20
    };

    setfillstyle(SOLID_FILL, WHITE);
    setcolor(WHITE);
    fillpoly(5, body);

    int nose[] =
    {
        x, y - 55,
        x - 18, y - 20,
        x + 18, y - 20
    };

    setfillstyle(SOLID_FILL, RED);
    setcolor(RED);
    fillpoly(3, nose);

    filledCircle(x, y - 8, 7, CYAN);

    // fins
    int leftFin[] =
    {
        x - 15, y + 10,
        x - 30, y + 35,
        x - 13, y + 28
    };

    int rightFin[] =
    {
        x + 15, y + 10,
        x + 30, y + 35,
        x + 13, y + 28
    };

    setfillstyle(SOLID_FILL, LIGHTRED);
    fillpoly(3, leftFin);
    fillpoly(3, rightFin);

    // fire
    setcolor(YELLOW);
    line(x - 6, y + 31, x, y + 47);
    line(x + 6, y + 31, x, y + 47);
}

// SOLAR SYSTEM

void drawSolarSystem(int cx, int cy)
{
    // orbit rings
    setcolor(COLOR(150, 150, 190));
    ellipse(cx, cy, 0, 360, 55, 24);
    ellipse(cx, cy, 0, 360, 82, 38);

    // sun
    filledCircle(cx, cy, 16, YELLOW);

    float a1 = orbitAngle;
    float a2 = -orbitAngle * 0.65;

    int p1x = cx + 55 * cos(a1);
    int p1y = cy + 24 * sin(a1);

    int p2x = cx + 82 * cos(a2);
    int p2y = cy + 38 * sin(a2);

    filledCircle(p1x, p1y, 6, LIGHTBLUE);
    filledCircle(p2x, p2y, 8, COLOR(220, 90, 70));
}

// SPACE STALL

void drawSpaceStall()
{
    int x = 815;
    int y = 310;
    int w = 330;
    int h = 210;

    drawStallBody(
        x, y, w, h,
        COLOR(35, 35, 80),
        COLOR(100, 55, 160),
        "SPACE SCIENCE",
        selectedStall == 3
    );

    // small stars inside
    setcolor(WHITE);

    line(x + 40, y + 45, x + 48, y + 45);
    line(x + 44, y + 41, x + 44, y + 49);

    line(x + 285, y + 45, x + 293, y + 45);
    line(x + 289, y + 41, x + 289, y + 49);

    drawSolarSystem(x + 175, y + 90);
    drawRocket(x + 65, y + 120);

    // telescope
    setcolor(LIGHTGRAY);
    setlinestyle(SOLID_LINE, 0, 4);
    line(x + 265, y + 110, x + 295, y + 88);

    setlinestyle(SOLID_LINE, 0, 1);

    filledRect(x + 284, y + 79, x + 306, y + 91, LIGHTGRAY);

    line(x + 280, y + 105, x + 266, y + 155);
    line(x + 280, y + 105, x + 298, y + 155);
}

// WALKWAY

void drawWalkway()
{
    // road/path
    int path[] =
    {
        0, 535,
        W, 535,
        W, 700,
        0, 700
    };

    setfillstyle(SOLID_FILL,
        nightMode ? COLOR(70, 70, 75) : COLOR(190, 185, 170));

    setcolor(nightMode ? COLOR(70,70,75) : COLOR(190,185,170));
    fillpoly(4, path);

    // paving lines
    setcolor(nightMode ? COLOR(90,90,95) : COLOR(160,155,145));

    for (int y = 565; y < 700; y += 35)
        line(0, y, W, y);

    for (int x = 0; x <= W; x += 100)
        line(x, 535, x + 25, 700);

    // border
    setcolor(WHITE);
    line(0, 535, W, 535);
}

// FLOWERS

void drawFlower(int x, int y, int color)
{
    setcolor(GREEN);
    line(x, y, x, y + 18);

    filledCircle(x - 4, y, 4, color);
    filledCircle(x + 4, y, 4, color);
    filledCircle(x, y - 4, 4, color);
    filledCircle(x, y + 4, 4, color);
    filledCircle(x, y, 3, YELLOW);
}

// LAMP

void drawLamp(int x, int y)
{
    setcolor(DARKGRAY);
    setlinestyle(SOLID_LINE, 0, 4);

    line(x, y, x, y - 85);

    setlinestyle(SOLID_LINE, 0, 1);

    setcolor(DARKGRAY);
    arc(x, y - 84, 0, 180, 14);

    int lampColor = nightMode ? YELLOW : COLOR(220, 220, 190);

    filledCircle(x, y - 83, 7, lampColor);
}

// PERSON / STUDENT

void drawPerson(int x, int y, int shirtColor, bool backpack)
{
    // head
    filledCircle(x, y - 45, 10, COLOR(235, 185, 140));

    // hair
    setcolor(BLACK);
    arc(x, y - 46, 10, 170, 9);

    // body/shirt
    filledRect(x - 9, y - 34, x + 9, y - 5, shirtColor);

    // arms
    setcolor(COLOR(235, 185, 140));
    setlinestyle(SOLID_LINE, 0, 3);

    line(x - 8, y - 30, x - 17, y - 10);
    line(x + 8, y - 30, x + 17, y - 12);

    // legs
    setcolor(COLOR(40, 45, 60));
    line(x - 5, y - 5, x - 9, y + 20);
    line(x + 5, y - 5, x + 10, y + 20);

    setlinestyle(SOLID_LINE, 0, 1);

    // shoes
    setcolor(BLACK);
    line(x - 9, y + 20, x - 15, y + 20);
    line(x + 10, y + 20, x + 16, y + 20);

    if (backpack)
    {
        filledRect(x + 9, y - 30, x + 16, y - 12, COLOR(80, 60, 140));
    }
}

// INFO BOARD

void drawInfoBoard()
{
    int x = 470;
    int y = 650;

    filledRect(x, y, x + 260, y + 38, COLOR(25, 40, 60));

    setcolor(WHITE);
    rectangle(x, y, x + 260, y + 38);

    setbkcolor(COLOR(25,40,60));
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

    if (selectedStall == 0)
    {
        setcolor(WHITE);
        outtextxy(x + 19, y + 8,
            (char*)"1: Energy  2: Robotics  3: Space");
    }
    else if (selectedStall == 1)
    {
        setcolor(LIGHTGREEN);
        outtextxy(x + 28, y + 8,
            (char*)"CLEAN ENERGY - WIND & SOLAR");
    }
    else if (selectedStall == 2)
    {
        setcolor(LIGHTCYAN);
        outtextxy(x + 33, y + 8,
            (char*)"ROBOTICS - SMART AUTOMATION");
    }
    else
    {
        setcolor(YELLOW);
        outtextxy(x + 25, y + 8,
            (char*)"SPACE - PLANETS & EXPLORATION");
    }

    setcolor(LIGHTGRAY);
    outtextxy(x + 67, y + 22,
        (char*)"D: Day/Night   ESC: Exit");
}

// UPDATE ANIMATION

void updateAnimation()
{
    turbineAngle += 0.08;
    orbitAngle += 0.035;
    robotAngle += 0.045;

    cloudX1 += 1;

    if (cloudX1 > W + 100)
        cloudX1 = -150;

    cloudX2 += 1;

    if (cloudX2 > W + 100)
        cloudX2 = -170;

    birdX += 2;

    if (birdX > W + 50)
        birdX = -100;

    visitorX1 += 2;

    if (visitorX1 > W + 30)
        visitorX1 = -30;

    visitorX2 -= 1;

    if (visitorX2 < -30)
        visitorX2 = W + 30;
}

// KEYBOARD

void handleKeyboard()
{
    if (kbhit())
    {
        int key = getch();

        if (key == 27)
        {
            running = false;
        }
        else if (key == 'd' || key == 'D')
        {
            nightMode = !nightMode;
        }
        else if (key == '1')
        {
            selectedStall = 1;
        }
        else if (key == '2')
        {
            selectedStall = 2;
        }
        else if (key == '3')
        {
            selectedStall = 3;
        }
        else if (key == '0')
        {
            selectedStall = 0;
        }
    }
}

// COMPLETE SCENE

void drawScene()
{
    drawSky();
    drawSunMoon();

    drawCloud(cloudX1, 70);
    drawCloud(cloudX2, 135);

    drawBird(birdX, 100);
    drawBird(birdX + 90, 145);
    drawBird(birdX + 180, 75);

    drawGround();

    // distant trees
    drawTree(35, 285, 0.65);
    drawTree(1160, 285, 0.7);

    drawFairBanner();
    drawFlags();

    // stalls
    drawEnergyStall();
    drawRoboticsStall();
    drawSpaceStall();

    // grass decoration
    drawFlower(25, 515, RED);
    drawFlower(405, 515, MAGENTA);
    drawFlower(790, 515, YELLOW);
    drawFlower(1170, 515, LIGHTRED);

    drawWalkway();

    // lamps
    drawLamp(35, 620);
    drawLamp(1160, 620);

    // visitors
    drawPerson(visitorX1, 600, RED, true);
    drawPerson(visitorX2, 625, COLOR(40,120,200), false);

    // stationary visitors
    drawPerson(320, 610, COLOR(240,150,40), true);
    drawPerson(785, 600, COLOR(180,70,170), false);
    drawPerson(1030, 625, COLOR(60,150,90), true);

    drawInfoBoard();
}

// MAIN

int main()
{
    initwindow(W, H, "Interactive Science & Technology Fair");

    int page = 0;

    while (running)
    {
        setactivepage(page);
        setvisualpage(1 - page);

        cleardevice();

        handleKeyboard();
        drawScene();
        updateAnimation();

        setvisualpage(page);

        delay(25);

        page = 1 - page;
    }

    closegraph();

    return 0;
}