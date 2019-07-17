#include "window.h"

Window::Window(GLuint width, GLuint height, const std::string& title)
   : mWindow(nullptr)
   , mWidth(width)
   , mHeight(height)
   , mTitle(title)
   , mKeyboard()
   , mProcessedKeyboard()
   , mLastCursorXPos(width / 2.0f) // TODO: Is this the best way to initialize a float from an int?
   , mLastCursorYPos(height / 2.0f)
   , mFirstCursorPosCallback(true)
{

}

Window::~Window()
{

}

bool Window::initialize()
{

}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
   glViewport(0, 0, width, height);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (action == GLFW_PRESS)
   {
      mKeyboard[key] = GL_TRUE;
   }
   else if (action == GLFW_RELEASE)
   {
      mKeyboard[key] = GL_FALSE;
   }

   mProcessedKeyboard[key] = GL_FALSE;
}

void Window::cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
   if (firstMouse)
   {
      lastX = xPos;
      lastY = yPos;
      firstMouse = false;
   }

   float xoffset = xPos - lastX;
   float yoffset = lastY - yPos; // Reversed since Y-coordinates go from the bottom to the top

   lastX = xPos;
   lastY = yPos;

   camera.processMouseMovement(xoffset, yoffset);
}

void Window::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
   camera.processMouseScroll(yOffset);
}

