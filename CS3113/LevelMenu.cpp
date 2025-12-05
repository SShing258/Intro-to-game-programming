#include "LevelMenu.h"

LevelMenu::LevelMenu() : Scene{{0.0f}, nullptr} {}

LevelMenu::LevelMenu(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode} {}

LevelMenu::~LevelMenu() {
    shutdown();
}



void LevelMenu::initialise() {
    mGameState.switchScene = false;
    mGameState.nextSceneID = 1;

    mGameState.bgm = LoadMusicStream("assets/13_oharaibou_140.ogg");
    SetMusicVolume(mGameState.bgm, 0.33f);
    PlayMusicStream(mGameState.bgm);

    mGameState.selectSound = LoadSound("assets/ok00.wav");
    SetSoundVolume(mGameState.selectSound, 0.33f);


    mGameState.camera = { 0 };
    mGameState.camera.target = mOrigin;
    mGameState.camera.offset = mOrigin;
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom = 1.0f;
}

void LevelMenu::update(float deltaTime) {
    UpdateMusicStream(mGameState.bgm);
    if (IsKeyPressed(KEY_ENTER)) {
        PlaySound(mGameState.selectSound);
        mGameState.switchScene = 1;
    }
}

void LevelMenu::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));
    DrawText("Danmaku Training", mOrigin.x - 375, mOrigin.y - 150, 80, BLACK);
    DrawText("Press ENTER to Start", mOrigin.x - 350, mOrigin.y + 50, 50, LIGHTGRAY);
    DrawText("Arrow keys to move, Shift to focus", mOrigin.x - 400, mOrigin.y + 120, 40, GRAY);
}

void LevelMenu::shutdown() {

    UnloadMusicStream(mGameState.bgm);
    UnloadSound(mGameState.selectSound);

}