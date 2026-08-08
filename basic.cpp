#include <graphics.h>
#include <conio.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>

using namespace std;

const int W = 1200;
const int H = 700;
const double PI = 3.14159265358979323846;

float eco = 0.0f;             // 0 = polluted, 1 = green
bool transforming = false;
float turbineAngle = 0;
float cloudMove = 0;
float birdMove = 0;
float waterMove = 0;

int frameCount = 0;

// PARTICLES

struct SmokeParticle
{
    float x, y;
    float vx, vy;
    float size;
    bool active;
};

const int MAX_SMOKE = 120;
SmokeParticle smoke[MAX_SMOKE];

struct Drop
{
    float x, y;
    float vy;
    bool active;
};

const int MAX_DROPS = 40;
Drop drops[MAX_DROPS];

// COLOR FUNCTIONS

int clamp255(int n)
{
    if (n < 0) return 0;
    if (n > 255) return 255;
    return n;
}

int blendColor(
    int r1, int g1, int b1,
    int r2, int g2, int b2,
    float t)
{
    int r = (int)(r1 + (r2 - r1) * t);
    int g = (int)(g1 + (g2 - g1) * t);
    int b = (int)(b1 + (b2 - b1) * t);

    return COLOR(clamp255(r), clamp255(g), clamp255(b));
}

void box(int x1, int y1, int x2, int y2, int color)
{
    setcolor(color);
    setfillstyle(SOLID_FILL, color);
    bar(x1, y1, x2, y2);
}

void fcircle(int x, int y, int r, int color)
{
    setcolor(color);
    setfillstyle(SOLID_FILL, color);
    fillellipse(x, y, r, r);
}

// GRADIENT SKY

void drawSky(float p)
{
    // Vertical gradient
    for (int y = 0; y < 470; y += 4)
    {
        float k = (float)y / 470.0f;

        int pollutedTopR = 48;
        int pollutedTopG = 54;
        int pollutedTopB = 62;

        int pollutedBottomR = 110;
        int pollutedBottomG = 108;
        int pollutedBottomB = 100;

        int greenTopR = 45;
        int greenTopG = 150;
        int greenTopB = 235;

        int greenBottomR = 165;
        int greenBottomG = 220;
        int greenBottomB = 250;

        int r1 = (int)(pollutedTopR +
                      (pollutedBottomR - pollutedTopR) * k);

        int g1 = (int)(pollutedTopG +
                      (pollutedBottomG - pollutedTopG) * k);

        int b1 = (int)(pollutedTopB +
                      (pollutedBottomB - pollutedTopB) * k);

        int r2 = (int)(greenTopR +
                      (greenBottomR - greenTopR) * k);

        int g2 = (int)(greenTopG +
                      (greenBottomG - greenTopG) * k);

        int b2 = (int)(greenTopB +
                      (greenBottomB - greenTopB) * k);

        int c = blendColor(r1, g1, b1, r2, g2, b2, p);

        setcolor(c);

        for (int j = 0; j < 4; j++)
            line(0, y + j, W, y + j);
    }
}

// SUN

void drawSun(float p)
{
    int x = 1015;
    int y = 125;

    int darkSun = COLOR(165, 120, 65);
    int cleanSun = COLOR(255, 215, 45);

    int c;

    if (p < 0.4f)
        c = darkSun;
    else
        c = cleanSun;

    int radius = 27 + (int)(8 * p);

    if (p > 0.25f)
    {
        // glow
        if (p > 0.65f)
        {
            fcircle(x, y, radius + 13, COLOR(255, 226, 100));
        }

        fcircle(x, y, radius, c);

        if (p > 0.7f)
        {
            setcolor(COLOR(255, 225, 70));
            setlinestyle(SOLID_LINE, 0, 2);

            for (int i = 0; i < 12; i++)
            {
                double a = i * 2 * PI / 12.0;

                int x1 = x + (int)(48 * cos(a));
                int y1 = y + (int)(48 * sin(a));

                int x2 = x + (int)(61 * cos(a));
                int y2 = y + (int)(61 * sin(a));

                line(x1, y1, x2, y2);
            }

            setlinestyle(SOLID_LINE, 0, 1);
        }
    }
}

// CLOUD

void cloud(int x, int y, int color)
{
    fcircle(x, y, 24, color);
    fcircle(x + 25, y - 12, 31, color);
    fcircle(x + 55, y - 5, 28, color);
    fcircle(x + 82, y + 4, 21, color);

    box(x, y, x + 80, y + 20, color);
}

void drawClouds(float p)
{
    int c = blendColor(
        65, 67, 70,
        245, 248, 250,
        p
    );

    int x1 = (int)(150 + cloudMove);
    int x2 = (int)(610 + cloudMove * 0.55f);

    x1 %= 1400;
    x2 %= 1400;

    x1 -= 120;
    x2 -= 120;

    cloud(x1, 120, c);
    cloud(x2, 175, c);
}

// DISTANT HILLS
void drawHills(float p)
{
    int c1 = blendColor(
        75, 75, 68,
        70, 145, 75,
        p
    );

    int c2 = blendColor(
        85, 82, 70,
        55, 125, 65,
        p
    );

    setcolor(c1);
    setfillstyle(SOLID_FILL, c1);

    int hill1[] =
    {
        0, 470,
        0, 390,
        110, 300,
        220, 410,
        350, 315,
        490, 470
    };

    fillpoly(6, hill1);

    setcolor(c2);
    setfillstyle(SOLID_FILL, c2);

    int hill2[] =
    {
        400, 470,
        530, 340,
        650, 415,
        770, 320,
        910, 420,
        1050, 330,
        1200, 420,
        1200, 470
    };

    fillpoly(8, hill2);
}

// GROUND

void drawGround(float p)
{
    int c = blendColor(
        94, 75, 50,
        47, 155, 66,
        p
    );

    box(0, 450, W, H, c);

    // Grass top line
    if (p > 0.5f)
    {
        setcolor(COLOR(30, 125, 45));

        for (int x = 0; x < W; x += 12)
        {
            line(x, 470, x + 4, 458);
            line(x + 4, 470, x + 9, 456);
        }
    }
}


// RIVER

void drawRiver(float p)
{
    int water = blendColor(
        76, 91, 43,
        35, 145, 220,
        p
    );

    setcolor(water);
    setfillstyle(SOLID_FILL, water);

    int river[] =
    {
        0, 565,
        200, 545,
        430, 558,
        650, 535,
        850, 555,
        1030, 535,
        1200, 550,
        1200, 700,
        0, 700
    };

    fillpoly(9, river);

    // Moving water highlights
    int shine = blendColor(
        105, 110, 65,
        170, 225, 250,
        p
    );

    setcolor(shine);

    for (int y = 590; y < 680; y += 27)
    {
        for (int x = -100; x < W; x += 170)
        {
            int xx = x + ((int)waterMove % 80);
            line(xx, y, xx + 75, y);
        }
    }
}

// FACTORY

void factoryWindow(int x, int y)
{
    box(x, y, x + 30, y + 25, COLOR(215, 145, 35));

    setcolor(COLOR(80, 70, 55));

    line(x + 15, y, x + 15, y + 25);
    line(x, y + 12, x + 30, y + 12);
}

void drawFactory(float p)
{
    if (p >= 0.90f)
        return;

    // shadow
    box(45, 422, 335, 480, COLOR(60, 60, 58));

    // main building
    box(55, 350, 330, 470, COLOR(98, 93, 87));

    // industrial roof
    setcolor(COLOR(80, 77, 73));
    setfillstyle(SOLID_FILL, COLOR(80, 77, 73));

    int roof[] =
    {
        55, 350,
        55, 322,
        105, 290,
        105, 325,
        160, 290,
        160, 325,
        215, 290,
        215, 350
    };

    fillpoly(8, roof);

    // doors
    box(75, 395, 125, 470, COLOR(55, 57, 58));
    box(260, 410, 305, 470, COLOR(55, 57, 58));

    factoryWindow(155, 380);
    factoryWindow(200, 380);

    // chimney 1
    box(95, 175, 137, 350, COLOR(72, 70, 68));
    box(88, 168, 144, 184, COLOR(50, 50, 50));

    // red-white chimney stripe
    box(95, 220, 137, 238, COLOR(145, 65, 55));

    // chimney 2
    box(220, 215, 255, 350, COLOR(75, 73, 70));
    box(213, 207, 262, 222, COLOR(50, 50, 50));
}

// SMOKE SYSTEM

void initParticles()
{
    for (int i = 0; i < MAX_SMOKE; i++)
        smoke[i].active = false;

    for (int i = 0; i < MAX_DROPS; i++)
        drops[i].active = false;
}

void createSmoke()
{
    if (eco > 0.83f)
        return;

    int amount = 1;

    if (eco < 0.30f)
        amount = 3;

    for (int k = 0; k < amount; k++)
    {
        for (int i = 0; i < MAX_SMOKE; i++)
        {
            if (!smoke[i].active)
            {
                if (rand() % 2 == 0)
                {
                    smoke[i].x = 116;
                    smoke[i].y = 165;
                }
                else
                {
                    smoke[i].x = 237;
                    smoke[i].y = 204;
                }

                smoke[i].vx =
                    0.20f + (rand() % 30) / 60.0f;

                smoke[i].vy =
                    -0.40f - (rand() % 30) / 40.0f;

                smoke[i].size =
                    8 + rand() % 12;

                smoke[i].active = true;

                break;
            }
        }
    }
}

void updateSmoke()
{
    for (int i = 0; i < MAX_SMOKE; i++)
    {
        if (!smoke[i].active)
            continue;

        // Wind drift
        smoke[i].x += smoke[i].vx;

        // Hot smoke rises
        smoke[i].y += smoke[i].vy;

        smoke[i].vx += 0.0015f;

        // Expansion
        smoke[i].size += 0.035f;

        if (smoke[i].y < 60 ||
            smoke[i].x > 500 ||
            smoke[i].size > 32)
        {
            smoke[i].active = false;
        }
    }
}

void drawSmoke()
{
    int c = blendColor(
        48, 48, 48,
        150, 150, 150,
        eco
    );

    for (int i = 0; i < MAX_SMOKE; i++)
    {
        if (smoke[i].active)
        {
            fcircle(
                (int)smoke[i].x,
                (int)smoke[i].y,
                (int)smoke[i].size,
                c
            );
        }
    }
}

// WASTE PIPE + GRAVITY DROPS

void drawWastePipe()
{
    if (eco > 0.86f)
        return;

    box(330, 455, 393, 480, COLOR(65, 65, 62));
    box(365, 470, 393, 530, COLOR(65, 65, 62));

 
}

void createDrop()
{
    if (eco > 0.80f)
        return;

    if (frameCount % 5 != 0)
        return;

    for (int i = 0; i < MAX_DROPS; i++)
    {
        if (!drops[i].active)
        {
            drops[i].x = 372 + rand() % 14;
            drops[i].y = 525;
            drops[i].vy = 0.5f;

            drops[i].active = true;
            break;
        }
    }
}

void updateDrops()
{
    const float gravity = 0.08f;

    for (int i = 0; i < MAX_DROPS; i++)
    {
        if (!drops[i].active)
            continue;

        // Gravity implementation
        drops[i].vy += gravity;
        drops[i].y += drops[i].vy;

        if (drops[i].y > 570)
            drops[i].active = false;
    }
}

void drawDrops()
{
    int dirty = COLOR(105, 125, 45);

    for (int i = 0; i < MAX_DROPS; i++)
    {
        if (drops[i].active)
        {
            fcircle(
                (int)drops[i].x,
                (int)drops[i].y,
                4,
                dirty
            );
        }
    }
}

// TREE

void drawTree(int x, int groundY, float p, float scale = 1.0f)
{
    int trunkH = (int)(85 * scale);

    // trunk
    box(
        x - (int)(7 * scale),
        groundY - trunkH,
        x + (int)(7 * scale),
        groundY,
        COLOR(100, 63, 35)
    );

    int branchY = groundY - (int)(65 * scale);

    if (p < 0.32f)
    {
        setcolor(COLOR(95, 67, 43));
        setlinestyle(SOLID_LINE, 0, 3);

        line(
            x, branchY,
            x - (int)(30 * scale),
            branchY - (int)(30 * scale)
        );

        line(
            x, branchY,
            x + (int)(30 * scale),
            branchY - (int)(35 * scale)
        );

        line(
            x,
            branchY - 15,
            x,
            branchY - 50
        );

        setlinestyle(SOLID_LINE, 0, 1);
    }
    else
    {
        int leaf = blendColor(
            95, 115, 45,
            28, 150, 50,
            p
        );

        int r = (int)((20 + 15 * p) * scale);

        fcircle(x, branchY - 37, r, leaf);
        fcircle(x - r + 5, branchY - 15, r, leaf);
        fcircle(x + r - 5, branchY - 15, r, leaf);
        fcircle(x, branchY, r, leaf);

        if (p > 0.75f)
        {
            fcircle(
                x - 10,
                branchY - 42,
                5,
                COLOR(75, 190, 70)
            );
        }
    }
}

void drawForest(float p)
{
    drawTree(465, 525, p, 0.9f);
    drawTree(555, 520, p, 1.0f);
    drawTree(650, 525, p, 0.85f);

    if (p > 0.48f)
    {
        drawTree(735, 525, p, 0.75f);
        drawTree(1120, 525, p, 0.85f);
    }
}

// TREE STUMPS / DEFORESTATION

void drawStump(int x, int y)
{
    box(x - 12, y - 25, x + 12, y,
        COLOR(100, 66, 35));

    fcircle(x, y - 25, 12, COLOR(145, 105, 65));

    setcolor(COLOR(90, 60, 35));
    ellipse(x, y - 25, 0, 360, 7, 4);
}

void drawDeforestation()
{
    if (eco < 0.4f)
    {
        drawStump(740, 520);
        drawStump(790, 525);
        drawStump(840, 518);
    }
}

// WIND TURBINE

void turbine(int x, int groundY, float angle, float scale)
{
    int hubY = groundY - (int)(180 * scale);

    // tower shadow
    setcolor(COLOR(150, 155, 155));
    setlinestyle(SOLID_LINE, 0, 10);

    line(x, groundY, x, hubY);

    // tower highlight
    setcolor(COLOR(230, 235, 235));
    setlinestyle(SOLID_LINE, 0, 5);

    line(x - 2, groundY, x - 2, hubY);

    fcircle(
        x,
        hubY,
        (int)(10 * scale),
        COLOR(245, 245, 240)
    );

    setcolor(COLOR(245, 245, 240));
    setlinestyle(SOLID_LINE, 0, 6);

    for (int i = 0; i < 3; i++)
    {
        double a = angle + i * 2.0 * PI / 3.0;

        int x2 = x +
            (int)(70 * scale * cos(a));

        int y2 = hubY +
            (int)(70 * scale * sin(a));

        line(x, hubY, x2, y2);

        fcircle(
            x2,
            y2,
            3,
            COLOR(245, 245, 240)
        );
    }

    setlinestyle(SOLID_LINE, 0, 1);
}

void drawTurbines(float p)
{
    if (p < 0.48f)
        return;

    turbine(890, 510, turbineAngle, 1.0f);
    turbine(1040, 505, turbineAngle + 0.5f, 0.82f);
}

// SOLAR PANELS

void solarPanel(int x, int y, float scale)
{
    int ww = (int)(120 * scale);
    int hh = (int)(65 * scale);

    int panel[] =
    {
        x, y,
        x + ww, y,
        x + ww - 18, y + hh,
        x - 18, y + hh
    };

    setcolor(COLOR(180, 220, 245));
    setfillstyle(SOLID_FILL, COLOR(20, 70, 135));

    fillpoly(4, panel);

    // grid
    setcolor(COLOR(125, 185, 220));

    line(
        x + ww / 3,
        y,
        x + ww / 3 - 12,
        y + hh
    );

    line(
        x + 2 * ww / 3,
        y,
        x + 2 * ww / 3 - 12,
        y + hh
    );

    line(
        x - 6,
        y + hh / 2,
        x + ww - 8,
        y + hh / 2
    );

    // reflection
    setcolor(COLOR(180, 225, 245));
    line(
        x + 10,
        y + 7,
        x + 40,
        y + 7
    );

    // stand
    setcolor(COLOR(70, 75, 78));
    setlinestyle(SOLID_LINE, 0, 5);

    line(
        x + ww / 2,
        y + hh,
        x + ww / 2,
        y + hh + 30
    );

    line(
        x + ww / 2 - 25,
        y + hh + 30,
        x + ww / 2 + 25,
        y + hh + 30
    );

    setlinestyle(SOLID_LINE, 0, 1);
}

void drawSolarFarm()
{
    if (eco > 0.52f)
    {
        solarPanel(95, 405, 0.9f);
        solarPanel(235, 415, 0.8f);
    }
}

// FLOWERS

void flower(int x, int y, int color)
{
    setcolor(COLOR(30, 125, 40));
    line(x, y, x, y - 17);

    fcircle(x, y - 20, 4, color);
    fcircle(x - 5, y - 20, 3, color);
    fcircle(x + 5, y - 20, 3, color);

    fcircle(x, y - 20, 2, YELLOW);
}

void drawFlowers()
{
    if (eco < 0.75f)
        return;

    flower(420, 535, LIGHTMAGENTA);
    flower(510, 545, YELLOW);
    flower(610, 535, LIGHTRED);
    flower(700, 545, LIGHTMAGENTA);
    flower(810, 535, YELLOW);
    flower(1080, 545, LIGHTRED);
}

// FISH

void fish(int x, int y, int color)
{
    setcolor(color);
    setfillstyle(SOLID_FILL, color);

    fillellipse(x, y, 13, 7);

    int tail[] =
    {
        x - 10, y,
        x - 23, y - 10,
        x - 23, y + 10
    };

    fillpoly(3, tail);

    fcircle(x + 6, y - 2, 1, BLACK);
}

void drawFish()
{
    if (eco < 0.72f)
        return;

    int offset = (frameCount / 3) % 300;

    fish(500 + offset, 610, COLOR(255, 180, 60));
    fish(850 - offset / 2, 650, COLOR(240, 210, 70));
}

// BIRDS

void bird(int x, int y, float scale)
{
    int r = (int)(13 * scale);

    setcolor(COLOR(40, 45, 50));
    setlinestyle(SOLID_LINE, 0, 2);

    arc(x, y, 20, 160, r);
    arc(x + 2 * r, y, 20, 160, r);

    setlinestyle(SOLID_LINE, 0, 1);
}

void drawBirds()
{
    if (eco < 0.70f)
        return;

    int x = ((int)birdMove % 800) + 300;

    bird(x, 125, 0.8f);
    bird(x + 65, 155, 0.65f);
    bird(x + 130, 110, 0.55f);
}

// GREEN ENERGY SIGN

void drawEcoSign()
{
    if (eco < 0.80f)
        return;

    box(925, 420, 1080, 470, COLOR(40, 100, 50));

    setcolor(WHITE);
    rectangle(925, 420, 1080, 470);

    setbkcolor(COLOR(40, 100, 50));
    setcolor(WHITE);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(944, 430, (char*)"CLEAN ENERGY");
    outtextxy(960, 450, (char*)"ZONE");

    setcolor(COLOR(85, 60, 35));
    setlinestyle(SOLID_LINE, 0, 5);

    line(945, 470, 945, 510);
    line(1060, 470, 1060, 510);

    setlinestyle(SOLID_LINE, 0, 1);
}

// HUD

void drawHUD()
{
    // Top translucent-style dark panel
    box(0, 0, W, 72, COLOR(24, 30, 37));

    // Green accent
    box(0, 68, W, 72, COLOR(45, 190, 80));

    setbkcolor(COLOR(24, 30, 37));

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(WHITE);

    outtextxy(
        25,
        17,
        (char*)"GREEN EARTH"
    );

    setcolor(COLOR(110, 205, 130));

    outtextxy(
        175,
        17,
        (char*)"vs"
    );

    setcolor(COLOR(235, 115, 90));

    outtextxy(
        220,
        17,
        (char*)"INDUSTRIAL POLLUTION"
    );

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    setcolor(COLOR(190, 195, 200));

    outtextxy(
        28,
        45,
        (char*)"Interactive Environmental Transformation Visualizer"
    );

    // Meter
    int meterX = 760;
    int meterY = 22;
    int meterW = 300;

    box(
        meterX,
        meterY,
        meterX + meterW,
        meterY + 20,
        COLOR(55, 60, 65)
    );

    int fill = (int)((meterW - 4) * eco);

    int c = blendColor(
        220, 65, 50,
        45, 205, 85,
        eco
    );

    if (fill > 0)
        box(
            meterX + 2,
            meterY + 2,
            meterX + 2 + fill,
            meterY + 18,
            c
        );

    char percent[50];

    sprintf(
        percent,
        "ECO HEALTH %d%%",
        (int)(eco * 100)
    );

    setcolor(WHITE);
    outtextxy(1070, 27, percent);

    // State badge
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

    if (eco < 0.25f)
    {
        setbkcolor(COLOR(24, 30, 37));
        setcolor(LIGHTRED);

        outtextxy(
            760,
            49,
            (char*)"STATUS: CRITICAL POLLUTION"
        );
    }
    else if (eco < 0.95f)
    {
        setbkcolor(COLOR(24, 30, 37));
        setcolor(YELLOW);

        outtextxy(
            760,
            49,
            (char*)"STATUS: ECOSYSTEM RECOVERING..."
        );
    }
    else
    {
        setbkcolor(COLOR(24, 30, 37));
        setcolor(LIGHTGREEN);

        outtextxy(
            760,
            49,
            (char*)"STATUS: SUSTAINABLE ENVIRONMENT"
        );
    }
}

// BOTTOM CONTROL PANEL

void drawControls()
{
    box(0, 665, W, 700, COLOR(22, 27, 32));

    setbkcolor(COLOR(22, 27, 32));
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

    setcolor(COLOR(230, 230, 230));

    outtextxy(
        25,
        678,
        (char*)"SPACE : Start/Pause Transformation"
    );

    outtextxy(
        370,
        678,
        (char*)"R : Reset"
    );

    outtextxy(
        490,
        678,
        (char*)"ESC : Exit"
    );

    if (!transforming && eco < 0.99f)
    {
        setcolor(YELLOW);

        outtextxy(
            790,
            678,
            (char*)"Press SPACE to restore Earth"
        );
    }
    else if (eco >= 0.99f)
    {
        setcolor(LIGHTGREEN);

        outtextxy(
            805,
            678,
            (char*)"GO GREEN - SAVE OUR PLANET!"
        );
    }
}

// SCENE MODULES

void drawPollutedScene()
{
    drawFactory(eco);
    drawDeforestation();
    drawWastePipe();
    drawSmoke();
    drawDrops();
}

void drawGreenScene()
{
    drawSun(eco);
    drawForest(eco);
    drawSolarFarm();
    drawTurbines(eco);
    drawFlowers();
    drawFish();
    drawBirds();
    drawEcoSign();
}

// UPDATE

void update()
{
    frameCount++;

    cloudMove += 0.25f;
    waterMove += 0.7f;
    birdMove += 0.7f;

    turbineAngle += 0.045f;

    if (turbineAngle > 2 * PI)
        turbineAngle -= 2 * PI;

    // Transition speed
  if (transforming && eco < 1.0f)
{
    // Faster and smooth transformation
    eco += 0.005f;

    if (eco >= 1.0f)
    {
        eco = 1.0f;
        transforming = false;
    }
}

    if (frameCount % 2 == 0)
        createSmoke();

    createDrop();

    updateSmoke();
    updateDrops();
}

// INPUT

bool keyboard()
{
    if (!kbhit())
        return true;

    char key = getch();

    if (key == 27)
        return false;

    if (key == ' ')
    {
        if (eco < 1.0f)
            transforming = !transforming;
    }

    if (key == 'r' || key == 'R')
    {
        eco = 0.0f;
        transforming = false;

        initParticles();
    }

    return true;
}

// MAIN

int main()
{
    srand((unsigned)time(NULL));

    initwindow(
        W,
        H,
        "Green Earth vs Industrial Pollution Visualizer"
    );

    initParticles();

    int page = 0;
    bool running = true;

    while (running)
    {
        // DOUBLE BUFFERING

        setactivepage(page);
        cleardevice();

        update();

        // DRAWING ORDER

        drawSky(eco);

        drawSun(eco);
        drawClouds(eco);

        drawHills(eco);
        drawGround(eco);

        drawRiver(eco);

        drawPollutedScene();
        drawGreenScene();

        drawHUD();
        drawControls();

        // Show completed frame
        setvisualpage(page);

        // Switch back buffer
        page = 1 - page;

        running = keyboard();

        // Approximately 60 FPS
        delay(16);
    }

    closegraph();

    return 0;
}