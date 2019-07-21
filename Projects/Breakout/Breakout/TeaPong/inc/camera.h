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
          float     yawInDeg,
          float     pitchInDeg,
          float     fieldOfViewYInDeg,
          float     movementSpeed,
          float     mouseSensitivity);
   ~Camera() = default;

   Camera(const Camera&) = default;
   Camera& operator=(const Camera&) = default;

   Camera(Camera&& rhs) noexcept;
   Camera& operator=(Camera&& rhs) noexcept;

   glm::vec3 getPosition();
   float     getFieldOfViewYInDeg();
   glm::mat4 getViewMatrix();

   void      processKeyboardInput(MovementDirection direction, float deltaTime);
   void      processMouseMovement(float xOffset, float yOffset);
   void      processScrollWheelMovement(float yOffset);

private:

   void      updateCameraVectors(); // TODO: Change the name of this function to something more appropriate like updateCoordinateSystem().

   glm::vec3 mPosition;
   glm::vec3 mFront;
   glm::vec3 mUp;
   glm::vec3 mRight;
   glm::vec3 mWorldUp;

   float     mYawInDeg;
   float     mPitchInDeg;

   float     mFieldOfViewYInDeg;

   float     mMovementSpeed;
   float     mMouseSensitivity;
};

#endif
