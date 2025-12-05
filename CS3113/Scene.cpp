#include "Scene.h"

Scene::Scene() : mOrigin{}, mGameState{} {}

Scene::Scene(Vector2 origin, const char *bgHexCode) : mOrigin{origin}, mBGColourHexCode{bgHexCode}, mGameState{} {

}