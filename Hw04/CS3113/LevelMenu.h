#include "Scene.h"
#include "LevelB.h"

class LevelMenu : public Scene {
public:
    LevelMenu();
    LevelMenu(Vector2 origin, const char *bgHexCode);
    ~LevelMenu();

    void initialise(int livesRemaining) override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};
