#include "win_state.h"

WinState::WinState(const std::shared_ptr<FiniteStateMachine>& finiteStateMachine,
                   const std::shared_ptr<Window>&             window,
                   const std::shared_ptr<Shader>&             gameObject3DExplosiveShader,
                   const std::shared_ptr<Ball>&               ball)
   : mFSM(finiteStateMachine)
   , mWindow(window)
   , mGameObject3DExplosiveShader(gameObject3DExplosiveShader)
   , mBall(ball)
   , mCameraPosition(0.0f, -30.0f, 10.0f)
   , mCameraTarget(0.0f, 0.0f, 5.0f)
   , mCameraUp(0.0f, 0.0f, 1.0f)
   , mCameraRight(0.0f)
   , mIdleOrbitalAngularVelocity(-50.0f)
   , mTimeWhenExplosionShouldBegin(0.0)
   , mExplode(false)
   , mSpeedOfExplodingFragments(2.0f)
   , mDistanceTravelledByExplodingFragments(0.0f)
{

}

void WinState::enter()
{
   float currRadius = mBall->getRadius();
   mBall->scale(7.5f / currRadius);
   mBall->setRadius(7.5f);

   mCameraPosition = glm::vec3(0.0f, -30.0f, 10.0f);
   mCameraTarget   = glm::vec3(0.0f, 0.0f, 5.0f);
   mCameraUp       = glm::vec3(0.0f, 0.0f, 1.0f);
   mCameraRight    = glm::vec3(0.0f);

   mTimeWhenExplosionShouldBegin          = glfwGetTime() + 3.0;
   mExplode                               = false;
   mSpeedOfExplodingFragments             = 2.0f;
   mDistanceTravelledByExplodingFragments = 0.0f;
}

void WinState::execute(float deltaTime)
{
   processInput(deltaTime);
   update(deltaTime);
   render(deltaTime);
}

void WinState::exit()
{

}

void WinState::processInput(float deltaTime)
{
   // Close the game
   if (mWindow->keyIsPressed(GLFW_KEY_ESCAPE)) { mWindow->setShouldClose(true); }
}

void WinState::update(float deltaTime)
{
   if (mDistanceTravelledByExplodingFragments > 120.0f)
   {
      mFSM->changeState("menu");
   }

   if (!mExplode && (glfwGetTime() > mTimeWhenExplosionShouldBegin))
   {
      mExplode = true;
   }

   if (mExplode)
   {
      mSpeedOfExplodingFragments += 0.025;
      mDistanceTravelledByExplodingFragments += mSpeedOfExplodingFragments * deltaTime;
   }

   // Rotate the camera CW around the positive Z axis
   glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(mIdleOrbitalAngularVelocity * deltaTime), glm::vec3(0.0f, 0.0f, 1.0f));
   mCameraPosition = glm::mat3(rotationMatrix) * mCameraPosition;
}

void WinState::render(float deltaTime)
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Enable depth testing for 3D objects
   glEnable(GL_DEPTH_TEST);

   mGameObject3DExplosiveShader->use();
   mGameObject3DExplosiveShader->setMat4("view", glm::lookAt(mCameraPosition, mCameraTarget, mCameraUp));
   mGameObject3DExplosiveShader->setVec3("cameraPos", mCameraPosition);
   if (mExplode)
   {
      mGameObject3DExplosiveShader->setFloat("distanceToMove", mDistanceTravelledByExplodingFragments);
   }
   else
   {
      mGameObject3DExplosiveShader->setFloat("distanceToMove", 0.0f);
   }

   glDisable(GL_CULL_FACE);
   mBall->render(*mGameObject3DExplosiveShader);
   glEnable(GL_CULL_FACE);

   mWindow->swapBuffers();
   mWindow->pollEvents();
}
