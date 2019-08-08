#include "ball.h"

Ball::Ball(const std::shared_ptr<Model>& model,
           const glm::vec3&              position,
           float                         angleOfRotInDeg,
           const glm::vec3&              axisOfRot,
           float                         scalingFactor,
           const glm::vec3&              velocity,
           float                         radius,
           float                         spinAngularVelocity)
   : MovableGameObject3D(model,
                         position,
                         angleOfRotInDeg,
                         axisOfRot,
                         scalingFactor,
                         velocity)
   , mRadius(radius)
   , mSpinAngularVelocity(spinAngularVelocity)
   , mSpinAngularVelocityScaledByBounce(spinAngularVelocity)
{

}

Ball::Ball(Ball&& rhs) noexcept
   : MovableGameObject3D(std::move(rhs))
   , mRadius(std::exchange(rhs.mRadius, 0.0f))
   , mSpinAngularVelocity(std::exchange(rhs.mSpinAngularVelocity, 0.0f))
   , mSpinAngularVelocityScaledByBounce(std::exchange(rhs.mSpinAngularVelocityScaledByBounce, 0.0f))
{

}

Ball& Ball::operator=(Ball&& rhs) noexcept
{
   GameObject3D::operator=(std::move(rhs));
   mRadius = std::exchange(rhs.mRadius, 0.0f);
   mSpinAngularVelocity = std::exchange(rhs.mSpinAngularVelocity, 0.0f);
   mSpinAngularVelocityScaledByBounce = std::exchange(rhs.mSpinAngularVelocityScaledByBounce, 0.0f);
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

   float reflectionDotProduct = 0.0f;
   bool  bounced = false;

   if ((currPos.x + mRadius) >= rightBoundary)       // Bounce left
   {
      // Dot product of the normalized velocity and the normalized normal of the boundary
      // This value ranges from 0 to 1
      // It is equal to 0 when the two vectors are perpendicular (tangent collision)
      // It is equal to 1 when the two vectors are parallel (direct collision)
      reflectionDotProduct = std::abs(glm::dot(glm::normalize(currVelocity), glm::vec3(-1.0f, 0.0f, 0.0f)));

      currVelocity.x       = -currVelocity.x;
      currPos.x            = rightBoundary - mRadius;
      bounced              = true;
   }
   else if ((currPos.x - mRadius) <= leftBoundary)   // Bounce right
   {
      reflectionDotProduct = std::abs(glm::dot(glm::normalize(currVelocity), glm::vec3(1.0f, 0.0f, 0.0f)));

      currVelocity.x       = -currVelocity.x;
      currPos.x            = leftBoundary + mRadius;
      bounced              = true;
   }

   if ((currPos.y + mRadius) >= topBoundary)         // Bounce down
   {
      reflectionDotProduct = std::abs(glm::dot(glm::normalize(currVelocity), glm::vec3(0.0f, -1.0f, 0.0f)));

      currVelocity.y       = -currVelocity.y;
      currPos.y            = topBoundary - mRadius;
      bounced              = true;
   }
   else if ((currPos.y - mRadius) <= bottomBoundary) // Bounce up
   {
      reflectionDotProduct = std::abs(glm::dot(glm::normalize(currVelocity), glm::vec3(0.0f, 1.0f, 0.0f)));

      currVelocity.y       = -currVelocity.y;
      currPos.y            = bottomBoundary + mRadius;
      bounced              = true;
   }

   if (bounced)
   {
      // The ball spins with its maximum angular velocity when a tangent collision occurs
      // The ball spins with its minimum angular velocity, that is, it doesn't spin, when a direct collision occurs
      mSpinAngularVelocityScaledByBounce = mSpinAngularVelocity * (1.0f - reflectionDotProduct);
   }

   this->setVelocity(currVelocity);
   this->setPosition(currPos);
   this->rotate(mSpinAngularVelocityScaledByBounce * deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
}
