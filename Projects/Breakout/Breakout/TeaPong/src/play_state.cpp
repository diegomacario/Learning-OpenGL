#include "play_state.h"

bool circleAndAABBCollided(const Ball& circle, const Paddle& AABB);

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

   // TODO: Perform collision detection here
   if (circleAndAABBCollided(*mBall, *mLeftPaddle))
   {
      // Check where it hit the board, and change velocity based on where it hit the board
      //GLfloat distanceBetweenCenters          = mBall->getPosition().y - mLeftPaddle->getPosition().y;
      //GLfloat distanceBetweenCentersInPercent = distanceBetweenCenters / (mLeftPaddle->getHeight() / 2.0f);

      // Then move accordingly
      //glm::vec2 oldVelocity = Ball->Velocity;

      //Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage;
      //Ball->Velocity.y = -Ball->Velocity.y;

      // Keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
      //Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
      // Fix sticky paddle
      //Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
      glm::vec3 ballVelocity = mBall->getVelocity();
      mBall->setVelocity(glm::vec3(-ballVelocity.x, ballVelocity.y, ballVelocity.z));
   }
   else if (circleAndAABBCollided(*mBall, *mRightPaddle))
   {
      glm::vec3 ballVelocity = mBall->getVelocity();
      mBall->setVelocity(glm::vec3(-ballVelocity.x, ballVelocity.y, ballVelocity.z));
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

bool circleAndAABBCollided(const Ball& circle, const Paddle& AABB)
{
   glm::vec2 centerOfCircle(circle.getPosition());

   glm::vec2 centerOfAABB(AABB.getPosition());
   glm::vec2 halfExtentsOfAABB(AABB.getWidth() / 2, AABB.getHeight() / 2);

   // Calculate the difference vector between both centers and clamp it to the AABB's half-extents
   glm::vec2 diff = centerOfCircle - centerOfAABB;
   glm::vec2 clampedDiff = glm::clamp(diff, -halfExtentsOfAABB, halfExtentsOfAABB);

   // Add the clamped difference vector to the AABB's center to get the point on the AABB's edge that is closest to the center of the circle
   glm::vec2 closest = centerOfAABB + clampedDiff;

   // Calculate the difference vector between the center of the circle and the closest point on the AABB's edge
   diff = closest - centerOfCircle;

   // Check if the distance between the center of the circle and the closest point on the AABB's edge is smaller than the radius of the circle, which would indicate a collision
   // Note that the check is not <= because in that case, a collision would also occur when the circle and the AABB are exactly touching each other,
   // which is the state in which we leave the circle and the AABB after they collide
   if (glm::length(diff) < circle.getRadius())
   {
      return true;
   }
   else
   {
      return false;
   }
}
