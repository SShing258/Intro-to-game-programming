#include "LevelMenu.h"

LevelMenu::LevelMenu() : Scene{{0.0f}, nullptr} {}

LevelMenu::LevelMenu(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode} {}

LevelMenu::~LevelMenu() {
    shutdown();
}

void LevelMenu::shutdown()
{

}

void LevelMenu::initialise(int livesRemaining) {
    mGameState.switchScene = false;
    mGameState.nextSceneID = 1;

    mGameState.StartSound= LoadSound("assets/coin.wav");
    SetSoundVolume(mGameState.StartSound, 0.33f);

    mGameState.player = nullptr;
    mGameState.enemy = nullptr;
    mGameState.map = nullptr;
    mGameState.bgm = { };
    mGameState.jumpSound = { };

    mGameState.camera = { 0 };
    mGameState.camera.target = mOrigin;
    mGameState.camera.offset = mOrigin;
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom = 1.0f;
}

void LevelMenu::update(float deltaTime) {

    if (IsKeyPressed(KEY_ENTER)) {
        PlaySound(mGameState.StartSound);
        mGameState.switchScene = 2;
    }
}

void LevelMenu::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));
    DrawText("The Platformer Game", mOrigin.x - 400, mOrigin.y - 150, 80, BLACK);
    DrawText("Press ENTER to Start", mOrigin.x - 350, mOrigin.y + 50, 50, LIGHTGRAY);
    DrawText("Use A/D to move, W to jump", mOrigin.x - 400, mOrigin.y + 120, 40, GRAY);
}
