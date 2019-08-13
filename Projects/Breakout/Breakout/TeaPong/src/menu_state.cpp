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
      glm::vec3 vecFromCameraPositionToCameraTarget        = glm::normalize(mCameraTarget - mCameraPosition);
      glm::vec3 cameraRight                                = glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), vecFromCameraPositionToCameraTarget)); // Want to rotate CCWISE around this vector to get to the top
      float     cameraAngularPositionWRTPositiveZAxisInDeg = glm::degrees(glm::acos(glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), -vecFromCameraPositionToCameraTarget)));
      float     distanceFromCameraPositionToCameraTarget   = glm::length(mCameraTarget - mCameraPosition);

      if (mFirstIterationOfTransitionToPlayState)
      {
         mSpeedOfRotationAroundPositiveZAxis    = -(360.0f - mCameraAngularPositionWRTNegativeYAxisInDeg) / 7.5;
         mSpeedOfRotationAroundCameraRight      = cameraAngularPositionWRTPositiveZAxisInDeg / 7.5;
         mSpeedOfMovementAwayFromTarget         = (90.5f - distanceFromCameraPositionToCameraTarget) / 7.5;
         mFirstIterationOfTransitionToPlayState = false;
      }

      glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(mSpeedOfRotationAroundPositiveZAxis * deltaTime), glm::vec3(0.0f, 0.0f, 1.0f));
      rotationMatrix = glm::rotate(rotationMatrix, glm::radians(mSpeedOfRotationAroundCameraRight * deltaTime), cameraRight);

      mCameraPosition = glm::mat3(rotationMatrix) * mCameraPosition;
      mCameraPosition += -vecFromCameraPositionToCameraTarget * (mSpeedOfMovementAwayFromTarget * deltaTime);

      std::cout << "timeSinceStartOfTransition = " << glfwGetTime() - mStartTimeOfTransition << '\n';

      //if (mCameraPosition.z > 94.0f && mCameraPosition.z < 96.0f)
      if (glfwGetTime() - mStartTimeOfTransition >= 7.3f)
      {
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
