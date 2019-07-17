#ifndef GAME_H
#define GAME_H

#include "model.h"
#include "renderer_2D.h"
#include "movable_game_object_2D.h"
#include "movable_game_object_3D.h"
#include "state.h"
#include "state_machine.h"
#include "window.h"

class Game
{
public:

   Game(GLuint windowWidth, GLuint windowHeight);
   ~Game();

   Game(const Game&) = delete;
   Game& operator=(const Game&) = delete;

   Game(Game&&) = delete;
   Game& operator=(Game&&) = delete;

   void initialize();
   void update(GLfloat deltaTime);

private:

   ResourceManager<Texture>                  mTextureManager;
   ResourceManager<Model>                    mModelManager;
   ResourceManager<Shader>                   mShaderManager;

   std::unique_ptr<Renderer2D>               mRenderer2D;

   std::unique_ptr<MovableGameObject2D>      mLeftPaddle;
   std::unique_ptr<MovableGameObject2D>      mRightPaddle;
   std::unique_ptr<MovableGameObject3D>      mBall;

   std::vector<std::unique_ptr<State<Game>>> mGameStates;
   std::unique_ptr<StateMachine<Game>>       mStateMachine;

   std::unique_ptr<Window>                   mWindow;
};

#endif
