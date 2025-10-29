/**
* Author: Anson Liang
* Assignment: Lunar Lander
* Date due: 2025-10-28, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/


#include "CS3113/Entity.h"

// Global Constants
constexpr int SCREEN_WIDTH = 1920,
        SCREEN_HEIGHT = 1080,
        FPS = 120;

constexpr char BG_COLOUR[] = "#000000";
constexpr Vector2 ORIGIN = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

constexpr int NUMBER_OF_TILES = 10;
constexpr float TILE_DIMENSION = 50.0f,
        // in m/ms², since delta time is in ms
        ACCELERATION_OF_GRAVITY = 0.981f,
        FIXED_TIMESTEP = 1.0f / 60.0f;
float gOscillationTime = 0.0f;
float gFuel = 100.0f;


// Global Variables
AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f,
        gTimeAccumulator = 0.0f;


Entity *gTiles;
Entity *gSpaceShip;
Entity *gYouWinText;
Entity *gYouLostText;


// Function Declarations
void initialise();

void processInput();

void update();

void render();

void shutdown();

void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hw03 Lunar Lander");


    gSpaceShip = new Entity(
        {ORIGIN.x, ORIGIN.y - 200.0f},
        {100.0f, 100.0f},
        //copyright free assets from: https://mattwalkden.itch.io/lunar-battle-pack
        "assets/game/Spaceships_red.png",
        MOVING
    );
    gSpaceShip->setAngle(270);
    gSpaceShip->setColliderDimensions({
        gSpaceShip->getScale().x / 1.5f,
        gSpaceShip->getScale().y / 1.5f
    });

    gYouWinText = new Entity(
        {ORIGIN.x, ORIGIN.y - 200.0f},
        {920.0f, 140.0f},
        "assets/game/YOU WIN.png",
        STATIC
    );

    gYouWinText->deactivate();

    gYouLostText = new Entity(
        {ORIGIN.x, ORIGIN.y - 200.0f},
        {1158.0f, 140.0f},
        "assets/game/YOU LOST.png",
        STATIC
    );

    gYouLostText->deactivate();

    gTiles = new Entity(
        {ORIGIN.x, ORIGIN.y + 400.0f},
        {TILE_DIMENSION * 10, TILE_DIMENSION},
        // assets from @see https://kenney.nl/assets/pixel-platformer-industrial-expansion
        "assets/game/tile_0000.png",
        STATIC
    );


    SetTargetFPS(FPS);
}

void processInput() {
    gSpaceShip->resetMovement();

    if (gSpaceShip->getObjecType() == MOVING && gFuel > 0.0f) {
        if (IsKeyDown(KEY_A)) {
            gSpaceShip->moveLeft();
            gFuel -= 0.05f;
        } else if (IsKeyDown(KEY_D)) {
            gSpaceShip->moveRight();
            gFuel -= 0.05f;
        }
        if (IsKeyDown(KEY_W)) {
            gSpaceShip->moveUp();
            gFuel -= 0.2f;
        } else if (IsKeyDown(KEY_S)) {
            gSpaceShip->moveDown();
            gFuel -= 0.1f;
        }
    }


    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
    // Delta time
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    // Fixed timestep
    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP) {
        gTimeAccumulator = deltaTime;
        return;
    }

    if (gSpaceShip->getPosition().x > SCREEN_WIDTH || gSpaceShip->getPosition().x < 0) {
        gYouLostText->activate();
        gSpaceShip->setObjectType(STATIC);
        gSpaceShip->resetMovement();
        gSpaceShip->setAcceleration({0,0,});
        gSpaceShip->setVeloctiy({0,0,});
    }
    if (gSpaceShip->getPosition().y > 1200) {
        gYouLostText->activate();
        gSpaceShip->setObjectType(STATIC);
        gSpaceShip->resetMovement();
        gSpaceShip->setAcceleration({0,0,});
        gSpaceShip->setVeloctiy({0,0,});
    }
    gOscillationTime += deltaTime;
    float oscillation = sin(gOscillationTime * 0.2f) * 500.0f;

    if (gSpaceShip->getObjecType() == MOVING) {
        gTiles->setPosition({oscillation + ORIGIN.x, gTiles->getPosition().y});
    }



    if (gSpaceShip->isCollidingBottom()) {
        gYouWinText->activate();
        gSpaceShip->setObjectType(STATIC);
        gSpaceShip->resetMovement();
        gSpaceShip->setAcceleration({0,0,});
        gSpaceShip->setVeloctiy({0,0,});
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        gSpaceShip->update(FIXED_TIMESTEP, gTiles, 1);
        gTiles->update(FIXED_TIMESTEP, nullptr, 0);

        deltaTime -= FIXED_TIMESTEP;
    }
}

void render() {
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    //Approved
    DrawText(TextFormat("Fuel: %03.02f percent", gFuel), 200, 80, 100, WHITE);
    gYouWinText->render();
    gYouLostText->render();

    gTiles->render();

    gSpaceShip->render();

    EndDrawing();
}

void shutdown() {
    delete gSpaceShip;
    delete gYouWinText;
    delete gYouLostText;
    delete gTiles;

    CloseWindow();
}

int main(void) {
    initialise();

    while (gAppStatus == RUNNING) {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}
