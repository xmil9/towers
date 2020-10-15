//
// Oct-2020, Michael Lindner
// MIT license
//
#include "glfw_window.h"
#include <cassert>

using namespace glfwutil;



namespace
{
///////////////////

void setSelf(GLFWwindow* wnd, Window* self)
{
   glfwSetWindowUserPointer(wnd, self);
}


Window* getSelf(GLFWwindow* wnd)
{
   return reinterpret_cast<Window*>(glfwGetWindowUserPointer(wnd));
}

} // namespace


namespace glfwutil
{
///////////////////

Window::~Window()
{
   destroy();
}


GlfwErr Window::create(int width, int height, const Utf8Char* title)
{
   if (m_wnd)
      destroy();

   m_wnd = glfwCreateWindow(width, height, title, nullptr, nullptr);
   if (m_wnd)
      setupCallbacks();

   return glfwGetError(nullptr);
}


void Window::destroy()
{
   if (m_wnd)
   {
      glfwDestroyWindow(m_wnd);
      m_wnd = nullptr;
   }
}


bool Window::shouldClose() const
{
   if (m_wnd)
      return glfwWindowShouldClose(m_wnd);
   return false;
}


void Window::setShouldClose(bool close)
{
   if (m_wnd)
      glfwSetWindowShouldClose(m_wnd, close ? GLFW_TRUE : GLFW_FALSE);
}


void Window::onWindowResized(int width, int height)
{
   glViewport(0, 0, width, height);
}


void Window::setupCallbacks()
{
   setSelf(m_wnd, this);
   glfwSetWindowSizeCallback(m_wnd, windowResizedCallback);
   glfwSetWindowCloseCallback(m_wnd, windowClosedCallback);
}


void Window::windowResizedCallback(GLFWwindow* wnd, int width, int height)
{
   Window* self = getSelf(wnd);
   assert(self);
   if (self)
      self->onWindowResized(width, height);
}


void Window::windowClosedCallback(GLFWwindow* wnd)
{
   Window* self = getSelf(wnd);
   assert(self);
   if (self)
      self->onWindowClosed();
}

} // namespace glfwutil
