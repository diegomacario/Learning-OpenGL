#include <array>
#include <random>

#include "collision.h"
#include "play_state.h"

void resolveCollisionBetweenBallAndPaddle(Ball& ball, const Paddle& paddle, const glm::vec2& vecFromCenterOfCircleToPointOfCollision);

PlayState::PlayState(const std::shared_ptr<FiniteStateMachine>&     finiteStateMachine,
                     const std::shared_ptr<Window>&                 window,
                     const std::shared_ptr<irrklang::ISoundEngine>& soundEngine,
                     const std::shared_ptr<Camera>&                 camera,
                     const std::shared_ptr<Shader>&                 gameObject3DShader,
                     const std::shared_ptr<GameObject3D>&           table,
                     const std::shared_ptr<Paddle>&                 leftPaddle,
                     const std::shared_ptr<Paddle>&                 rightPaddle,
                     const std::shared_ptr<Ball>&                   ball)
   : mFSM(finiteStateMachine)
   , mWindow(window)
   , mSoundEngine(soundEngine)
   , mCamera(camera)
   , mGameObject3DShader(gameObject3DShader)
   , mTable(table)
   , mLeftPaddle(leftPaddle)
   , mRightPaddle(rightPaddle)
   , mBall(ball)
   , mBallIsInPlay(false)
   , mBallIsFalling(false)
   , mPointsScoredByLeftPaddle(0)
   , mPointsScoredByRightPaddle(0)
{

}

void PlayState::enter()
{
   if (mFSM->getPreviousStateID() != "pause")
   {
      resetCamera();
      resetScene();
      mPointsScoredByLeftPaddle  = 0;
      mPointsScoredByRightPaddle = 0;
   }
}

void PlayState::execute(float deltaTime)
{
   processInput(deltaTime);

   if (mBallIsInPlay)
   {
      update(deltaTime);
   }

   render();
}

void PlayState::exit()
{
   if (mFSM->getCurrentStateID() != "pause")
   {
      if (mFSM->getCurrentStateID() == "win")
      {
         mLeftPaddle->setPosition(glm::vec3(-45.0f, 0.0f, 0.0f));
         mRightPaddle->setPosition(glm::vec3(45.0f, 0.0f, 0.0f));
      }
      else
      {
         resetScene();
      }
   }
}

void PlayState::processInput(float deltaTime)
{
   // Close the game
   if (mWindow->keyIsPressed(GLFW_KEY_ESCAPE)) { mWindow->setShouldClose(true); }

   // Pause the game
   if (mWindow->keyIsPressed(GLFW_KEY_P) && !mWindow->keyHasBeenProcessed(GLFW_KEY_P))
   {
      mWindow->setKeyAsProcessed(GLFW_KEY_P);
      mFSM->changeState("pause");
   }

   // Release the ball
   if (!mBallIsInPlay && mWindow->keyIsPressed(GLFW_KEY_SPACE))
   {
      calculateInitialDirectionOfBall();
      mBallIsInPlay = true;
   }

   // Reset the camera
   if (mWindow->keyIsPressed(GLFW_KEY_R)) { resetCamera(); }

   // Move the camera
   if (mWindow->keyIsPressed(GLFW_KEY_W)) { mCamera->processKeyboardInput(Camera::MovementDirection::Forward, deltaTime); }
   if (mWindow->keyIsPressed(GLFW_KEY_S)) { mCamera->processKeyboardInput(Camera::MovementDirection::Backward, deltaTime); }
   if (mWindow->keyIsPressed(GLFW_KEY_A)) { mCamera->processKeyboardInput(Camera::MovementDirection::Left, deltaTime); }
   if (mWindow->keyIsPressed(GLFW_KEY_D)) { mCamera->processKeyboardInput(Camera::MovementDirection::Right, deltaTime); }

   // Orient the camera
   if (mWindow->mouseMoved())
   {
      mCamera->processMouseMovement(mWindow->getCursorXOffset(), mWindow->getCursorYOffset());
      mWindow->resetMouseMoved();
   }

   // Zoom in or out
   if (mWindow->scrollWheelMoved())
   {
      mCamera->processScrollWheelMovement(mWindow->getScrollYOffset());
      mGameObject3DShader->use();
      mGameObject3DShader->setMat4("projection", mCamera->getPerspectiveProjectionMatrix());
      mWindow->resetScrollWheelMoved();
   }

   // Move the paddles
   if (mWindow->keyIsPressed(GLFW_KEY_Z)) { mLeftPaddle->moveAlongLine(deltaTime, 60.0f, Paddle::MovementDirection::Up); }
   if (mWindow->keyIsPressed(GLFW_KEY_X)) { mLeftPaddle->moveAlongLine(deltaTime, 60.0f, Paddle::MovementDirection::Down); }
   if (mWindow->keyIsPressed(GLFW_KEY_N)) { mRightPaddle->moveAlongLine(deltaTime, 60.0f, Paddle::MovementDirection::Up); }
   if (mWindow->keyIsPressed(GLFW_KEY_M)) { mRightPaddle->moveAlongLine(deltaTime, 60.0f, Paddle::MovementDirection::Down); }
}

void PlayState::update(float deltaTime)
{
   if (!mBallIsFalling && ballIsOutsideOfHorizontalRange())
   {
      updateScore();

      glm::vec3 currVelocity     = mBall->getVelocity();
      glm::vec3 freeFallVelocity = glm::vec3(currVelocity.x * 0.25, currVelocity.y * 0.25, -15.0f);
      mBall->setVelocity(freeFallVelocity);

      mBallIsFalling = true;
   }

   if (mBallIsFalling)
   {
      mBall->moveInFreeFall(deltaTime);

      if (mBall->getPosition().z < -45.0f)
      {
         if (mPointsScoredByLeftPaddle == 3 || mPointsScoredByRightPaddle == 3)
         {
            mFSM->changeState("win");
            return;
         }
         else
         {
            resetScene();
         }
      }
   }
   else
   {
      // TODO: Get the vertical range the table
      mBall->moveWithinVerticalRange(deltaTime, 60.0f);

      glm::vec2 vecFromCenterOfCircleToPointOfCollision;

      if (circleAndAABBCollided(*mBall, *mLeftPaddle, vecFromCenterOfCircleToPointOfCollision))
      {
         playSoundOfCollision();
         resolveCollisionBetweenBallAndPaddle(*mBall, *mLeftPaddle, vecFromCenterOfCircleToPointOfCollision);
      }
      else if (circleAndAABBCollided(*mBall, *mRightPaddle, vecFromCenterOfCircleToPointOfCollision))
      {
         playSoundOfCollision();
         resolveCollisionBetweenBallAndPaddle(*mBall, *mRightPaddle, vecFromCenterOfCircleToPointOfCollision);
      }
   }
}

void PlayState::render()
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Enable depth testing for 3D objects
   glEnable(GL_DEPTH_TEST);

   mGameObject3DShader->use();
   mGameObject3DShader->setMat4("view", mCamera->getViewMatrix());
   mGameObject3DShader->setVec3("cameraPos", mCamera->getPosition());

   mTable->render(*mGameObject3DShader);

   mLeftPaddle->render(*mGameObject3DShader);
   mRightPaddle->render(*mGameObject3DShader);

   glDisable(GL_CULL_FACE);
   mBall->render(*mGameObject3DShader);
   glEnable(GL_CULL_FACE);

   mWindow->swapBuffers();
   mWindow->pollEvents();
}

void PlayState::calculateInitialDirectionOfBall()
{
   std::array<glm::vec3, 4> initialDirections = {glm::vec3( 0.725f,  1.0f, 0.0f),  // Upper right diagonal
                                                 glm::vec3( 0.725f, -1.0f, 0.0f),  // Lower right diagonal
                                                 glm::vec3(-0.725f, -1.0f, 0.0f),  // Lower left diagonal
                                                 glm::vec3(-0.725f,  1.0f, 0.0f)}; // Upper left diagonal

   std::random_device              randomDevice;
   std::mt19937                    randomNumberGenerator(randomDevice());
   std::uniform_int_distribution<> uniformIntDistribution(0, 3);

   int randomIndex = uniformIntDistribution(randomNumberGenerator);

   mBall->setVelocity(glm::length(mBall->getVelocity()) * glm::normalize(initialDirections[randomIndex]));
}

bool PlayState::ballIsOutsideOfHorizontalRange()
{
   glm::vec3 currentPosition = mBall->getPosition();
   float     radius          = mBall->getRadius();

   if ((currentPosition.x + radius < -50.0f) || (currentPosition.x - radius > 50.0f))
   {
      return true;
   }

   return false;
}

void PlayState::updateScore()
{
   glm::vec3 currPos = mBall->getPosition();

   if (currPos.x > 0.0f)
   {
      ++mPointsScoredByLeftPaddle;
   }
   else
   {
      ++mPointsScoredByRightPaddle;
   }
}

void PlayState::resetScene()
{
   mBall->reset();

   mLeftPaddle->setPosition(glm::vec3(-45.0f, 0.0f, 0.0f));
   mRightPaddle->setPosition(glm::vec3(45.0f, 0.0f, 0.0f));

   mBallIsInPlay  = false;
   mBallIsFalling = false;
}

void PlayState::resetCamera()
{
   mCamera->reposition(glm::vec3(0.0f, 0.0f, 95.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f),
                       0.0f,
                       0.0f,
                       45.0f);

   mGameObject3DShader->use();
   mGameObject3DShader->setMat4("projection", mCamera->getPerspectiveProjectionMatrix());
}

void PlayState::playSoundOfCollision()
{
   static double lastPlayed = 0;

   if (glfwGetTime() > lastPlayed + 1)
   {
      mSoundEngine->play2D("sounds/ping_pong_hit.wav", false);
      lastPlayed = glfwGetTime();
   }
}

void resolveCollisionBetweenBallAndPaddle(Ball& ball, const Paddle& paddle, const glm::vec2& vecFromCenterOfCircleToPointOfCollision)
{
   glm::vec3 currVelocity = ball.getVelocity();
   glm::vec3 currPos      = ball.getPosition();

   CollisionDirection collisionDirection = determineDirectionOfCollisionBetweenCircleAndAABB(vecFromCenterOfCircleToPointOfCollision);

   if (collisionDirection == CollisionDirection::Left || collisionDirection == CollisionDirection::Right)
   {
      // Horizontal collision

      // The direction in which the ball bounces off of the paddle depends on the distance between the point where it hits the paddle and the center of the paddle
      // If it hits the center of the paddle, it bounces off completely horizontally
      // If it hits either end of the paddle, it bounces off with the maximum possible angle
      // Note that the speed of the ball is always the same; only its direction changes

      float distanceBetweenCenters              = ball.getPosition().y - paddle.getPosition().y;
      float distanceFromCenterOfPaddleInPercent = distanceBetweenCenters / (paddle.getHeight() / 2.0f);

      float currSpeed = glm::length(currVelocity);
      currVelocity.x  = -currVelocity.x;
      currVelocity.y  = ball.getInitialVelocity().y * distanceFromCenterOfPaddleInPercent;
      currVelocity    = glm::normalize(currVelocity) * currSpeed;

      float horizontalPenetration = ball.getRadius() - glm::abs(vecFromCenterOfCircleToPointOfCollision.x);

      if (collisionDirection == CollisionDirection::Left)
      {
         // Make sure the ball is moving to the right
         if (currVelocity.x < 0.0f)
         {
            currVelocity.x = -1 * currVelocity.x;
         }

         // Move the ball to the right so that it doesn't overlap with the paddle
         currPos.x += horizontalPenetration;
      }
      else // CollisionDirection::Right
      {
         // Make sure the ball is moving to the left
         if (currVelocity.x > 0.0f)
         {
            currVelocity.x = -1 * currVelocity.x;
         }

         // Move the ball to the left so that it doesn't overlap with the paddle
         currPos.x -= horizontalPenetration;
      }
   }
   else
   {
      // Vertical collision
      currVelocity.y = -currVelocity.y;

      float verticalPenetration = ball.getRadius() - glm::abs(vecFromCenterOfCircleToPointOfCollision.y);

      if (collisionDirection == CollisionDirection::Up)
      {
         // Make sure the ball is moving downwards
         if (currVelocity.y > 0.0f)
         {
            currVelocity.y = -1 * currVelocity.y;
         }

         // Move the ball downwards so that it doesn't overlap with the paddle
         currPos.y -= verticalPenetration;
      }
      else // CollisionDirection::Down
      {
         // Make sure the ball is moving upwards
         if (currVelocity.y < 0.0f)
         {
            currVelocity.y = -1 * currVelocity.y;
         }

         // Move the ball upwards so that it doesn't overlap with the paddle
         currPos.y += verticalPenetration;
      }
   }

   ball.setVelocity(currVelocity);
   ball.setPosition(currPos);
}
