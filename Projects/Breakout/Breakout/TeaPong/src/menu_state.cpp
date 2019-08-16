#include "constants.h"
#include "menu_state.h"

MenuState::MenuState(const std::shared_ptr<FiniteStateMachine>& finiteStateMachine,
                     const std::shared_ptr<Window>&             window,
                     const std::shared_ptr<Shader>&             gameObject3DShader,
                     const std::shared_ptr<GameObject3D>&       table,
                     const std::shared_ptr<Paddle>&             leftPaddle,
                     const std::shared_ptr<Paddle>&             rightPaddle,
                     const std::shared_ptr<Ball>&               ball)
   : mFSM(finiteStateMachine)
   , mWindow(window)
   , mGameObject3DShader(gameObject3DShader)
   , mTable(table)
   , mLeftPaddle(leftPaddle)
   , mRightPaddle(rightPaddle)
   , mBall(ball)
   , mCameraPosition(0.0f, -30.0f, 10.0f)
   , mCameraTarget(0.0f, 0.0f, 5.0f)
   , mCameraUp(0.0f, 0.0f, 1.0f)
   , mCameraRight(0.0f)
   , mCameraIdleOrbitalAngularVelocity(-15.0f)
   , mTransitionToPlayState(false)
   , mFirstIterationOfTransitionToPlayState(false)
   , mHorizontalSpeedOfRotation(0.0f)
   , mVerticalSpeedOfRotation(0.0f)
   , mSpeedOfMovementAwayFromTarget(0.0f)
   , mDoneRotatingHorizontally(0.0f)
   , mDoneRotatingVertically(0.0f)
   , mDoneMovingAwayFromTarget(0.0f)

{

}

void MenuState::enter()
{

}

void MenuState::execute(float deltaTime)
{
   processInput(deltaTime);
   update(deltaTime);
   render();
}

void MenuState::exit()
{

}

void MenuState::processInput(float deltaTime)
{
   // Close the game
   if (mWindow->keyIsPressed(GLFW_KEY_ESCAPE)) { mWindow->setShouldClose(true); }

   if (mWindow->keyIsPressed(GLFW_KEY_SPACE) && !mTransitionToPlayState)
   {
      mTransitionToPlayState = true;
      mFirstIterationOfTransitionToPlayState = true;
   }
}

void MenuState::update(float deltaTime)
{
   if (mTransitionToPlayState)
   {
      float cameraCCWAngularPosWRTNegYAxisInDeg = calculateCCWAngularPosWRTNegYAxisInDeg(mCameraPosition);
      float cameraAngularPosWRTPosZAxisInDeg    = calculateAngularPosWRTPosZAxisInDeg(mCameraPosition);

      if (mFirstIterationOfTransitionToPlayState)
      {
         mHorizontalSpeedOfRotation     = -(360.0f - cameraCCWAngularPosWRTNegYAxisInDeg) / 10.0f;
         mVerticalSpeedOfRotation       = cameraAngularPosWRTPosZAxisInDeg / 10.0f;
         mSpeedOfMovementAwayFromTarget = (90.0f - glm::length(mCameraPosition - mCameraTarget)) / 10.0f;

         mFirstIterationOfTransitionToPlayState = false;
      }

      // Rotate the camera horizontally
      if (!mDoneRotatingHorizontally)
      {
         glm::mat3 horizontalRotationMatrix = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(mHorizontalSpeedOfRotation * deltaTime), glm::vec3(0.0f, 0.0f, 1.0f)));
         mCameraPosition = horizontalRotationMatrix * mCameraPosition;
         mCameraUp       = horizontalRotationMatrix * mCameraUp;
      }

      updateCoordinateFrameOfCamera();

      // Rotate the camera vertically
      if (!mDoneRotatingVertically)
      {
         glm::mat3 verticalRotationMatrix = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(mVerticalSpeedOfRotation * deltaTime), mCameraRight));
         mCameraPosition = verticalRotationMatrix * mCameraPosition;
         mCameraUp       = verticalRotationMatrix * mCameraUp;
      }

      // Move the camera away from the target
      if (!mDoneMovingAwayFromTarget)
      {
         mCameraPosition += (mSpeedOfMovementAwayFromTarget * deltaTime) * glm::normalize(mCameraPosition - mCameraTarget);
      }

      updateCoordinateFrameOfCamera();

      if (mDoneRotatingHorizontally && mDoneRotatingVertically && mDoneMovingAwayFromTarget)
      {
         mFSM->changeState("play");
      }
   }
   else
   {
      // Rotate the camera CCW around the positive Z axis
      glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(mCameraIdleOrbitalAngularVelocity * deltaTime), glm::vec3(0.0f, 0.0f, 1.0f));
      mCameraPosition = glm::mat3(rotationMatrix) * mCameraPosition;
      std::cout << "cameraCCWAngularPosWRTNegYAxisInDeg = " << calculateCCWAngularPosWRTNegYAxisInDeg(mCameraPosition) << '\n' << '\n';
   }
}

void MenuState::render()
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Enable depth testing for 3D objects
   glEnable(GL_DEPTH_TEST);

   mGameObject3DShader->use();
   mGameObject3DShader->setMat4("view", glm::lookAt(mCameraPosition, mCameraTarget, mCameraUp));
   mGameObject3DShader->setVec3("cameraPos", mCameraPosition);

   mTable->render(*mGameObject3DShader);

   mLeftPaddle->render(*mGameObject3DShader);
   mRightPaddle->render(*mGameObject3DShader);

   glDisable(GL_CULL_FACE);
   mBall->render(*mGameObject3DShader);
   glEnable(GL_CULL_FACE);
}

float calculateCCWAngularPosWRTNegYAxisInDeg(const glm::vec3& point)
{
   float result = 0.0f;

   if ((point.x < 0.0f && point.y < 0.0f) || (point.x < 0.0f && point.y > 0.0f)) // Lower left quadrant or upper left quadrant
   {
      result = glm::degrees(glm::acos(glm::dot(glm::normalize(point), glm::vec3(0.0f, -1.0f, 0.0f))));
   }
   else if ((point.x > 0.0f && point.y > 0.0f) || (point.x > 0.0f && point.y < 0.0f)) // Upper right quadrant or lower right quadrant
   {
      result = 360.0f - glm::degrees(glm::acos(glm::dot(glm::normalize(point), glm::vec3(0.0f, -1.0f, 0.0f))));
   }
   else if (point.x == 0.0f && point.y > 0.0f) // Aligned with the positive Y axis
   {
      result = 180.0f;
   }
   else if (point.x == 0.0f && point.y < 0.0f) // Aligned with the negative Y axis
   {
      result = 0.0f;
   }
   else if (point.x > 0.0f && point.y == 0.0f) // Aligned with the positive X axis
   {
      result = 270.0f;
   }
   else if (point.x < 0.0f && point.y == 0.0f) // Aligned with the negative X axis
   {
      result = 90.0f;
   }

   return result;
}

float calculateAngularPosWRTPosZAxisInDeg(const glm::vec3& point)
{
   float result = 0.0f;

   if (point.x != 0.0f || point.y != 0.0f)
   {
      result = glm::degrees(glm::acos(glm::dot(glm::normalize(point), glm::vec3(0.0f, 0.0f, 1.0f))));
   }
   else if (point.z > 0.0f) // Aligned with the positive Z axis
   {
      result = 0.0f;
   }
   else if (point.z < 0.0f) // Aligned with the negative Z axis
   {
      result = 180.0f;
   }

   return result;
}

void MenuState::updateCoordinateFrameOfCamera()
{
   glm::vec3 vecFromCameraPositionToCameraTarget = glm::normalize(mCameraTarget - mCameraPosition);
   mCameraRight                                  = glm::normalize(glm::cross(mCameraUp, vecFromCameraPositionToCameraTarget));
   mCameraUp                                     = glm::normalize(glm::cross(vecFromCameraPositionToCameraTarget, mCameraRight));
}
