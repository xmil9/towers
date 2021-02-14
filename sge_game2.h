//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "sge_coords.h"
#include "sge_input.h"
#include "sge_main_window.h"
#include "sge_renderer2.h"
#include "sge_types.h"
#include "opengl_util/gfl_lib.h"
#include <string>


namespace sge
{
///////////////////

class Game2
{
 public:
   Game2(PixCoordi wndWidth, PixCoordi wndHeight, const std::string& wndTitle);

   virtual bool setup();
   virtual void run();

 protected:
   virtual void processInput();
   virtual void updateState() {}
   virtual void render();
   virtual void renderItems() {}

   const MainWindow& MainWnd() const { return m_mainWnd; }
   MainWindow& MainWnd() { return m_mainWnd; }
   Renderer2& renderer() { return m_renderer; }
   const Renderer2& renderer() const { return m_renderer; }

   bool isPaused() const { return m_isPaused; }
   void setPaused(bool pause) { m_isPaused = pause; }
   PixPos mousePosition() const { return m_mainWnd.mousePosition(); }

   // Event handling.
   virtual void onMainWindowChanged(MainWindow& src, std::string_view event,
                                    const esl::ObservedEventData& data);
   virtual void onMainWindowResize(const glm::ivec2& newSize);
   virtual void onInputChanged(Input& src, std::string_view event,
                               const esl::ObservedEventData& data);
   virtual void onMouseMoved(const glm::vec2& /*delta*/) {}
   virtual void onMouseScrolled(const glm::vec2& /*delta*/) {}
   virtual void onMouseButtonChanged(gfl::MouseButton button, int action,
                                     const glm::vec2& pos);
   virtual void onLeftButtonPressed(const glm::vec2& /*pos*/) {}
   virtual void onLeftButtonReleased(const glm::vec2& /*pos*/) {}
   virtual void onRightButtonPressed(const glm::vec2& /*pos*/) {}
   virtual void onRightButtonReleased(const glm::vec2& /*pos*/) {}
   virtual void onKeyPolled(gfl::Key /*key*/, float /*frameLengthSecs*/) {}

 private:
   bool setupUi();
   bool setupMainWindow();
   bool setupInput();

 private:
   gfl::Lib m_glfw;
   PixCoordi m_wndWidth = 0;
   PixCoordi m_wndHeight = 0;
   std::string m_wndTitle;
   MainWindow m_mainWnd;
   Input m_input;
   FrameClock m_frameClock;
   bool m_isPaused = true;
   Renderer2 m_renderer;
};

} // namespace sge
