//
// Nov-2020, Michael Lindner
// MIT license
//
#include "game3.h"
#include "opengl_util/gll_debug.h"


namespace
{

///////////////////

constexpr gfl::Lib::ContextVersion OpenGLVersion{4, 6};
constexpr float MouseSensitivity = 0.05f;
constexpr float ScrollSensitivity = 2.0f;
constexpr float MovementSpeed = 2.5f;
constexpr float MsToSecs = 0.001f;

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
   if (!setupOutput())
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
   if (m_glfw.init(gfl::Lib::DebugOuput::On) != GLFW_NO_ERROR)
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
   m_mainWnd.addObserver([this](sp::MainWindow& src, std::string_view event,
                                const esl::ObservedEventData& data) {
      onMainWindowChanged(src, event, data);
   });

   if (m_mainWnd.create(800, 800, "towers") != GLFW_NO_ERROR)
      return false;

   m_mainWnd.setCursorMode(GLFW_CURSOR_DISABLED);
   return true;
}


bool Game3::setupInput()
{
   m_input.addObserver([this](sp::Input& /*src*/, std::string_view event,
                              const esl::ObservedEventData& data) {
      onInputChanged(m_input, event, data);
   });
   return true;
}


bool Game3::setupOutput()
{
   if (gll::haveDebugContext())
      gll::setDebugOutputCallback();
   return true;
}


void Game3::onMainWindowChanged(sp::MainWindow& /*src*/, std::string_view event,
                                const esl::ObservedEventData& data)
{
   if (event == sp::WindowResizedEvent)
   {
      const sp::WindowResizedData& resizeData =
         static_cast<const sp::WindowResizedData&>(data);
      onMainWindowResize(resizeData.newSize);
   }
}


void Game3::onMainWindowResize(const glm::ivec2& newSize)
{
   m_renderer.setFrustumAspect(static_cast<float>(newSize.x) /
                               static_cast<float>(newSize.y));
}


void Game3::onInputChanged(sp::Input& /*src*/, std::string_view event,
                           const esl::ObservedEventData& data)
{
   if (event == sp::MouseMovedEvent)
   {
      const auto movedData = static_cast<const sp::MouseMovedData&>(data);
      onMouseMoved(movedData.delta);
   }
   else if (event == sp::MouseScrolledEvent)
   {
      const auto scrolledData = static_cast<const sp::MouseScrolledData&>(data);
      onMouseScrolled(scrolledData.delta);
   }
   else if (event == sp::KeyPolledEvent)
   {
      const auto polledData = static_cast<const sp::KeyPolledData&>(data);
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


std::optional<sp::DirectionXZ> Game3::getCameraDirection(gfl::Key key) const
{
   switch (key)
   {
   case GLFW_KEY_W:
      return sp::DirectionXZ::Forward;
   case GLFW_KEY_S:
      return sp::DirectionXZ::Backward;
   case GLFW_KEY_A:
      return sp::DirectionXZ::Left;
   case GLFW_KEY_D:
      return sp::DirectionXZ::Right;
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
