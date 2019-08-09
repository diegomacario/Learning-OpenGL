#include "paddle.h"

Paddle::Paddle(const std::shared_ptr<Model>& model,
           const glm::vec3&              position,
           float                         angleOfRotInDeg,
           const glm::vec3&              axisOfRot,
           float                         scalingFactor,
           const glm::vec3&              velocity,
           float                         length)
   : MovableGameObject3D(model,
                         position,
                         angleOfRotInDeg,
                         axisOfRot,
                         scalingFactor,
                         velocity)
   , mLength(length)
{

}

Paddle::Paddle(Paddle&& rhs) noexcept
   : MovableGameObject3D(std::move(rhs))
   , mLength(std::exchange(rhs.mLength, 0.0f))
{

}

Paddle& Paddle::operator=(Paddle&& rhs) noexcept
{
   GameObject3D::operator=(std::move(rhs));
   mLength = std::exchange(rhs.mLength, 0.0f);
   return *this;
}

void Paddle::moveWithinLine(float deltaTime, float lineLength)
{

}

float Paddle::getLength() const
{
   return mLength;
}
