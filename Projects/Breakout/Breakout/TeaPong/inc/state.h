#ifndef STATE_H
#define STATE_H

template <class TOwner>
class State
{
public:

   State() = default;
   virtual ~State() = default;

   State(const State&) = delete;
   State& operator=(const State&) = delete;

   State(State&&) = delete;
   State& operator=(State&&) = delete;

   virtual void enter(TOwner* owner) = 0;
   virtual void execute(TOwner* owner) = 0;
   virtual void exit(TOwner* owner) = 0;
};

#endif
