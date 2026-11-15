#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <stdio.h>
#include <math.h>

// ============================================================
// VILLAGE PARADISE
// Animated Village View using WinBGIm
// ============================================================

const int WIDTH  = 1200;
const int HEIGHT = 700;

bool nightMode = false;
bool rainOn = false;

int cloudX1 = 80;
int cloudX2 = 650;
int birdX = 100;
int boatX = 100;

int animationFrame = 0;
int animationSpeed = 2;
int currentPage = 0;


// ============================================================
// HELPERS
// ============================================================

int rgb(int r, int g, int b)
{
    return COLOR(r, g, b);
}

void filledCircle(int x, int y, int radius, int color)
{
    setcolor(color);
    setfillstyle(SOLID_FILL, color);
    fillellipse(x, y, radius, radius);
}

void filledEllipse(int x, int y, int rx, int ry, int color)
{
    setcolor(color);
    setfillstyle(SOLID_FILL, color);
    fillellipse(x, y, rx, ry);
}

void polygon(int points, int shape[], int color)
{
    setcolor(color);
    setfillstyle(SOLID_FILL, color);
    fillpoly(points, shape);
}


// ============================================================
// SKY
// ============================================================

void drawSky()
{
    int topR, topG, topB;
    int bottomR, bottomG, bottomB;

    if (nightMode)
    {
        topR = 8;
        topG = 16;
        topB = 45;

        bottomR = 40;
        bottomG = 65;
        bottomB = 100;
    }
    else
    {
        topR = 75;
        topG = 165;
        topB = 225;

        bottomR = 205;
        bottomG = 235;
        bottomB = 245;
    }

    for (int y = 0; y < 430; y += 5)
    {
        int r = topR + (bottomR - topR) * y / 430;
        int g = topG + (bottomG - topG) * y / 430;
        int b = topB + (bottomB - topB) * y / 430;

        setfillstyle(SOLID_FILL, rgb(r, g, b));
        bar(0, y, WIDTH, y + 5);
    }
}


// ============================================================
// SUN, MOON AND STARS
// ============================================================

void drawSunMoon()
{
    if (!nightMode)
    {
        filledCircle(1030, 100, 57, rgb(255, 215, 90));
        filledCircle(1030, 100, 44, rgb(255, 235, 120));
        filledCircle(1030, 100, 32, rgb(255, 245, 180));
    }
    else
    {
        int stars[][2] =
        {
            {60, 45},   {145, 100}, {230, 55},  {310, 115},
            {395, 40},  {480, 90},  {560, 50},  {640, 125},
            {720, 55},  {800, 105}, {890, 45},  {1100, 60},
            {1150, 130},{520, 155}, {275, 160}, {920, 145},
            {130, 175}, {700, 170}, {830, 165}, {420, 150}
        };

        for (int i = 0; i < 20; i++)
        {
            int radius =
                ((animationFrame / 8 + i) % 5 == 0) ? 2 : 1;

            filledCircle(stars[i][0], stars[i][1],
                         radius, rgb(245, 245, 220));
        }

        filledCircle(1030, 100, 42, rgb(245, 245, 215));
        filledCircle(1046, 87, 38, rgb(17, 30, 58));
    }
}


// ============================================================
// CLOUDS
// ============================================================

void drawCloud(int x, int y, int scale)
{
    int color = nightMode ? rgb(100, 115, 135)
                          : rgb(245, 250, 250);

    int shadow = nightMode ? rgb(70, 82, 105)
                           : rgb(205, 225, 235);

    filledEllipse(x + 45 * scale / 100, y + 12,
                  55 * scale / 100,
                  16 * scale / 100, shadow);

    filledEllipse(x, y,
                  32 * scale / 100,
                  18 * scale / 100, color);

    filledEllipse(x + 32 * scale / 100,
                  y - 12 * scale / 100,
                  35 * scale / 100,
                  27 * scale / 100, color);

    filledEllipse(x + 67 * scale / 100, y,
                  38 * scale / 100,
                  21 * scale / 100, color);

    filledEllipse(x + 98 * scale / 100, y + 3,
                  28 * scale / 100,
                  16 * scale / 100, color);
}

void drawClouds()
{
    drawCloud(cloudX1, 95, 90);
    drawCloud(cloudX2, 145, 110);
    drawCloud((cloudX1 + 450) % 1400 - 100, 60, 65);
}


// ============================================================
// BIRDS
// ============================================================

void drawBird(int x, int y, int size, int phase)
{
    int color = nightMode ? rgb(190, 195, 200)
                          : rgb(45, 45, 40);

    setcolor(color);
    setlinestyle(SOLID_LINE, 0, 2);

    int flap = ((animationFrame / 5) + phase) % 2;

    if (flap)
    {
        line(x - size, y - size / 2, x, y);
        line(x, y, x + size, y - size / 2);
    }
    else
    {
        line(x - size, y + size / 3, x, y);
        line(x, y, x + size, y + size / 3);
    }

    setlinestyle(SOLID_LINE, 0, 1);
}

void drawBirds()
{
    drawBird(birdX,      125, 12, 0);
    drawBird(birdX + 45, 105,  9, 1);
    drawBird(birdX + 80, 140,  8, 0);
}


// ============================================================
// DISTANT HILLS
// ============================================================

void drawHills()
{
    int back = nightMode ? rgb(48, 75, 75)
                         : rgb(105, 155, 135);

    int front = nightMode ? rgb(35, 80, 58)
                          : rgb(75, 145, 85);

    int hill1[] =
    {
        0, 365, 120, 245, 245, 360,
        380, 220, 530, 365, 680, 250,
        830, 370, 1000, 235, 1200, 365,
        1200, 430, 0, 430
    };

    polygon(11, hill1, back);

    int hill2[] =
    {
        0, 390, 160, 315, 300, 390,
        470, 300, 625, 390, 800, 310,
        970, 385, 1100, 305, 1200, 380,
        1200, 445, 0, 445
    };

    polygon(11, hill2, front);

    // Distant tree line
    setcolor(nightMode ? rgb(25, 65, 45)
                       : rgb(45, 115, 55));

    for (int x = 20; x < WIDTH; x += 35)
    {
        int baseY = 410;
        int h = 15 + (x % 12);

        line(x, baseY, x, baseY - h);
        line(x, baseY - h + 4, x - 5, baseY - h + 12);
        line(x, baseY - h + 4, x + 5, baseY - h + 12);
    }
}


// ============================================================
// LAND AND FIELD
// ============================================================

void drawLand()
{
    int land = nightMode ? rgb(35, 83, 45)
                         : rgb(80, 165, 65);

    setfillstyle(SOLID_FILL, land);
    bar(0, 400, WIDTH, HEIGHT);

    // Farmland
    int field = nightMode ? rgb(65, 95, 42)
                          : rgb(155, 185, 65);

    int fieldShape[] =
    {
        0, 440,
        350, 430,
        450, 500,
        0, 545
    };

    polygon(4, fieldShape, field);

    setcolor(nightMode ? rgb(85, 110, 50)
                       : rgb(185, 205, 80));

    for (int i = 0; i < 8; i++)
    {
        line(0, 455 + i * 11,
             360 + i * 10, 440 + i * 7);
    }

    // Village path
    int path[] =
    {
        600, 435,
        690, 435,
        890, 700,
        555, 700,
        640, 515
    };

    polygon(5, path,
            nightMode ? rgb(95, 85, 65)
                      : rgb(205, 175, 120));

    // Path texture
    setcolor(nightMode ? rgb(115, 100, 75)
                       : rgb(185, 155, 105));

    for (int i = 0; i < 20; i++)
    {
        int x = 640 + (i * 37) % 150;
        int y = 490 + (i * 47) % 190;

        line(x, y, x + 6 + i % 5, y);
    }
}


// ============================================================
// RIVER
// ============================================================

void drawRiver()
{
    int water = nightMode ? rgb(25, 80, 105)
                          : rgb(45, 150, 190);

    int river[] =
    {
        0, 565,
        160, 550,
        300, 565,
        420, 600,
        520, 700,
        0, 700
    };

    polygon(6, river, water);

    // Moving waves
    setcolor(nightMode ? rgb(90, 155, 175)
                       : rgb(160, 225, 235));

    for (int row = 0; row < 4; row++)
    {
        int y = 585 + row * 28;
        int offset = (animationFrame * (row % 2 + 1)) % 70;

        for (int x = -50 + offset; x < 420; x += 90)
        {
            int maxX = 280 + (y - 565) * 2;

            if (x + 25 < maxX)
                arc(x, y, 20, 160, 24);
        }
    }

    // Water shimmer
    for (int i = 0; i < 15; i++)
    {
        int x = (i * 57 + animationFrame) % 370;
        int y = 590 + (i * 23) % 100;

        setcolor(nightMode ? rgb(85, 155, 180)
                           : rgb(135, 210, 225));

        line(x, y, x + 18, y);
    }
}

void drawRiverBank()
{
    int bankColor = nightMode ? rgb(75, 75, 48)
                              : rgb(165, 145, 85);

    setcolor(bankColor);
    setlinestyle(SOLID_LINE, 0, 3);

    line(0, 565, 160, 550);
    line(160, 550, 300, 565);
    line(300, 565, 420, 600);
    line(420, 600, 520, 700);

    setlinestyle(SOLID_LINE, 0, 1);

    // Small stones beside the river
    for (int i = 0; i < 14; i++)
    {
        int x = 20 + i * 28;
        int y;

        if (x < 160)
            y = 565 - x * 15 / 160;
        else if (x < 300)
            y = 550 + (x - 160) * 15 / 140;
        else
            y = 565 + (x - 300) * 35 / 120;

        filledEllipse(x, y - 3, 5 + i % 4, 3,
                      nightMode ? rgb(75, 80, 70)
                                : rgb(135, 135, 110));
    }
}


// ============================================================
// TREES
// ============================================================

void drawTree(int x, int groundY, int scale)
{
    int trunk = nightMode ? rgb(65, 45, 32)
                          : rgb(110, 70, 38);

    int darkGreen = nightMode ? rgb(20, 65, 38)
                              : rgb(28, 105, 45);

    int green = nightMode ? rgb(30, 85, 45)
                          : rgb(45, 145, 55);

    int lightGreen = nightMode ? rgb(40, 100, 50)
                               : rgb(80, 175, 65);

    int h = 125 * scale / 100;
    int w = 13 * scale / 100;

    // Shadow under tree
    filledEllipse(x + 12 * scale / 100, groundY + 2,
                  45 * scale / 100,
                  8 * scale / 100,
                  nightMode ? rgb(25, 65, 35)
                            : rgb(65, 130, 55));

    int trunkShape[] =
    {
        x - w,     groundY,
        x - w / 2, groundY - h,
        x + w / 2, groundY - h,
        x + w,     groundY
    };

    polygon(4, trunkShape, trunk);

    // Trunk texture
    setcolor(nightMode ? rgb(85, 58, 38)
                       : rgb(145, 95, 50));

    line(x - 3, groundY - 15,
         x - 3, groundY - h + 30);

    line(x + 4, groundY - 10,
         x + 4, groundY - h + 45);

    // Branches
    setcolor(trunk);
    setlinestyle(SOLID_LINE, 0, 3);

    line(x, groundY - h + 35 * scale / 100,
         x - 30 * scale / 100, groundY - h);

    line(x, groundY - h + 45 * scale / 100,
         x + 30 * scale / 100,
         groundY - h + 5 * scale / 100);

    setlinestyle(SOLID_LINE, 0, 1);

    // Leaves
    filledCircle(x - 30 * scale / 100, groundY - h,
                 30 * scale / 100, darkGreen);

    filledCircle(x + 30 * scale / 100, groundY - h,
                 30 * scale / 100, green);

    filledCircle(x, groundY - h - 20 * scale / 100,
                 34 * scale / 100, lightGreen);

    filledCircle(x - 20 * scale / 100,
                 groundY - h - 25 * scale / 100,
                 25 * scale / 100, green);

    filledCircle(x + 20 * scale / 100,
                 groundY - h - 25 * scale / 100,
                 25 * scale / 100, darkGreen);

    // Leaf highlights
    for (int i = 0; i < 10; i++)
    {
        int lx = x + ((i * 23) % 65 - 32) * scale / 100;
        int ly = groundY - h - ((i * 17) % 40) * scale / 100;

        filledCircle(lx, ly, 3 * scale / 100 + 1, lightGreen);
    }
}


// ============================================================
// HOUSE YARD
// ============================================================

void drawYard(int x, int y, int width)
{
    int yardColor = nightMode ? rgb(105, 95, 65)
                              : rgb(205, 180, 125);

    filledEllipse(x, y, width, 20, yardColor);

    setcolor(nightMode ? rgb(85, 78, 55)
                       : rgb(175, 150, 105));

    for (int i = 0; i < 10; i++)
    {
        int dx = (i * 31) % (width * 2 - 20) - width + 10;
        int dy = (i * 7) % 15 - 5;

        line(x + dx, y + dy, x + dx + 5, y + dy);
    }
}


// ============================================================
// VILLAGE HOUSE
// ============================================================

void drawHouse(int x, int y, int scale)
{
    int w = 170 * scale / 100;
    int h = 110 * scale / 100;
    int roofH = 65 * scale / 100;

    int wall = nightMode ? rgb(125, 105, 75)
                         : rgb(225, 195, 145);

    int wallShadow = nightMode ? rgb(90, 78, 60)
                               : rgb(195, 160, 115);

    int roof = nightMode ? rgb(90, 48, 42)
                         : rgb(155, 65, 48);

    int roofLight = nightMode ? rgb(125, 70, 60)
                              : rgb(195, 90, 65);

    int wood = nightMode ? rgb(65, 42, 30)
                         : rgb(115, 70, 40);

    // House shadow
    filledEllipse(x + w / 2, y + 4,
                  w / 2 + 15, 10,
                  nightMode ? rgb(80, 75, 55)
                            : rgb(175, 150, 100));

    // Main wall
    setcolor(wall);
    setfillstyle(SOLID_FILL, wall);
    bar(x, y - h, x + w, y);

    // Side wall shadow
    int side[] =
    {
        x + w - 25 * scale / 100, y - h,
        x + w, y - h,
        x + w, y,
        x + w - 25 * scale / 100, y
    };

    polygon(4, side, wallShadow);

    // Wall texture
    setcolor(wallShadow);

    for (int i = 0; i < 12; i++)
    {
        int tx = x + 8 + (i * 37) % (w - 16);
        int ty = y - h + 8 + (i * 29) % (h - 16);

        line(tx, ty, tx + 6, ty);
    }

    // Roof
    int roofShape[] =
    {
        x - 15 * scale / 100, y - h,
        x + w / 2, y - h - roofH,
        x + w + 15 * scale / 100, y - h
    };

    polygon(3, roofShape, roof);

    // Roof texture
    setcolor(roofLight);

    for (int i = 1; i <= 5; i++)
    {
        int leftX = x - 15 * scale / 100
                    + (w / 2 + 15 * scale / 100) * i / 6;

        int rightX = x + w + 15 * scale / 100
                     - (w / 2 + 15 * scale / 100) * i / 6;

        int lineY = y - h - roofH * i / 6;

        line(leftX, lineY, rightX, lineY);
    }

    // Roof border
    setcolor(wood);
    setlinestyle(SOLID_LINE, 0, 2);

    line(x - 15 * scale / 100, y - h,
         x + w / 2, y - h - roofH);

    line(x + w / 2, y - h - roofH,
         x + w + 15 * scale / 100, y - h);

    setlinestyle(SOLID_LINE, 0, 1);

    // Door
    int doorW = 32 * scale / 100;
    int doorH = 65 * scale / 100;
    int doorX = x + w / 2 - doorW / 2;

    setfillstyle(SOLID_FILL, wood);
    bar(doorX, y - doorH, doorX + doorW, y);

    setcolor(rgb(155, 105, 65));

    line(doorX + doorW / 3, y - doorH + 4,
         doorX + doorW / 3, y - 3);

    line(doorX + 2 * doorW / 3, y - doorH + 4,
         doorX + 2 * doorW / 3, y - 3);

    filledCircle(doorX + doorW - 6,
                 y - doorH / 2, 2, rgb(245, 210, 100));

    // Windows
    int windowColor = nightMode ? rgb(250, 210, 100)
                                : rgb(105, 180, 200);

    int size = 28 * scale / 100;
    int windowY = y - h + 35 * scale / 100;

    int leftX = x + 20 * scale / 100;
    int rightX = x + w - 20 * scale / 100 - size;

    setfillstyle(SOLID_FILL, windowColor);
    bar(leftX, windowY, leftX + size, windowY + size);
    bar(rightX, windowY, rightX + size, windowY + size);

    setcolor(wood);
    setlinestyle(SOLID_LINE, 0, 2);

    rectangle(leftX, windowY,
              leftX + size, windowY + size);

    rectangle(rightX, windowY,
              rightX + size, windowY + size);

    line(leftX + size / 2, windowY,
         leftX + size / 2, windowY + size);

    line(leftX, windowY + size / 2,
         leftX + size, windowY + size / 2);

    line(rightX + size / 2, windowY,
         rightX + size / 2, windowY + size);

    line(rightX, windowY + size / 2,
         rightX + size, windowY + size / 2);

    setlinestyle(SOLID_LINE, 0, 1);

    // Doorstep
    setfillstyle(SOLID_FILL,
                 nightMode ? rgb(95, 85, 70)
                           : rgb(175, 155, 125));

    bar(doorX - 5, y, doorX + doorW + 5, y + 5);
}


// ============================================================
// WINDMILL
// ============================================================

void drawWindmill(int x, int groundY)
{
    int tower = nightMode ? rgb(155, 145, 120)
                          : rgb(235, 225, 195);

    int towerShape[] =
    {
        x - 28, groundY,
        x - 15, groundY - 135,
        x + 15, groundY - 135,
        x + 28, groundY
    };

    polygon(4, towerShape, tower);

    int roofShape[] =
    {
        x - 23, groundY - 135,
        x,      groundY - 160,
        x + 23, groundY - 135
    };

    polygon(3, roofShape,
            nightMode ? rgb(100, 55, 40)
                      : rgb(155, 75, 45));

    // Small door
    setfillstyle(SOLID_FILL, rgb(90, 65, 45));
    bar(x - 8, groundY - 30, x + 8, groundY);

    // Rotating blades
    int cx = x;
    int cy = groundY - 125;
    int angle = (animationFrame * 3) % 360;

    setcolor(nightMode ? rgb(205, 205, 190)
                       : rgb(250, 245, 220));

    setlinestyle(SOLID_LINE, 0, 4);

    for (int i = 0; i < 4; i++)
    {
        int a = angle + i * 90;
        double radians = a * 3.14159265 / 180.0;

        int endX = cx + (int)(55 * cos(radians));
        int endY = cy - (int)(55 * sin(radians));

        line(cx, cy, endX, endY);
    }

    setlinestyle(SOLID_LINE, 0, 1);
    filledCircle(cx, cy, 6, rgb(100, 70, 45));
}


// ============================================================
// FENCE
// ============================================================

void drawFence(int x, int y, int length)
{
    int color = nightMode ? rgb(110, 95, 70)
                          : rgb(190, 155, 105);

    setcolor(color);
    setlinestyle(SOLID_LINE, 0, 3);

    line(x, y - 25, x + length, y - 25);
    line(x, y - 10, x + length, y - 10);

    for (int i = 0; i <= length; i += 25)
    {
        line(x + i, y, x + i, y - 35);
    }

    setlinestyle(SOLID_LINE, 0, 1);
}


// ============================================================
// BOAT
// ============================================================

void drawBoat()
{
    int x = boatX;
    int y = 615;

    // Reflection
    setcolor(nightMode ? rgb(65, 125, 145)
                       : rgb(105, 190, 210));

    line(x - 25, y + 27, x + 25, y + 27);
    line(x - 15, y + 33, x + 15, y + 33);

    // Boat body
    int boat[] =
    {
        x - 45, y,
        x + 45, y,
        x + 30, y + 20,
        x - 28, y + 20
    };

    polygon(4, boat, rgb(110, 60, 35));

    setcolor(rgb(160, 100, 55));
    line(x - 35, y + 7, x + 35, y + 7);

    // Mast
    setcolor(rgb(80, 45, 25));
    line(x, y, x, y - 55);

    // Sail
    int sail[] =
    {
        x + 3, y - 52,
        x + 3, y - 5,
        x + 35, y - 5
    };

    polygon(3, sail,
            nightMode ? rgb(180, 185, 170)
                      : rgb(250, 240, 205));
}


// ============================================================
// FLOWERS AND GRASS
// ============================================================

void drawFlower(int x, int y, int color)
{
    setcolor(rgb(35, 115, 45));
    line(x, y, x, y - 16);

    filledCircle(x - 4, y - 18, 4, color);
    filledCircle(x + 4, y - 18, 4, color);
    filledCircle(x, y - 22, 4, color);
    filledCircle(x, y - 15, 4, color);
    filledCircle(x, y - 18, 3, rgb(255, 220, 60));
}

void drawGrassAndFlowers()
{
    setcolor(nightMode ? rgb(50, 110, 55)
                       : rgb(55, 145, 55));

    for (int i = 0; i < 55; i++)
    {
        int x = 470 + (i * 71) % 710;
        int y = 490 + (i * 37) % 170;

        if (x > 900 || (x < 600 && y < 560))
        {
            line(x, y, x - 4, y - 10);
            line(x, y, x + 2, y - 13);
            line(x, y, x + 6, y - 9);
        }
    }

    drawFlower(490, 520, rgb(245, 90, 120));
    drawFlower(530, 545, rgb(255, 210, 55));
    drawFlower(950, 535, rgb(210, 100, 225));
    drawFlower(1030, 555, rgb(250, 100, 110));
    drawFlower(1120, 570, rgb(255, 215, 60));
}


// ============================================================
// RAIN
// ============================================================

void drawRain()
{
    setcolor(nightMode ? rgb(115, 155, 180)
                       : rgb(180, 220, 235));

    for (int i = 0; i < 160; i++)
    {
        int x = (i * 79 + animationFrame * 9) % (WIDTH + 50);
        int y = (i * 53 + animationFrame * 14) % HEIGHT;

        line(x, y, x - 6, y + 14);
    }
}


// ============================================================
// CONTROLS
// ============================================================

void drawControls()
{
    int background = rgb(25, 35, 40);

    setfillstyle(SOLID_FILL, background);
    bar(15, 650, 730, 688);

    setbkcolor(background);
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

    char text[200];

    sprintf(text,
            "D: Day/Night   R: Rain %s   +/-: Speed [%d]   ESC: Exit",
            rainOn ? "ON" : "OFF", animationSpeed);

    outtextxy(28, 664, text);
}


// ============================================================
// COMPLETE SCENE
// ============================================================

void drawScene()
{
    drawSky();
    drawSunMoon();
    drawClouds();
    drawBirds();

    drawHills();
    drawLand();
    drawRiver();
    drawRiverBank();

    // Background trees
    drawTree(55, 460, 65);
    drawTree(1120, 465, 80);
    drawTree(930, 440, 55);

    // House yards
    drawYard(397, 472, 110);
    drawYard(853, 472, 125);

    // Houses
    drawHouse(320, 470, 90);
    drawHouse(760, 470, 110);

    drawWindmill(635, 440);

    // Fences
    drawFence(200, 495, 130);
    drawFence(965, 500, 150);

    // Foreground trees
    drawTree(80, 550, 100);
    drawTree(1170, 555, 105);

    drawBoat();
    drawGrassAndFlowers();

    if (rainOn)
        drawRain();

    drawControls();
}


// ============================================================
// ANIMATION UPDATE
// ============================================================

void updateAnimation()
{
    animationFrame++;

    cloudX1 += animationSpeed;

    if (cloudX1 > WIDTH + 150)
        cloudX1 = -180;

    if (animationFrame % 2 == 0)
        cloudX2 += animationSpeed;

    if (cloudX2 > WIDTH + 150)
        cloudX2 = -200;

    birdX += animationSpeed + 1;

    if (birdX > WIDTH + 100)
        birdX = -150;

    boatX += animationSpeed;

    if (boatX > 350)
        boatX = -60;

    // Prevent integer overflow during long runs
    if (animationFrame >= 100000)
        animationFrame = 0;
}


// ============================================================
// KEYBOARD
// ============================================================

bool handleKeyboard()
{
    if (kbhit())
    {
        int key = getch();

        if (key == 27)
            return false;

        if (key == 'd' || key == 'D')
            nightMode = !nightMode;

        if (key == 'r' || key == 'R')
            rainOn = !rainOn;

        if ((key == '+' || key == '=') && animationSpeed < 6)
            animationSpeed++;

        if ((key == '-' || key == '_') && animationSpeed > 1)
            animationSpeed--;
    }

    return true;
}


// ============================================================
// MAIN
// ============================================================

int main()
{
    initwindow(WIDTH, HEIGHT, "Village Paradise");

    bool running = true;

    while (running)
    {
        currentPage = 1 - currentPage;

        setactivepage(currentPage);
        cleardevice();

        drawScene();

        setvisualpage(currentPage);

        running = handleKeyboard();
        updateAnimation();

        delay(25);
    }

    closegraph();
    return 0;
}