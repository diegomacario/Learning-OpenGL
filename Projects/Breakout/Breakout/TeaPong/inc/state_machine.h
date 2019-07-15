#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "state.h"

// TODO: Replace pointers with something that will prevent deletion/nullness
template <class TOwner>
class StateMachine
{
public:

   StateMachine(TOwner* owner, State<TOwner>* initialState);
   ~StateMachine() = default;

   StateMachine(const StateMachine&) = delete;
   StateMachine& operator=(const StateMachine&) = delete;

   StateMachine(StateMachine&&) = delete;
   StateMachine& operator=(StateMachine&&) = delete;

   void update() const;
   void changeState(State<TOwner>* newState);

private:

   TOwner*        mOwner;
   State<TOwner>* mCurrentState;
};

template <class TOwner>
StateMachine<TOwner>::StateMachine(TOwner* owner, State<TOwner>* initialState)
   : mOwner(owner)
   , mCurrentState(initialState)
{

}

template <class TOwner>
void StateMachine<TOwner>::update() const
{
   mCurrentState->execute(mOwner);
}

template <class TOwner>
void StateMachine<TOwner>::changeState(State<TOwner>* newState)
{
   mCurrentState->exit(mOwner);
   mCurrentState = newState;
   mCurrentState->enter(mOwner);
}

#endif
