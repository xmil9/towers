//
// Nov-2020, Michael Lindner
// MIT license
//
#include "game2.h"
#include "map_data.h"
#include "mesh2.h"
#include "gll_debug.h"
#include <iostream>


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
   return (setupUi() && setupInput() && setupOutput() && setupResources() &&
           setupRenderer() && setupTerrain() && setupSpriteData() && setupAttackers() &&
           setupBackground());
}


void Game2::cleanup()
{
}


void Game2::run()
{
   while (!m_mainWnd.shouldClose())
   {
      m_frameClock.nextLap();
      processInput();
      updateState();
      render();
   }
}


bool Game2::setupUi()
{
   if (m_glfw.init(glfwl::Lib::DebugOuput::On) != GLFW_NO_ERROR)
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
   m_mainWnd.addObserver([this](MainWindow& src, std::string_view msg,
                                const Observed<MainWindow>::MsgData& data) {
      onMainWindowChanged(src, msg, data);
   });

   if (m_mainWnd.create(MainWndWidth, MainWndHeight, "towers") != GLFW_NO_ERROR)
      return false;

   // Hide mouse cursor and capture its events even outside the main window.
   // m_mainWnd.setCursorMode(GLFW_CURSOR_DISABLED);
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


bool Game2::setupOutput()
{
   if (gll::haveDebugContext())
      gll::setDebugOutputCallback();
   return true;
}


bool Game2::setupResources()
{
   return m_resources.loadTexture("attacker",
                                  m_resources.texturePath() / "attacker.png") &&
          m_resources.loadTexture("map1", m_resources.texturePath() / "map1.png");
}


bool Game2::setupRenderer()
{
   return m_renderer.setup(&m_resources, MainWndWidth, MainWndHeight);
}


bool Game2::setupTerrain()
{
   m_coordSys = std::make_unique<MapCoordSys>(PixDim{MainWndWidth, MainWndHeight});

   std::optional<MapData> mapData = loadMapData(m_resources.mapPath() / "map.json");
   if (!mapData)
      return false;
   m_map = std::make_unique<Map>(std::move(*mapData));

   return true;
}


bool Game2::setupSpriteData()
{
   // Coord system for vertex coordinates is:
   // (0, 0) - left-top, (1, 1) - right-bottom
   const std::vector<Mesh2::Point> positions = {
      {0.f, 1.f}, {1.f, 1.f}, {1.f, 0.f}, {0.f, 0.f}};
   // Triangle vertices are ordered ccw.
   const std::vector<Mesh2::VertexIdx> indices = {0, 1, 2, 2, 3, 0};
   // Coord system for texture coordinates is:
   // (0, 0) - left-bottom, (1, 1) - right-top
   const std::vector<Mesh2::Point> texCoords = positions;

   Mesh2 mesh;
   mesh.setPositions(std::move(positions));
   mesh.setIndices(std::move(indices));
   mesh.setTextureCoords(std::move(texCoords));
   m_stdSpriteRenderer = std::make_unique<SpriteRenderer>(&m_resources);
   m_stdSpriteRenderer->setMesh(mesh);

   return true;
}


bool Game2::setupAttackers()
{
   assert(!!m_coordSys);
   assert(!!m_map);

   const std::string texId{"attacker"};
   SpriteLook look{texId};

   SpriteForm form1{{}, {}, Angle_t{0.f}};
   assert(!!m_stdSpriteRenderer);
   Sprite sprite1{m_stdSpriteRenderer.get(), look, form1};

   m_attackers.emplace_back(
      sprite1, m_coordSys->makeEquivalentMapSize(.03f, m_resources.getTextureSize(texId)),
      0, .001f, OffsetPath{&m_map->path(), NormVec{0.001, 0.002}}, *m_coordSys);

   SpriteForm form2{{}, {}, Angle_t::fromDegrees(0.f)};
   assert(!!m_stdSpriteRenderer);
   Sprite sprite2{m_stdSpriteRenderer.get(), look, form2};

   m_attackers.emplace_back(
      sprite2, m_coordSys->makeEquivalentMapSize(.015f, m_resources.getTextureSize(texId)),
      0, .002f, OffsetPath{&m_map->path(), NormVec{-0.001, 0.003}}, *m_coordSys);
   return true;
}


bool Game2::setupBackground()
{
   const std::string texId{"map1"};
   SpriteLook look{texId};
   SpriteForm form{{0.f, 0.f}, {MainWndWidth, MainWndHeight}, Angle_t{0.f}};
   assert(!!m_stdSpriteRenderer);
   m_background = std::make_unique<Sprite>(m_stdSpriteRenderer.get(), look, form);

   return true;
}


void Game2::processInput()
{
   m_input.process(m_mainWnd, m_frameClock.lapLength(MsToSecs));
}


void Game2::updateState()
{
   for (auto& attacker : m_attackers)
      attacker.update();
}


void Game2::render()
{
   m_renderer.beginRendering();
   m_background->render(m_renderer.shaders());
   for (const auto& attacker : m_attackers)
      attacker.render(m_renderer.shaders());

   m_mainWnd.swapBuffers();
}


void Game2::onMainWindowChanged(MainWindow& /*src*/, std::string_view msg,
                                const Observed<MainWindow>::MsgData& data)
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


void Game2::onKeyPolled(glfwl::Key /*key*/, float /*frameLengthSecs*/)
{
}
