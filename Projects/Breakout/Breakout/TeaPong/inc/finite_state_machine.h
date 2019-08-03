#ifndef FINITE_STATE_MACHINE_H
#define FINITE_STATE_MACHINE_H

#include "state.h"

class FiniteStateMachine
{
public:

   FiniteStateMachine(State* initialState);
   ~FiniteStateMachine() = default;

   FiniteStateMachine(const FiniteStateMachine&) = delete;
   FiniteStateMachine& operator=(const FiniteStateMachine&) = delete;

   FiniteStateMachine(FiniteStateMachine&&) = delete;
   FiniteStateMachine& operator=(FiniteStateMachine&&) = delete;

   void executeCurrentState(float deltaTime) const;
   void changeState(State* newState);

private:

   State* mCurrentState;
};

#endif
