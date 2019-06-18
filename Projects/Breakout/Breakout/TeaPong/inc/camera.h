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

class Camera
{
public:

   Camera(glm::vec3 position,
          glm::vec3 worldUp,
          float     yaw,
          float     pitch);

   glm::vec3 getPosition();
   float     getZoom();
   glm::mat4 getViewMatrix();

   void processKeyboard(MovementDirection direction, float deltaTime);
   void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
   void processMouseScroll(float yOffset);

private:

   glm::vec3 mPosition;
   glm::vec3 mFront;
   glm::vec3 mUp;
   glm::vec3 mRight;
   glm::vec3 mWorldUp;

   float mYaw;
   float mPitch;

   float mMovementSpeed;
   float mMouseSensitivity;
   float mZoom;

   void updateCameraVectors();
};

#endif
