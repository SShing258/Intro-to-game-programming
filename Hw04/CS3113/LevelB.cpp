#include "LevelB.h"

LevelB::LevelB() : Scene{{0.0f}, nullptr} {
}

LevelB::LevelB(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode} {
}

LevelB::~LevelB() { shutdown(); }

void LevelB::initialise(int livesRemaning) {
    mGameState.switchScene = 0;
    mGameState.nextSceneID = 0;

    //https://xdeviruchi.itch.io/16-bit-fantasy-adventure-music-pack
    mGameState.bgm = LoadMusicStream("assets/time_for_adventure.mp3");
    SetMusicVolume(mGameState.bgm, 0.33f);
    PlayMusicStream(mGameState.bgm);

    //https://brackeysgames.itch.io/brackeys-platformer-bundle
    mGameState.jumpSound = LoadSound("assets/jump.wav");
    SetSoundVolume(mGameState.jumpSound, 0.33f);

    mGameState.LostSound = LoadSound("assets/hurt.wav");
    SetSoundVolume(mGameState.LostSound, 0.33f);

    /*
       ----------- MAP -----------
    */
    mGameState.map = new Map(
        LEVEL_WIDTH, LEVEL_HEIGHT, // map grid cols & rows
        (unsigned int *) mLevelData, // grid data
        "assets/oak_woods_tileset.png", // texture filepath
        TILE_DIMENSION, // tile size
        24, 24, // texture cols & rows
        mOrigin // in-game origin
    );

    /*
       ----------- PROTAGONIST -----------
    */
    std::map<Direction, std::vector<int> > playerAnimationAtlas = {
        {RIGHT, {0, 1, 2, 3, 4, 5}},
        {LEFT, {6, 7, 8, 9, 10, 11}},

    };

    std::map<Direction, std::vector<int> > enemyAnimationAtlas = {
        {RIGHT, {0, 1, 2, 3, 4, 5}},
        {LEFT, {6, 7, 8, 9, 10, 11}},

    };
    //https://pixelfrog-assets.itch.io/tiny-swords
    mGameState.player = new Entity(
        {mOrigin.x - TILE_DIMENSION * 22, mOrigin.y}, // position
        {250.0f, 250.0f}, // scale
        "assets/player_walk.png", // texture file address
        ATLAS, // single image or atlas?
        {2, 6}, // atlas dimensions
        playerAnimationAtlas, // actual atlas
        PLAYER // entity type
    );

    mGameState.enemy = new Entity(
        {mOrigin.x - TILE_DIMENSION * 5, mOrigin.y}, // position
        {250.0f, 250.0f}, // scale
        "assets/enemy_walk.png", // texture file address
        ATLAS, // single image or atlas?
        {2, 6}, // atlas dimensions
        enemyAnimationAtlas, // actual atlas
        NPC // entity type
    );


    mGameState.enemy->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    mGameState.enemy->setColliderDimensions({
        mGameState.enemy->getScale().x / 3.5f,
        mGameState.enemy->getScale().y / 3.0f
    });
    mGameState.enemy->setSpeed(80.0f);
    mGameState.enemy->setAIType(JUMPER);
    mGameState.enemy->setJumpingPower(1500.0f);


    mGameState.player->setLives(livesRemaning);
    mGameState.player->setJumpingPower(550.0f);
    mGameState.player->setColliderDimensions({
        mGameState.player->getScale().x / 3.5f,
        mGameState.player->getScale().y / 3.0f
    });
    mGameState.player->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});


    /*
       ----------- CAMERA -----------
    */
    mGameState.camera = {0}; // zero initialize
    mGameState.camera.target = mGameState.player->getPosition(); // camera follows player
    mGameState.camera.offset = mOrigin; // camera offset to center of screen
    mGameState.camera.rotation = 0.0f; // no rotation
    mGameState.camera.zoom = 1.0f; // default zoom
}

void LevelB::update(float deltaTime) {
    UpdateMusicStream(mGameState.bgm);

    mGameState.enemy->update(
        deltaTime, // delta time / fixed timestep
        mGameState.player, // player
        mGameState.map, // map
        nullptr, // collidable entities
        0 // col. entity count
    );

    mGameState.player->update(
        deltaTime, // delta time / fixed timestep
        mGameState.player, // player
        mGameState.map, // map
        nullptr, // collidable entities
        0 // col. entity count
    );

    if (mGameState.player->isColliding(mGameState.enemy)) {
        mGameState.nextSceneID = 2;
        mGameState.switchScene = 1;
    }

    if (mGameState.player->getPosition().x > TILE_DIMENSION*42) {
        mGameState.nextSceneID = 3;
        mGameState.switchScene = 2;
    }

    Vector2 currentPlayerPosition = {mGameState.player->getPosition().x, mOrigin.y};

    if (mGameState.player->getPosition().y > 1000.0f) {
        mGameState.nextSceneID = 2;
        mGameState.switchScene = 1;
    }

    panCamera(&mGameState.camera, &currentPlayerPosition);
}

void LevelB::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.player->render();
    mGameState.player->displayCollider();
    mGameState.enemy->render();
    mGameState.map->render();
    DrawText(TextFormat("Lives: %02i", mGameState.player->getLives() ), 200, 160, 40, BLUE);
}

void LevelB::shutdown() {
    delete mGameState.player;
    delete mGameState.map;

    UnloadMusicStream(mGameState.bgm);
    UnloadSound(mGameState.jumpSound);
}
