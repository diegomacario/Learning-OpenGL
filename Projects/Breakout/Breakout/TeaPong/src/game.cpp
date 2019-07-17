#include "game.h"

Game::Game(GLuint windowWidth, GLuint windowHeight)
   : mTextureManager()
   , mModelManager()
   , mShaderManager()
   , mRenderer2D()
   , mLeftPaddle()
   , mRightPaddle()
   , mBall()
   , mGameStates()
   , mStateMachine()
   , mWindow()
{
   initialize();
}

Game::~Game()
{

}

void Game::initialize()
{

}

void Game::update(GLfloat deltaTime)
{
   mStateMachine->update();
}
