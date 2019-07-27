#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <bitset>

// TODO: Take advantage of inlining in this class.
class Window
{
public:

   Window(GLuint widthInPix, GLuint heightInPix, const std::string& title);
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

   GLuint getWidthInPix() const;
   GLuint getHeightInPix() const;

   // Keyboard
   bool   keyIsPressed(int key) const;
   bool   keyHasBeenProcessed(int key) const;
   void   setKeyAsProcessed(int key);

   // Cursor
   bool   mouseMoved() const;
   void   resetMouseMoved();
   double getCursorXOffset() const;
   double getCursorYOffset() const;

   // Scroll wheel
   bool   scrollWheelMoved() const;
   void   resetScrollWheelMoved();
   double getScrollYOffset() const;

private:

   void   setInputCallbacks();
   void   framebufferSizeCallback(GLFWwindow* window, int width, int height);
   void   keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
   void   cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
   void   scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

   GLFWwindow*                    mWindow;

   GLuint                         mWidthInPix;
   GLuint                         mHeightInPix;
   std::string                    mTitle;

   // Keyboard
   std::bitset<GLFW_KEY_LAST + 1> mKeys;
   std::bitset<GLFW_KEY_LAST + 1> mProcessedKeys;

   // Cursor
   bool                           mMouseMoved;
   bool                           mFirstCursorPosCallback;
   double                         mLastCursorXPos;
   double                         mLastCursorYPos;
   double                         mCursorXOffset;
   double                         mCursorYOffset;

   // Scroll wheel
   bool                           mScrollWheelMoved;
   double                         mScrollYOffset;
};

#endif
