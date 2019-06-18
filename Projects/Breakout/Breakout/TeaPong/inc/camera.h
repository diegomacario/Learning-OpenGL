#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
          float     pitch,
          float     fieldOfViewY,
          float     movementSpeed,
          float     mouseSensitivity);

   glm::vec3 getPosition();
   float     getFieldOfViewY();
   glm::mat4 getViewMatrix();

   void processKeyboard(MovementDirection direction, float deltaTime);
   void processMouseMovement(float xOffset, float yOffset);
   void processMouseScroll(float yOffset);

private:

   glm::vec3 mPosition;
   glm::vec3 mFront;
   glm::vec3 mUp;
   glm::vec3 mRight;
   glm::vec3 mWorldUp;

   float mYaw;
   float mPitch;

   float mFieldOfViewY;

   float mMovementSpeed;
   float mMouseSensitivity;

   void updateCameraVectors();
};

#endif
