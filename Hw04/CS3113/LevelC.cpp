#include "LevelC.h"

LevelC::LevelC() : Scene{{0.0f}, nullptr} {}
LevelC::LevelC(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode} {}
LevelC::~LevelC() { shutdown(); }

void LevelC::initialise(int livesRemaning) {
    mGameState.switchScene = 0;
    mGameState.nextSceneID = 0;  // back to menu on win


    mGameState.bgm = LoadMusicStream("assets/time_for_adventure.mp3");
    SetMusicVolume(mGameState.bgm, 0.33f);
    PlayMusicStream(mGameState.bgm);

    mGameState.jumpSound = LoadSound("assets/jump.wav");
    SetSoundVolume(mGameState.jumpSound, 0.33f);


    mGameState.map = new Map(
        50, 10,
        (unsigned int*)mLevelData,
        "assets/oak_woods_tileset.png",
        TILE_DIMENSION,
        24, 24,
        mOrigin
    );


    std::map<Direction, std::vector<int>> playerAtlas = {
        {RIGHT, {0,1,2,3,4,5}},
        {LEFT,  {6,7,8,9,10,11}}
    };

    mGameState.player = new Entity(
        {mOrigin.x - TILE_DIMENSION * 22, mOrigin.y},
        {250.0f, 250.0f},
        "assets/player_walk.png",
        ATLAS,
        {2, 6},
        playerAtlas,
        PLAYER
    );

    mGameState.player->setLives(livesRemaning);
    mGameState.player->setJumpingPower(550.0f);
    mGameState.player->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    mGameState.player->setColliderDimensions({
        mGameState.player->getScale().x / 3.5f,
        mGameState.player->getScale().y / 3.0f
    });


    std::map<Direction, std::vector<int>> enemyAtlas = {
        {RIGHT, {0,1,2,3,4,5}},
        {LEFT,  {6,7,8,9,10,11}}
    };

    mGameState.enemy = new Entity(
        {mOrigin.x + TILE_DIMENSION * 10, mOrigin.y},
        {250.0f, 250.0f},
        "assets/enemy_walk.png",
        ATLAS,
        {2, 6},
        enemyAtlas,
        NPC
    );

    mGameState.enemy->setAIType(JUMPERFOLLOWER);
    mGameState.enemy->setJumpingPower(900.0f);
    mGameState.enemy->setSpeed(80.0f);
    mGameState.enemy->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    mGameState.enemy->setColliderDimensions({
        mGameState.enemy->getScale().x / 3.5f,
        mGameState.enemy->getScale().y / 3.0f
    });


    mGameState.camera = {0};
    mGameState.camera.target = mGameState.player->getPosition();
    mGameState.camera.offset = mOrigin;
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom = 1.0f;
}

void LevelC::update(float deltaTime) {
    UpdateMusicStream(mGameState.bgm);

    mGameState.enemy->update(deltaTime, mGameState.player, mGameState.map, nullptr, 0);
    mGameState.player->update(deltaTime, mGameState.player, mGameState.map, nullptr, 0);
    mGameState.player->setSpeed(mGameState.player->getSpeed() + 100.0f*deltaTime);

    if (mGameState.player->isColliding(mGameState.enemy)) {
        mGameState.nextSceneID = 3;
        mGameState.switchScene = 1;
    }

    if (mGameState.player->getPosition().x > TILE_DIMENSION * 38) {
        mGameState.enemy->deactivate();
        if (IsKeyPressed(KEY_ENTER)) {

            mGameState.nextSceneID = 0;
            mGameState.switchScene = 2;
        }

    }

    if (mGameState.player->getPosition().y > 1200.0f) {
        mGameState.nextSceneID = 3;
        mGameState.switchScene = 1;
    }




    Vector2 target = {mGameState.player->getPosition().x, mOrigin.y};
    panCamera(&mGameState.camera, &target);
}

void LevelC::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.map->render();
    mGameState.enemy->render();
    mGameState.player->render();
    mGameState.player->displayCollider();

    DrawText(TextFormat("Lives: %02i", mGameState.player->getLives()), 200, 160, 40, GOLD);

    if (mGameState.player->getPosition().x > TILE_DIMENSION * 38) {
        DrawText("YOU WIN!", mOrigin.x + 600, mOrigin.y - 200, 100, GOLD);
        DrawText("Press ENTER to return to menu", mOrigin.x + 650, mOrigin.y - 80, 50, WHITE);
    }
}

void LevelC::shutdown() {
    delete mGameState.player;
    delete mGameState.enemy;
    delete mGameState.map;

    UnloadMusicStream(mGameState.bgm);
    UnloadSound(mGameState.jumpSound);
}