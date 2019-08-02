#include "play_state.h"

void PlayState::enter(Game* owner)
{

}

void PlayState::execute(Game* owner)
{
   processInput(owner);
   update(owner);
   render(owner);
}

void PlayState::exit(Game* owner)
{

}

void PlayState::processInput(Game* owner)
{
   if (owner->mWindow->keyIsPressed(GLFW_KEY_ESCAPE))
      owner->mWindow->setShouldClose(true);

   if (owner->mWindow->keyIsPressed(GLFW_KEY_W))
      owner->mCamera->processKeyboardInput(MovementDirection::Forward, owner->mDeltaTime);
   if (owner->mWindow->keyIsPressed(GLFW_KEY_S))
      owner->mCamera->processKeyboardInput(MovementDirection::Backward, owner->mDeltaTime);
   if (owner->mWindow->keyIsPressed(GLFW_KEY_A))
      owner->mCamera->processKeyboardInput(MovementDirection::Left, owner->mDeltaTime);
   if (owner->mWindow->keyIsPressed(GLFW_KEY_D))
      owner->mCamera->processKeyboardInput(MovementDirection::Right, owner->mDeltaTime);

   if (owner->mWindow->mouseMoved())
   {
      owner->mCamera->processMouseMovement(owner->mWindow->getCursorXOffset(), owner->mWindow->getCursorYOffset());
      owner->mWindow->resetMouseMoved();
   }

   if (owner->mWindow->scrollWheelMoved())
   {
      owner->mCamera->processScrollWheelMovement(owner->mWindow->getScrollYOffset());
      owner->mWindow->resetScrollWheelMoved();
   }
}

void PlayState::update(Game* owner)
{

}

void PlayState::render(Game* owner)
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Render
   // ----------------------------------------------------------------------------------------------------

   // Enable depth testing for 3D objects
   glEnable(GL_DEPTH_TEST);

   auto gameObject3DShader = owner->mShaderManager.getResource("game_object_3D");
   gameObject3DShader->use();
   gameObject3DShader->setMat4("view", owner->mCamera->getViewMatrix());
   gameObject3DShader->setVec3("cameraPos", owner->mCamera->getPosition());

   owner->mTable->render(*gameObject3DShader);

   glDisable(GL_CULL_FACE);
   owner->mBall->render(*gameObject3DShader);
   glEnable(GL_CULL_FACE);
}
