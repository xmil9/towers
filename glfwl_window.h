//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "glfw/glfw3.h"
#include "glfwl_types.h"


namespace glfwl
{
///////////////////

class Window
{
 public:
   Window() = default;
   virtual ~Window();
   Window(const Window&) = delete;
   Window(Window&& other);

   Window& operator=(const Window&) = delete;
   Window& operator=(Window&& other);
   explicit operator bool() const { return m_wnd != nullptr; }
   bool operator!() const { return !operator bool(); }

   GlfwErr create(int width, int height, const Utf8Char* title);
   void destroy();
   GLFWwindow* handle() { return m_wnd; }
   void swapBuffers();
   void pollEvents();

   bool shouldClose() const;
   void setShouldClose(bool close);

   // Sets the mouse cursor mode. Values:
   // GLFW_CURSOR_NORMAL, GLFW_CURSOR_HIDDEN, GLFW_CURSOR_DISABLED
   void setCursorMode(int mode);

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
   virtual void onWindowMouseMoved(double /*xpos*/, double /*ypos*/) {}
   virtual void onWindowScrolled(double /*xoffset*/, double /*yoffset*/) {}
   virtual void onWindowKeyChanged(int /*key*/, int /*scancode*/, int /*action*/,
                                   int /*mods*/)
   {
   }

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
   static void windowCursorPosCallback(GLFWwindow* wnd, double xpos, double ypos);
   static void windowScrollCallback(GLFWwindow* wnd, double xoffset, double yoffset);
   static void windowKeyCallback(GLFWwindow* wnd, int key, int scancode, int action,
                                 int mods);

 private:
   GLFWwindow* m_wnd = nullptr;
};

} // namespace glfwl
