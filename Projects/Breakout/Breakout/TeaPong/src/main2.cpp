#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "game.h"

int main(int argc, char* argv[])
{
   Game game();

   if (game.initialize(1280, 720, "TeaPong"))
   {
      std::cout << "Error - main - Failed to initialize the game" << "\n";
      return -1;     
   }

   game.gameLoop();

   return 0;
}
