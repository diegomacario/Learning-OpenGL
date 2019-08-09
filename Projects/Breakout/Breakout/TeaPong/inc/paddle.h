#ifndef PADDLE_H
#define PADDLE_H

#include "movable_game_object_3D.h"

class Paddle : public MovableGameObject3D
{
public:

   Paddle(const std::shared_ptr<Model>& model,
          const glm::vec3&              position,
          float                         angleOfRotInDeg,
          const glm::vec3&              axisOfRot,
          float                         scalingFactor,
          const glm::vec3&              velocity);
   ~Paddle() = default;

   Paddle(const Paddle&) = default;
   Paddle& operator=(const Paddle&) = default;

   Paddle(Paddle&& rhs) noexcept;
   Paddle& operator=(Paddle&& rhs) noexcept;

private:


};

#endif
