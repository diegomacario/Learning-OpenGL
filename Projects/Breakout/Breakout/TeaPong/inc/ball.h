#ifndef BALL_H
#define BALL_H

#include "movable_game_object_3D.h"

class Ball : public MovableGameObject3D
{
public:

   Ball(const std::shared_ptr<Model>& model,
        const glm::vec3&              position,
        float                         angleOfRotInDeg,
        const glm::vec3&              axisOfRot,
        float                         scalingFactor,
        const glm::vec3&              velocity,
        float                         radius,
        float                         spinAngularVelocity);
   ~Ball() = default;

   Ball(const Ball&) = default;
   Ball& operator=(const Ball&) = default;

   Ball(Ball&& rhs) noexcept;
   Ball& operator=(Ball&& rhs) noexcept;

   void move(float deltaTime, float tableWidth, float tableHeight);

private:

   float mRadius;
   float mSpinAngularVelocity;
   float mSpinAngularVelocityScaledByBounce;
};

#endif