//
// Oct-2020, Michael Lindner
// MIT license
//
#include "gfl_window.h"
#include <cassert>

using namespace gfl;


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


namespace gfl
{
///////////////////

Window::~Window()
{
   destroy();
}


Window::Window(Window&& other) noexcept : m_wnd{other.m_wnd}
{
   other.m_wnd = nullptr;
}


Window& Window::operator=(Window&& other) noexcept
{
   destroy();
   m_wnd = other.m_wnd;
   other.m_wnd = nullptr;
   return *this;
}


Err Window::create(int width, int height, const Utf8Char* title)
{
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


void Window::swapBuffers()
{
   if (m_wnd)
      glfwSwapBuffers(m_wnd);
}


void Window::pollEvents()
{
   glfwPollEvents();
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


std::optional<std::pair<double, double>> Window::cursorPosition() const
{
   if (m_wnd)
   {
      double x = 0.;
      double y = 0.;
      glfwGetCursorPos(m_wnd, &x, &y);
      return std::pair(x, y);
   }
   return std::nullopt;
}


void Window::setCursorMode(int mode)
{
   if (m_wnd)
      glfwSetInputMode(m_wnd, GLFW_CURSOR, mode);
}


void Window::onWindowResized(int width, int height)
{
   // By default adjust viewport accordingly.
   glViewport(0, 0, width, height);
}


void Window::setupCallbacks()
{
   setSelf(m_wnd, this);
   glfwSetWindowPosCallback(m_wnd, windowPosCallback);
   glfwSetWindowSizeCallback(m_wnd, windowSizeCallback);
   glfwSetWindowCloseCallback(m_wnd, windowCloseCallback);
   glfwSetWindowRefreshCallback(m_wnd, windowRefreshCallback);
   glfwSetWindowFocusCallback(m_wnd, windowFocusCallback);
   glfwSetWindowIconifyCallback(m_wnd, windowIconifyCallback);
   glfwSetWindowMaximizeCallback(m_wnd, windowMaximizeCallback);
   glfwSetFramebufferSizeCallback(m_wnd, framebufferSizeCallback);
   glfwSetWindowContentScaleCallback(m_wnd, windowContentScaleCallback);
   glfwSetCursorPosCallback(m_wnd, windowCursorPosCallback);
   glfwSetScrollCallback(m_wnd, windowScrollCallback);
   glfwSetKeyCallback(m_wnd, windowKeyCallback);
   glfwSetMouseButtonCallback(m_wnd, windowMouseButtonCallback);
}


void Window::windowPosCallback(GLFWwindow* wnd, int xpos, int ypos)
{
   Window* self = getSelf(wnd);
   assert(self);
   if (self)
      self->onWindowMoved(xpos, ypos);
}


void Window::windowSizeCallback(GLFWwindow* wnd, int width, int height)
{
   Window* self = getSelf(wnd);
   assert(self);
   if (self)
      self->onWindowResized(width, height);
}


void Window::windowCloseCallback(GLFWwindow* wnd)
{
   Window* self = getSelf(wnd);
   assert(self);
   if (self)
      self->onWindowClosed();
}


void Window::windowRefreshCallback(GLFWwindow* wnd)
{
   Window* self = getSelf(wnd);
   assert(self);
   if (self)
      self->onWindowRefreshed();
}


void Window::windowFocusCallback(GLFWwindow* wnd, int focused)
{
   Window* self = getSelf(wnd);
   assert(self);
   if (self)
      self->onWindowFocusChanged(focused);
}


void Window::windowIconifyCallback(GLFWwindow* wnd, int iconified)
{
   Window* self = getSelf(wnd);
   assert(self);
   if (self)
      self->onWindowIconifyChanged(iconified);
}


void Window::windowMaximizeCallback(GLFWwindow* wnd, int maximized)
{
   Window* self = getSelf(wnd);
   assert(self);
   if (self)
      self->onWindowMaximizeChanged(maximized);
}


void Window::framebufferSizeCallback(GLFWwindow* wnd, int width, int height)
{
   Window* self = getSelf(wnd);
   assert(self);
   if (self)
      self->onFramebufferResized(width, height);
}


void Window::windowContentScaleCallback(GLFWwindow* wnd, float xscale, float yscale)
{
   Window* self = getSelf(wnd);
   assert(self);
   if (self)
      self->onWindowContentScaled(xscale, yscale);
}


void Window::windowCursorPosCallback(GLFWwindow* wnd, double xpos, double ypos)
{
   Window* self = getSelf(wnd);
   assert(self);
   if (self)
      self->onWindowMouseMoved(xpos, ypos);
}


void Window::windowScrollCallback(GLFWwindow* wnd, double xoffset, double yoffset)
{
   Window* self = getSelf(wnd);
   assert(self);
   if (self)
      self->onWindowScrolled(xoffset, yoffset);
}


void Window::windowKeyCallback(GLFWwindow* wnd, Key key, int scancode, int action,
                               int mods)
{
   Window* self = getSelf(wnd);
   assert(self);
   if (self)
      self->onWindowKeyChanged(key, scancode, action, mods);
}


void Window::windowMouseButtonCallback(GLFWwindow* wnd, MouseButton button, int action,
                                       int mods)
{
   double x = 0.;
   double y = 0.;
   glfwGetCursorPos(wnd, &x, &y);

   Window* self = getSelf(wnd);
   assert(self);
   if (self)
      self->onWindowMouseButtonChanged(button, action, mods, x, y);
}

} // namespace gfl
