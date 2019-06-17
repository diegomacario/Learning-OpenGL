#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum class MovementDirection
{
   Forward,
   Backward,
   Left,
   Right
};

// Default camera values
const float YAW         =  0.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera
{
public:

   Camera(glm::vec3 position,
          glm::vec3 worldUp,
          float     yaw,
          float     pitch);

   glm::mat4 GetViewMatrix();

   void ProcessKeyboard(MovementDirection direction, float deltaTime);
   void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
   void ProcessMouseScroll(float yOffset);

private:

   glm::vec3 position;
   glm::vec3 front;
   glm::vec3 up;
   glm::vec3 right;
   glm::vec3 worldUp;

   float pitch;
   float yaw;

   float movementSpeed;
   float mouseSensitivity;
   float zoom;

   // Calculates the front vector using the camera's (updated) Euler angles
   void updateCameraVectors();
};

#endif
