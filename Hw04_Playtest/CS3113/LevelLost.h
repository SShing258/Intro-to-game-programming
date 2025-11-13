#include "Scene.h"

class LevelLost : public Scene {
public:
    LevelLost();
    LevelLost(Vector2 origin, const char *bgHexCode);
    ~LevelLost();

    void initialise(int livesRemaining) override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};
