#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

#include <bitset>

class Window
{
public:

   Window(GLuint width, GLuint height, const std::string& title);
   ~Window();

   Window(const Window&) = delete;
   Window& operator=(const Window&) = delete;

   Window(Window&&) = delete;
   Window& operator=(Window&&) = delete;

   bool initialize();

private:

   GLFWwindow*                    mWindow;

   GLuint                         mWidth;
   GLuint                         mHeight;
   std::string                    mTitle;

   std::bitset<GLFW_KEY_LAST + 1> mKeyboard;
   std::bitset<GLFW_KEY_LAST + 1> mProcessedKeyboard;

   float                          mLastCursorXPos;
   float                          mLastCursorYPos;
   float                          mCursorXOffset;
   float                          mCursorYOffset;
   bool                           mFirstCursorPosCallback;

   float                          mScrollYOffset;

   void framebufferSizeCallback(GLFWwindow* window, int width, int height);
   void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
   void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
   void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
};

#endif
