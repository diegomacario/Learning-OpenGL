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
   , mModelMatrix(1.0f)
{
   calculateModelMatrix();
}

GameObject3D::GameObject3D(GameObject3D&& rhs) noexcept
   : mModel(std::move(rhs.mModel))
   , mPosition(std::exchange(rhs.mPosition, glm::vec3()))
   , mAngleOfRotInDeg(std::exchange(rhs.mAngleOfRotInDeg, 0.0f))
   , mAxisOfRot(std::exchange(rhs.mAxisOfRot, glm::vec3()))
   , mScalingFactors(std::exchange(rhs.mScalingFactors, glm::vec3()))
   , mModelMatrix(std::exchange(rhs.mModelMatrix, glm::mat4(1.0f)))
{

}

GameObject3D& GameObject3D::operator=(GameObject3D&& rhs) noexcept
{
   mModel           = std::move(rhs.mModel);
   mPosition        = std::exchange(rhs.mPosition, glm::vec3());
   mAngleOfRotInDeg = std::exchange(rhs.mAngleOfRotInDeg, 0.0f);
   mAxisOfRot       = std::exchange(rhs.mAxisOfRot, glm::vec3());
   mScalingFactors  = std::exchange(rhs.mScalingFactors, glm::vec3());
   mModelMatrix     = std::exchange(rhs.mModelMatrix, glm::mat4(1.0f));
   return *this;
}

void GameObject3D::render(const Shader& shader) const
{
   mModel->render(shader);
}

glm::mat4 GameObject3D::getModelMatrix() const
{
   // TODO: Come up with a system to only recalculate the model matrix when necessary.
   return mModelMatrix;
}

void GameObject3D::calculateModelMatrix()
{
   // 3) Translate the model
   mModelMatrix = glm::translate(glm::mat4(1.0f), mPosition);

   // 2) Rotate the model
   // TODO: This if statement prevents the model matrix from becoming a zero matrix. Is there a cleaner way to do this?
   if (mAxisOfRot != glm::vec3(0.0f, 0.0f, 0.0f))
   {
      mModelMatrix = glm::rotate(mModelMatrix, glm::radians(mAngleOfRotInDeg), mAxisOfRot);
   }

   // 1) Scale the model
   // TODO: This if statement prevents the model matrix from becoming a zero matrix. Is there a cleaner way to do this?
   if (mScalingFactors != glm::vec3(0.0f, 0.0f, 0.0f))
   {
      mModelMatrix = glm::scale(mModelMatrix, mScalingFactors);
   }
}
