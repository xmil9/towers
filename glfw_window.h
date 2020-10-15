//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "glfw/glfw3.h"
#include "glfw_types.h"


namespace glfwutil
{
///////////////////

class Window
{
 public:
   Window() = default;
   virtual ~Window();
   Window(const Window&) = delete;
   Window(Window&&) = default;

   Window& operator=(const Window&) = delete;
   Window& operator=(Window&&) = default;
   explicit operator bool() const { return m_wnd != nullptr; }
   bool operator!() const { return !operator bool(); }

   GlfwErr create(int width, int height, const Utf8Char* title);
   void destroy();
   GLFWwindow* handle();
   void swapBuffers();

   bool shouldClose() const;
   void setShouldClose(bool close);

 protected:
   virtual void onWindowMoved(int /*xpos*/, int /*ypos*/) {}
   virtual void onWindowResized(int width, int height);
   virtual void onWindowClosed() {}
   virtual void onWindowRefreshed() {}
   virtual void onWindowFocusChanged(int /*focused*/) {}
   virtual void onWindowIconifyChanged(int /*iconified*/) {}
   virtual void onWindowMaximizeChanged(int /*maximized*/) {}
   virtual void onFramebufferResized(int /*width*/, int /*height*/) {}
   virtual void onWindowContentScaled(float /*xscale*/, float /*yscale*/) {}

 private:
   void setupCallbacks();
   static void windowPosCallback(GLFWwindow* wnd, int xpos, int ypos);
   static void windowSizeCallback(GLFWwindow* wnd, int width, int height);
   static void windowCloseCallback(GLFWwindow* wnd);
   static void windowRefreshCallback(GLFWwindow* wnd);
   static void windowFocusCallback(GLFWwindow* wnd, int focused);
   static void windowIconifyCallback(GLFWwindow* wnd, int iconified);
   static void windowMaximizeCallback(GLFWwindow* wnd, int maximized);
   static void framebufferSizeCallback(GLFWwindow* wnd, int width, int height);
   static void windowContentScaleCallback(GLFWwindow* wnd, float xscale, float yscale);

 private:
   GLFWwindow* m_wnd = nullptr;
};


inline GLFWwindow* Window::handle()
{
   return m_wnd;
}

} // namespace glfwutil
