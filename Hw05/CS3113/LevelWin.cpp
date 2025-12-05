#include "LevelWin.h"

LevelWin::LevelWin() : Scene{{0.0f}, nullptr} {}

LevelWin::LevelWin(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode} {}

LevelWin::~LevelWin() {
    shutdown();
}


void LevelWin::initialise() {
    mGameState.bgm = LoadMusicStream("assets/14_players_score_125.ogg");
    SetMusicVolume(mGameState.bgm, 0.33f);
    PlayMusicStream(mGameState.bgm);

    mGameState.selectSound = LoadSound("assets/ok00.wav");
    SetSoundVolume(mGameState.selectSound, 0.33f);

    mGameState.switchScene = 0;
    mGameState.nextSceneID = 0;


    mGameState.camera = {};
    mGameState.camera.target = mOrigin;
    mGameState.camera.offset = mOrigin;
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom = 1.0f;
}

void LevelWin::update(float deltaTime) {
    UpdateMusicStream(mGameState.bgm);
    if (IsKeyPressed(KEY_ENTER)) {
        PlaySound(mGameState.selectSound);
        mGameState.switchScene = 2;
        mGameState.nextSceneID = 0;
    }
}

void LevelWin::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));
    DrawText("YOU WON!", mOrigin.x - 200 , mOrigin.y - 150, 80, GOLD);
    DrawText("Press ENTER to return to Menu", mOrigin.x - 425, mOrigin.y + 50, 50, LIGHTGRAY);
}

void LevelWin::shutdown() {

    UnloadMusicStream(mGameState.bgm);
    UnloadSound(mGameState.selectSound);

}