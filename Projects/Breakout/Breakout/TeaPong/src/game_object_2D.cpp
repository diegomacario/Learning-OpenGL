#include "game_object_2D.h"

GameObject2D::GameObject2D(const std::shared_ptr<Texture>& texture,
                           const glm::vec2&                posOfTopLeftCornerInPix,
                           float                           angleOfRotInDeg,
                           float                           widthInPix,
                           float                           heightInPix)
   : mTexture(texture)
   , mPosOfTopLeftCornerInPix(posOfTopLeftCornerInPix)
   , mAngleOfRotInDeg(angleOfRotInDeg)
   , mWidthInPix(widthInPix)
   , mHeightInPix(heightInPix)
{

}

GameObject2D::GameObject2D(GameObject2D&& rhs) noexcept
   : mTexture(std::move(rhs.mTexture))
   , mPosOfTopLeftCornerInPix(std::exchange(rhs.mPosOfTopLeftCornerInPix, glm::vec2()))
   , mAngleOfRotInDeg(std::exchange(rhs.mAngleOfRotInDeg, 0.0f))
   , mWidthInPix(std::exchange(rhs.mWidthInPix, 0.0f))
   , mHeightInPix(std::exchange(rhs.mHeightInPix, 0.0f))
{

}

GameObject2D& GameObject2D::operator=(GameObject2D&& rhs) noexcept
{
   mTexture                 = std::move(rhs.mTexture);
   mPosOfTopLeftCornerInPix = std::exchange(rhs.mPosOfTopLeftCornerInPix, glm::vec2());
   mAngleOfRotInDeg         = std::exchange(rhs.mAngleOfRotInDeg, 0.0f);
   mWidthInPix              = std::exchange(rhs.mWidthInPix, 0.0f);
   mHeightInPix             = std::exchange(rhs.mHeightInPix, 0.0f);
   return *this;
}

std::shared_ptr<Texture> GameObject2D::getTexture() const
{
   return mTexture;
}

glm::vec2 GameObject2D::getPosOfTopLeftCornerInPix() const
{
   return mPosOfTopLeftCornerInPix;
}

float GameObject2D::getAngleOfRotInDeg() const
{
   return mAngleOfRotInDeg;
}

float GameObject2D::getWidthInPix() const
{
   return mWidthInPix;
}

float GameObject2D::getHeightInPix() const
{
   return mWidthInPix;
}
