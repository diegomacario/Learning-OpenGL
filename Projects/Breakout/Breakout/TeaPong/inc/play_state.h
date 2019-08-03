#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include "game.h"

class PlayState : public State
{
public:

   PlayState(const std::shared_ptr<Window>&              window,
             const std::shared_ptr<Camera>&              camera,
             const std::shared_ptr<Shader>&              gameObject3DShader,
             const std::shared_ptr<GameObject3D>&        table,
             const std::shared_ptr<MovableGameObject2D>& leftPaddle,
             const std::shared_ptr<MovableGameObject2D>& rightPaddle,
             const std::shared_ptr<MovableGameObject3D>& ball);
   ~PlayState() = default;

   PlayState(const PlayState&) = delete;
   PlayState& operator=(const PlayState&) = delete;

   PlayState(PlayState&&) = delete;
   PlayState& operator=(PlayState&&) = delete;

   void enter() override;
   void execute(float deltaTime) override;
   void exit() override;

private:

   void processInput(float deltaTime);
   void update();
   void render();

   std::shared_ptr<Window>              mWindow;

   std::shared_ptr<Camera>              mCamera;

   std::shared_ptr<Shader>              mGameObject3DShader;

   std::shared_ptr<GameObject3D>        mTable;
   std::shared_ptr<MovableGameObject2D> mLeftPaddle;
   std::shared_ptr<MovableGameObject2D> mRightPaddle;
   std::shared_ptr<MovableGameObject3D> mBall;
};

#endif
