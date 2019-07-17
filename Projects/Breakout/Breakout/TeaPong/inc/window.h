#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
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

   bool   initialize();

   bool   shouldClose() const;
   void   setShouldClose(bool shouldClose); // TODO: Could this be considered to be const?
   void   swapBuffers();                    // TODO: Could this be considered to be const?
   void   pollEvents();                     // TODO: Could this be considered to be const?

   // Keyboard
   bool   isKeyPressed(int key) const;
   bool   hasKeyBeenProcessed(int key) const;
   void   setKeyAsProcessed(int key);

   // Cursor
   double getCursorXOffset() const;
   double getCursorYOffset() const;

   // Scroll wheel
   double getScrollYOffset() const;

   bool                           mCursorMoved;

private:

   GLFWwindow*                    mWindow;

   GLuint                         mWidth;
   GLuint                         mHeight;
   std::string                    mTitle;

   // Keyboard
   std::bitset<GLFW_KEY_LAST + 1> mKeys;
   std::bitset<GLFW_KEY_LAST + 1> mProcessedKeys;

   // Cursor
   bool                           mFirstCursorPosCallback;
   double                         mLastCursorXPos;
   double                         mLastCursorYPos;
   double                         mCursorXOffset;
   double                         mCursorYOffset;

   // Scroll wheel
   double                         mScrollYOffset;

   void framebufferSizeCallback(GLFWwindow* window, int width, int height);
   void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
   void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
   void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
};

#endif
