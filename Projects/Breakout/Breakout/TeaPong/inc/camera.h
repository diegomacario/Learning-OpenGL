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
          float     aspectRatio,
          float     near,
          float     far,
          float     movementSpeed,
          float     mouseSensitivity);
   ~Camera() = default;

   Camera(const Camera&) = default;
   Camera& operator=(const Camera&) = default;

   Camera(Camera&& rhs) noexcept;
   Camera& operator=(Camera&& rhs) noexcept;

   glm::vec3 getPosition();
   glm::mat4 getViewMatrix();
   glm::mat4 getPerspectiveProjectionMatrix();

   void      processKeyboardInput(MovementDirection direction, float deltaTime);
   void      processMouseMovement(float xOffset, float yOffset);
   void      processScrollWheelMovement(float yOffset);

private:

   void      updateCoordinateFrame();

   glm::vec3 mPosition;
   glm::vec3 mFront;
   glm::vec3 mUp;
   glm::vec3 mRight;
   glm::vec3 mWorldUp;

   float     mYawInDeg;
   float     mPitchInDeg;

   float     mFieldOfViewYInDeg;
   float     mAspectRatio;
   float     mNear;
   float     mFar;

   float     mMovementSpeed;
   float     mMouseSensitivity;
};

#endif
