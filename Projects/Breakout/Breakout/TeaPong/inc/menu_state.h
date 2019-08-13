#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "game.h"

class MenuState : public State
{
public:

   MenuState(const std::shared_ptr<FiniteStateMachine>& finiteStateMachine,
             const std::shared_ptr<Window>&             window,
             const std::shared_ptr<Shader>&             gameObject3DShader,
             const std::shared_ptr<GameObject3D>&       table,
             const std::shared_ptr<Paddle>&             leftPaddle,
             const std::shared_ptr<Paddle>&             rightPaddle,
             const std::shared_ptr<Ball>&               ball);
   ~MenuState() = default;

   MenuState(const MenuState&) = delete;
   MenuState& operator=(const MenuState&) = delete;

   MenuState(MenuState&&) = delete;
   MenuState& operator=(MenuState&&) = delete;

   void enter() override;
   void execute(float deltaTime) override;
   void exit() override;

private:

   void processInput(float deltaTime);
   void update(float deltaTime);
   void render();

   std::shared_ptr<FiniteStateMachine> mFSM;

   std::shared_ptr<Window>             mWindow;

   std::shared_ptr<Shader>             mGameObject3DShader;

   std::shared_ptr<GameObject3D>       mTable;
   std::shared_ptr<Paddle>             mLeftPaddle;
   std::shared_ptr<Paddle>             mRightPaddle;
   std::shared_ptr<Ball>               mBall;

   glm::vec3 mCameraPosition;
   glm::vec3 mCameraTarget;
   float     mCameraOrbitalAngularVelocity;
   float     mCameraAngularPositionWRTNegativeYAxisInDeg;

   bool   mTransitionToPlayState;
   bool   mFirstIterationOfTransitionToPlayState;

   double mStartTimeOfTransition;

   float mSpeedOfRotationAroundPositiveZAxis;
   float mSpeedOfRotationAroundCameraRight;
   float mSpeedOfMovementAwayFromTarget;
};

#endif
