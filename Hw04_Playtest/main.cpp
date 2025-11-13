/**
* Author: Anson Liang
* Assignment: Rise of the AI
* Date due: 2025-11-08, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/


#include "CS3113/LevelC.h"
#include "CS3113/LevelMenu.h"
#include "CS3113/LevelLost.h"


// Global Constants
constexpr int SCREEN_WIDTH = 1920 * 1.5,
        SCREEN_HEIGHT = 1080,
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
LevelB *gLevelB = nullptr;
LevelC *gLevelC = nullptr;
LevelLost *gLevelLost = nullptr;


int glives = 3;


// Function Declarations
void switchToScene(Scene *scene);

void initialise();

void processInput();

void update();

void render();

void shutdown();

void switchToScene(Scene *scene) {
    gCurrentScene = scene;
    gCurrentScene->initialise(glives);
}


void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Platformer");
    InitAudioDevice();

    gLevelMenu = new LevelMenu(ORIGIN, "#F5F5DC");
    gLevelA = new LevelA(ORIGIN, "#C0897E");
    gLevelB = new LevelB(ORIGIN, "#011627");
    gLevelC = new LevelC(ORIGIN, "#011627");
    gLevelLost = new LevelLost(ORIGIN, "#000000");

    gLevels.push_back(gLevelMenu);
    gLevels.push_back(gLevelA);
    gLevels.push_back(gLevelB);
    gLevels.push_back(gLevelC);
    gLevels.push_back(gLevelLost);

    switchToScene(gLevels[0]); // Start with menu

    SetTargetFPS(FPS);
}

void processInput() {
    Entity *player = gCurrentScene->getState().player;
    if (player != nullptr) {
        player->resetMovement();

        if (IsKeyDown(KEY_A)) player->moveLeft();
        else if (IsKeyDown(KEY_D)) player->moveRight();

        if (IsKeyPressed(KEY_W) && player->isCollidingBottom()) {
            player->jump();
            PlaySound(gCurrentScene->getState().jumpSound);
        }

        if (GetLength(player->getMovement()) > 1.0f)
            player->normaliseMovement();
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
    delete gLevelB;

    for (int i = 0; i < NUMBER_OF_LEVELS; i++) gLevels[i] = nullptr;

    CloseAudioDevice();
    CloseWindow();
}

int main(void) {
    initialise();

    while (gAppStatus == RUNNING) {
        processInput();
        update();

        if (gCurrentScene->getState().switchScene > 0) {
            if (gCurrentScene->getState().switchScene == 1) {
                glives -= 1;
                PlaySound(gCurrentScene->getState().LostSound);
            }
            if (glives == 0) {
                glives = 3;
                switchToScene(gLevels[4]);
            } else {
                int id = gCurrentScene->getState().nextSceneID;
                switchToScene(gLevels[id]);
            }
        }

        render();
    }

    shutdown();

    return 0;
}
