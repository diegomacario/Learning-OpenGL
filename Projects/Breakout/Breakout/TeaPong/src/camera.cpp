#include "camera.h"

Camera::Camera(glm::vec3 position,
               glm::vec3 worldUp,
               float     yaw,
               float     pitch)
   : front(glm::vec3(0.0f, 0.0f, -1.0f))
   , movementSpeed(SPEED)
   , mouseSensitivity(SENSITIVITY)
   , zoom(ZOOM)
{
   position = position;
   worldUp  = worldUp;
   yaw      = yaw;
   pitch    = pitch;

   updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
   return glm::lookAt(position, position + front, up);
}

void Camera::ProcessKeyboard(MovementDirection direction, float deltaTime)
{
   float velocity = movementSpeed * deltaTime;
   if (direction == MovementDirection::Forward)
      position += front * velocity;
   if (direction == MovementDirection::Backward)
      position -= front * velocity;
   if (direction == MovementDirection::Left)
      position -= right * velocity;
   if (direction == MovementDirection::Right)
      position += right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
{
   xoffset *= mouseSensitivity;
   yoffset *= mouseSensitivity;

   yaw   += xoffset;
   pitch += yoffset;

   // Make sure that when pitch is out of bounds, screen doesn't get flipped
   if (constrainPitch)
   {
      if (pitch > 89.0f)
         pitch = 89.0f;
      if (pitch < -89.0f)
         pitch = -89.0f;
   }

   // Update Front, Right and Up Vectors using the updated Euler angles
   updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
   if (zoom >= 1.0f && zoom <= 45.0f)
      zoom -= yoffset;
   if (zoom <= 1.0f)
      zoom = 1.0f;
   if (zoom >= 45.0f)
      zoom = 45.0f;
}

// Calculates the front vector using the camera's (updated) Euler angles
void Camera::updateCameraVectors()
{
   // Calculate the new Front vector
   glm::vec3 newFront;

   newFront.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
   newFront.y = sin(glm::radians(pitch));
   newFront.z = -1.0f * cos(glm::radians(yaw)) * cos(glm::radians(pitch));

   front   = glm::normalize(newFront);

   // Also re-calculate the Right and Up vector
   // Normalize the vectors, because their length gets closer to 0 the more you look up or down,
   // which results in slower movement.
   right = glm::normalize(glm::cross(front, worldUp));
   up    = glm::normalize(glm::cross(right, front));
}
