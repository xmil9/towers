//
// Nov-2020, Michael Lindner
// MIT license
//
#include "game3.h"


namespace
{

///////////////////

constexpr gfl::Lib::ContextVersion OpenGLVersion{4, 6};
constexpr float MouseSensitivity = 0.05f;
constexpr float ScrollSensitivity = 2.0f;
constexpr float MovementSpeed = 2.5f;

} // namespace


///////////////////

Game3::Game3() : m_glfw{OpenGLVersion, GLFW_OPENGL_CORE_PROFILE}
{
}


bool Game3::setup()
{
   if (!setupUi())
      return false;
   if (!setupInput())
      return false;
   if (!m_renderer.setup())
      return false;
   return true;
}


void Game3::cleanup()
{
   m_renderer.cleanup();
}


void Game3::run()
{
   while (!m_mainWnd.shouldClose())
   {
      m_frameClock.nextLap();
      m_input.process(m_mainWnd, m_frameClock.lapLength(MsToSecs));
      // advanceState();
      m_renderer.renderFrame();
      m_mainWnd.swapBuffers();
   }
}


bool Game3::setupUi()
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


bool Game3::setupMainWindow()
{
   m_mainWnd.setInputController(&m_input);
   m_mainWnd.addObserver([this](MainWindow& src, std::string_view msg,
                                const Observed<MainWindow>::MsgData& data) {
      onMainWindowChanged(src, msg, data);
   });

   if (m_mainWnd.create(800, 800, "towers") != GLFW_NO_ERROR)
      return false;

   m_mainWnd.setCursorMode(GLFW_CURSOR_DISABLED);
   return true;
}


bool Game3::setupInput()
{
   m_input.addObserver(
      [this](Input& /*src*/, std::string_view msg, const Observed<Input>::MsgData& data) {
         onInputChanged(m_input, msg, data);
      });
   return true;
}


void Game3::onMainWindowChanged(MainWindow& /*src*/, std::string_view msg,
                                const Observed<MainWindow>::MsgData& data)
{
   if (msg == WindowResizedMsg)
   {
      const WindowResizedMsgData& resizeData =
         static_cast<const WindowResizedMsgData&>(data);
      onMainWindowResize(resizeData.newSize);
   }
}


void Game3::onMainWindowResize(const glm::ivec2& newSize)
{
   m_renderer.setFrustumAspect(static_cast<float>(newSize.x) /
                               static_cast<float>(newSize.y));
}


void Game3::onInputChanged(Input& /*src*/, std::string_view msg,
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


void Game3::onMouseMoved(const glm::vec2& delta)
{
   m_renderer.updateCameraDirection(delta * MouseSensitivity);
}


void Game3::onMouseScrolled(const glm::vec2& delta)
{
   m_renderer.updateFrustumFov(delta.y * ScrollSensitivity);
}


void Game3::onKeyPolled(gfl::Key key, float frameLengthSecs)
{
   updateCameraPosition(key, frameLengthSecs);
}


std::optional<DirectionXZ> Game3::getCameraDirection(gfl::Key key) const
{
   switch (key)
   {
   case GLFW_KEY_W:
      return DirectionXZ::Forward;
   case GLFW_KEY_S:
      return DirectionXZ::Backward;
   case GLFW_KEY_A:
      return DirectionXZ::Left;
   case GLFW_KEY_D:
      return DirectionXZ::Right;
   default:
      return std::nullopt;
   }
}


void Game3::updateCameraPosition(gfl::Key key, float frameLengthSecs)
{
   const auto camDir = getCameraDirection(key);
   if (camDir)
      m_renderer.updateCameraPosition(*camDir, frameLengthSecs * MovementSpeed);
}
