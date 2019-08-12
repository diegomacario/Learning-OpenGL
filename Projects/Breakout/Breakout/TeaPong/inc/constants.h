#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <glm/glm.hpp>

namespace Constants
{
   //const glm::vec3 kInitialBallVelocity = glm::vec3(27.5f, 37.5f, 0.0f);
   const glm::vec3 kInitialBallVelocity = glm::vec3(17.5f, 27.5f, 0.0f);

   namespace PlayStateConstants
   {
      const glm::vec3 kInitialCameraPosition = glm::vec3(0.0f, 0.0f, 95.0f);
      const glm::vec3 kInitialCameraWorldUp  = glm::vec3(0.0f, 0.0f, 1.0f);
      const float     kInitialCameraYaw      = 0.0f;
      const float     kInitialCameraPitch    = 0.0f;

      const float     kTeapotRadius = 2.5f;
   }

   namespace MenuStateConstants
   {
      const glm::vec3 kInitialCameraPosition = glm::vec3(0.0f, -10.0f, 3.0f);
      const glm::vec3 kInitialCameraWorldUp  = glm::vec3(0.0f, 0.0f, 1.0f);
      const float     kInitialCameraYaw      = 0.0f;
      const float     kInitialCameraPitch    = 90.0f;

      const float     kTeapotRadius = 7.5f;
   }
}

#endif
