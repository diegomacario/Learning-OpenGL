#ifndef FINITE_STATE_MACHINE_H
#define FINITE_STATE_MACHINE_H

#include <unordered_map>
#include <memory>

#include "state.h"

class FiniteStateMachine
{
public:

   FiniteStateMachine() = default;
   ~FiniteStateMachine() = default;

   FiniteStateMachine(const FiniteStateMachine&) = delete;
   FiniteStateMachine& operator=(const FiniteStateMachine&) = delete;

   FiniteStateMachine(FiniteStateMachine&&) = delete;
   FiniteStateMachine& operator=(FiniteStateMachine&&) = delete;

   void        initialize(std::unordered_map<std::string, std::shared_ptr<State>>&& states,
                          const std::string&                                        initialStateID);
   void        executeCurrentState(float deltaTime) const;
   void        changeState(const std::string& newStateID);

   std::string getPreviousStateID() const;
   std::string getCurrentStateID() const;

private:

   std::unordered_map<std::string, std::shared_ptr<State>> mStates;
   std::shared_ptr<State>                                  mCurrentState;
   std::string                                             mPreviousStateID;
   std::string                                             mCurrentStateID;
};

#endif
