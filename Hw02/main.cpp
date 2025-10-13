#include "CS3113/cs3113.h"
#include <math.h>

// Global Constants
constexpr int SCREEN_WIDTH = 1600,
        SCREEN_HEIGHT = 900,
        FPS = 60,
        SIZE = 100;
constexpr float MAX_AMP = 10.0f,
        PLAYER_SPEED = 500.0f;


constexpr char BG_COLOUR[] = "#000000";
constexpr Vector2 ORIGIN = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
constexpr Vector2 TOP_POSITION = {SCREEN_WIDTH / 2, 0.0f};
constexpr Vector2 TOP_SCALE = {SCREEN_WIDTH, 1.0f};
constexpr Vector2 BOT_POSITION = {SCREEN_WIDTH / 2, SCREEN_HEIGHT};
constexpr Vector2 BOT_SCALE = {SCREEN_WIDTH, 1.0f};
constexpr Vector2 L_POSITION = {0.0f, SCREEN_HEIGHT / 2};
constexpr Vector2 L_SCALE = {1.0, SCREEN_HEIGHT};
constexpr Vector2 R_POSITION = {SCREEN_WIDTH, SCREEN_HEIGHT / 2};
constexpr Vector2 R_SCALE = {1.0f, SCREEN_HEIGHT};


// Soul image from Undertale, Winning Text from Undertale textbox generator
constexpr char UT_SOUL1[] = "assets/game/undertale_soul1.png";
constexpr char UT_SOUL2[] = "assets/game/undertale_soul2.png";
constexpr char UT_SOUL3[] = "assets/game/undertale_soul3.png";
constexpr char Player1[] = "assets/game/Player1.png";
constexpr char Player2[] = "assets/game/Player2.png";
constexpr char Player1Win[] = "assets/game/Player1WinText.png";
constexpr char Player2Win[] = "assets/game/Player2WinText.png";


// Global Variables
AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f,
      gBallSpeed     = 300.0f,
      gSpeedIncreaseFactor = 90.0f;
int   gSoulNumber    = 1,
      PlayerWon      = 0;



//Winning Textures
float gWinTextAngle = 0.0f;
float gWinTextScaleFactor = 2.5f;


Color gWinTextColor = WHITE;

Texture2D gPlayer1WinTextTexture;
Texture2D gPlayer2WinTextTexture;

Vector2 gWinTextPosition = {(float) SCREEN_WIDTH / 2, (float) SCREEN_HEIGHT * 5 / 7},
        gWinTextScale = {SIZE * 5.720f * gWinTextScaleFactor, SIZE * 1.520f * gWinTextScaleFactor};

//Pong1 - Soul
float gSoul1Angle = 0.0f;

Vector2 gSoul1Scale = {(float) SIZE / 5, (float) SIZE / 5},
        gSoul1Position = ORIGIN,
        gSoul1Movement = {0.0f, 0.0f};

Color gSoul1Color = WHITE;

Texture2D gSoul1Texture;


//Pong2 - Soul
float gSoul2Angle = 0.0f;

Vector2 gSoul2Scale = {(float) SIZE / 5, (float) SIZE / 5},
        gSoul2Position = ORIGIN,
        gSoul2Movement = {0.0f, 0.0f};

Color gSoul2Color = WHITE;

Texture2D gSoul2Texture;


//Pong3 - Soul
float gSoul3Angle = 0.0f;

Vector2 gSoul3Scale = {(float) SIZE / 5, (float) SIZE / 5},
        gSoul3Position = ORIGIN,
        gSoul3Movement = {0.0f, 0.0f};

Color gSoul3Color = WHITE;

Texture2D gSoul3Texture;


//Paddle  1 - Player1 on the left
float gPlayer1Angle = 0.0f;
Color gPlayer1Color = WHITE;
bool gPlayer1Moving = false;

Vector2 gPlayer1Scale = {(float) SIZE / 5, (float) SIZE},
        gPlayer1Position = {SCREEN_WIDTH / 10, SCREEN_HEIGHT / 2},
        gPlayer1Movement = {0.0f, 0.0f};

Texture2D gPlayer1Texture;


//Paddle 2 - Player2 on the right
float gPlayer2Angle = 0.0f;
Color gPlayer2Color = WHITE;
bool gPlayer2Moving = false,
        gComputerControlled = false;

Vector2 gPlayer2Scale = {(float) SIZE / 5, (float) SIZE},
        gPlayer2Position = {SCREEN_WIDTH * 9 / 10, SCREEN_HEIGHT / 2},
        gPlayer2Movement = {0.0f, 0.0f};

Texture2D gPlayer2Texture;


// Function Declarations
void initialise();

void processInput();

void update();

void render();

void shutdown();

bool isColliding(const Vector2 *positionA, const Vector2 *scaleA, const Vector2 *positionB, const Vector2 *scaleB);

bool isColliding(const Vector2 *positionA, const Vector2 *scaleA,
                 const Vector2 *positionB, const Vector2 *scaleB) {
    float xDistance = fabs(positionA->x - positionB->x) - ((scaleA->x + scaleB->x) / 2.0f);
    float yDistance = fabs(positionA->y - positionB->y) - ((scaleA->y + scaleB->y) / 2.0f);

    if (xDistance < 0.0f && yDistance < 0.0f) return true;

    return false;
}


void isSoulColliding(Vector2 *position, const Vector2 *scale, Vector2 *movement) {
    float collisionCorrection = 20.0f;
    float wrongMovementPatternCollisionCorrection = 20.0f;
    if (isColliding(position, scale, &gPlayer1Position, &gPlayer1Scale)) {
        if (ColorIsEqual(gPlayer1Color, BLUE)) {
            if (!gPlayer1Moving) {
                if ((float) rand() / (float) RAND_MAX > 0.5f) {
                    gPlayer1Color = WHITE;
                } else {
                    gPlayer1Color = ORANGE;
                }
                gBallSpeed += gSpeedIncreaseFactor;
                movement->x *= -1.0f;
                position->x += collisionCorrection;
            } else {
                position->x -= wrongMovementPatternCollisionCorrection;
            }
        } else if (ColorIsEqual(gPlayer1Color, ORANGE)) {
            if (gPlayer1Moving) {
                if ((float) rand() / (float) RAND_MAX > 0.5f) {
                    gPlayer1Color = WHITE;
                } else {
                    gPlayer1Color = BLUE;
                }
                gBallSpeed += gSpeedIncreaseFactor;
                movement->x *= -1.0f;
                position->x += collisionCorrection;
            } else {
                position->x -= wrongMovementPatternCollisionCorrection;
            }
        } else {
            if ((float) rand() / (float) RAND_MAX > 0.5f) {
                gPlayer1Color = ORANGE;
            } else {
                gPlayer1Color = BLUE;
            }
            gBallSpeed += gSpeedIncreaseFactor;
            movement->x *= -1.0f;
            position->x += collisionCorrection;
        }
    }

    if (isColliding(position, scale, &gPlayer2Position, &gPlayer2Scale)) {
        if (ColorIsEqual(gPlayer2Color, BLUE)) {
            if (!gPlayer2Moving) {
                if ((float) rand() / (float) RAND_MAX > 0.5f) {
                    gPlayer2Color = WHITE;
                } else {
                    gPlayer2Color = ORANGE;
                }
                gBallSpeed += gSpeedIncreaseFactor;
                movement->x *= -1.0f;
                position->x -= collisionCorrection;
            } else {
                position->x += wrongMovementPatternCollisionCorrection;
            }
        } else if (ColorIsEqual(gPlayer2Color, ORANGE)) {
            if (gPlayer2Moving) {
                if ((float) rand() / (float) RAND_MAX > 0.5f) {
                    gPlayer2Color = WHITE;
                } else {
                    gPlayer2Color = BLUE;
                }
                gBallSpeed += gSpeedIncreaseFactor;
                movement->x *= -1.0f;
                position->x -= collisionCorrection;
            } else {
                position->x += wrongMovementPatternCollisionCorrection;
            }
        } else {
            if ((float) rand() / (float) RAND_MAX > 0.5f) {
                gPlayer2Color = ORANGE;
            } else {
                gPlayer2Color = BLUE;
            }
            gBallSpeed += gSpeedIncreaseFactor;
            movement->x *= -1.0f;
            position->x -= collisionCorrection;
        }
    }


    if (isColliding(position, scale, &TOP_POSITION, &TOP_SCALE)) {
        movement->y *= -1.0f;
    }
    if (isColliding(position, scale, &BOT_POSITION, &BOT_SCALE)) {
        movement->y *= -1.0f;
    }
    if (isColliding(position, scale, &L_POSITION, &L_SCALE)) {
        PlayerWon = 2;
    }
    if (isColliding(position, scale, &R_POSITION, &R_SCALE)) {
        PlayerWon = 1;
    }
}

void renderObject(const Texture2D *texture, const Vector2 *position,
                  const Vector2 *scale, const float gAngle, const Color *gTint) {
    // Whole texture (UV coordinates)
    Rectangle textureArea = {
        // top-left corner
        0.0f, 0.0f,

        // bottom-right corner (of texture)
        static_cast<float>(texture->width),
        static_cast<float>(texture->height)
    };

    // Destination rectangle – centred on gPosition
    Rectangle destinationArea = {
        position->x,
        position->y,
        static_cast<float>(scale->x),
        static_cast<float>(scale->y)
    };

    // Origin inside the source texture (centre of the texture)
    Vector2 originOffset = {
        static_cast<float>(scale->x) / 2.0f,
        static_cast<float>(scale->y) / 2.0f
    };

    // Render the texture on screen
    DrawTexturePro(
        *texture,
        textureArea, destinationArea, originOffset, gAngle, *gTint
    );
}

void randSoulMovement(Vector2 *soulMovement) {
    float randomSoulMovementX = rand();
    while (randomSoulMovementX > 0.2f * RAND_MAX && randomSoulMovementX < 0.8f * RAND_MAX) {
        randomSoulMovementX = rand();
    }

    soulMovement->x = (randomSoulMovementX / (float) RAND_MAX - 0.5f) * 2.0f;
    soulMovement->y = ((float) rand() / (float) RAND_MAX - 0.5f) * 2.0f;
}

// Function Definitions
void initialise() {
    srand((unsigned int) time(NULL));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures");

    gSoul1Texture = LoadTexture(UT_SOUL1);
    gSoul2Texture = LoadTexture(UT_SOUL2);
    gSoul3Texture = LoadTexture(UT_SOUL3);
    gPlayer1Texture = LoadTexture(Player1);
    gPlayer2Texture = LoadTexture(Player2);
    gPlayer1WinTextTexture = LoadTexture(Player1Win);
    gPlayer2WinTextTexture = LoadTexture(Player2Win);

    randSoulMovement(&gSoul1Movement);
    randSoulMovement(&gSoul2Movement);
    randSoulMovement(&gSoul3Movement);

    SetTargetFPS(FPS);
}

void processInput() {
    if (PlayerWon == 0) {
        if (IsKeyPressed(KEY_ONE)) {
            gSoulNumber = 1;
            gSoul2Position = ORIGIN,
                    gSoul3Position = ORIGIN,
                    randSoulMovement(&gSoul2Movement);
            randSoulMovement(&gSoul3Movement);
        } else if (IsKeyPressed(KEY_TWO)) {
            gSoulNumber = 2;
            gSoul3Position = ORIGIN,
                    randSoulMovement(&gSoul3Movement);
        } else if (IsKeyPressed(KEY_THREE)) {
            gSoulNumber = 3;
        }

        gPlayer1Movement = {0.0f, 0.0f};
        if (!gComputerControlled) {
            gPlayer2Movement = {0.0f, 0.0f};
        }
    }


    //Don't press for too long, its buggy
    if (IsKeyPressed(KEY_T)) {
        gComputerControlled = !gComputerControlled;
        gPlayer2Movement.y = 1.0f;
    }

    //Player 1 controls
    if (IsKeyDown(KEY_W) && gPlayer1Position.y > gPlayer1Scale.y / 2) {
        gPlayer1Movement.y = -1;
        gPlayer1Moving = true;
    } else if (IsKeyDown(KEY_S) && gPlayer1Position.y < SCREEN_HEIGHT - gPlayer1Scale.y / 2) {
        gPlayer1Movement.y = 1;
        gPlayer1Moving = true;
    } else {
        gPlayer1Moving = false;
    }


    //Player 2 controls
    if (!gComputerControlled) {
        if (IsKeyDown(KEY_I) && gPlayer2Position.y > gPlayer1Scale.y / 2) {
            gPlayer2Movement.y = -1;
            gPlayer2Moving = true;
        } else if (IsKeyDown(KEY_K) && gPlayer2Position.y < SCREEN_HEIGHT - gPlayer2Scale.y / 2) {
            gPlayer2Movement.y = 1;
            gPlayer2Moving = true;
        } else {
            gPlayer2Moving = false;
        }
    } else {
        if (isColliding(&gPlayer2Position, &gPlayer2Scale, &TOP_POSITION, &TOP_SCALE)
            || isColliding(&gPlayer2Position, &gPlayer2Scale, &BOT_POSITION, &BOT_SCALE)) {
            gPlayer2Movement.y *= -1.0;
        }
    }

    //Ball Collision logical handling
    isSoulColliding(&gSoul1Position, &gSoul1Scale, &gSoul1Movement);
    isSoulColliding(&gSoul2Position, &gSoul2Scale, &gSoul2Movement);
    isSoulColliding(&gSoul3Position, &gSoul3Scale, &gSoul3Movement);
    if (GetLength(&gSoul1Movement) > 1.0f) Normalise(&gSoul1Movement);
    if (GetLength(&gSoul2Movement) > 1.0f) Normalise(&gSoul2Movement);
    if (GetLength(&gSoul2Movement) > 1.0f) Normalise(&gSoul2Movement);


    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
    // Delta time
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    if (PlayerWon == 0) {
        gSoul1Angle = atan2f(gSoul1Movement.y, gSoul1Movement.x) * 180 / PI - 90;
        gSoul1Position = {
            gSoul1Position.x + gBallSpeed * gSoul1Movement.x * deltaTime,
            gSoul1Position.y + gBallSpeed * gSoul1Movement.y * deltaTime
        };
        if (gSoulNumber == 2 || gSoulNumber == 3) {
            gSoul2Angle = atan2f(gSoul2Movement.y, gSoul2Movement.x) * 180 / PI - 90;
            gSoul2Position = {
                gSoul2Position.x + gBallSpeed * gSoul2Movement.x * deltaTime,
                gSoul2Position.y + gBallSpeed * gSoul2Movement.y * deltaTime
            };
            if (gSoulNumber == 3) {
            }
        }

        if (gSoulNumber == 3) {
            gSoul3Angle = atan2f(gSoul3Movement.y, gSoul3Movement.x) * 180 / PI - 90;
            gSoul3Position = {
                gSoul3Position.x + gBallSpeed * gSoul3Movement.x * deltaTime,
                gSoul3Position.y + gBallSpeed * gSoul3Movement.y * deltaTime
            };
        }


        gPlayer1Position = {
            gPlayer1Position.x + PLAYER_SPEED * gPlayer1Movement.x * deltaTime,
            gPlayer1Position.y + PLAYER_SPEED * gPlayer1Movement.y * deltaTime
        };

        gPlayer2Position = {
            gPlayer2Position.x + PLAYER_SPEED * gPlayer2Movement.x * deltaTime,
            gPlayer2Position.y + PLAYER_SPEED * gPlayer2Movement.y * deltaTime
        };
    } else {
        gSoul1Movement = {0.0f, 0.0f};
        gSoul2Movement = {0.0f, 0.0f};
        gSoul3Movement = {0.0f, 0.0f};
    }
}

void render() {
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    renderObject(&gSoul1Texture, &gSoul1Position, &gSoul1Scale, gSoul1Angle, &gSoul1Color);

    if (gSoulNumber == 2 || gSoulNumber == 3) {
        renderObject(&gSoul2Texture, &gSoul2Position, &gSoul2Scale, gSoul2Angle, &gSoul2Color);
    }

    if (gSoulNumber == 3) {
        renderObject(&gSoul3Texture, &gSoul3Position, &gSoul3Scale, gSoul3Angle, &gSoul3Color);
    }


    renderObject(&gPlayer1Texture, &gPlayer1Position, &gPlayer1Scale, gPlayer1Angle, &gPlayer1Color);
    renderObject(&gPlayer2Texture, &gPlayer2Position, &gPlayer2Scale, gPlayer2Angle, &gPlayer2Color);


    if (PlayerWon == 1) {
        renderObject(&gPlayer1WinTextTexture, &gWinTextPosition, &gWinTextScale, gWinTextAngle, &gWinTextColor);
    } else if (PlayerWon == 2) {
        renderObject(&gPlayer2WinTextTexture, &gWinTextPosition, &gWinTextScale, gWinTextAngle, &gWinTextColor);
    }

    EndDrawing();
}

void shutdown() {
    CloseWindow();
    UnloadTexture(gSoul1Texture);
    UnloadTexture(gSoul2Texture);
    UnloadTexture(gSoul3Texture);
    UnloadTexture(gPlayer1Texture);
    UnloadTexture(gPlayer2Texture);
    UnloadTexture(gPlayer1WinTextTexture);
    UnloadTexture(gPlayer2WinTextTexture);
}

int main(void) {
    initialise();

    while (gAppStatus == RUNNING) {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}
