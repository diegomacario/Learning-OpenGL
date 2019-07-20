#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "game.h"

int main(int argc, char* argv[])
{
   Game game(1280, 720, "TeaPong");

   game.gameLoop();

   return 0;
}
