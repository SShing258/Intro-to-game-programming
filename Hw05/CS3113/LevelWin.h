#include "Scene.h"

class LevelWin : public Scene {
public:
    LevelWin();
    LevelWin(Vector2 origin, const char *bgHexCode);
    ~LevelWin();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};
