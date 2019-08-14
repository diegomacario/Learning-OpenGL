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
   , mCameraOrbitalAngularVelocity(-15.0f)
   , mCameraAngularPositionWRTNegativeYAxisInDeg(0.0f)
   , mTransitionToPlayState(false)
   , mFirstIterationOfTransitionToPlayState(false)
   , mSpeedOfRotationAroundPositiveZAxis(0.0f)
   , mSpeedOfRotationAroundCameraRight(0.0f)
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

      mStartTimeOfTransition = glfwGetTime();
   }
}

void MenuState::update(float deltaTime)
{
   if (mTransitionToPlayState)
   {
      if (mFirstIterationOfTransitionToPlayState)
      {
         mCameraUp = glm::vec3(0.0, 0.0, 1.0f);

         float cameraAngularPositionWRTPositiveZAxisInDeg = glm::degrees(glm::acos(glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), -glm::normalize(mCameraTarget - mCameraPosition))));
         float distanceFromCameraPositionToCameraTarget   = glm::length(mCameraTarget - mCameraPosition);

         mRemainingDegreesAroundPositiveZAxis = 360.0f - mCameraAngularPositionWRTNegativeYAxisInDeg;
         mRemainingDegreesAroundCameraRight = cameraAngularPositionWRTPositiveZAxisInDeg;
         mRemainingLengthToTravel = 90.5f - distanceFromCameraPositionToCameraTarget;

         mSpeedOfRotationAroundPositiveZAxis    = -mRemainingDegreesAroundPositiveZAxis / 7.5f;
         mSpeedOfRotationAroundCameraRight      = mRemainingDegreesAroundCameraRight / 7.5f;
         mSpeedOfMovementAwayFromTarget         = mRemainingLengthToTravel / 7.5f;
         mFirstIterationOfTransitionToPlayState = false;
      }

      glm::vec3 vecFromCameraPositionToCameraTarget = glm::normalize(mCameraTarget - mCameraPosition);
      glm::vec3 cameraRight                         = glm::normalize(glm::cross(mCameraUp, vecFromCameraPositionToCameraTarget)); // Want to rotate CCWISE around this vector to get to the top
      mCameraUp                                     = glm::normalize(glm::cross(vecFromCameraPositionToCameraTarget, cameraRight));

      float degsToMove;
      degsToMove = std::abs(mSpeedOfRotationAroundPositiveZAxis * deltaTime);
      glm::mat4 rotationMatrixAroundPositiveZAxis;
      if (mDoneAroundZ)
      {
         std::cout << "2here pos z = " << mRemainingDegreesAroundPositiveZAxis << '\n';
         rotationMatrixAroundPositiveZAxis = glm::mat4(1.0f);
      }
      else if (mRemainingDegreesAroundPositiveZAxis - degsToMove < 0.0f)
      {
         std::cout << "1here pos z = " << mRemainingDegreesAroundPositiveZAxis << '\n';
         rotationMatrixAroundPositiveZAxis = glm::rotate(glm::mat4(1.0f), glm::radians(-mRemainingDegreesAroundPositiveZAxis), glm::vec3(0.0f, 0.0f, 1.0f));
         mRemainingDegreesAroundPositiveZAxis = 0.0f;
         mDoneAroundZ = true;
      }
      else
      {
         rotationMatrixAroundPositiveZAxis = glm::rotate(glm::mat4(1.0f), glm::radians(-degsToMove), glm::vec3(0.0f, 0.0f, 1.0f));
         mRemainingDegreesAroundPositiveZAxis -= degsToMove;
      }

      // The math is exploding here
      degsToMove = std::abs(mSpeedOfRotationAroundCameraRight * deltaTime);
      glm::mat4 rotationMatrixAroundCameraRight;
      //if (mRemainingDegreesAroundCameraRight == 0.0f)
      if (mDoneAroundRight)
      {
         std::cout << "2here cam r = " << mRemainingDegreesAroundCameraRight << '\n';
         rotationMatrixAroundCameraRight = glm::mat4(1.0f);
      }
      //else if (mRemainingDegreesAroundCameraRight - degsToMove < 0.0f)
      else if (mRemainingDegreesAroundCameraRight < 10.0f)
      {
         std::cout << "1here cam r = " << mRemainingDegreesAroundCameraRight << '\n';
         //rotationMatrixAroundCameraRight = glm::rotate(glm::mat4(1.0f), glm::radians(mRemainingDegreesAroundCameraRight), cameraRight);
         rotationMatrixAroundCameraRight = glm::mat4(1.0f);
         mRemainingDegreesAroundCameraRight = 0.0f;
         mDoneAroundRight = true;
      }
      else
      {
         rotationMatrixAroundCameraRight = glm::rotate(glm::mat4(1.0f), glm::radians(degsToMove), cameraRight);
         mRemainingDegreesAroundCameraRight -= degsToMove;
      }

      mCameraUp = glm::normalize(glm::mat3(rotationMatrixAroundCameraRight) * glm::mat3(rotationMatrixAroundPositiveZAxis) * mCameraUp);
      mCameraPosition = glm::mat3(rotationMatrixAroundCameraRight) * glm::mat3(rotationMatrixAroundPositiveZAxis) * mCameraPosition;
      mCameraPosition += -vecFromCameraPositionToCameraTarget * (mSpeedOfMovementAwayFromTarget * deltaTime);

      //if (mRemainingDegreesAroundPositiveZAxis == 0.0f && mRemainingDegreesAroundCameraRight == 0.0f)
      if (mDoneAroundZ && mDoneAroundRight)
      {
         std::cout << "mRemainingDegreesAroundPositiveZAxis = " << mRemainingDegreesAroundPositiveZAxis << '\n';
         std::cout << "mRemainingDegreesAroundCameraRight = " << mRemainingDegreesAroundCameraRight << '\n';
         std::cout << "mRemainingLengthToTravel = " << mRemainingLengthToTravel << '\n';
         mFSM->changeState("play");
      }
   }
   else
   {
      // Rotate the camera around the teapot
      mCameraAngularPositionWRTNegativeYAxisInDeg += -mCameraOrbitalAngularVelocity * deltaTime;
      if (mCameraAngularPositionWRTNegativeYAxisInDeg >= 360.0f)
      {
         mCameraAngularPositionWRTNegativeYAxisInDeg -= 360.0f;
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
   mGameObject3DShader->setMat4("view", glm::lookAt(mCameraPosition, mCameraTarget, glm::vec3(0.0f, 0.0f, 1.0f)));
   mGameObject3DShader->setVec3("cameraPos", mCameraPosition);

   mTable->render(*mGameObject3DShader);

   mLeftPaddle->render(*mGameObject3DShader);
   mRightPaddle->render(*mGameObject3DShader);

   glDisable(GL_CULL_FACE);
   mBall->render(*mGameObject3DShader);
   glEnable(GL_CULL_FACE);
}
