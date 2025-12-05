#include "LevelC.h"

LevelC::LevelC() : Scene{{0.0f}, nullptr} {}
LevelC::LevelC(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode} {}
LevelC::~LevelC() { shutdown(); }

void LevelC::initialise() {

}

void LevelC::update(float deltaTime) {

}

void LevelC::render() {

}

void LevelC::shutdown() {

}