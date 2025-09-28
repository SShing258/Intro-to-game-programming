/**
* Author: Anson Liang
* Assignment: Simple 2D Scene
* Date due: 2025-09-27, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
*
* Header file and main template from CS3113 GitHub repo
**/

#include "CS3113/cs3113.h"
#include <math.h>

// Global Constants
constexpr int   SCREEN_WIDTH  = 1600,
                SCREEN_HEIGHT = 900,
                FPS           = 60,
                SIZE          = 100;

//constexpr char    BG_COLOUR[] = "#FFFFFF";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr Vector2 SUN_BASE_SIZE   = { (float) SIZE * 5, (float) SIZE * 5};
constexpr Vector2 MOON_BASE_SIZE   = { (float) SIZE / 5, (float) SIZE / 5};
constexpr Vector2 EARTH_BASE_SIZE   = { (float) SIZE, (float) SIZE };

// Image owned by Nintendo — please don't sue me.
constexpr char Sun[]  = "assets/game/sun.png";
constexpr char Moon[]  = "assets/game/moon.png";
constexpr char Earth[]  = "assets/game/earth.png";

// Global Variables
AppStatus gAppStatus     = RUNNING;
float gPulseTime     = 0.0f;

//Sun
float     gSunAngle         = 0.0f;
Vector2   gSunPosition      = ORIGIN;
Vector2   gSunScale         = SUN_BASE_SIZE;

//Moon
float     gMoonAngle         = 0.0f;
Vector2   gMoonPosition      = ORIGIN;
Vector2   gMoonScale         = MOON_BASE_SIZE;

//Earth
float     gEarthAngle         = 0.0f;
Vector2   gEarthPosition      = ORIGIN;
Vector2   gEarthScale         = EARTH_BASE_SIZE;
bool      gEarthIsBehind      = false;


float     gPreviousTicks = 0.0f;

Texture2D gSunTexture;
Texture2D gMoonTexture;
Texture2D gEarthTexture;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();


// Render object abstraction adaptation from lesson 04
void renderObject(const Texture2D *texture, const Vector2 *position,
                  const Vector2 *scale, const float *angle)
{
    // Whole texture (UV coordinates)
    Rectangle textureArea = {
        // top-left corner
        0.0f, 0.0f,

        // bottom-right corner (of texture)
        static_cast<float>(texture->width),
        static_cast<float>(texture->height)
    };

    // Destination rectangle – centred on gPosition
    Rectangle destinationArea = {
        position->x,
        position->y,
        static_cast<float>(scale->x),
        static_cast<float>(scale->y)
    };

    // Origin inside the source texture (centre of the texture)
    Vector2 originOffset = {
        static_cast<float>(scale->x) / 2.0f,
        static_cast<float>(scale->y) / 2.0f
    };

    // Render the texture on screen
    DrawTexturePro(
        *texture,
        textureArea, destinationArea, originOffset,
        *angle, WHITE
    );
}


// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures");

    gSunTexture = LoadTexture(Sun);
    gMoonTexture = LoadTexture(Moon);
    gEarthTexture = LoadTexture(Earth);

    SetTargetFPS(FPS);
}

void processInput() 
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{
    // Delta time
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    gPulseTime += 1.0f * deltaTime;


    gSunPosition = {
        400.0f * cos(gPulseTime) + ORIGIN.x,
        100.0f * sin(gPulseTime) + ORIGIN.y
    };

    gSunAngle += 10.0f * deltaTime;
    gEarthAngle -= 100.0f * deltaTime;

    gEarthPosition = {
         200.0f * sin(gPulseTime + 0.5f*PI) + gSunPosition.x,
         gSunPosition.y
    };

    gEarthScale = {
            EARTH_BASE_SIZE.x + 100.0f * sin(gPulseTime),
            EARTH_BASE_SIZE.y + 100.0f * sin(gPulseTime)
    };

    if (sin(gPulseTime) > -0.5f) {
        gEarthIsBehind = false;
    }else {
        gEarthIsBehind = true;
         "#808080";
    }

    gMoonPosition = {
        gEarthPosition.x + EARTH_BASE_SIZE.x * cos(gPulseTime * 2.0f * PI),
        gEarthPosition.y + EARTH_BASE_SIZE.y * sin(gPulseTime * 2.0f * PI)
    };

}

void render()
{
    BeginDrawing();

    if (!gEarthIsBehind) {
        ClearBackground(ColorFromHex("#808080"));
        renderObject(&gSunTexture, &gSunPosition, &gSunScale, &gSunAngle);
        renderObject(&gMoonTexture, &gMoonPosition, &gMoonScale, &gMoonAngle);
        renderObject(&gEarthTexture, &gEarthPosition, &gEarthScale, &gEarthAngle);

    }else {
        ClearBackground(ColorFromHex("#FFFFFF"));
        renderObject(&gMoonTexture, &gMoonPosition, &gMoonScale, &gMoonAngle);
        renderObject(&gEarthTexture, &gEarthPosition, &gEarthScale, &gEarthAngle);
        renderObject(&gSunTexture, &gSunPosition, &gSunScale, &gSunAngle);

    }

    EndDrawing();
}

void shutdown() { CloseWindow(); }

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}