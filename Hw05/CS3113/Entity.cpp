#include "Entity.h"

Entity::Entity() : mPosition{0.0f, 0.0f}, mMovement{0.0f, 0.0f},
                   mVelocity{0.0f, 0.0f}, mAcceleration{0.0f, 0.0f},
                   mScale{DEFAULT_SIZE, DEFAULT_SIZE},
                   mColliderDimensions{DEFAULT_SIZE, DEFAULT_SIZE},
                   mTexture{NULL}, mTextureType{SINGLE}, mAngle{0.0f},
                   mSpriteSheetDimensions{}, mDirection{RIGHT},
                   mAnimationAtlas{{}}, mAnimationIndices{}, mFrameSpeed{0},
                   mEntityType{NOTHING} {
    lives = 3;
}

Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath,
               EntityType entityType) : mPosition{position}, mVelocity{0.0f, 0.0f},
                                        mAcceleration{0.0f, 0.0f}, mScale{scale}, mMovement{0.0f, 0.0f},
                                        mColliderDimensions{scale}, mTexture{LoadTexture(textureFilepath)},
                                        mTextureType{SINGLE}, mDirection{RIGHT}, mAnimationAtlas{{}},
                                        mAnimationIndices{}, mFrameSpeed{0}, mSpeed{DEFAULT_SPEED},
                                        mAngle{0.0f}, mEntityType{entityType} {
    lives = 3;
}

Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath,
               TextureType textureType, Vector2 spriteSheetDimensions, std::map<Direction,
                   std::vector<int> > animationAtlas, EntityType entityType) : mPosition{position},
                                                                               mVelocity{0.0f, 0.0f},
                                                                               mAcceleration{0.0f, 0.0f},
                                                                               mMovement{0.0f, 0.0f}, mScale{scale},
                                                                               mColliderDimensions{scale},
                                                                               mTexture{LoadTexture(textureFilepath)},
                                                                               mTextureType{ATLAS},
                                                                               mSpriteSheetDimensions{
                                                                                   spriteSheetDimensions
                                                                               },
                                                                               mAnimationAtlas{animationAtlas},
                                                                               mDirection{RIGHT},
                                                                               mAnimationIndices{
                                                                                   animationAtlas.at(RIGHT)
                                                                               },
                                                                               mFrameSpeed{DEFAULT_FRAME_SPEED},
                                                                               mAngle{0.0f},
                                                                               mSpeed{DEFAULT_SPEED},
                                                                               mEntityType{entityType} {
    lives = 3;
}

Entity::~Entity() {
    UnloadTexture(mTexture);

    for (auto b: bullets) {
        delete b;
    }
    bullets.clear();

    for (auto b: enemyBullets) {
        delete b;
    }
    enemyBullets.clear();
}


void Entity::checkCollisionY(Entity *collidableEntities, int collisionCheckCount) {
    for (int i = 0; i < collisionCheckCount; i++) {
        // STEP 1: For every entity that our player can collide with...
        Entity *collidableEntity = &collidableEntities[i];

        if (isColliding(collidableEntity)) {
            // STEP 2: Calculate the distance between its centre and our centre
            //         and use that to calculate the amount of overlap between
            //         both bodies.
            float yDistance = fabs(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap = fabs(yDistance - (mColliderDimensions.y / 2.0f) -
                                  (collidableEntity->mColliderDimensions.y / 2.0f));

            // STEP 3: "Unclip" ourselves from the other entity, and zero our
            //         vertical velocity.
            if (mVelocity.y > 0) {
                mPosition.y -= yOverlap;
                mVelocity.y = 0;
                mIsCollidingBottom = true;
            } else if (mVelocity.y < 0) {
                mPosition.y += yOverlap;
                mVelocity.y = 0;
                mIsCollidingTop = true;

                if (collidableEntity->mEntityType == BLOCK)
                    collidableEntity->deactivate();
            }
        }
    }
}

void Entity::checkCollisionX(Entity *collidableEntities, int collisionCheckCount) {
    for (int i = 0; i < collisionCheckCount; i++) {
        Entity *collidableEntity = &collidableEntities[i];

        if (isColliding(collidableEntity)) {
            // When standing on a platform, we're always slightly overlapping
            // it vertically due to gravity, which causes false horizontal
            // collision detections. So the solution I found is only resolve X
            // collisions if there's significant Y overlap, preventing the
            // platform we're standing on from acting like a wall.
            float yDistance = fabs(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap = fabs(
                yDistance - (mColliderDimensions.y / 2.0f) - (collidableEntity->mColliderDimensions.y / 2.0f));

            // Skip if barely touching vertically (standing on platform)
            if (yOverlap < Y_COLLISION_THRESHOLD) continue;

            float xDistance = fabs(mPosition.x - collidableEntity->mPosition.x);
            float xOverlap = fabs(
                xDistance - (mColliderDimensions.x / 2.0f) - (collidableEntity->mColliderDimensions.x / 2.0f));

            if (mVelocity.x > 0) {
                mPosition.x -= xOverlap;
                mVelocity.x = 0;

                // Collision!
                mIsCollidingRight = true;
            } else if (mVelocity.x < 0) {
                mPosition.x += xOverlap;
                mVelocity.x = 0;

                // Collision!
                mIsCollidingLeft = true;
            }
        }
    }
}

void Entity::checkCollisionY(Map *map) {
    if (map == nullptr) return;

    constexpr float probeOffsetX = 2.0f;

    const Vector2 topCentreProbe = {mPosition.x, mPosition.y - (mColliderDimensions.y / 2.0f)};

    const Vector2 topLeftProbe = {
        mPosition.x - (mColliderDimensions.x / 2.0f) + probeOffsetX,
        mPosition.y - (mColliderDimensions.y / 2.0f)
    };
    const Vector2 topRightProbe = {
        mPosition.x + (mColliderDimensions.x / 2.0f) - probeOffsetX,
        mPosition.y - (mColliderDimensions.y / 2.0f)
    };

    const Vector2 bottomCentreProbe = {mPosition.x, mPosition.y + (mColliderDimensions.y / 2.0f)};

    const Vector2 bottomLeftProbe = {
        mPosition.x - (mColliderDimensions.x / 2.0f) + probeOffsetX,
        mPosition.y + (mColliderDimensions.y / 2.0f)
    };
    const Vector2 bottomRightProbe = {
        mPosition.x + (mColliderDimensions.x / 2.0f) - probeOffsetX,
        mPosition.y + (mColliderDimensions.y / 2.0f)
    };

    float xOverlap = 0.0f;
    float yOverlap = 0.0f;

    // COLLISION ABOVE (jumping upward)
    if ((map->isSolidTileAt(topCentreProbe, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(topLeftProbe, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(topRightProbe, &xOverlap, &yOverlap)) && mVelocity.y < 0.0f) {
        mPosition.y += yOverlap; // push down
        mVelocity.y = 0.0f;
        mIsCollidingTop = true;
    }

    // COLLISION BELOW (falling downward)
    if ((map->isSolidTileAt(bottomCentreProbe, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(bottomLeftProbe, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(bottomRightProbe, &xOverlap, &yOverlap)) && mVelocity.y > 0.0f) {
        mPosition.y -= yOverlap; // push up
        mVelocity.y = 0.0f;
        mIsCollidingBottom = true;
    }
}

void Entity::checkCollisionX(Map *map) {
    if (!map) return;

    const float halfW = mColliderDimensions.x / 2.0f;
    const float halfH = mColliderDimensions.y / 2.0f;

    Vector2 leftProbes[3] = {
        {mPosition.x - halfW, mPosition.y - halfH + 2}, // top
        {mPosition.x - halfW, mPosition.y}, // mid
        {mPosition.x - halfW, mPosition.y + halfH - 2} // bottom
    };

    Vector2 rightProbes[3] = {
        {mPosition.x + halfW, mPosition.y - halfH + 2},
        {mPosition.x + halfW, mPosition.y},
        {mPosition.x + halfW, mPosition.y + halfH - 2}
    };

    float bestXOverlap = 0;
    bool hitLeft = false;
    bool hitRight = false;

    for (const auto &p: leftProbes) {
        float xOverlap, yOverlap;
        if (map->isSolidTileAt(p, &xOverlap, &yOverlap)) {
            hitLeft = true;
            if (bestXOverlap == 0 || xOverlap > bestXOverlap) {
                bestXOverlap = xOverlap;
            }
        }
    }

    for (const auto &p: rightProbes) {
        float xOverlap, yOverlap;
        if (map->isSolidTileAt(p, &xOverlap, &yOverlap)) {
            hitRight = true;
            if (bestXOverlap == 0 || xOverlap > bestXOverlap) {
                bestXOverlap = xOverlap;
            }
        }
    }


    if (hitRight && mVelocity.x > 0.0f) {
        mPosition.x -= bestXOverlap;
        mVelocity.x = 0;
        mIsCollidingRight = true;
    }


    if (hitLeft && mVelocity.x < 0.0f) {
        mPosition.x += bestXOverlap;
        mVelocity.x = 0;
        mIsCollidingLeft = true;
    }
}


bool Entity::isColliding(Entity *other) const {
    if (!other->isActive() || other == this) return false;

    float xDistance = fabs(mPosition.x - other->getPosition().x) -
                      ((mColliderDimensions.x + other->getColliderDimensions().x) / 2.0f);
    float yDistance = fabs(mPosition.y - other->getPosition().y) -
                      ((mColliderDimensions.y + other->getColliderDimensions().y) / 2.0f);

    if (xDistance < 0.0f && yDistance < 0.0f) return true;

    return false;
}

void Entity::animate(float deltaTime) {
    if (mEntityType == PLAYER) {
        mAnimationIndices = mAnimationAtlas.at(mDirection);
    } else {
        mAnimationIndices = mAnimationAtlas.at(NOTMOVING);
    }


    mAnimationTime += deltaTime;
    float framesPerSecond = 1.0f / mFrameSpeed;

    if (mAnimationTime >= framesPerSecond) {
        mAnimationTime = 0.0f;

        mCurrentFrameIndex++;
        mCurrentFrameIndex %= mAnimationIndices.size();
    }
}


void Entity::update(float deltaTime, Entity *player, Map *map,
                    Entity *collidableEntities, int collisionCheckCount) {
    if (mEntityStatus == INACTIVE) return;

    if (mEntityType == BULLET) {
        mVelocity.x = mMovement.x * mSpeed;
        mVelocity.y = mMovement.y * mSpeed;

        mPosition.x += mVelocity.x * deltaTime;
        mPosition.y += mVelocity.y * deltaTime;

        if (mPosition.y < -200 || mPosition.y > 2000) {
            mEntityStatus = INACTIVE;
        }

        return;
    }

    if (mEntityType == ENEMYBULLET) {
        mVelocity.x = mMovement.x * mSpeed;
        mVelocity.y = mMovement.y * mSpeed;

        mPosition.x += mVelocity.x * deltaTime;
        mPosition.y += mVelocity.y * deltaTime;

        if (mPosition.y < -200 || mPosition.y > 2000) {
            mEntityStatus = INACTIVE;
        }
        if (mSpellCard == SPELLCARD3) {
            mAngle += mSpinSpeed * deltaTime;
        }


        return;
    }

    if (mIsInvincible) {
        mInvincibleTimer += deltaTime;
        if (mInvincibleTimer >= mInvincibleDuration) {
            mIsInvincible = false;
        }
    }


    resetColliderFlags();

    mVelocity.x = mMovement.x * mSpeed;
    mVelocity.y = mMovement.y * mSpeed;

    mVelocity.x += mAcceleration.x * deltaTime;
    mVelocity.y += mAcceleration.y * deltaTime;


    mPosition.y += mVelocity.y * deltaTime;
    checkCollisionY(collidableEntities, collisionCheckCount);
    checkCollisionY(map);

    mPosition.x += mVelocity.x * deltaTime;
    checkCollisionX(collidableEntities, collisionCheckCount);
    checkCollisionX(map);

    if (mTextureType == ATLAS) {
        animate(deltaTime);
    }

    if (mEntityType == NPC) {
        if (mSpellCard == SPELLCARD1) {
            mEnemyShootTimer += deltaTime;

            if (mEnemyShootTimer >= mEnemyShootCooldown) {
                mEnemyShootTimer = 0.0f;
                spellCard1(deltaTime);
            }
        }

        if (mSpellCard == SPELLCARD2) {
            mEnemyShootTimer += deltaTime;

            if (mEnemyShootTimer >= mEnemyShootCooldown) {
                mEnemyShootTimer = 0.0f;
                spellCard2(deltaTime, player);
            }
        }

        if (mSpellCard == SPELLCARD3) {
            spellCard3(deltaTime);
        }
    }


    if (mFiringMode == FIRING) {
        mShootTimer += deltaTime;

        if (mShootTimer >= mShootCooldown) {
            mShootTimer = 0.0f;
            spawnBullets();
        }
    }
}


void Entity::spellCard3(float deltaTime) {
    mSpellCard3Time += deltaTime;
    mSpellCard3Timer += deltaTime;

    float tRaw = mSpellCard3Time / 60.0f;
    if (tRaw > 1.0f) tRaw = 1.0f;

    float t = tRaw * tRaw * (3 - 2 * tRaw); //Smoother steps

    mSpellCard3CurrentCooldown =
            mSpellCard3MinCooldown * (1.0f - t) +
            mSpellCard3MaxCooldown * t;

    if (mSpellCard3Timer < mSpellCard3CurrentCooldown)
        return;

    mSpellCard3Timer = 0.0f;

    float randomX = GetRandomValue(50, 910);
    Vector2 spawn = {randomX, -50.f};
    Vector2 mv = {0.f, 1.f};

    Entity *b = new Entity(
        spawn,
        {64, 64},
        "assets/yukariBulletSpellCard3.png",
        ENEMYBULLET
    );


    float spin = GetRandomValue(60, 240);
    if (GetRandomValue(0, 1) == 0) spin = -spin;
    b->setSpinSpeed(spin);

    b->setSpellCard(SPELLCARD3);
    b->setColliderDimensions({
        b->getScale().x / 2.f,
        b->getScale().y / 2.f
    });

    b->setSpeed(220.f);
    b->setMovement(mv);
    b->setAngle(180);

    enemyBullets.push_back(b);
}


void Entity::spellCard2(float deltaTime, Entity *mTargetPlayer) {
    mSpellCard2BurstTimer += deltaTime;
    if (mSpellCard2BurstTimer < mSpellCard2BurstCooldown)
        return;
    mSpellCard2BurstTimer = 0.0f;

    float offsetX;

    if (mSpellCard2FiringLeft) {
        offsetX = -80.0f;
    } else {
        offsetX = 80.0f;
    }

    const float offsetY = 0.0f;

    const Vector2 spawnPos = {
        mPosition.x + offsetX,
        mPosition.y + offsetY
    };

    if (!mTargetPlayer) return;

    Vector2 playerPos = mTargetPlayer->getPosition();

    Vector2 dir = {
        playerPos.x - spawnPos.x,
        playerPos.y - spawnPos.y
    };

    Normalise(&dir);

    Entity *b = new Entity(
        spawnPos,
        {24, 24},
        "assets/yukariBulletSpellCard2.png",
        ENEMYBULLET
    );

    b->setColliderDimensions({
        b->getScale().x / 2.0f,
        b->getScale().y / 2.0f
    });

    b->setSpeed(200);
    b->setMovement(dir);

    float angle = atan2(dir.x, -dir.y) * RAD2DEG;
    b->setAngle(angle);

    enemyBullets.push_back(b);

    if (mSpellCard2FiringLeft) {
        mSpellCard2ShotsFiredLeft++;
        if (mSpellCard2ShotsFiredLeft >= 10) {
            mSpellCard2ShotsFiredLeft = 0;
            mSpellCard2FiringLeft = false;
        }
    } else {
        mSpellCard2ShotsFiredRight++;
        if (mSpellCard2ShotsFiredRight >= 10) {
            mSpellCard2ShotsFiredRight = 0;
            mSpellCard2FiringLeft = true;
        }
    }
}


void Entity::spellCard1(float deltaTime) {
    mSpellCard1SpinSpeed += mSpellCard1SpinAccel * deltaTime;
    mSpellCard1SpinDegrees += mSpellCard1SpinSpeed * deltaTime;

    while (mSpellCard1SpinDegrees >= 360.0f) mSpellCard1SpinDegrees -= 360.0f;
    while (mSpellCard1SpinDegrees < 0.0f) mSpellCard1SpinDegrees += 360.0f;

    for (int i = 0; i < 3; i++) {
        float angleDeg = mSpellCard1SpinDegrees + i * 120.0f;

        if (angleDeg >= 360.0f) angleDeg -= 360.0f;

        float radians = angleDeg * DEG2RAD;

        Vector2 mv = {sinf(radians), -cosf(radians)};

        Entity *b = new Entity(
            {mPosition.x, mPosition.y},
            {24, 24},
            "assets/yukariBulletSpellCard1.png",
            ENEMYBULLET
        );

        b->setColliderDimensions({
            b->getScale().x / 2.0f,
            b->getScale().y / 2.0f
        });

        b->setSpeed(200);
        b->setMovement(mv);
        b->setAngle(angleDeg);

        enemyBullets.push_back(b);
    }
}


void Entity::spawnBullets() {
    Vector2 playerPos = mPosition;

    float offsetX1 = 25.0f;
    float offsetX2 = 45.0f;
    float offsetY = -30.0f;

    std::vector<Vector2> spawnPoints = {
        {playerPos.x - offsetX1, playerPos.y + offsetY},
        {playerPos.x + offsetX1, playerPos.y + offsetY},
        {playerPos.x - offsetX2, playerPos.y + offsetY},
        {playerPos.x + offsetX2, playerPos.y + offsetY}
    };

    for (int i = 0; i < spawnPoints.size(); i++) {
        Entity *b = new Entity(
            spawnPoints[i],
            {20, 20},
            "assets/sanaeBullets.png",
            BULLET
        );

        b->setColliderDimensions(
            {
                b->getScale().x / 2.0f,
                b->getScale().y / 2.0f
            }
        );
        b->setSpeed(1000);
        Vector2 mv;

        if (mMovementMode == FOCUS) {
            mv = {0, -1};
        } else {
            bool left = (i % 2 == 0);

            if (i < 2) {
                if (left) {
                    mv = {-0.05f, -1};
                } else {
                    mv = {0.05f, -1};
                }
            } else {
                if (left) {
                    mv = {-0.25f, -1};
                } else {
                    mv = {0.25f, -1};
                }
            }
        }


        b->setMovement(mv);
        const float angle = atan2(mv.x, -mv.y) * RAD2DEG + 90;
        b->setAngle(angle);

        bullets.push_back(b);
    }
}


void Entity::render() {
    if (mEntityStatus == INACTIVE) return;

    for (const auto b: bullets) {
        if (b->isActive()) b->render();
    }

    for (const auto b: enemyBullets) {
        if (b->isActive()) b->render();
    }


    Rectangle textureArea;

    switch (mTextureType) {
        case SINGLE:
            // Whole texture (UV coordinates)
            textureArea = {
                // top-left corner
                0.0f, 0.0f,

                // bottom-right corner (of texture)
                static_cast<float>(mTexture.width),
                static_cast<float>(mTexture.height)
            };
            break;
        case ATLAS:
            textureArea = getUVRectangle(
                &mTexture,
                mAnimationIndices[mCurrentFrameIndex],
                mSpriteSheetDimensions.x,
                mSpriteSheetDimensions.y
            );

        default: break;
    }

    // Destination rectangle – centred on gPosition
    Rectangle destinationArea = {
        mPosition.x,
        mPosition.y,
        static_cast<float>(mScale.x),
        static_cast<float>(mScale.y)
    };

    // Origin inside the source texture (centre of the texture)
    Vector2 originOffset = {
        static_cast<float>(mScale.x) / 2.0f,
        static_cast<float>(mScale.y) / 2.0f
    };

    // Render the texture on screen
    DrawTexturePro(
        mTexture,
        textureArea, destinationArea, originOffset,
        mAngle, WHITE
    );


    // displayCollider();
}

void Entity::displayCollider() {
    // draw the collision box
    Rectangle colliderBox = {
        mPosition.x - mColliderDimensions.x / 2.0f,
        mPosition.y - mColliderDimensions.y / 2.0f,
        mColliderDimensions.x,
        mColliderDimensions.y
    };

    DrawRectangleLines(
        colliderBox.x, // Top-left X
        colliderBox.y, // Top-left Y
        colliderBox.width, // Width
        colliderBox.height, // Height
        GREEN // Color
    );
}

void Entity::displaySmallCircle() {
    DrawCircleGradient(mPosition.x, mPosition.y, 7, RED, PINK);
}

void Entity::drawHealthBar() {
    if (mEntityType == NPC) {
        const float radiusOuter = mScale.y * 0.6f;
        const float radiusInner = radiusOuter - 6;

        const float hpPercent = (float) mHP / (float) mMaxHP;

        const float endAngle = 360.0f * hpPercent;
        const Vector2 center = {mPosition.x, mPosition.y};

        DrawRing(center, radiusInner, radiusOuter, 0, 360, 64, DARKGRAY);
        DrawRing(center, radiusInner, radiusOuter, 360 - endAngle, 360, 64,RED);
        DrawRingLines(center, radiusInner, radiusOuter, 0, 360, 64, BLACK);
    }
}

void Entity::switchSpellCard(const int spellCard) {
    if (spellCard == 1) {
        mSpellCard = SPELLCARD1;
    } else if (spellCard == 2) {
        mSpellCard = SPELLCARD2;
    } else if (spellCard == 3) {
        mSpellCard = SPELLCARD3;
    }else if (spellCard == 4) {

        mSpellCard = WON;
    }else {
        printf("Invalid Spell Card: %d\n", spellCard);
    }
};
