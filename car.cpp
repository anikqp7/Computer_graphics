#include <graphics.h>
#include <conio.h>
#include <stdlib.h>

#define W 800
#define H 600

// Function Declarations
void drawCyberpunkSky();
void drawBackgroundBuildings();
void drawForegroundBuildings();
void drawFlyoverAndRoad();
void drawRealisticCar(int x, int y, int type);
void drawBillboard(int textX);

int main() {
    int gd = DETECT, gm, page = 0;
    initgraph(&gd, &gm, (char*)"");

    int flyoverCarX = -100;
    int roadCarX = W + 100;
    int textX = W - 250;

    while (1) {
        setactivepage(page);
        cleardevice();

        // 1. Atmosphere: Cyberpunk Foggy Sky Horizon
        drawCyberpunkSky();

        // 2. Parallax Layers: Background & Foreground Cityscape
        drawBackgroundBuildings();
        drawForegroundBuildings();

        // 3. Futuristic LED Signboard
        drawBillboard(textX);
        textX -= 3; // Smooth Scroll
        if (textX < 140) textX = 560;

        // 4. Infrastructure (Roads, Wet Reflection, Pillars)
        drawFlyoverAndRoad();

        // 5. Dynamic Vehicles with Neon Light Trails
        // Fast Sports Car on Flyover (Magenta Underglow)
        drawRealisticCar(flyoverCarX, 325, 1);
        flyoverCarX += 9;
        if (flyoverCarX > W + 100) flyoverCarX = -120;

        // Cyber Sedan on Lower Road (Cyan Underglow)
        drawRealisticCar(roadCarX, 515, 2);
        roadCarX -= 5;
        if (roadCarX < -120) roadCarX = W + 100;

        // Exit Logic
        if (kbhit() && getch() == 27) break; // ESC Exit

        // Double Buffering Swap
        setvisualpage(page);
        page = 1 - page;
        delay(25);
    }

    closegraph();
    return 0;
}

// Atmospheric Cyberpunk Gradient Sky
void drawCyberpunkSky() {
    setfillstyle(SOLID_FILL, BLACK);
    bar(0, 0, W, 250);
    
    // Horizon Magenta Fog Glow
    for (int y = 250; y < 400; y += 10) {
        setfillstyle(SOLID_FILL, (y > 340) ? DARKGRAY : MAGENTA);
        bar(0, y, W, y + 10);
    }
}

// Layer 1: Distant Shaded Skyscrapers (3D Depth)
void drawBackgroundBuildings() {
    int bgX[] = {10, 110, 230, 360, 490, 620, 710};
    int bgW[] = {80, 90, 100, 85, 95, 80, 85};
    int bgH[] = {300, 340, 280, 360, 310, 290, 330};

    for (int i = 0; i < 7; i++) {
        setfillstyle(SOLID_FILL, BLUE); // Darker tone for depth
        bar(bgX[i], H - bgH[i], bgX[i] + bgW[i], 420);
    }
}

// Layer 2: Detailed Foreground Cyber Buildings with Neon Trims
void drawForegroundBuildings() {
    int fgX[] = {40, 180, 320, 500, 650};
    int fgW[] = {110, 115, 130, 120, 110};
    int fgH[] = {420, 480, 410, 460, 430};

    for (int i = 0; i < 5; i++) {
        int topY = H - fgH[i];
        
        // Building Base Structure
        setfillstyle(SOLID_FILL, DARKGRAY);
        bar(fgX[i], topY, fgX[i] + fgW[i], 420);

        // Cyber Neon Edges (Glow Outlines)
        setcolor((i % 2 == 0) ? CYAN : MAGENTA);
        setlinestyle(SOLID_LINE, 1, 2);
        rectangle(fgX[i], topY, fgX[i] + fgW[i], 420);
        setlinestyle(SOLID_LINE, 1, 1);

        // Matrix Windows Effect (Randomized)
        for (int wy = topY + 20; wy < 380; wy += 22) {
            for (int wx = fgX[i] + 12; wx < fgX[i] + fgW[i] - 12; wx += 18) {
                if ((wx + wy + rand()) % 3 == 0) {
                    setfillstyle(SOLID_FILL, (rand() % 2 == 0) ? YELLOW : CYAN);
                    bar(wx, wy, wx + 8, wy + 12);
                }
            }
        }
    }
}

// High-Tech LED Digital Signboard
void drawBillboard(int textX) {
    // Metal Support Truss
    setcolor(LIGHTGRAY);
    line(250, 110, 250, 170);
    line(530, 110, 530, 170);

    // Neon Frame
    setfillstyle(SOLID_FILL, BLACK);
    bar(160, 60, 620, 120);
    setcolor(MAGENTA);
    setlinestyle(SOLID_LINE, 1, 3);
    rectangle(158, 58, 622, 122);
    setlinestyle(SOLID_LINE, 1, 1);

    // Grid Lines on Display Screen for LED Feel
    setcolor(DARKGRAY);
    for (int y = 65; y < 115; y += 6) line(165, y, 615, y);

    // Dynamic Scrolling Text
    setcolor(CYAN);
    outtextxy(textX, 85, (char*)"WELCOME TO CYBERPUNK CITY 2077 - NIGHT CITY SYSTEM ONLINE");
}

// Flyover, Road & Wet Surface Reflection Logic
void drawFlyoverAndRoad() {
    // Lower Wet Road
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(0, 480, W, H);

    // Wet Road Reflections (Neon Color Streaks)
    setcolor(BLUE);
    for (int rx = 20; rx < W; rx += 80) line(rx, 530, rx + 30, 530);
    setcolor(MAGENTA);
    for (int rx = 60; rx < W; rx += 100) line(rx, 550, rx + 40, 550);

    // Lane Dividers
    setcolor(YELLOW);
    for (int x = 0; x < W; x += 50) bar(x, 535, x + 25, 538);

    // Heavy Concrete Flyover Pillars
    setfillstyle(SOLID_FILL, BLACK);
    bar(80, 340, 120, 480);
    bar(360, 340, 400, 480);
    bar(640, 340, 680, 480);

    // Elevated Flyover Deck
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(0, 335, W, 355);
    setcolor(CYAN);
    setlinestyle(SOLID_LINE, 1, 2);
    line(0, 335, W, 335); // Top Neon Rail
    setcolor(MAGENTA);
    line(0, 355, W, 355); // Bottom Neon Rail
    setlinestyle(SOLID_LINE, 1, 1);
}

// Realistic Cyberpunk Car Model with Underglow & Lights
void drawRealisticCar(int x, int y, int type) {
    if (type == 1) { 
        // Futuristic Sports Car (Flyover - Moving Right)
        // Underglow Effect
        setfillstyle(SOLID_FILL, MAGENTA);
        bar(x - 5, y + 18, x + 65, y + 22);

        // Car Body Structure
        setfillstyle(SOLID_FILL, BLACK);
        bar(x, y + 8, x + 60, y + 18); // Lower body
        bar(x + 15, y, x + 42, y + 8);  // Roof/Cabin

        // Glass Windshield
        setfillstyle(SOLID_FILL, CYAN);
        bar(x + 30, y + 2, x + 40, y + 7);

        // Neon Lights & Exhaust Trail
        setfillstyle(SOLID_FILL, WHITE);
        fillellipse(x + 58, y + 12, 3, 2); // Headlight
        setfillstyle(SOLID_FILL, RED);
        bar(x - 10, y + 10, x, y + 13);    // Red Tail Light Speed Trail
    } else { 
        // Cyber Sedan (Lower Road - Moving Left)
        // Underglow Effect
        setfillstyle(SOLID_FILL, CYAN);
        bar(x - 5, y + 22, x + 70, y + 26);

        // Car Body Structure
        setfillstyle(SOLID_FILL, BLACK);
        bar(x, y + 10, x + 65, y + 22);
        bar(x + 18, y, x + 48, y + 10);

        // Windshield
        setfillstyle(SOLID_FILL, MAGENTA);
        bar(x + 20, y + 2, x + 32, y + 9);

        // Lights & Trail
        setfillstyle(SOLID_FILL, YELLOW);
        fillellipse(x + 2, y + 15, 3, 2); // Headlight
        setfillstyle(SOLID_FILL, RED);
        bar(x + 65, y + 12, x + 78, y + 15); // Tail Light Speed Trail
    }
}