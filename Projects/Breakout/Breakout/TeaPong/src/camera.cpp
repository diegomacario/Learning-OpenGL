#include "camera.h"

Camera::Camera(glm::vec3 position,
               glm::vec3 worldUp,
               float     yaw,
               float     pitch)
   : mPosition(position)
   , mFront()
   , mUp()
   , mRight()
   , mWorldUp(worldUp)
   , mYaw(yaw)
   , mPitch(pitch)
   , mMovementSpeed(2.5f)
   , mMouseSensitivity(0.1f)
   , mZoom(45.0f)
{
   updateCameraVectors();
}

glm::vec3 Camera::getPosition()
{
   return mPosition;
}

float Camera::getZoom()
{
   return mZoom;
}

glm::mat4 Camera::getViewMatrix()
{
   return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

void Camera::processKeyboard(MovementDirection direction, float deltaTime)
{
   float velocity = mMovementSpeed * deltaTime;

   switch (direction)
   {
   case MovementDirection::Forward:
      mPosition += mFront * velocity;
      break;
   case MovementDirection::Backward:
      mPosition -= mFront * velocity;
      break;
   case MovementDirection::Left:
      mPosition -= mRight * velocity;
      break;
   case MovementDirection::Right:
      mPosition += mRight * velocity;
      break;
   }
}

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
   xOffset *= mMouseSensitivity;
   yOffset *= mMouseSensitivity;

   mYaw   += xOffset;
   mPitch += yOffset;

   // Make sure that when the pitch is out of bounds, the screen doesn't get flipped
   if (constrainPitch)
   {
      if (mPitch > 89.0f)
      {
         mPitch = 89.0f;
      }
      else if (mPitch < -89.0f)
      {
         mPitch = -89.0f;
      }
   }

   // Update the front, right and up vectors using the updated Euler angles
   updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset)
{
   if (mZoom >= 1.0f && mZoom <= 45.0f)
   {
      mZoom -= yOffset;
   }
   else if (mZoom < 1.0f)
   {
      mZoom = 1.0f;
   }
   else if (mZoom > 45.0f)
   {
      mZoom = 45.0f;
   }
}

void Camera::updateCameraVectors()
{
   // Calculate the new front vector
   glm::vec3 newFront;
   newFront.x = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
   newFront.y = sin(glm::radians(mPitch));
   newFront.z = -1.0f * cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
   mFront = glm::normalize(newFront);

   // Calculate the new right and up vectors
   mRight = glm::normalize(glm::cross(mFront, mWorldUp));
   mUp    = glm::normalize(glm::cross(mRight, mFront));
}
