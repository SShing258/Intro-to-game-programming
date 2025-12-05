

#ifndef ENTITY_H
#define ENTITY_H

#include "Map.h"

enum Direction { NOTMOVING, LEFT, UP, RIGHT, DOWN }; // For walking

enum MovementMode { UNFOCUS, FOCUS };

enum FiringMode { NOFIRING, FIRING };

enum EntityStatus { ACTIVE, INACTIVE };

enum EntityType { PLAYER, BLOCK, PLATFORM, NPC, BULLET, ENEMYBULLET, NOTHING};

enum SpellCards { NOSPELLCARD, SPELLCARD1, SPELLCARD2, SPELLCARD3, WON};





class Entity {
private:
    int mMaxHP = 100;
    int mHP = 100;


    int lives;
    Vector2 mPosition;
    Vector2 mMovement;
    Vector2 mVelocity;
    Vector2 mAcceleration;

    Vector2 mScale;
    Vector2 mColliderDimensions;

    Texture2D mTexture;
    TextureType mTextureType;
    Vector2 mSpriteSheetDimensions;

    std::map<Direction, std::vector<int> > mAnimationAtlas;
    std::vector<int> mAnimationIndices;
    Direction mDirection;
    MovementMode mMovementMode;
    int mFrameSpeed;

    int mCurrentFrameIndex = 0;
    float mAnimationTime = 0.0f;

    bool mIsJumping = false;
    float mJumpingPower = 0.0f;

    int mSpeed;
    float mAngle;

    SpellCards mSpellCard = NOSPELLCARD;

    FiringMode mFiringMode = NOFIRING;
    float mShootCooldown = 0.07f;
    float mShootTimer = 0.0f;
    bool mWantsToShoot = false;

    float mEnemyShootCooldown = 0.07f;
    float mEnemyShootTimer = 0.0f;

    float mSpellCard1SpinDegrees = 0.0f;
    float mSpellCard1SpinSpeed   = 60.0f;
    float mSpellCard1SpinAccel   = 120.0f;


    int   mSpellCard2ShotsFiredLeft  = 0;
    int   mSpellCard2ShotsFiredRight = 0;
    bool  mSpellCard2FiringLeft      = true;
    float mSpellCard2BurstCooldown   = 0.03f;
    float mSpellCard2BurstTimer      = 0.0f;


    float mSpellCard3Time = 0.0f;
    float mSpellCard3MinCooldown = 0.8f;
    float mSpellCard3MaxCooldown = 0.05f;
    float mSpellCard3CurrentCooldown = 0.8f;
    float mSpellCard3Timer = 0.0f;
    float mSpinSpeed = 0.0f;

    float mInvincibleTimer = 0.0f;
    float mInvincibleDuration = 1.0f;
    bool mIsInvincible = false;




    bool mIsCollidingTop = false;
    bool mIsCollidingBottom = false;
    bool mIsCollidingRight = false;
    bool mIsCollidingLeft = false;

    EntityStatus mEntityStatus = ACTIVE;
    EntityType mEntityType;


    std::vector<Entity *> bullets = {};
    std::vector<Entity *> enemyBullets = {};


    void checkCollisionY(Entity *collidableEntities, int collisionCheckCount);

    void checkCollisionY(Map *map);

    void checkCollisionX(Entity *collidableEntities, int collisionCheckCount);

    void checkCollisionX(Map *map);

    void resetColliderFlags() {
        mIsCollidingTop = false;
        mIsCollidingBottom = false;
        mIsCollidingRight = false;
        mIsCollidingLeft = false;
    }

    void animate(float deltaTime);

public:
    bool isColliding(Entity *other) const;
    static constexpr int DEFAULT_SIZE = 250;
    static constexpr int DEFAULT_SPEED = 400;
    static constexpr int DEFAULT_FRAME_SPEED = 14;
    static constexpr float Y_COLLISION_THRESHOLD = 0.5f;

    Entity();

    Entity(Vector2 position, Vector2 scale, const char *textureFilepath,
           EntityType entityType);

    Entity(Vector2 position, Vector2 scale, const char *textureFilepath,
           TextureType textureType, Vector2 spriteSheetDimensions,
           std::map<Direction, std::vector<int> > animationAtlas,
           EntityType entityType);

    ~Entity();

    void update(float deltaTime, Entity *player, Map *map,
                Entity *collidableEntities, int collisionCheckCount);

    void spellCard3(float deltaTime);

    void spellCard2(float deltaTime, Entity *mTargetPlayer);

    void spellCard1(float deltaTime);

    void spawnBullets();



    void render();


    bool isInvincible() const { return mIsInvincible; }
    void startInvincibility() {
        mInvincibleTimer = 0.0f;
        mIsInvincible = true;
    }


    int getHP() const { return mHP; }
    int getMaxHP() const { return mMaxHP; }

    void setHP(const int hp) { mHP = hp; }
    void setMaxHP(const int maxhp) { mMaxHP = maxhp; }

    void takeDamage(const int dmg) {
        mHP -= dmg;
        if (mHP < 0) mHP = 0;
    }


    void normaliseMovement() { Normalise(&mMovement); }

    void jump() { mIsJumping = true; }
    void activate() { mEntityStatus = ACTIVE; }
    void deactivate() { mEntityStatus = INACTIVE; }

    void displayCollider();

    void displaySmallCircle();

    void drawHealthBar();

    void switchSpellCard(int spellCard);

    bool isActive() { return mEntityStatus == ACTIVE ? true : false; }

    void moveUp() {
        mMovement.y = -1;
        mDirection = NOTMOVING;
    }

    void moveDown() {
        mMovement.y = 1;
        mDirection = NOTMOVING;
    }

    void moveLeft() {
        mMovement.x = -1;
        mDirection = LEFT;
    }

    void moveRight() {
        mMovement.x = 1;
        mDirection = RIGHT;
    }

    void notMoving() {
        mDirection = NOTMOVING;
    }

    void focus() {
        mSpeed = DEFAULT_SPEED * 0.5;
        mMovementMode = FOCUS;
    }

    void unfocus() {
        mSpeed = DEFAULT_SPEED;
        mMovementMode = UNFOCUS;
    }

    void resetMovement() {
        mMovement = {0.0f, 0.0f};
    }

    void characterFiring() {
        mFiringMode = FIRING;
    };


    Vector2 getPosition() const { return mPosition; }
    Vector2 getMovement() const { return mMovement; }
    Vector2 getVelocity() const { return mVelocity; }
    Vector2 getAcceleration() const { return mAcceleration; }
    Vector2 getScale() const { return mScale; }
    Vector2 getColliderDimensions() const { return mColliderDimensions; }
    Vector2 getSpriteSheetDimensions() const { return mSpriteSheetDimensions; }
    Texture2D getTexture() const { return mTexture; }
    TextureType getTextureType() const { return mTextureType; }
    Direction getDirection() const { return mDirection; }
    MovementMode getMovementMode() const { return mMovementMode; }
    int getFrameSpeed() const { return mFrameSpeed; }
    float getJumpingPower() const { return mJumpingPower; }
    bool isJumping() const { return mIsJumping; }
    int getSpeed() const { return mSpeed; }
    float getAngle() const { return mAngle; }
    EntityType getEntityType() const { return mEntityType; }
    int getLives() const { return lives; }
    bool getIsCollidingLeft() const { return mIsCollidingLeft; }
    bool getIsCollidingRight() const { return mIsCollidingRight; }
    bool getWantsToShoot() const { return mWantsToShoot; }
    FiringMode getFiringMode() const { return mFiringMode; }
    SpellCards getSpellCard() const { return mSpellCard; }
    std::vector<Entity *> getBullets() { return bullets; }
    std::vector<Entity *> getEnemyBullets() { return enemyBullets; }


    bool isCollidingTop() const { return mIsCollidingTop; }
    bool isCollidingBottom() const { return mIsCollidingBottom; }

    std::map<Direction, std::vector<int> > getAnimationAtlas() const { return mAnimationAtlas; }

    void setPosition(Vector2 newPosition) { mPosition = newPosition; }
    void setMovement(Vector2 newMovement) { mMovement = newMovement; }
    void setAcceleration(Vector2 newAcceleration) { mAcceleration = newAcceleration; }
    void setScale(Vector2 newScale) { mScale = newScale; }
    void setTexture(const char *textureFilepath) { mTexture = LoadTexture(textureFilepath); }
    void setColliderDimensions(Vector2 newDimensions) { mColliderDimensions = newDimensions; }
    void setSpriteSheetDimensions(Vector2 newDimensions) { mSpriteSheetDimensions = newDimensions; }
    void setSpeed(int newSpeed) { mSpeed = newSpeed; }
    void setFrameSpeed(int newSpeed) { mFrameSpeed = newSpeed; }
    void setJumpingPower(float newJumpingPower) { mJumpingPower = newJumpingPower; }
    void setAngle(float newAngle) { mAngle = newAngle; }
    void setEntityType(EntityType entityType) { mEntityType = entityType; }
    void setWantsToShoot(bool shoot) { mWantsToShoot = shoot; }
    void setFiringMode(FiringMode firingMode) { mFiringMode = firingMode; }
    void setSpellCard(SpellCards spellCard) { mSpellCard = spellCard; }
    void setSpinSpeed(float s) { mSpinSpeed = s; }

    void setDirection(Direction newDirection) {
        mDirection = newDirection;

        if (mTextureType == ATLAS) mAnimationIndices = mAnimationAtlas.at(mDirection);
    }

    void setMovementMode(MovementMode newMovement) { mMovementMode = newMovement; }

    void setLives(int live) {
        lives = live;
    }
};

#endif // ENTITY_H
