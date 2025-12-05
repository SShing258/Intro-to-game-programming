/**
* Author: Anson Liang
* Assignment: Final Project
* Date due: 2025-12-05, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/


#include "CS3113/LevelC.h"
#include "CS3113/LevelMenu.h"
#include "CS3113/LevelLost.h"
#include "CS3113/ShaderProgram.h"
#include "CS3113/LevelA.h"
#include "CS3113/LevelB.h"
#include "CS3113/LevelWin.h"


// Global Constants


constexpr int SCREEN_WIDTH = 480*2,
        SCREEN_HEIGHT = 640*2,
        FPS = 120,
        NUMBER_OF_LEVELS = 2;

constexpr Vector2 ORIGIN = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f,
        gTimeAccumulator = 0.0f;

Scene *gCurrentScene = nullptr;
std::vector<Scene *> gLevels = {};


LevelMenu *gLevelMenu = nullptr;
LevelA *gLevelA = nullptr;
LevelLost *gLevelLost = nullptr;
LevelWin *gLevelWin = nullptr;




// Function Declarations
void switchToScene(Scene *scene);

void initialise();

void processInput();

void update();

void render();

void shutdown();

void switchToScene(Scene *scene) {
    gCurrentScene = scene;
    gCurrentScene->initialise();
}


void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Danmaku Training");
    InitAudioDevice();

    gLevelMenu = new LevelMenu(ORIGIN, "#F5F5DC");
    gLevelA = new LevelA(ORIGIN, "#C0897E");
    gLevelLost = new LevelLost(ORIGIN, "#000000");
    gLevelWin = new LevelWin(ORIGIN, "#000000");

    gLevels.push_back(gLevelMenu);
    gLevels.push_back(gLevelA);
    gLevels.push_back(gLevelLost);
    gLevels.push_back(gLevelWin);


    switchToScene(gLevels[0]);

    SetTargetFPS(FPS);
}

void processInput() {
    Entity *sanea = gCurrentScene->getState().sanea;
    if (sanea != nullptr) {
        sanea->resetMovement();

        if (IsKeyUp(KEY_UP) && IsKeyUp(KEY_DOWN) && IsKeyUp(KEY_LEFT) && IsKeyUp(KEY_RIGHT)) {
            sanea->notMoving();
        }

        if (IsKeyDown(KEY_UP)) {
            sanea->moveUp();
        } else if (IsKeyDown(KEY_DOWN)) {
            sanea->moveDown();
        }

        if (IsKeyDown(KEY_LEFT)) {
            sanea->moveLeft();
        } else if (IsKeyDown(KEY_RIGHT)) {
            sanea->moveRight();
        };

        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            sanea->focus();
        }else {
            sanea->unfocus();
        }

        if (IsKeyDown(KEY_Z)) {
            sanea->setFiringMode(FIRING);
        }else {
            sanea->setFiringMode(NOFIRING);
        }


        if (GetLength(sanea->getMovement()) > 1.0f)
            sanea->normaliseMovement();
    }
}

void update() {
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP) {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }
}

void render() {
    BeginDrawing();
    BeginMode2D(gCurrentScene->getState().camera);

    gCurrentScene->render();

    EndMode2D();
    EndDrawing();
}

void shutdown() {
    delete gLevelA;

    for (int i = 0; i < NUMBER_OF_LEVELS; i++) gLevels[i] = nullptr;

    CloseAudioDevice();
    CloseWindow();
}

int main(void) {
    initialise();
    SetTraceLogLevel(LOG_WARNING);

    while (gAppStatus == RUNNING) {
        processInput();
        update();

        if (gCurrentScene->getState().switchScene > 0) {
                const int id = gCurrentScene->getState().nextSceneID;
                switchToScene(gLevels[id]);
        }

        render();
    }

    shutdown();

    return 0;
}
