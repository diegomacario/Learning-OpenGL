#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H

#include "game.h"

class PauseState : public State
{
public:

   PauseState(const std::shared_ptr<FiniteStateMachine>& finiteStateMachine,
              const std::shared_ptr<Window>&             window);
   ~PauseState() = default;

   PauseState(const PauseState&) = delete;
   PauseState& operator=(const PauseState&) = delete;

   PauseState(PauseState&&) = delete;
   PauseState& operator=(PauseState&&) = delete;

   void enter() override;
   void execute(float deltaTime) override;
   void exit() override;

private:

   void processInput(float deltaTime);

   std::shared_ptr<FiniteStateMachine> mFSM;

   std::shared_ptr<Window>             mWindow;
};

#endif
