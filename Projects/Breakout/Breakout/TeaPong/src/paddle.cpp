#include "paddle.h"

Paddle::Paddle(const std::shared_ptr<Model>& model,
           const glm::vec3&              position,
           float                         angleOfRotInDeg,
           const glm::vec3&              axisOfRot,
           float                         scalingFactor,
           const glm::vec3&              velocity)
   : MovableGameObject3D(model,
                         position,
                         angleOfRotInDeg,
                         axisOfRot,
                         scalingFactor,
                         velocity)
{

}

Paddle::Paddle(Paddle&& rhs) noexcept
   : MovableGameObject3D(std::move(rhs))
{

}

Paddle& Paddle::operator=(Paddle&& rhs) noexcept
{
   GameObject3D::operator=(std::move(rhs));
   return *this;
}
