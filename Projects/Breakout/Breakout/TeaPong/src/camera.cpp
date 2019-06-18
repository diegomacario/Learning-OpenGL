#include "camera.h"

Camera::Camera(glm::vec3 position,
               glm::vec3 worldUp,
               float     yaw,
               float     pitch,
               float     fieldOfViewY,
               float     movementSpeed,
               float     mouseSensitivity)
   : mPosition(position)
   , mFront()
   , mUp()
   , mRight()
   , mWorldUp(worldUp)
   , mYaw(yaw)
   , mPitch(pitch)
   , mFieldOfViewY(fieldOfViewY)
   , mMovementSpeed(movementSpeed)
   , mMouseSensitivity(mouseSensitivity)
{
   updateCameraVectors();
}

glm::vec3 Camera::getPosition()
{
   return mPosition;
}

float Camera::getFieldOfViewY()
{
   return mFieldOfViewY;
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

void Camera::processMouseMovement(float xOffset, float yOffset)
{
   xOffset *= mMouseSensitivity;
   yOffset *= mMouseSensitivity;

   mYaw   += xOffset;
   mPitch += yOffset;

   // Make sure that when the pitch is out of bounds, the screen doesn't get flipped
   if (mPitch > 89.0f)
   {
      mPitch = 89.0f;
   }
   else if (mPitch < -89.0f)
   {
      mPitch = -89.0f;
   }

   // Update the front, right and up vectors using the updated Euler angles
   updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset)
{
   // The larger the FOV, the smaller things appear on the screen
   // The smaller the FOV, the larger things appear on the screen
   if (mFieldOfViewY >= 1.0f && mFieldOfViewY <= 45.0f)
   {
      mFieldOfViewY -= yOffset;
   }
   else if (mFieldOfViewY < 1.0f)
   {
      mFieldOfViewY = 1.0f;
   }
   else if (mFieldOfViewY > 45.0f)
   {
      mFieldOfViewY = 45.0f;
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
