#include "movable_game_object_2D.h"

MovableGameObject2D::MovableGameObject2D(const std::shared_ptr<Texture>& texture,
                                         const glm::vec2&                posOfTopLeftCornerInPix,
                                         float                           angleOfRotInDeg,
                                         const glm::vec2&                dimensionsInPix,
                                         const glm::vec2&                velocity)
   : GameObject2D(texture,
                  posOfTopLeftCornerInPix,
                  angleOfRotInDeg,
                  dimensionsInPix)
   , mVelocity(velocity)
{

}

MovableGameObject2D::MovableGameObject2D(MovableGameObject2D&& rhs) noexcept
   : GameObject2D(std::move(rhs))
   , mVelocity(std::exchange(rhs.mVelocity, glm::vec2(0.0f, 0.0f)))
{

}

MovableGameObject2D& MovableGameObject2D::operator=(MovableGameObject2D&& rhs) noexcept
{
   GameObject2D::operator=(std::move(rhs));
   mVelocity = std::exchange(rhs.mVelocity, glm::vec2(0.0f, 0.0f));
}
