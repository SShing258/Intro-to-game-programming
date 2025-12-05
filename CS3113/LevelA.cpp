#include "LevelA.h"
#include "Map.h"


LevelA::LevelA() : Scene{{0.0f}, nullptr},
                   mEffects({0.0f, 0.0f}, 960, 1280) {
}

LevelA::LevelA(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode},
                                                        mEffects(origin, 960, 1280) {
}

LevelA::~LevelA() { shutdown(); }


void LevelA::initialise() {
    mEffects.startFadeIn(1.0f);
    gShader.load("shaders/vertex.glsl", "shaders/fragment.glsl");


    mGameState.switchScene = 0;
    mGameState.nextSceneID = 0;


    mGameState.bgm = LoadMusicStream("assets/09_sky_ruins_98.ogg");
    SetMusicVolume(mGameState.bgm, 0.33f);
    PlayMusicStream(mGameState.bgm);


    mGameState.damageSound = LoadSound("assets/damage00.wav");
    SetSoundVolume(mGameState.damageSound, 0.33f);

    mGameState.deadSound = LoadSound("assets/pldead00.wav");
    SetSoundVolume(mGameState.deadSound, 0.33f);

    mGameState.nextSpellCardSound = LoadSound("assets/enep01.wav");
    SetSoundVolume(mGameState.nextSpellCardSound, 0.33f);


    /*
       ----------- MAP -----------
    */
    mGameState.map = new Map(
        LEVEL_WIDTH, LEVEL_HEIGHT, // map grid cols & rows
        (unsigned int *) mLevelData, // grid data
        "assets/TileSet v1.0.png", // texture filepath
        TILE_DIMENSION, // tile sizes
        10, 10, // texture cols & rows
        mOrigin // in-game origin
    );

    /*
       ----------- PROTAGONIST -----------
    */
    std::map<Direction, std::vector<int> > sanaeAnimationAtlas = {
        {NOTMOVING, {1, 2, 4, 5, 7}},
        {LEFT, {8, 9, 10, 11, 12}},
        {RIGHT, {16, 17, 18, 19, 20}}

    };

    mGameState.sanea = new Entity(
        {mOrigin.x, mOrigin.y + 8 * 50.0f}, // position
        {64.0f * 1.5f, 96.0f * 1.5f}, // scale
        "assets/saneaMoveSpriteSheet.png", // texture file address
        ATLAS, // single image or atlas?
        {3, 8}, // atlas dimensions
        sanaeAnimationAtlas, // actual atlas
        PLAYER // entity type
    );


    mGameState.sanea->setLives(3);
    mGameState.sanea->setSpeed(400);
    mGameState.sanea->setColliderDimensions({
        mGameState.sanea->getScale().x / 10.0f,
        mGameState.sanea->getScale().y / 10.0f
    });


    std::map<Direction, std::vector<int> > enemyAnimationAtlas = {
        {
            NOTMOVING, {
                0, 0, 0,
                1, 1, 1,
                2, 2, 2,
                3, 3, 3,
                4, 4, 4,
                5, 5, 5,
                6, 6, 6,
                7, 7, 7,
                8, 8, 8,
                9, 9, 9,
                10, 10, 10
            }
        },

        {LEFT, {}},
        {RIGHT, {}}
    };


    mGameState.enemy = new Entity(
        {mOrigin.x, mOrigin.y - 8 * 50.0f}, // position
        {250.0f * 1.5, 176.0f * 1.5}, // scale
        "assets/yukariEnemy.png", // texture file address
        ATLAS, // single image or atlas?
        {1, 11}, // atlas dimensions
        enemyAnimationAtlas, // actual atlas
        NPC // entity type
    );

    mGameState.enemy->setMaxHP(2500);
    mGameState.enemy->setHP(2500);
    mGameState.enemy->setDirection(NOTMOVING);
    mGameState.enemy->setSpeed(0);

    mGameState.enemy->switchSpellCard(spellCardLevels);

    mGameState.enemy->setColliderDimensions({
        mGameState.enemy->getScale().x / 2.33f,
        mGameState.enemy->getScale().y / 1.5f
    });

    /*
       ----------- CAMERA -----------
    */
    mGameState.camera = {0}; // zero initialize
    mGameState.camera.target = mOrigin; //mGameState.sanea->getPosition(); // camera follows player
    mGameState.camera.offset = mOrigin; // camera offset to center of screen
    mGameState.camera.rotation = 0.0f; // no rotation
    mGameState.camera.zoom = 1.0f; // default zoom
}

void LevelA::update(float deltaTime) {
    UpdateMusicStream(mGameState.bgm);

    mEffects.updateFade(deltaTime);


    if (mGameState.enemy->getHP() <= 0) {
        for (auto *b: mGameState.sanea->getBullets()) {
            if (b) b->deactivate();
        }

        for (auto *b: mGameState.enemy->getEnemyBullets()) {
            if (b) b->deactivate();
        }

        spellCardLevels += 1;
        PlaySound(mGameState.nextSpellCardSound);
        mGameState.enemy->switchSpellCard(spellCardLevels);
        mGameState.enemy->setMaxHP(2500);
        mGameState.enemy->setHP(2500);
    }


    mGameState.sanea->update(
        deltaTime, // delta time / fixed timestep
        mGameState.sanea, // player
        mGameState.map, // map
        nullptr, // collidable entities
        0 // col. entity count
    );

    mGameState.enemy->update(
        deltaTime, // delta time / fixed timestep
        mGameState.sanea, // player
        mGameState.map, // map
        nullptr, // collidable entities
        0 // col. entity count
    );

    bool playerWasHit = false;

    for (const auto &b: mGameState.enemy->getEnemyBullets()) {
        if (b->isActive() && mGameState.sanea->isColliding(b)) {
            if (!mGameState.sanea->isInvincible()) {
                mGameState.sanea->setLives(
                    mGameState.sanea->getLives() - 1
                );

                mGameState.sanea->setPosition({mOrigin.x, mOrigin.y + 8 * TILE_DIMENSION});

                mGameState.sanea->startInvincibility();

                playerWasHit = true;
            }

            b->deactivate();
        }
        b->update(deltaTime, mGameState.sanea, nullptr, mGameState.sanea, 0);
    }

    if (playerWasHit) {
        PlaySound(mGameState.deadSound);
        for (auto &b: mGameState.enemy->getEnemyBullets()) {
            b->deactivate();
        }
    }


    for (const auto &b: mGameState.sanea->getBullets()) {
        if (b->isActive() && mGameState.enemy->isColliding(b)) {
            mGameState.enemy->takeDamage(1);
            PlaySound(mGameState.damageSound);
            b->deactivate();
        }
        b->update(deltaTime, mGameState.sanea, nullptr, mGameState.enemy, 0);
    }


    if (mGameState.sanea->getLives() <= 0) {
        gShader.end();
        mGameState.nextSceneID = 2;
        mGameState.switchScene = 1;
        spellCardLevels = 1;
    }

    if (mGameState.enemy->getSpellCard() == WON) {
        gShader.end();
        mGameState.nextSceneID = 3;
        mGameState.switchScene = 1;
        spellCardLevels = 0;
    }

    mGameState.map->updateBackground(deltaTime);

}

void LevelA::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));
    mGameState.map->renderBackground();

    mGameState.sanea->render();
    mGameState.enemy->render();

    mGameState.map->render();

    if (mGameState.sanea->getMovementMode() == FOCUS) {
        mGameState.sanea->displaySmallCircle();
    }

    mGameState.enemy->drawHealthBar();

    if (mGameState.enemy->getSpellCard() == SPELLCARD3) {
        gShader.begin();
        gShader.setVector2("lightPosition", mGameState.sanea->getPosition());
    } else {
        gShader.end();
    }

    int lives = mGameState.sanea->getLives();
    DrawText(TextFormat("Lives: %d", lives), 700, 20, 40, RED);
    mEffects.drawFade();
}

void LevelA::shutdown() {
    delete mGameState.sanea;
    delete mGameState.map;

    UnloadMusicStream(mGameState.bgm);
    UnloadSound(mGameState.nextSpellCardSound);
    UnloadSound(mGameState.deadSound);
    UnloadSound(mGameState.damageSound);
}
