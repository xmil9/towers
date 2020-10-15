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

   GlfwErr create(int width, int height, const Utf8Char* title);
   void destroy();
   GLFWwindow* handle();

   bool shouldClose() const;
   void setShouldClose(bool close);

protected:
   virtual void onWindowResized(int width, int height);
   virtual void onWindowClosed() {}

private:
   void setupCallbacks();
   static void windowResizedCallback(GLFWwindow* wnd, int width, int height);
   static void windowClosedCallback(GLFWwindow* wnd);

private:
   GLFWwindow* m_wnd = nullptr;
};


inline GLFWwindow* Window::handle()
{
   return m_wnd;
}

} // namespace glfwutil
