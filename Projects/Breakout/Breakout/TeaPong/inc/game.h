#ifndef GAME_H
#define GAME_H

#include "model.h"
#include "renderer_2D.h"
#include "movable_game_object_2D.h"
#include "movable_game_object_3D.h"
#include "camera.h"
#include "state.h"
#include "state_machine.h"
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

   bool initialize(GLuint widthInPix, GLuint heightInPix, const std::string& title);
   void update(GLfloat deltaTime);
   void gameLoop();

private:

   std::unique_ptr<Window>                   mWindow;

   std::unique_ptr<StateMachine<Game>>       mStateMachine;
   std::vector<std::unique_ptr<State<Game>>> mGameStates;

   std::unique_ptr<Camera>                   mCamera;

   std::unique_ptr<Renderer2D>               mRenderer2D;

   ResourceManager<Model>                    mModelManager;
   ResourceManager<Texture>                  mTextureManager;
   ResourceManager<Shader>                   mShaderManager;

   std::unique_ptr<GameObject2D>             mBackground;
   std::unique_ptr<MovableGameObject2D>      mLeftPaddle;
   std::unique_ptr<MovableGameObject2D>      mRightPaddle;
   std::unique_ptr<MovableGameObject3D>      mBall;
};

#endif
