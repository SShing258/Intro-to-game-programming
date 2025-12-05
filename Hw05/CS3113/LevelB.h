#pragma once

#include "raylib.h"
#include "Scene.h"
#include "Entity.h"
#include "Map.h"
#include <map>
#include <vector>





#ifndef LEVELB_H
#define LEVELB_H


class LevelB : public Scene {



public:
    static constexpr float TILE_DIMENSION       = 75.0f,
                        ACCELERATION_OF_GRAVITY = 981.0f,
                        END_GAME_THRESHOLD      = 800.0f;

    LevelB();
    LevelB(Vector2 origin, const char *bgHexCode);
    ~LevelB();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif