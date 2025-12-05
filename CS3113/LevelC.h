#include "Scene.h"

#ifndef LEVELC_H
#define LEVELC_H

#include "Scene.h"

class LevelC : public Scene {

public:
    static constexpr float TILE_DIMENSION       = 75.0f,
                            ACCELERATION_OF_GRAVITY = 981.0f;

    LevelC();
    LevelC(Vector2 origin, const char *bgHexCode);
    ~LevelC();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif