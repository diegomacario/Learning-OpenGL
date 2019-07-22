#include <iostream>

#include <shader_loader.h>
#include <texture_loader.h>
#include <model_loader.h>
#include "game.h"

Game::Game()
   : mWindow()
   , mStateMachine()
   , mGameStates()
   , mCamera()
   , mRenderer2D()
   , mModelManager()
   , mTextureManager()
   , mShaderManager()
   , mBackground()
   , mLeftPaddle()
   , mRightPaddle()
   , mBall()
{

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
   glm::mat4 perspectiveProjection = glm::perspective(glm::radians(mCamera->getFieldOfViewYInDeg()),                                               // FOVY
                                                      static_cast<GLfloat>(mWindow->getWidth()) / static_cast<GLfloat>(mWindow->getHeight()), // Aspect ratio
                                                      0.1f,                                                                                   // Near
                                                      100.0f);                                                                                // Far
   auto gameObject3DShader = mShaderManager.loadResource<ShaderLoader>("game_object_3D", "shaders/game_object_3D.vs", "shaders/game_object_3D.fs");
   gameObject3DShader->use();
   gameObject3DShader->setMat4("projection", perspectiveProjection);

   // Load the models
   mModelManager.loadResource<ModelLoader>("teapot", "models/teapot_w_emissive/teapot.obj");

   // Load the textures
   mTextureManager.loadResource<TextureLoader>("table_cloth", "textures/table_cloth.jpg");

   // Create the game objects
   mBackground = std::make_unique<GameObject2D>(mTextureManager.getResource("table_cloth"),
                                                glm::vec2(0.0f, 0.0f),
                                                0.0f,
                                                static_cast<float>(mWindow->getWidth()),
                                                static_cast<float>(mWindow->getHeight()));

   mBall = std::make_unique<MovableGameObject3D>(mModelManager.getResource("teapot"),
                                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                                 90.0f,
                                                 glm::vec3(1.0f, 0.0f, 0.0f),
                                                 glm::vec3(0.1f, 0.1f, 0.1f),
                                                 glm::vec3(0.0f, 0.0f, 0.0f));

   return true;
}

void Game::update(GLfloat deltaTime)
{
   //mStateMachine->update();
}

void Game::gameLoop()
{
   double deltaTime = 0.0;
   double lastFrame = 0.0;

   while (!mWindow->shouldClose())
   {
      double currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      // Process input
      // ----------------------------------------------------------------------------------------------------

      if (mWindow->isKeyPressed(GLFW_KEY_ESCAPE))
         mWindow->setShouldClose(true);

      if (mWindow->isKeyPressed(GLFW_KEY_W))
         mCamera->processKeyboardInput(MovementDirection::Forward, deltaTime);
      if (mWindow->isKeyPressed(GLFW_KEY_S))
         mCamera->processKeyboardInput(MovementDirection::Backward, deltaTime);
      if (mWindow->isKeyPressed(GLFW_KEY_A))
         mCamera->processKeyboardInput(MovementDirection::Left, deltaTime);
      if (mWindow->isKeyPressed(GLFW_KEY_D))
         mCamera->processKeyboardInput(MovementDirection::Right, deltaTime);

      if (mWindow->mouseMoved())
      {
         mCamera->processMouseMovement(mWindow->getCursorXOffset(), mWindow->getCursorYOffset());
         mWindow->resetMouseMoved();
      }

      if (mWindow->scrollWheelMoved())
      {
         mCamera->processScrollWheelMovement(mWindow->getScrollYOffset());
         mWindow->resetScrollWheelMoved();
      }

      // ----------------------------------------------------------------------------------------------------

      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Render
      // ----------------------------------------------------------------------------------------------------

      // Disable depth testing for 2D objects
      glDisable(GL_DEPTH_TEST);

      mRenderer2D->render(*mBackground);

      // Enable depth testing for 3D objects
      glEnable(GL_DEPTH_TEST);

      auto gameObject3DShader = mShaderManager.getResource("game_object_3D");
      gameObject3DShader->use(); // TODO: This is being done twice. Here and inside GameObject3D::Render().
      gameObject3DShader->setMat4("model", mBall->getModelMatrix()); // TODO: This shoud be done internally
      gameObject3DShader->setMat4("view", mCamera->getViewMatrix());
      mBall->render(*gameObject3DShader, true);

      // ----------------------------------------------------------------------------------------------------

      mWindow->swapBuffers();
      mWindow->pollEvents();
   }
}
