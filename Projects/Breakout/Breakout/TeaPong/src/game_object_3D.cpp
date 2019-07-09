#include <glm/gtc/matrix_transform.hpp>

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
   , mModelMat(1.0f)
{
   calculateModelMatrix();
}

GameObject3D::GameObject3D(GameObject3D&& rhs) noexcept
   : mModel(std::move(rhs.mModel))
   , mPosition(std::exchange(rhs.mPosition, glm::vec3()))
   , mAngleOfRotInDeg(std::exchange(rhs.mAngleOfRotInDeg, 0.0f))
   , mAxisOfRot(std::exchange(rhs.mAxisOfRot, glm::vec3()))
   , mScalingFactors(std::exchange(rhs.mScalingFactors, glm::vec3()))
   , mModelMat(std::exchange(rhs.mModelMat, glm::mat4(1.0f)))
{

}

GameObject3D& GameObject3D::operator=(GameObject3D&& rhs) noexcept
{
   mModel           = std::move(rhs.mModel);
   mPosition        = std::exchange(rhs.mPosition, glm::vec3());
   mAngleOfRotInDeg = std::exchange(rhs.mAngleOfRotInDeg, 0.0f);
   mAxisOfRot       = std::exchange(rhs.mAxisOfRot, glm::vec3());
   mScalingFactors  = std::exchange(rhs.mScalingFactors, glm::vec3());
   mModelMat        = std::exchange(rhs.mModelMat, glm::mat4(1.0f));
   return *this;
}

glm::mat4 GameObject3D::getModelMatrix() const
{
   // TODO: Come up with a system to only re-calculate the model matrix when necessary.
   return mModelMat;
}

void GameObject3D::calculateModelMatrix()
{
   // 3) Translate the model
   mModelMat = glm::translate(glm::mat4(1.0f), mPosition);

   // 2) Rotate the model
   mModelMat = glm::rotate(mModelMat, mAngleOfRotInDeg, mAxisOfRot);

   // 1) Scale the model
   mModelMat = glm::scale(mModelMat, mScalingFactors);
}
