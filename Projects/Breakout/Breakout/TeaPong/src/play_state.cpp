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
   // Close the game
   if (mWindow->keyIsPressed(GLFW_KEY_ESCAPE)) { mWindow->setShouldClose(true); }

   // Move the camera
   if (mWindow->keyIsPressed(GLFW_KEY_W))      { mCamera->processKeyboardInput(Camera::MovementDirection::Forward, deltaTime); }
   if (mWindow->keyIsPressed(GLFW_KEY_S))      { mCamera->processKeyboardInput(Camera::MovementDirection::Backward, deltaTime); }
   if (mWindow->keyIsPressed(GLFW_KEY_A))      { mCamera->processKeyboardInput(Camera::MovementDirection::Left, deltaTime); }
   if (mWindow->keyIsPressed(GLFW_KEY_D))      { mCamera->processKeyboardInput(Camera::MovementDirection::Right, deltaTime); }

   // Orient the camera
   if (mWindow->mouseMoved())
   {
      mCamera->processMouseMovement(mWindow->getCursorXOffset(), mWindow->getCursorYOffset());
      mWindow->resetMouseMoved();
   }

   // Zoom in or out
   if (mWindow->scrollWheelMoved())
   {
      mCamera->processScrollWheelMovement(mWindow->getScrollYOffset());
      mGameObject3DShader->use();
      mGameObject3DShader->setMat4("projection", mCamera->getPerspectiveProjectionMatrix());
      mWindow->resetScrollWheelMoved();
   }

   // Move the paddles
   if (mWindow->keyIsPressed(GLFW_KEY_Z)) { mLeftPaddle->moveAlongLine(deltaTime, 60.0f, Paddle::MovementDirection::Up); }
   if (mWindow->keyIsPressed(GLFW_KEY_X)) { mLeftPaddle->moveAlongLine(deltaTime, 60.0f, Paddle::MovementDirection::Down); }
   if (mWindow->keyIsPressed(GLFW_KEY_N)) { mRightPaddle->moveAlongLine(deltaTime, 60.0f, Paddle::MovementDirection::Up); }
   if (mWindow->keyIsPressed(GLFW_KEY_M)) { mRightPaddle->moveAlongLine(deltaTime, 60.0f, Paddle::MovementDirection::Down); }
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
