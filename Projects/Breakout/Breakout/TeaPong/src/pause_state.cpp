#include "pause_state.h"

   PauseState::PauseState(const std::shared_ptr<FiniteStateMachine>& finiteStateMachine,
                          const std::shared_ptr<Window>&             window,
                          const std::shared_ptr<Camera>&             camera,
                          const std::shared_ptr<Shader>&             gameObject3DShader,
                          const std::shared_ptr<GameObject3D>&       table,
                          const std::shared_ptr<Paddle>&             leftPaddle,
                          const std::shared_ptr<Paddle>&             rightPaddle,
                          const std::shared_ptr<Ball>&               ball)
   : mFSM(finiteStateMachine)
   , mWindow(window)
   , mCamera(camera)
   , mGameObject3DShader(gameObject3DShader)
   , mTable(table)
   , mLeftPaddle(leftPaddle)
   , mRightPaddle(rightPaddle)
   , mBall(ball)
{

}

void PauseState::enter()
{

}

void PauseState::execute(float deltaTime)
{
   processInput(deltaTime);
   render();
}

void PauseState::exit()
{

}

void PauseState::processInput(float deltaTime)
{
   // Close the game
   if (mWindow->keyIsPressed(GLFW_KEY_ESCAPE)) { mWindow->setShouldClose(true); }

   // Unpause the game
   if (mWindow->keyIsPressed(GLFW_KEY_P) && !mWindow->keyHasBeenProcessed(GLFW_KEY_P))
   {
      mWindow->setKeyAsProcessed(GLFW_KEY_P);
      mFSM->changeState("play");
   }

   // Reset the camera
   if (mWindow->keyIsPressed(GLFW_KEY_R)) { resetCamera(); }

   // Move the camera
   if (mWindow->keyIsPressed(GLFW_KEY_W)) { mCamera->processKeyboardInput(Camera::MovementDirection::Forward, deltaTime); }
   if (mWindow->keyIsPressed(GLFW_KEY_S)) { mCamera->processKeyboardInput(Camera::MovementDirection::Backward, deltaTime); }
   if (mWindow->keyIsPressed(GLFW_KEY_A)) { mCamera->processKeyboardInput(Camera::MovementDirection::Left, deltaTime); }
   if (mWindow->keyIsPressed(GLFW_KEY_D)) { mCamera->processKeyboardInput(Camera::MovementDirection::Right, deltaTime); }

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

   mWindow->pollEvents();
}

void PauseState::render()
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

   mWindow->swapBuffers();
   mWindow->pollEvents();
}

void PauseState::resetCamera()
{
   mCamera->reposition(glm::vec3(0.0f, 0.0f, 95.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f),
                       0.0f,
                       0.0f,
                       45.0f);

   mGameObject3DShader->use();
   mGameObject3DShader->setMat4("projection", mCamera->getPerspectiveProjectionMatrix());
}
