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

         glm::vec3 futureCameraPos                           = horizontalRotationMatrix * mCameraPosition;
         float     futureCameraCCWAngularPosWRTNegYAxisInDeg = calculateCCWAngularPosWRTNegYAxisInDeg(futureCameraPos);
         // ---
         //if (futureCameraCCWAngularPosWRTNegYAxisInDeg < cameraCCWAngularPosWRTNegYAxisInDeg)
         //{
         //   calculateCCWAngularPosWRTNegYAxisInDeg(futureCameraPos);
         //}
         // ---
         if (futureCameraCCWAngularPosWRTNegYAxisInDeg < cameraCCWAngularPosWRTNegYAxisInDeg)
         {
            std::cout << "mDoneRotatingHorizontally" << '\n';
            std::cout << "cameraCCWAngularPosWRTNegYAxisInDeg          = " << cameraCCWAngularPosWRTNegYAxisInDeg << '\n';
            std::cout << "futureCameraCCWAngularPosWRTNegYAxisInDeg    = " << futureCameraCCWAngularPosWRTNegYAxisInDeg << '\n' << '\n';
            // If we rotate by what is dictated by (mHorizontalSpeedOfRotation * deltaTime), we would overshoot the desired destination (e.g. we might be rotating from 359 to 1 instead of from 359 to 0)
            // So we modify the rotation matrix so that it only makes us rotate the exact number of degrees left to reach 0
            horizontalRotationMatrix  = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-(360.0f - cameraCCWAngularPosWRTNegYAxisInDeg)), glm::vec3(0.0f, 0.0f, 1.0f)));
            mDoneRotatingHorizontally = true;
         }

         mCameraPosition = horizontalRotationMatrix * mCameraPosition;
         mCameraUp       = horizontalRotationMatrix * mCameraUp;
      }

      updateCoordinateFrameOfCamera();

      // Rotate the camera vertically
      if (!mDoneRotatingVertically)
      {
         glm::mat3 verticalRotationMatrix = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(mVerticalSpeedOfRotation * deltaTime), mCameraRight));

         cameraAngularPosWRTPosZAxisInDeg                 = calculateAngularPosWRTPosZAxisInDeg(mCameraPosition);
         glm::vec3 futureCameraPos                        = verticalRotationMatrix * mCameraPosition;
         float     futureCameraAngularPosWRTPosZAxisInDeg = calculateAngularPosWRTPosZAxisInDeg(futureCameraPos);
         if (futureCameraAngularPosWRTPosZAxisInDeg > cameraAngularPosWRTPosZAxisInDeg)
         {
            std::cout << "mDoneRotatingVertically" << '\n';
            std::cout << "cameraAngularPosWRTPosZAxisInDeg             = " << cameraAngularPosWRTPosZAxisInDeg << '\n';
            std::cout << "futureCameraAngularPosWRTPosZAxisInDeg       = " << futureCameraAngularPosWRTPosZAxisInDeg << '\n' << '\n';
            // If we rotate by what is dictated by (mVerticalSpeedOfRotation * deltaTime), we would overshoot the desired destination (e.g. we might be rotating from 0.1 to 0.2 instead of from 0.1 to 0)
            // So we modify the rotation matrix so that it only makes us rotate the exact number of degrees left to reach 0
            verticalRotationMatrix  = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(cameraAngularPosWRTPosZAxisInDeg), glm::vec3(0.0f, 0.0f, 1.0f)));
            mDoneRotatingVertically = true;
         }

         mCameraPosition = verticalRotationMatrix * mCameraPosition;
         mCameraUp       = verticalRotationMatrix * mCameraUp;
      }

      // Move the camera away from the target
      if (!mDoneMovingAwayFromTarget)
      {
         glm::vec3 distanceToMoveAwayFromTarget = glm::normalize(mCameraPosition - mCameraTarget) * (mSpeedOfMovementAwayFromTarget * deltaTime);
         glm::vec3 futureCameraPos              = mCameraPosition + distanceToMoveAwayFromTarget;
         if (glm::length(futureCameraPos - mCameraTarget) > 90.0f)
         {
            std::cout << "mDoneMovingAwayFromTarget" << '\n';
            std::cout << "glm::length(mCameraPosition - mCameraTarget) = " << glm::length(mCameraPosition - mCameraTarget) << '\n';
            std::cout << "glm::length(futureCameraPos - mCameraTarget) = " << glm::length(futureCameraPos - mCameraTarget) << '\n' << '\n';
            // If we move by what is dictated by (mSpeedOfMovementAwayFromTarget * deltaTime),
            // we would overshoot the desired destination (e.g. the length of (mCameraPosition - mCameraTarget) might be increasing from 89 to 91 instead of from 89 to 90)
            // So we modify the length we move so that it only makes us move the exact distance left to reach 90
            distanceToMoveAwayFromTarget = glm::normalize(mCameraPosition - mCameraTarget) * (90.0f - glm::length(mCameraPosition - mCameraTarget));
            mDoneMovingAwayFromTarget = true;
         }

         mCameraPosition += distanceToMoveAwayFromTarget;
      }

      updateCoordinateFrameOfCamera();

      if (mDoneRotatingHorizontally && mDoneRotatingVertically && mDoneMovingAwayFromTarget)
      {
         std::cout << "mDoneRotatingHorizontally && mDoneRotatingVertically && mDoneMovingAwayFromTarget" << '\n';
         std::cout << "mCameraPosition.x                            = " << mCameraPosition.x << '\n';
         std::cout << "mCameraPosition.y                            = " << mCameraPosition.y << '\n';
         std::cout << "mCameraPosition.z                            = " << mCameraPosition.z << '\n' << '\n';
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

   // The math of the first two if statements must be wrong
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
