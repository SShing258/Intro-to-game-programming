#include "Effects.h"

Effects::Effects(Vector2 origin, float windowWidth, float windowHeight) : mAlpha{SOLID}, 
                     mEffectSpeed{DEFAULT_SPEED},
                     mOverlayWidth{windowWidth}, 
                     mOverlayHeight{windowHeight}, mCurrentEffect{NONE},
                     mViewOffset{{}}, mOrigin{origin}, 
                     mMaxWindowDimensions{windowWidth, windowHeight}
{
}

void Effects::drawOverlay()
{
    float left = mViewOffset.x - mOverlayWidth  / 2.0f;
    float top  = mOrigin.y - mOverlayHeight / 2.0f;
    DrawRectangle(left,
                  top,
                  mOverlayWidth,
                  mOverlayHeight,
                  Fade(BLACK, mAlpha));
}

void Effects::update(float deltaTime, Vector2 *viewOffset)
{
    if (viewOffset != nullptr) mViewOffset = *viewOffset;

    float diagonalRatio = mMaxWindowDimensions.y / mMaxWindowDimensions.x;

    switch (mCurrentEffect)
    {
        case FADEIN:
            mAlpha -= mEffectSpeed * deltaTime;

            if (mAlpha <= TRANSPARENT)
            {
                mAlpha = TRANSPARENT;
                mCurrentEffect = NONE;
            }

            break;
        
        case FADEOUT:
            mAlpha += mEffectSpeed * deltaTime;

            if (mAlpha >= SOLID)
            {
                mAlpha = SOLID;
                mCurrentEffect = NONE;
            }

            break;

        case SHRINK:
            mOverlayHeight -= mEffectSpeed * SIZE_SPEED_MULTIPLIER * deltaTime * diagonalRatio;
            mOverlayWidth  -= mEffectSpeed * SIZE_SPEED_MULTIPLIER * deltaTime;

            if (mOverlayHeight <= 0.0f ||
                mOverlayWidth  <= 0.0f)
            {
                mOverlayHeight = 0.0f;
                mOverlayWidth  = 0.0f;
                mCurrentEffect = NONE;
            }
            break;

        case GROW:
            mOverlayHeight += mEffectSpeed * SIZE_SPEED_MULTIPLIER * deltaTime * diagonalRatio;
            mOverlayWidth  += mEffectSpeed * SIZE_SPEED_MULTIPLIER * deltaTime;

            if (mOverlayHeight >= mMaxWindowDimensions.y ||
                mOverlayWidth  >= mMaxWindowDimensions.x)
            {
                mOverlayHeight = mMaxWindowDimensions.y;
                mOverlayWidth  = mMaxWindowDimensions.x;
                mCurrentEffect = NONE;
            }
            break;

        case NONE:
        default:
            break;
    }
}

void Effects::start(EffectType effectType)
{
    mCurrentEffect = effectType;

    switch (mCurrentEffect)
    {
        case FADEIN:
            mAlpha = SOLID;
            break;

        case FADEOUT:
            mAlpha = TRANSPARENT;
            break;

        case SHRINK:
            mOverlayHeight = mMaxWindowDimensions.y;
            mOverlayWidth  = mMaxWindowDimensions.x;
            break;

        case GROW:
            mOverlayHeight = 0.0f;
            mOverlayWidth  = 0.0f;
            break;

        case NONE:
        default:
            break;
    }
}

void Effects::startFadeIn(float duration) {
    fadingIn = true;
    fadeDuration = duration;
    fadeTimer = 0.0f;
}

void Effects::updateFade(float deltaTime) {
    if (!fadingIn) return;

    fadeTimer += deltaTime;

    if (fadeTimer >= fadeDuration) {
        fadeTimer = fadeDuration;
        fadingIn = false;
    }
}

void Effects::drawFade() const {
    if (!fadingIn) return;

    float alpha = 1.0f - (fadeTimer / fadeDuration);
    if (alpha < 0.0f) alpha = 0.0f;
    if (alpha > 1.0f) alpha = 1.0f;

    const Color fadeColor = { 0, 0, 0, (unsigned char)(alpha * 255) };
    DrawRectangle(0, 0, 960, 1280, fadeColor);
}


void Effects::render()
{
    switch (mCurrentEffect)
    {
        case FADEIN:
        case FADEOUT:
        case SHRINK:
        case GROW:
            drawOverlay();
            break;

        case NONE:
    
    default:
        break;
    }
}