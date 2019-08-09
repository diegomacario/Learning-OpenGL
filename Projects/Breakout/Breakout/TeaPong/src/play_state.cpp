#include "play_state.h"

PlayState::PlayState(const std::shared_ptr<Window>&       window,
                     const std::shared_ptr<Camera>&       camera,
                     const std::shared_ptr<Shader>&       gameObject3DShader,
                     const std::shared_ptr<GameObject3D>& table,
                     const std::shared_ptr<Paddle>&       leftPaddle,
                     const std::shared_ptr<Paddle>&       rightPaddle,
                     const std::shared_ptr<Ball>&         ball)
   : mWindow(window)
   , mCamera(camera)
   , mGameObject3DShader(gameObject3DShader)
   , mTable(table)
   , mLeftPaddle(leftPaddle)
   , mRightPaddle(rightPaddle)
   , mBall(ball)
{

}

void PlayState::enter()
{

}

void PlayState::execute(float deltaTime)
{
   processInput(deltaTime);
   update(deltaTime);
   render();
}

void PlayState::exit()
{

}

void PlayState::processInput(float deltaTime)
{
   if (mWindow->keyIsPressed(GLFW_KEY_ESCAPE)) { mWindow->setShouldClose(true); }
   if (mWindow->keyIsPressed(GLFW_KEY_W))      { mCamera->processKeyboardInput(MovementDirection::Forward, deltaTime); }
   if (mWindow->keyIsPressed(GLFW_KEY_S))      { mCamera->processKeyboardInput(MovementDirection::Backward, deltaTime); }
   if (mWindow->keyIsPressed(GLFW_KEY_A))      { mCamera->processKeyboardInput(MovementDirection::Left, deltaTime); }
   if (mWindow->keyIsPressed(GLFW_KEY_D))      { mCamera->processKeyboardInput(MovementDirection::Right, deltaTime); }

   if (mWindow->mouseMoved())
   {
      mCamera->processMouseMovement(mWindow->getCursorXOffset(), mWindow->getCursorYOffset());
      mWindow->resetMouseMoved();
   }

   if (mWindow->scrollWheelMoved())
   {
      mCamera->processScrollWheelMovement(mWindow->getScrollYOffset());

      mGameObject3DShader->use();
      mGameObject3DShader->setMat4("projection", mCamera->getPerspectiveProjectionMatrix());

      mWindow->resetScrollWheelMoved();
   }

   // Move the left paddle
   if (mWindow->keyIsPressed(GLFW_KEY_Z))
   {
      // Move up
      glm::vec3 currPos = mLeftPaddle->getPosition();
      if ((currPos.y + (mLeftPaddle->getLength() / 2.0f)) < 30.0f)
      {
         mLeftPaddle->translate(mLeftPaddle->getVelocity() * deltaTime);
      }
   }
   if (mWindow->keyIsPressed(GLFW_KEY_X))
   {
      // Move down
      glm::vec3 currPos = mLeftPaddle->getPosition();
      if ((currPos.y - (mLeftPaddle->getLength() / 2.0f)) > -30.0f)
      {
         mLeftPaddle->translate(-mLeftPaddle->getVelocity() * deltaTime);
      }
   }

   // Move the right paddle
   if (mWindow->keyIsPressed(GLFW_KEY_N))
   {
      // Move up
      glm::vec3 currPos = mRightPaddle->getPosition();
      if ((currPos.y + (mRightPaddle->getLength() / 2.0f)) < 30.0f)
      {
         mRightPaddle->translate(mRightPaddle->getVelocity() * deltaTime);
      }
   }
   if (mWindow->keyIsPressed(GLFW_KEY_M))
   {
      // Move down
      glm::vec3 currPos = mRightPaddle->getPosition();
      if ((currPos.y - (mRightPaddle->getLength() / 2.0f)) > -30.0f)
      {
         mRightPaddle->translate(-mRightPaddle->getVelocity() * deltaTime);
      }
   }
}

void PlayState::update(float deltaTime)
{
   // TODO: Get the dimensions from the table
   mBall->moveWithinArea(deltaTime, 100.0f, 60.0f);

   // TODO: Perform collision detection here

   // TODO: Check if we have a winner here
}

void PlayState::render()
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Enable depth testing for 3D objects
   glEnable(GL_DEPTH_TEST);

   mGameObject3DShader->use();
   mGameObject3DShader->setMat4("view", mCamera->getViewMatrix());
   mGameObject3DShader->setVec3("cameraPos", mCamera->getPosition());

   mTable->render(*mGameObject3DShader);

   mLeftPaddle->render(*mGameObject3DShader);
   mRightPaddle->render(*mGameObject3DShader);

   glDisable(GL_CULL_FACE);
   mBall->render(*mGameObject3DShader);
   glEnable(GL_CULL_FACE);
}
