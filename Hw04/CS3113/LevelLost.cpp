#include "LevelLost.h"

LevelLost::LevelLost() : Scene{{0.0f}, nullptr} {}

LevelLost::LevelLost(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode} {}

LevelLost::~LevelLost() {
    shutdown();
}
void LevelLost::shutdown() {

}

void LevelLost::initialise(int livesRemaining) {
    mGameState.switchScene = false;
    mGameState.nextSceneID = 0;

    mGameState.player = nullptr;
    mGameState.enemy = nullptr;
    mGameState.map = nullptr;
    mGameState.bgm = {};
    mGameState.jumpSound = {};

    mGameState.camera = {};
    mGameState.camera.target = mOrigin;
    mGameState.camera.offset = mOrigin;
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom = 1.0f;
}

void LevelLost::update(float deltaTime) {
    if (IsKeyPressed(KEY_ENTER)) {
        mGameState.switchScene = 2;
        mGameState.nextSceneID = 0;
    }
}

void LevelLost::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));
    DrawText("YOU LOST!", mOrigin.x - 150 , mOrigin.y - 150, 80, RED);
    DrawText("Press ENTER to return to Menu", mOrigin.x - 350, mOrigin.y + 50, 50, LIGHTGRAY);
}
