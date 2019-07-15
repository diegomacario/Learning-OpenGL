#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include "game.h"

class PlayState : public State<Game>
{
public:

   PlayState() = default;
   ~PlayState() = default;

   PlayState(const PlayState&) = delete;
   PlayState& operator=(const PlayState&) = delete;

   PlayState(PlayState&&) = delete;
   PlayState& operator=(PlayState&&) = delete;

   void enter(Game* owner);
   void execute(Game* owner);
   void exit(Game* owner);

private:

   void processInput(Game* owner);
   void update(Game* owner);
   void render(Game* owner);
};

#endif
