//
// Nov-2020, Michael Lindner
// MIT license
//
#include "game2.h"


namespace
{

///////////////////

constexpr glfwl::Lib::ContextVersion OpenGLVersion{4, 6};
constexpr float MouseSensitivity = 0.05f;
constexpr float ScrollSensitivity = 2.0f;
constexpr float MovementSpeed = 2.5f;

} // namespace


///////////////////

Game2::Game2() : m_glfw{OpenGLVersion, GLFW_OPENGL_CORE_PROFILE}
{
}


bool Game2::setup()
{
   if (!setupUi())
      return false;
   if (!setupInput())
      return false;
   if (!setupRenderer())
      return false;
   return true;
}


void Game2::cleanup()
{
}


void Game2::run()
{
   while (!m_mainWnd.shouldClose())
   {
      m_frameClock.nextLap();
      m_input.process(m_mainWnd, m_frameClock.lapLength(MsToSecs));
      // advanceState();
      m_renderer.render(m_sprites);
      m_mainWnd.swapBuffers();
   }
}


bool Game2::setupUi()
{
   if (m_glfw.init() != GLFW_NO_ERROR)
      return false;

   if (!setupMainWindow())
      return false;

   glfwMakeContextCurrent(m_mainWnd.handle());

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
      return false;

   return true;
}


bool Game2::setupMainWindow()
{
   m_mainWnd.setInputController(&m_input);

   if (m_mainWnd.create(800, 800, "towers") != GLFW_NO_ERROR)
      return false;

   m_mainWnd.setCursorMode(GLFW_CURSOR_DISABLED);

   m_mainWnd.addObserver([this](AppWindow& src, std::string_view msg,
                                const Observed<AppWindow>::MsgData& data) {
      onMainWindowChanged(src, msg, data);
   });

   return true;
}


bool Game2::setupInput()
{
   m_input.addObserver(
      [this](Input& /*src*/, std::string_view msg, const Observed<Input>::MsgData& data) {
         onInputChanged(m_input, msg, data);
      });
   return true;
}


bool Game2::setupRenderer()
{
   m_renderer.setResources(&m_resources);
   return m_renderer.setupShaders();
}


void Game2::onMainWindowChanged(AppWindow& /*src*/, std::string_view msg,
                                const Observed<AppWindow>::MsgData& data)
{
   if (msg == WindowResizedMsg)
   {
      const WindowResizedMsgData& resizeData =
         static_cast<const WindowResizedMsgData&>(data);
      onMainWindowResize(resizeData.newSize);
   }
}


void Game2::onMainWindowResize(const glm::ivec2& newSize)
{
   m_renderer.setFrustumSize(newSize.x, newSize.y);
}


void Game2::onInputChanged(Input& /*src*/, std::string_view msg,
                           const Observed<Input>::MsgData& data)
{
   if (msg == MouseMovedMsg)
   {
      const auto movedData = static_cast<const MouseMovedMsgData&>(data);
      onMouseMoved(movedData.delta);
   }
   else if (msg == MouseScrolledMsg)
   {
      const auto scrolledData = static_cast<const MouseScrolledMsgData&>(data);
      onMouseScrolled(scrolledData.delta);
   }
   else if (msg == KeyPolledMsg)
   {
      const auto polledData = static_cast<const KeyPolledMsgData&>(data);
      onKeyPolled(polledData.key, polledData.frameLengthSecs);
   }
}


void Game2::onMouseMoved(const glm::vec2& /*delta*/)
{
}


void Game2::onMouseScrolled(const glm::vec2& /*delta*/)
{
}


void Game2::onKeyPolled(Key_t /*key*/, float /*frameLengthSecs*/)
{
}
