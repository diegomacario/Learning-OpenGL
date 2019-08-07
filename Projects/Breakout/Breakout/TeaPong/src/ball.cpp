#include "ball.h"

Ball::Ball(const std::shared_ptr<Model>& model,
           const glm::vec3&              position,
           float                         angleOfRotInDeg,
           const glm::vec3&              axisOfRot,
           float                         scalingFactor,
           const glm::vec3&              velocity,
           float                         radius,
           float                         spinAngularVelocityInDeg)
   : MovableGameObject3D(model,
                         position,
                         angleOfRotInDeg,
                         axisOfRot,
                         scalingFactor,
                         velocity)
   , mRadius(radius)
   , mSpinAngularVelocityInDeg(spinAngularVelocityInDeg)
{

}

Ball::Ball(Ball&& rhs) noexcept
   : MovableGameObject3D(std::move(rhs))
   , mRadius(std::exchange(rhs.mRadius, 0.0f))
   , mSpinAngularVelocityInDeg(std::exchange(rhs.mSpinAngularVelocityInDeg, 0.0f))
{

}

Ball& Ball::operator=(Ball&& rhs) noexcept
{
   GameObject3D::operator=(std::move(rhs));
   mRadius = std::exchange(rhs.mRadius, 0.0f);
   mSpinAngularVelocityInDeg = std::exchange(rhs.mSpinAngularVelocityInDeg, 0.0f);
   return *this;
}

void Ball::move(float deltaTime, float tableWidth, float tableHeight)
{
   glm::vec3 oldPos       = this->getPosition();
   glm::vec3 currVelocity = this->getVelocity();
   glm::vec3 currPos      = oldPos + (currVelocity * deltaTime);

   float rightBoundary  =   tableWidth  / 2.0f;
   float leftBoundary   =  -tableWidth  / 2.0f;
   float topBoundary    =   tableHeight / 2.0f;
   float bottomBoundary =  -tableHeight / 2.0f;

   bool bounced = false;

   if ((currPos.x + mRadius) >= rightBoundary)       // Bounce left
   {
      currVelocity.x = -currVelocity.x;
      currPos.x      = rightBoundary - mRadius;
      bounced        = true;
   }
   else if ((currPos.x - mRadius) <= leftBoundary)   // Bounce right
   {
      currVelocity.x = -currVelocity.x;
      currPos.x      = leftBoundary + mRadius;
      bounced        = true;
   }

   if ((currPos.y + mRadius) >= topBoundary)         // Bounce down
   {
      currVelocity.y = -currVelocity.y;
      currPos.y      = topBoundary - mRadius;
      bounced        = true;
   }
   else if ((currPos.y - mRadius) <= bottomBoundary) // Bounce up
   {
      currVelocity.y = -currVelocity.y;
      currPos.y      = bottomBoundary + mRadius;
      bounced        = true;
   }

   if (bounced)
   {
      mSpinAngularVelocityInDeg = -mSpinAngularVelocityInDeg;
   }

   this->setVelocity(currVelocity);
   this->setPosition(currPos);
   this->rotate(mSpinAngularVelocityInDeg * deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
}
