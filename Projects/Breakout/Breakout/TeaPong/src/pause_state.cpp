#include "pause_state.h"

PauseState::PauseState(const std::shared_ptr<FiniteStateMachine>& finiteStateMachine,
                       const std::shared_ptr<Window>&             window)
   : mFSM(finiteStateMachine)
   , mWindow(window)
{

}

void PauseState::enter()
{

}

void PauseState::execute(float deltaTime)
{
   processInput(deltaTime);
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

   mWindow->pollEvents();
}
