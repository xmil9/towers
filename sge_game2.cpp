//
// Nov-2020, Michael Lindner
// MIT license
//
#include "sge_game2.h"


namespace
{
///////////////////

constexpr gfl::Lib::ContextVersion OpenGLVersion{4, 6};

} // namespace


namespace sge
{
///////////////////

Game2::Game2(PixCoordi wndWidth, PixCoordi wndHeight, const std::string& wndTitle)
: m_glfw{OpenGLVersion, GLFW_OPENGL_CORE_PROFILE}, m_wndWidth{wndWidth},
  m_wndHeight{wndHeight}, m_wndTitle{wndTitle}
{
}


bool Game2::setup()
{
   return setupUi() && setupInput();
}


void Game2::run()
{
   m_frameClock.nextLap();
   float processLag = 0.f;

   while (!m_mainWnd.shouldClose())
   {
      m_frameClock.nextLap();
      if (!m_isPaused)
         processLag += m_frameClock.lapLength();

      processInput();

      if (!m_isPaused)
      {
         // Catch up with the game state for the time that was used for rendering and
         // processing input. This standardizes the game speed across machines with
         // various processing speeds.
         constexpr float UpdateGranularityMs = 25.f;
         while (processLag >= UpdateGranularityMs)
         {
            updateState();
            processLag -= UpdateGranularityMs;
         }
      }

      render();
   }
}


void Game2::render()
{
   m_renderer.clearScene();

   m_renderer.beginRendering();
   renderItems();

   m_mainWnd.swapBuffers();
}


void Game2::processInput()
{
   m_input.process(m_mainWnd, m_frameClock.lapLength(MsToSecs));
}


void Game2::onMainWindowChanged(MainWindow& /*src*/, std::string_view event,
                                const esl::ObservedEventData& data)
{
   if (event == WindowResizedEvent)
   {
      const WindowResizedData& resizeData = static_cast<const WindowResizedData&>(data);
      onMainWindowResize(resizeData.newSize);
   }
}


void Game2::onMainWindowResize(const glm::ivec2& newSize)
{
   m_renderer.setFrustumSize(newSize.x, newSize.y);
}


void Game2::onInputChanged(Input& /*src*/, std::string_view event,
                           const esl::ObservedEventData& data)
{
   if (event == MouseMovedEvent)
   {
      const auto& movedData = static_cast<const MouseMovedData&>(data);
      onMouseMoved(movedData.delta);
   }
   else if (event == MouseScrolledEvent)
   {
      const auto& scrolledData = static_cast<const MouseScrolledData&>(data);
      onMouseScrolled(scrolledData.delta);
   }
   else if (event == MouseButtonChangedEvent)
   {
      const auto& buttonData = static_cast<const MouseButtonChangedData&>(data);
      onMouseButtonChanged(buttonData.button, buttonData.action, buttonData.pos);
   }
   else if (event == KeyPolledEvent)
   {
      const auto& polledData = static_cast<const KeyPolledData&>(data);
      onKeyPolled(polledData.key, polledData.frameLengthSecs);
   }
}


void Game2::onMouseButtonChanged(gfl::MouseButton button, int action,
                                 const glm::vec2& pos)
{
   if (button == GLFW_MOUSE_BUTTON_1)
   {
      if (action == GLFW_PRESS)
         onLeftButtonPressed(pos);
      else
         onLeftButtonReleased(pos);
   }
   else
   {
      if (action == GLFW_PRESS)
         onRightButtonPressed(pos);
      else
         onRightButtonReleased(pos);
   }
}


bool Game2::setupUi()
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


bool Game2::setupMainWindow()
{
   m_mainWnd.setInputController(&m_input);
   m_mainWnd.addObserver([this](MainWindow& src, std::string_view event,
                                const esl::ObservedEventData& data) {
      onMainWindowChanged(src, event, data);
   });

   if (m_mainWnd.create(m_wndWidth, m_wndHeight, m_wndTitle.c_str()) != GLFW_NO_ERROR)
      return false;

   // Hide mouse cursor and capture its events even outside the main window.
   // m_mainWnd.setCursorMode(GLFW_CURSOR_DISABLED);
   return true;
}


bool Game2::setupInput()
{
   m_input.addObserver(
      [this](Input& /*src*/, std::string_view event, const esl::ObservedEventData& data) {
         onInputChanged(m_input, event, data);
      });
   return true;
}

} // namespace sge
