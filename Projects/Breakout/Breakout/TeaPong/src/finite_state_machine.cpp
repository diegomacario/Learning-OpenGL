#include "finite_state_machine.h"

FiniteStateMachine::FiniteStateMachine(State* initialState)
   : mCurrentState(initialState)
{
   // TODO: Call the enter() function of initialState here
}

void FiniteStateMachine::executeCurrentState(float deltaTime) const
{
   mCurrentState->execute(deltaTime);
}

void FiniteStateMachine::changeState(State* newState)
{
   mCurrentState->exit();
   mCurrentState = newState;
   mCurrentState->enter();
}
