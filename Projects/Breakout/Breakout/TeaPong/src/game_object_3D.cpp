#include "game_object_3D.h"

GameObject3D::GameObject3D(const std::shared_ptr<Model>& model,
                           const glm::vec3&              position,
                           float                         angleOfRotInDeg,
                           const glm::vec3&              axisOfRot,
                           const glm::vec3&              scalingFactors)
   : mModel(model)
   , mPosition(position)
   , mAngleOfRotInDeg(angleOfRotInDeg)
   , mAxisOfRot(axisOfRot)
   , mScalingFactors(scalingFactors)
{

}

GameObject3D::GameObject3D(GameObject3D&& rhs) noexcept
   : mModel(std::move(rhs.mModel))
   , mPosition(std::exchange(rhs.mPosition, glm::vec3()))
   , mAngleOfRotInDeg(std::exchange(rhs.mAngleOfRotInDeg, 0.0f))
   , mAxisOfRot(std::exchange(rhs.mAxisOfRot, glm::vec3()))
   , mScalingFactors(std::exchange(rhs.mScalingFactors, glm::vec3()))
{

}

GameObject3D& GameObject3D::operator=(GameObject3D&& rhs) noexcept
{
   mModel           = std::move(rhs.mModel);
   mPosition        = std::exchange(rhs.mPosition, glm::vec3());
   mAngleOfRotInDeg = std::exchange(rhs.mAngleOfRotInDeg, 0.0f);
   mAxisOfRot       = std::exchange(rhs.mAxisOfRot, glm::vec3());
   mScalingFactors  = std::exchange(rhs.mScalingFactors, glm::vec3());
   return *this;
}
