#include <glm/gtc/matrix_transform.hpp>

#include "game_object_3D.h"

GameObject3D::GameObject3D(const std::shared_ptr<Model>& model,
                           const glm::vec3&              position,
                           float                         angleOfRotInDeg,
                           const glm::vec3&              axisOfRot,
                           float                         scalingFactor)
   : mModel(model)
   , mPosition(position)
   , mRotationMatrix(1.0f)
   , mScalingFactor(scalingFactor)
   , mModelMatrix(1.0f)
   , mCalculateModelMatrix(true)
{
   // TODO: This if statement prevents the model matrix from becoming a zero matrix. Is there a cleaner way to do this?
   if (axisOfRot != glm::vec3(0.0f, 0.0f, 0.0f))
   {
      mRotationMatrix = glm::rotate(mRotationMatrix, glm::radians(angleOfRotInDeg), axisOfRot);
   }

   calculateModelMatrix();
}

GameObject3D::GameObject3D(GameObject3D&& rhs) noexcept
   : mModel(std::move(rhs.mModel))
   , mPosition(std::exchange(rhs.mPosition, glm::vec3()))
   , mRotationMatrix(std::exchange(rhs.mRotationMatrix, glm::mat4(1.0f)))
   , mScalingFactor(std::exchange(rhs.mScalingFactor, 0.0f))
   , mModelMatrix(std::exchange(rhs.mModelMatrix, glm::mat4(1.0f)))
   , mCalculateModelMatrix(std::exchange(rhs.mCalculateModelMatrix, true))
{

}

GameObject3D& GameObject3D::operator=(GameObject3D&& rhs) noexcept
{
   mModel                = std::move(rhs.mModel);
   mPosition             = std::exchange(rhs.mPosition, glm::vec3());
   mRotationMatrix       = std::exchange(rhs.mRotationMatrix, glm::mat4(1.0f));
   mScalingFactor        = std::exchange(rhs.mScalingFactor, 0.0f);
   mModelMatrix          = std::exchange(rhs.mModelMatrix, glm::mat4(1.0f));
   mCalculateModelMatrix = std::exchange(rhs.mCalculateModelMatrix, true);
   return *this;
}

void GameObject3D::render(const Shader& shader) const
{
   mModel->render(shader);
}

glm::vec3 GameObject3D::getPosition() const
{
   return mPosition;
}

glm::mat4 GameObject3D::getModelMatrix() const
{
   if (mCalculateModelMatrix)
   {
      calculateModelMatrix();
   }

   return mModelMatrix;
}

void GameObject3D::translate(const glm::vec3& translation)
{
   mPosition += translation;
   mCalculateModelMatrix = true;
}

void GameObject3D::rotate(float angleOfRotInDeg, const glm::vec3& axisOfRot)
{
   // TODO: This if statement prevents the model matrix from becoming a zero matrix. Is there a cleaner way to do this?
   if (axisOfRot != glm::vec3(0.0f, 0.0f, 0.0f))
   {
      mRotationMatrix *= glm::rotate(mRotationMatrix, glm::radians(angleOfRotInDeg), axisOfRot);
   }

   mCalculateModelMatrix = true;
}

void GameObject3D::scale(float scalingFactor)
{
   mScalingFactor *= scalingFactor;
   mCalculateModelMatrix = true;
}

void GameObject3D::calculateModelMatrix() const
{
   // 3) Translate the model
   mModelMatrix = glm::translate(glm::mat4(1.0f), mPosition);

   // 2) Rotate the model
   mModelMatrix *= mRotationMatrix;

   // 1) Scale the model
   // TODO: This if statement prevents the model matrix from becoming a zero matrix. Is there a cleaner way to do this?
   if (mScalingFactor != 0.0f)
   {
      mModelMatrix = glm::scale(mModelMatrix, glm::vec3(mScalingFactor));
   }

   mCalculateModelMatrix = false;
}
