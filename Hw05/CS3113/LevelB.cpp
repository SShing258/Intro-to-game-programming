#include "LevelB.h"
#include "Map.h"

LevelB::LevelB() : Scene{{0.0f}, nullptr} {
}

LevelB::LevelB(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode} {
}

LevelB::~LevelB() { shutdown(); }

void LevelB::initialise() {
}


void LevelB::update(float deltaTime) {

}

void LevelB::render() {

}

void LevelB::shutdown() {

}
