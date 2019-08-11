#include "constants.h"
#include "collision.h"
#include "play_state.h"

PlayState::PlayState(const std::shared_ptr<Window>&       window,
                     const std::shared_ptr<Camera>&       camera,
                     const std::shared_ptr<Shader>&       gameObject3DShader,
                     const std::shared_ptr<GameObject3D>& table,
                     const std::shared_ptr<Paddle>&       leftPaddle,
                     const std::shared_ptr<Paddle>&       rightPaddle,
                     const std::shared_ptr<Ball>&         ball)
   : mWindow(window)
   , mCamera(camera)
   , mGameObject3DShader(gameObject3DShader)
   , mTable(table)
   , mLeftPaddle(leftPaddle)
   , mRightPaddle(rightPaddle)
   , mBall(ball)
{

}

void PlayState::enter()
{

}

void PlayState::execute(float deltaTime)
{
   processInput(deltaTime);
   update(deltaTime);
   render();
}

void PlayState::exit()
{

}

void PlayState::processInput(float deltaTime)
{
   // Close the game
   if (mWindow->keyIsPressed(GLFW_KEY_ESCAPE)) { mWindow->setShouldClose(true); }

   // Move the camera
   if (mWindow->keyIsPressed(GLFW_KEY_W))      { mCamera->processKeyboardInput(Camera::MovementDirection::Forward, deltaTime); }
   if (mWindow->keyIsPressed(GLFW_KEY_S))      { mCamera->processKeyboardInput(Camera::MovementDirection::Backward, deltaTime); }
   if (mWindow->keyIsPressed(GLFW_KEY_A))      { mCamera->processKeyboardInput(Camera::MovementDirection::Left, deltaTime); }
   if (mWindow->keyIsPressed(GLFW_KEY_D))      { mCamera->processKeyboardInput(Camera::MovementDirection::Right, deltaTime); }

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
   // TODO: Get the dimensions from the table
   mBall->moveWithinArea(deltaTime, 100.0f, 60.0f);

   glm::vec2 vecFromCenterOfCircleToPointOfCollision;

   if (circleAndAABBCollided(*mBall, *mLeftPaddle, vecFromCenterOfCircleToPointOfCollision))
   {
      resolveCollisionBetweenBallAndPaddle(*mBall, *mLeftPaddle, vecFromCenterOfCircleToPointOfCollision);
   }
   else if (circleAndAABBCollided(*mBall, *mRightPaddle, vecFromCenterOfCircleToPointOfCollision))
   {
      resolveCollisionBetweenBallAndPaddle(*mBall, *mRightPaddle, vecFromCenterOfCircleToPointOfCollision);
   }

   // TODO: Check if we have a winner here
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

      GLfloat distanceBetweenCenters              = ball.getPosition().y - paddle.getPosition().y;
      GLfloat distanceFromCenterOfPaddleInPercent = distanceBetweenCenters / (paddle.getHeight() / 2.0f);

      float currSpeed = glm::length(currVelocity);
      currVelocity.x  = -currVelocity.x;
      currVelocity.y  = Constants::kInitialBallVelocity.y * distanceFromCenterOfPaddleInPercent;
      currVelocity    = glm::normalize(currVelocity) * currSpeed;

      float horizontalPenetration = ball.getRadius() - std::abs(vecFromCenterOfCircleToPointOfCollision.x);

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

      float verticalPenetration = ball.getRadius() - std::abs(vecFromCenterOfCircleToPointOfCollision.y);

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
