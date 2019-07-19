#include <iostream>

#include <shader_loader.h>
#include <texture_loader.h>
#include <model_loader.h>
#include "game.h"

Game::Game(GLuint width, GLuint height, const std::string& title)
   : mWindow()
   , mStateMachine()
   , mGameStates()
   , mCamera()
   , mRenderer2D()
   , mModelManager()
   , mTextureManager()
   , mShaderManager()
   , mLeftPaddle()
   , mRightPaddle()
   , mBall()
{
   initialize(width, height, title);
}

Game::~Game()
{

}

bool Game::initialize(GLuint width, GLuint height, const std::string& title)
{
   // Initialize the window
   mWindow = std::make_unique<Window>(width, height, title);

   if (!mWindow->initialize())
   {
      std::cout << "Error - Game::initialize - Failed to initialize the window" << "\n";
      return false;
   }

   // Initialize the camera
   mCamera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 1.0f), // Position
                                      glm::vec3(0.0f, 1.0f, 0.0f), // World up
                                      0.0f,                        // Yaw
                                      0.0f,                        // Pitch
                                      45.0f,                       // Fovy
                                      2.5f,                        // Movement speed
                                      0.1f);                       // Mouse sensitivity

   // Initialize the 2D renderer
   glm::mat4 orthographicProjection = glm::ortho(0.0f,                                       // Left
                                                 static_cast<GLfloat>(mWindow->getWidth()),  // Right
                                                 static_cast<GLfloat>(mWindow->getHeight()), // Bottom
                                                 0.0f,                                       // Top
                                                -1.0f,                                       // Near
                                                 1.0f);                                      // Far
   auto gameObject2DShader = mShaderManager.loadResource<ShaderLoader>("game_object_2D", "shaders/game_object_2D.vs", "shaders/game_object_2D.fs");
   gameObject2DShader->use();
   gameObject2DShader->setInt("image", 0);
   gameObject2DShader->setMat4("projection", orthographicProjection);
   mRenderer2D = std::make_unique<Renderer2D>(gameObject2DShader);

   // Load the shaders
   glm::mat4 projection = glm::perspective(glm::radians(mCamera->getFieldOfViewY()),                                               // FOVY
                                           static_cast<GLfloat>(mWindow->getWidth()) / static_cast<GLfloat>(mWindow->getHeight()), // Aspect ratio
                                           0.1f,                                                                                   // Near
                                           100.0f);                                                                                // Far

   // Load the models
   mModelManager.loadResource<ModelLoader>("teapot", "objects/teapot/teapot.obj");

   // Load the textures
   mTextureManager.loadResource<TextureLoader>("table_cloth", "textures/table_cloth.jpg");
}

void Game::update(GLfloat deltaTime)
{
   mStateMachine->update();
}
