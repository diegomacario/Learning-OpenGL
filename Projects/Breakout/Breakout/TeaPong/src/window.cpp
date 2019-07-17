#include "window.h"

Window::Window(GLuint width, GLuint height, const std::string& title)
   : mWindow(nullptr)
   , mWidth(width)
   , mHeight(height)
   , mTitle(title)
   , mKeyboard()
   , mProcessedKeyboard()
   , mLastCursorXPos(0.0f)
   , mLastCursorYPos(0.0f)
   , mCursorXOffset(0.0f)
   , mCursorYOffset(0.0f)
   , mFirstCursorPosCallback(true)
   , mScrollYOffset(0.0f)
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
   mWidth = width;
   mHeight = height;
   glViewport(0, 0, width, height);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (action == GLFW_PRESS)
   {
      mKeyboard.set(key);
   }
   else if (action == GLFW_RELEASE)
   {
      mKeyboard.reset(key);
   }

   mProcessedKeyboard.reset(key);
}

void Window::cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
   if (mFirstCursorPosCallback)
   {
      mLastCursorXPos = xPos;
      mLastCursorYPos = yPos;
      mFirstCursorPosCallback = false;
   }

   // TODO: Ideally this function would tell the camera to update its position based on the offsets. I'm going to make the camera ask the window if it should update its position. Is there a better way to do this?
   mCursorXOffset = xPos - mLastCursorXPos;
   mCursorYOffset = mLastCursorYPos - yPos; // Reversed since the Y-coordinates of the mouse go from the bottom to the top

   mLastCursorXPos = xPos;
   mLastCursorYPos = yPos;
}

void Window::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
   // TODO: Ideally this function would tell the camera to update its FOVY based on the Y offset. I'm going to make the camera ask the window if it should update its FOVY. Is there a better way to do this?
   mScrollYOffset = yOffset;
}

