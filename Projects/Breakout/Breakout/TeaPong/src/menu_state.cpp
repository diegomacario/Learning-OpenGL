#include "constants.h"
#include "menu_state.h"

MenuState::MenuState(const std::shared_ptr<FiniteStateMachine>& finiteStateMachine,
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
}

void MenuState::update(float deltaTime)
{

}

void MenuState::render()
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
