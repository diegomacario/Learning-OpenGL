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
   , mCameraOrbitalAngularVelocity(-15.0f)
   , mCameraAngularPosWRTNegYAxisInDeg(0.0f)
   , mTransitionToPlayState(false)
   , mFirstIterationOfTransitionToPlayState(false)
   , mSpeedOfRotAroundPosZAxis(0.0f)
   , mSpeedOfRotAroundCameraRight(0.0f)
   , mSpeedOfMovementAwayFromTarget(0.0f)
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
      if (mFirstIterationOfTransitionToPlayState)
      {
         float cameraAngularPosWRTPosZAxisInDeg         = glm::degrees(glm::acos(glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), glm::normalize(mCameraPosition - mCameraTarget))));
         float distanceFromCameraPositionToCameraTarget = glm::length(mCameraTarget - mCameraPosition);

         mRemainingDegAroundPosZAxis                    = 360.0f - mCameraAngularPosWRTNegYAxisInDeg;
         mRemainingDegAroundCameraRight                 = cameraAngularPosWRTPosZAxisInDeg;
         mRemainingLengthToTravel                       = 90.0f - distanceFromCameraPositionToCameraTarget;

         mSpeedOfRotAroundPosZAxis                      = -mRemainingDegAroundPosZAxis / 10.0f;
         mSpeedOfRotAroundCameraRight                   = mRemainingDegAroundCameraRight / 10.0f;
         mSpeedOfMovementAwayFromTarget                 = mRemainingLengthToTravel / 10.0f;
         mFirstIterationOfTransitionToPlayState         = false;
      }

      glm::vec3 vecFromCameraPositionToCameraTarget     = glm::normalize(mCameraTarget - mCameraPosition);
      glm::vec3 cameraRight                             = glm::normalize(glm::cross(mCameraUp, vecFromCameraPositionToCameraTarget)); // Want to rotate CCWISE around this vector to get to the top
      mCameraUp                                         = glm::normalize(glm::cross(vecFromCameraPositionToCameraTarget, cameraRight));

      float degsToMove;

      // Around positive Z
      // -----------------------------------------------------------------------------------------------------------

      degsToMove = std::abs(mSpeedOfRotAroundPosZAxis * deltaTime);
      glm::mat4 rotationMatrixAroundPositiveZAxis;
      if (mDoneAroundZ)
      {
         rotationMatrixAroundPositiveZAxis = glm::mat4(1.0f);
      }
      else if (mRemainingDegAroundPosZAxis - degsToMove < 0.0f)
      {
         rotationMatrixAroundPositiveZAxis = glm::rotate(glm::mat4(1.0f), glm::radians(-mRemainingDegAroundPosZAxis), glm::vec3(0.0f, 0.0f, 1.0f));
         mRemainingDegAroundPosZAxis = 0.0f;
         mDoneAroundZ = true;
      }
      else
      {
         rotationMatrixAroundPositiveZAxis = glm::rotate(glm::mat4(1.0f), glm::radians(-degsToMove), glm::vec3(0.0f, 0.0f, 1.0f));
         mRemainingDegAroundPosZAxis -= degsToMove;
      }

      // Around camera right
      // -----------------------------------------------------------------------------------------------------------

      // The math is exploding here
      degsToMove                                = std::abs(mSpeedOfRotAroundCameraRight * deltaTime);
      glm::mat4 rotationMatrixAroundCameraRight = glm::rotate(glm::mat4(1.0f), glm::radians(degsToMove), cameraRight);
      glm::vec3 futureCameraPos                 = glm::mat3(rotationMatrixAroundCameraRight) * mCameraPosition;
      float futureRemainingDegAroundCameraRight = glm::degrees(glm::acos(glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), glm::normalize(futureCameraPos - mCameraTarget))));
      if (mDoneAroundRight)
      {
         rotationMatrixAroundCameraRight = glm::mat4(1.0f);
      }
      else if (futureRemainingDegAroundCameraRight > mRemainingDegAroundCameraRight)
      {
         rotationMatrixAroundCameraRight = glm::rotate(glm::mat4(1.0f), glm::radians(mRemainingDegAroundCameraRight), cameraRight);
         mDoneAroundRight = true;
      }
      else
      {
         rotationMatrixAroundCameraRight = glm::rotate(glm::mat4(1.0f), glm::radians(degsToMove), cameraRight);
      }

      mCameraUp        = glm::normalize(glm::mat3(rotationMatrixAroundCameraRight) * glm::mat3(rotationMatrixAroundPositiveZAxis) * mCameraUp);
      mCameraPosition  = glm::mat3(rotationMatrixAroundCameraRight) * glm::mat3(rotationMatrixAroundPositiveZAxis) * mCameraPosition;

      if (mDoneAroundRight)
      {
         mCameraPosition = glm::vec3(0.0f, 0.0f, mCameraPosition.z);
      }

      futureCameraPos = mCameraPosition + glm::normalize(mCameraPosition - mCameraTarget) * (mSpeedOfMovementAwayFromTarget * deltaTime);
      if (mDoneMovingAway)
      {
         // Do nothing
      }
      else if (glm::length(futureCameraPos - mCameraTarget) > 90.0f)
      {
         mCameraPosition += glm::normalize(mCameraPosition - mCameraTarget) * (90.0f - glm::length(mCameraPosition - mCameraTarget));
         mDoneMovingAway = true;
      }
      else
      {
         mCameraPosition += glm::normalize(mCameraPosition - mCameraTarget) * (mSpeedOfMovementAwayFromTarget * deltaTime);
      }

      if (!mDoneAroundRight)
      {
         mRemainingDegAroundCameraRight = glm::degrees(glm::acos(glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), glm::normalize(mCameraPosition - mCameraTarget))));
      }
      else
      {
         mRemainingDegAroundCameraRight = 0.0f;
      }

      std::cout << "camera.x = " << mCameraPosition.x << '\n';
      std::cout << "camera.y = " << mCameraPosition.y << '\n';
      std::cout << "camera.z = " << mCameraPosition.z << '\n' << '\n';

      if (mDoneAroundZ)
      {
         std::cout << "**************************************************** mDoneAroundZ = " << '\n';
      }

      if (mDoneAroundRight)
      {
         std::cout << "**************************************************** mDoneAroundRight = " << '\n';
      }

      if (mDoneAroundZ && mDoneAroundRight && mDoneMovingAway)
      {
         std::cout << "Final pos = " << '\n';
         std::cout << "camera.x = " << mCameraPosition.x << '\n';
         std::cout << "camera.y = " << mCameraPosition.y << '\n';
         std::cout << "camera.z = " << mCameraPosition.z << '\n';
         mFSM->changeState("play");
      }
   }
   else
   {
      // Rotate the camera around the teapot
      mCameraAngularPosWRTNegYAxisInDeg += -mCameraOrbitalAngularVelocity * deltaTime;
      if (mCameraAngularPosWRTNegYAxisInDeg >= 360.0f)
      {
         mCameraAngularPosWRTNegYAxisInDeg -= 360.0f;
      }

      glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(mCameraOrbitalAngularVelocity * deltaTime), glm::vec3(0.0f, 0.0f, 1.0f));
      mCameraPosition = glm::mat3(rotationMatrix) * mCameraPosition;
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
