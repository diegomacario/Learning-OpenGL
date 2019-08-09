#include <iostream>

#include "shader_loader.h"
#include "texture_loader.h"
#include "model_loader.h"
#include "play_state.h"
#include "game.h"

Game::Game()
   : mWindow()
   , mFSM()
   , mPlayState()
   , mCamera()
   , mRenderer2D()
   , mModelManager()
   , mTextureManager()
   , mShaderManager()
   , mTable()
   , mLeftPaddle()
   , mRightPaddle()
   , mBall()
{

}

Game::~Game()
{

}

bool Game::initialize(GLuint widthInPix, GLuint heightInPix, const std::string& title)
{
   // Initialize the window
   mWindow = std::make_shared<Window>(widthInPix, heightInPix, title);

   if (!mWindow->initialize())
   {
      std::cout << "Error - Game::initialize - Failed to initialize the window" << "\n";
      return false;
   }

   // Initialize the camera
   GLfloat aspectRatio = static_cast<GLfloat>(mWindow->getWidthInPix()) / static_cast<GLfloat>(mWindow->getHeightInPix());

   mCamera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 95.0f), // Position
                                      glm::vec3(0.0f, 1.0f, 0.0f),  // World up
                                      0.0f,                         // Yaw
                                      0.0f,                         // Pitch
                                      45.0f,                        // Fovy
                                      aspectRatio,                  // Aspect ratio
                                      0.1f,                         // Near
                                      120.0f,                       // Far
                                      10.0f,                        // Movement speed
                                      0.1f);                        // Mouse sensitivity

   // Initialize the 2D renderer
   glm::mat4 orthoProj = glm::ortho(0.0f,                                            // Left
                                    static_cast<GLfloat>(mWindow->getWidthInPix()),  // Right
                                    static_cast<GLfloat>(mWindow->getHeightInPix()), // Bottom
                                    0.0f,                                            // Top
                                   -1.0f,                                            // Near
                                    1.0f);                                           // Far

   auto gameObj2DShader = mShaderManager.loadResource<ShaderLoader>("game_object_2D",
                                                                    "shaders/game_object_2D.vs",
                                                                    "shaders/game_object_2D.fs");
   gameObj2DShader->use();
   gameObj2DShader->setInt("image", 0);
   gameObj2DShader->setMat4("projection", orthoProj);
   mRenderer2D = std::make_unique<Renderer2D>(gameObj2DShader);

   // Initialize the 3D shader
   auto gameObj3DShader = mShaderManager.loadResource<ShaderLoader>("game_object_3D",
                                                                    "shaders/game_object_3D.vs",
                                                                    "shaders/game_object_3D.fs");
   gameObj3DShader->use();
   gameObj3DShader->setMat4("projection", mCamera->getPerspectiveProjectionMatrix());
   gameObj3DShader->setVec3("pointLights[0].worldPos", glm::vec3(40.0f, 30.0f, 80.0f));
   gameObj3DShader->setVec3("pointLights[0].color", glm::vec3(1.0f, 1.0f, 1.0f));
   gameObj3DShader->setFloat("pointLights[0].constantAtt", 1.0f);
   gameObj3DShader->setFloat("pointLights[0].linearAtt", 0.01f);
   gameObj3DShader->setFloat("pointLights[0].quadraticAtt", 0.0f);
   gameObj3DShader->setInt("numPointLightsInScene", 1);

   // Load the models
   mModelManager.loadResource<ModelLoader>("table", "models/table/table.obj");
   mModelManager.loadResource<ModelLoader>("paddle", "models/paddle/paddle.obj");
   mModelManager.loadResource<ModelLoader>("teapot", "models/teapot/teapot.obj");

   mTable = std::make_shared<GameObject3D>(mModelManager.getResource("table"),
                                           glm::vec3(0.0f),
                                           90.0f,
                                           glm::vec3(1.0f, 0.0f, 0.0f),
                                           1.0f);

   mLeftPaddle = std::make_shared<Paddle>(mModelManager.getResource("paddle"),
                                          glm::vec3(-45.0f, 0.0f, 0.0f),
                                          90.0f,
                                          glm::vec3(1.0f, 0.0f, 0.0f),
                                          1.0f,
                                          glm::vec3(0.0f, 10.0f, 0.0f));

   mRightPaddle = std::make_shared<Paddle>(mModelManager.getResource("paddle"),
                                           glm::vec3(45.0f, 0.0f, 0.0f),
                                           90.0f,
                                           glm::vec3(1.0f, 0.0f, 0.0f),
                                           1.0f,
                                           glm::vec3(0.0f, 10.0f, 0.0f));

   mBall = std::make_shared<Ball>(mModelManager.getResource("teapot"),
                                  glm::vec3(0.0f),
                                  90.0f,
                                  glm::vec3(1.0f, 0.0f, 0.0f),
                                  1.0f,
                                  glm::vec3(10.0f, 20.0f, 0.0f),
                                  7.5f,
                                  1000.0f);

   // Initialize the states
   mPlayState = std::make_unique<PlayState>(mWindow,
                                            mCamera,
                                            gameObj3DShader,
                                            mTable,
                                            mLeftPaddle,
                                            mRightPaddle,
                                            mBall);

   // Initialize the FSM
   mFSM = std::make_unique<FiniteStateMachine>(mPlayState.get());

   return true;
}

void Game::executeGameLoop()
{
   double currentFrame = 0.0;
   double lastFrame    = 0.0;
   float  deltaTime    = 0.0f;

   while (!mWindow->shouldClose())
   {
      currentFrame = glfwGetTime();
      deltaTime    = static_cast<float>(currentFrame - lastFrame);
      lastFrame    = currentFrame;

      mFSM->executeCurrentState(deltaTime);

      mWindow->swapBuffers();
      mWindow->pollEvents();
   }
}
