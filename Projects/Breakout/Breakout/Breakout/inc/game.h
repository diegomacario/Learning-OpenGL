/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H
#include <vector>
#include <tuple>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game_level.h"

// Represents the current state of the game
enum GameState
{
   GAME_ACTIVE,
   GAME_MENU,
   GAME_WIN
};

// Represents the four possible (collision) directions
// Joey made the mistake of inverting UP and DOWN
enum Direction
{
    UP,    // (0.0f, 1.0f)
    RIGHT, // (1.0f, 0.0f)
    DOWN,  // (0.0f, -1.0f)
    LEFT   // (-1.0f, 0.0f)
};

// 0) Did they collide?
// 1) In what direction was the ball moving?
// 2) Center of circle - closest point on AABB
typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100, 20);
// Initial velocity of the player paddle
const GLfloat PLAYER_VELOCITY(500.0f);
// Radius of the ball object
const GLfloat BALL_RADIUS = 12.5f;
// Initial velocity of the Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
   // Game state
   GameState              State;
   GLboolean              Keys[1024];
   GLuint                 Width, Height;
   std::vector<GameLevel> Levels;
   GLuint                 Level;

   // Constructor/Destructor
   Game(GLuint width, GLuint height);
   ~Game();

   // Initialize game state (load all shaders/textures/levels)
   void Init();

   // GameLoop
   void ProcessInput(GLfloat dt);
   void Update(GLfloat dt);
   void Render();

   void DoCollisions();

   // Reset
   void ResetLevel();
   void ResetPlayer();
};

#endif
