#include "game.h"

Game::Game(GLuint windowWidth, GLuint windowHeight)
   : mWindowWidth(windowWidth)
   , mWindowHeight(windowHeight)
   , mKeyboard()
   , mProcessedKeyboard()
   , mTextureManager()
   , mModelManager()
   , mShaderManager()
   , mRenderer2D()
   , mLeftPaddle()
   , mRightPaddle()
   , mBall()
   , mGameStates()
   , mStateMachine()
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
