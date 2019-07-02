#include "game_object_2D.h"

GameObject2D::GameObject2D(const std::shared_ptr<Texture>& texture,
                           const glm::vec2&                posOfTopLeftCornerInPix,
                           float                           angleOfRotInDeg,
                           const glm::vec2&                dimensionsInPix)
   : mTexture(texture)
   , mPosOfTopLeftCornerInPix(posOfTopLeftCornerInPix)
   , mAngleOfRotInDeg(angleOfRotInDeg)
   , mDimensionsInPix(dimensionsInPix)
{

}

GameObject2D::GameObject2D(GameObject2D&& rhs) noexcept
   : mTexture(std::move(rhs.mTexture))
   , mPosOfTopLeftCornerInPix(std::exchange(rhs.mPosOfTopLeftCornerInPix, glm::vec2(0.0f, 0.0f)))
   , mAngleOfRotInDeg(std::exchange(rhs.mAngleOfRotInDeg, 0.0f))
   , mDimensionsInPix(std::exchange(rhs.mDimensionsInPix, glm::vec2(0.0f, 0.0f)))
{

}

GameObject2D& GameObject2D::operator=(GameObject2D&& rhs) noexcept
{
   mTexture                 = std::move(rhs.mTexture);
   mPosOfTopLeftCornerInPix = std::exchange(rhs.mPosOfTopLeftCornerInPix, glm::vec2(0.0f, 0.0f));
   mAngleOfRotInDeg         = std::exchange(rhs.mAngleOfRotInDeg, 0.0f);
   mDimensionsInPix         = std::exchange(rhs.mDimensionsInPix, glm::vec2(0.0f, 0.0f));
   return *this;
}
