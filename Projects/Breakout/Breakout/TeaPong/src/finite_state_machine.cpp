#include <iostream>
#include <string>

#include "finite_state_machine.h"

void FiniteStateMachine::initialize(std::unordered_map<std::string, std::shared_ptr<State>>&& states,
                                    const std::string&                                        initialStateID)
{
   mStates = std::move(states);

   auto it = mStates.find(initialStateID);
   if (it != mStates.end())
   {
      mCurrentState = it->second;
      // TODO: Call the enter() function of initialState here
   }
   else
   {
      std::cout << "Error - FiniteStateMachine::FiniteStateMachine - A state with the following ID does not exist: " << initialStateID << "\n";
   }
}

void FiniteStateMachine::executeCurrentState(float deltaTime) const
{
   mCurrentState->execute(deltaTime);
}

void FiniteStateMachine::changeState(const std::string& newStateID)
{
   auto it = mStates.find(newStateID);
   if (it != mStates.end())
   {
      mCurrentState->exit();
      mCurrentState = it->second;
      mCurrentState->enter();
   }
   else
   {
      std::cout << "Error - FiniteStateMachine::changeState - A state with the following ID does not exist: " << newStateID << "\n";
   }
}
