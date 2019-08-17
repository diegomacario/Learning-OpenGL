#ifndef GAME_H
#define GAME_H

#include "model.h"
#include "renderer_2D.h"
#include "movable_game_object_2D.h"
#include "movable_game_object_3D.h"
#include "ball.h"
#include "paddle.h"
#include "camera.h"
#include "state.h"
#include "finite_state_machine.h"
#include "window.h"

class Game
{
public:

   Game();
   ~Game();

   Game(const Game&) = delete;
   Game& operator=(const Game&) = delete;

   Game(Game&&) = delete;
   Game& operator=(Game&&) = delete;

   bool  initialize(unsigned int widthInPix, unsigned int heightInPix, const std::string& title);
   void  executeGameLoop();

private:

   std::shared_ptr<FiniteStateMachine> mFSM;

   std::shared_ptr<Window>             mWindow;

   std::shared_ptr<Camera>             mCamera;

   std::shared_ptr<Renderer2D>         mRenderer2D;

   ResourceManager<Model>              mModelManager;
   ResourceManager<Texture>            mTextureManager;
   ResourceManager<Shader>             mShaderManager;

   std::shared_ptr<GameObject3D>       mTable;
   std::shared_ptr<Paddle>             mLeftPaddle;
   std::shared_ptr<Paddle>             mRightPaddle;
   std::shared_ptr<Ball>               mBall;
};

#endif
