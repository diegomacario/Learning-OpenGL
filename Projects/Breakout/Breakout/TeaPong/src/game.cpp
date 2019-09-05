#include <iostream>

#include "constants.h"
#include "shader_loader.h"
#include "texture_loader.h"
#include "model_loader.h"
#include "menu_state.h"
#include "play_state.h"
#include "pause_state.h"
#include "win_state.h"
#include "game.h"

Game::Game()
   : mFSM()
   , mWindow()
   //, mSoundEngine(irrklang::createIrrKlangDevice(), [=](irrklang::ISoundEngine* soundEngine){soundEngine->drop();})
   , mCamera()
   , mRenderer2D()
   , mModelManager()
   , mTextureManager()
   , mShaderManager()
   , mTitle()
   , mTable()
   , mLeftPaddle()
   , mRightPaddle()
   , mBall()
{

}

Game::~Game()
{

}

bool Game::initialize(unsigned int widthInPix, unsigned int heightInPix, const std::string& title)
{
   // Initialize the window
   mWindow = std::make_shared<Window>(widthInPix, heightInPix, title);

   if (!mWindow->initialize())
   {
      std::cout << "Error - Game::initialize - Failed to initialize the window" << "\n";
      return false;
   }

   // Initialize the camera
   float aspectRatio = static_cast<float>(mWindow->getWidthInPix()) / static_cast<float>(mWindow->getHeightInPix());

   mCamera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 95.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f),
                                      0.0f,
                                      0.0f,
                                      45.0f,       // Fovy
                                      aspectRatio, // Aspect ratio
                                      0.1f,        // Near
                                      130.0f,      // Far
                                      20.0f,       // Movement speed
                                      0.1f);       // Mouse sensitivity

   // Initialize the 2D renderer
   glm::mat4 orthoProj = glm::ortho(0.0f,                                          // Left
                                    static_cast<float>(mWindow->getWidthInPix()),  // Right
                                    static_cast<float>(mWindow->getHeightInPix()), // Bottom
                                    0.0f,                                          // Top
                                   -1.0f,                                          // Near
                                    1.0f);                                         // Far

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
   gameObj3DShader->setVec3("pointLights[0].worldPos", glm::vec3(0.0f, 0.0f, 100.0f));
   gameObj3DShader->setVec3("pointLights[0].color", glm::vec3(1.0f, 1.0f, 1.0f));
   gameObj3DShader->setFloat("pointLights[0].constantAtt", 1.0f);
   gameObj3DShader->setFloat("pointLights[0].linearAtt", 0.01f);
   gameObj3DShader->setFloat("pointLights[0].quadraticAtt", 0.0f);
   gameObj3DShader->setInt("numPointLightsInScene", 1);

   // Initialize the explosive 3D shader
   auto gameObj3DExplosiveShader = mShaderManager.loadResource<ShaderLoader>("game_object_3D_explosive",
                                                                             "shaders/game_object_3D.vs",
                                                                             "shaders/game_object_3D.fs",
                                                                             "shaders/game_object_3D_explosive.gs");
   gameObj3DExplosiveShader->use();
   gameObj3DExplosiveShader->setMat4("projection", mCamera->getPerspectiveProjectionMatrix());
   gameObj3DExplosiveShader->setVec3("pointLights[0].worldPos", glm::vec3(0.0f, 0.0f, 100.0f));
   gameObj3DExplosiveShader->setVec3("pointLights[0].color", glm::vec3(1.0f, 1.0f, 1.0f));
   gameObj3DExplosiveShader->setFloat("pointLights[0].constantAtt", 1.0f);
   gameObj3DExplosiveShader->setFloat("pointLights[0].linearAtt", 0.01f);
   gameObj3DExplosiveShader->setFloat("pointLights[0].quadraticAtt", 0.0f);
   gameObj3DExplosiveShader->setInt("numPointLightsInScene", 1);

   // Load the models
   mModelManager.loadResource<ModelLoader>("title", "models/title/title.obj");
   mModelManager.loadResource<ModelLoader>("table", "models/table/table.obj");
   mModelManager.loadResource<ModelLoader>("paddle", "models/paddle/paddle.obj");
   mModelManager.loadResource<ModelLoader>("teapot", "models/teapot/teapot.obj");

   mTitle = std::make_shared<GameObject3D>(mModelManager.getResource("title"),
                                           glm::vec3(0.0f, 0.0f, 13.75f),
                                           90.0f,
                                           glm::vec3(1.0f, 0.0f, 0.0f),
                                           1.0f);

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
                                          glm::vec3(0.0f, 30.0f, 0.0f),
                                          3.5f,
                                          7.5f);

   mRightPaddle = std::make_shared<Paddle>(mModelManager.getResource("paddle"),
                                           glm::vec3(45.0f, 0.0f, 0.0f),
                                           90.0f,
                                           glm::vec3(1.0f, 0.0f, 0.0f),
                                           1.0f,
                                           glm::vec3(0.0f, 30.0f, 0.0f),
                                           3.5f,
                                           7.5f);

   mBall = std::make_shared<Ball>(mModelManager.getResource("teapot"),
                                  glm::vec3(0.0f, 0.0f, 1.96875 * (7.5f / 2.5f)),
                                  90.0f,
                                  glm::vec3(1.0f, 0.0f, 0.0f),
                                  7.5f / 2.5f,
                                  glm::vec3(35.0f, 45.0f, 0.0f),
                                  7.5f,
                                  1000.0f);

   // Create the FSM
   mFSM = std::make_shared<FiniteStateMachine>();

   // Initialize the states
   std::unordered_map<std::string, std::shared_ptr<State>> mStates;

   mStates["menu"] = std::make_shared<MenuState>(mFSM,
                                                 mWindow,
                                                 gameObj3DShader,
                                                 mTitle,
                                                 mTable,
                                                 mLeftPaddle,
                                                 mRightPaddle,
                                                 mBall);

   mStates["play"] = std::make_shared<PlayState>(mFSM,
                                                 mWindow,
                                                 //mSoundEngine,
                                                 mCamera,
                                                 gameObj3DShader,
                                                 mTable,
                                                 mLeftPaddle,
                                                 mRightPaddle,
                                                 mBall);

   mStates["pause"] = std::make_shared<PauseState>(mFSM,
                                                   mWindow,
                                                   mCamera,
                                                   gameObj3DShader,
                                                   mTable,
                                                   mLeftPaddle,
                                                   mRightPaddle,
                                                   mBall);

   mStates["win"] = std::make_shared<WinState>(mFSM,
                                               mWindow,
                                               gameObj3DExplosiveShader,
                                               mBall);

   // Initialize the FSM
   mFSM->initialize(std::move(mStates), "menu");

   //irrklang::ISound* backgroundMusic = mSoundEngine->play2D("sounds/podington_bear_filaments.mp3", true, false, true);
   //backgroundMusic->setVolume(0.3f);

   return true;
}

GLenum glCheckError_(const char *file, int line)
{
   GLenum errorCode;
   while ((errorCode = glGetError()) != GL_NO_ERROR)
   {
      std::string error;
      switch (errorCode)
      {
      case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
      case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
      case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
      case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
      case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
      }
      std::cout << error << " | " << file << " (" << line << ")" << std::endl;
   }
   return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

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

      glCheckError();
   }
}
