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

constexpr gfl::Lib::ContextVersion OpenGLVersion{4, 6};
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
           setupRenderer() && setupTerrain() && setupSpriteData() && setupAnimations() &&
           setupAttackers() && setupDefenders() && setupBackground());
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
          m_resources.loadTexture("defender",
                                  m_resources.texturePath() / "defender.png") &&
          m_resources.loadTexture("explosion1",
                                  m_resources.texturePath() / "explosion1.png") &&
          m_resources.loadTexture("explosion2",
                                  m_resources.texturePath() / "explosion2.png") &&
          m_resources.loadTexture("explosion3",
                                  m_resources.texturePath() / "explosion3.png") &&
          m_resources.loadTexture("explosion4",
                                  m_resources.texturePath() / "explosion4.png") &&
          m_resources.loadTexture("defender_firing1",
                                  m_resources.texturePath() / "defender_firing1.png") &&
          m_resources.loadTexture("defender_firing2",
                                  m_resources.texturePath() / "defender_firing2.png") &&
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


bool Game2::setupAnimations()
{
   assert(!!m_coordSys);
   assert(!!m_stdSpriteRenderer);

   const SpriteForm form{PixDim{40.f, 40.f}, Angle_t{0.f}};
   std::vector<Sprite> sprites{
      Sprite{m_stdSpriteRenderer.get(), SpriteLook{"explosion1"}, form},
      Sprite{m_stdSpriteRenderer.get(), SpriteLook{"explosion2"}, form},
      Sprite{m_stdSpriteRenderer.get(), SpriteLook{"explosion3"}, form},
      Sprite{m_stdSpriteRenderer.get(), SpriteLook{"explosion4"}, form},
   };
   std::vector<int> frames{15, 15, 15, 15};
   m_explosion = std::make_unique<Animation>(sprites, frames, false, m_coordSys.get());

   const PixDim firingSize = m_coordSys->toRenderCoords(
      m_coordSys->makeEquivalentMapSize(.04f, m_resources.getTextureSize("defender")));
   const SpriteForm form2{firingSize, Angle_t{0.f}};
   std::vector<Sprite> sprites2{
      Sprite{m_stdSpriteRenderer.get(), SpriteLook{"defender_firing1"}, form2},
      Sprite{m_stdSpriteRenderer.get(), SpriteLook{"defender_firing2"}, form2},
   };
   std::vector<int> frames2{10, 10};
   m_defenderFiring =
      std::make_unique<Animation>(sprites2, frames2, true, m_coordSys.get());

   return true;
}


bool Game2::setupAttackers()
{
   assert(!!m_coordSys);
   assert(!!m_map);
   assert(!!m_stdSpriteRenderer);

   const std::string texId{"attacker"};
   SpriteLook look{texId};

   Sprite sprite1{m_stdSpriteRenderer.get(), look, SpriteForm{}};
   m_attackers.emplace_back(
      sprite1, m_coordSys->makeEquivalentMapSize(.03f, m_resources.getTextureSize(texId)),
      2000, .001f, OffsetPath{&m_map->path(), NormVec{0.001, 0.002}}, 0, *m_explosion,
      m_coordSys.get());

   Sprite sprite2{m_stdSpriteRenderer.get(), look, SpriteForm{}};
   m_attackers.emplace_back(
      sprite2,
      m_coordSys->makeEquivalentMapSize(.015f, m_resources.getTextureSize(texId)), 800,
      .002f, OffsetPath{&m_map->path(), NormVec{-0.001, 0.003}}, 0, *m_explosion,
      m_coordSys.get());

   Sprite sprite3{m_stdSpriteRenderer.get(), look, SpriteForm{}};
   m_attackers.emplace_back(
      sprite3, m_coordSys->makeEquivalentMapSize(.03f, m_resources.getTextureSize(texId)),
      2000, .001f, OffsetPath{&m_map->path(), NormVec{0.001, 0.002}}, 100, *m_explosion,
      m_coordSys.get());

   Sprite sprite4{m_stdSpriteRenderer.get(), look, SpriteForm{}};
   m_attackers.emplace_back(
      sprite4,
      m_coordSys->makeEquivalentMapSize(.015f, m_resources.getTextureSize(texId)), 800,
      .002f, OffsetPath{&m_map->path(), NormVec{-0.001, 0.003}}, 10, *m_explosion,
      m_coordSys.get());

   return true;
}


bool Game2::setupDefenders()
{
   assert(!!m_coordSys);
   assert(!!m_map);
   assert(!!m_stdSpriteRenderer);

   const std::string texId{"defender"};
   SpriteLook look{texId};

   Sprite sprite{m_stdSpriteRenderer.get(), look, SpriteForm{}};

   m_defenders.emplace_back(
      sprite, m_coordSys->makeEquivalentMapSize(.04f, m_resources.getTextureSize(texId)),
      NormPos{.387f, .476f}, .1f, 5, *m_defenderFiring, m_coordSys.get(), m_attackers);

   Sprite sprite2{m_stdSpriteRenderer.get(), look, SpriteForm{}};

   m_defenders.emplace_back(
      sprite2, m_coordSys->makeEquivalentMapSize(.04f, m_resources.getTextureSize(texId)),
      NormPos{.45f, .276f}, .1f, 5, *m_defenderFiring, m_coordSys.get(), m_attackers);

   return true;
}


bool Game2::setupBackground()
{
   const std::string texId{"map1"};
   SpriteLook look{texId};
   SpriteForm form{{MainWndWidth, MainWndHeight}, Angle_t{0.f}};
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
   for (auto& defender : m_defenders)
      defender.update();

   // Remove destroyed attackers.
   m_attackers.erase(
      std::remove_if(m_attackers.begin(), m_attackers.end(),
                     [](const auto& attacker) { return attacker.canBeRemoved(); }),
      m_attackers.end());
}


void Game2::render()
{
   m_renderer.beginRendering();
   m_background->render(m_renderer.shaders(), {0.f, 0.f});

   for (auto& attacker : m_attackers)
      attacker.render(m_renderer.shaders());
   for (auto& defender : m_defenders)
      defender.render(m_renderer.shaders());

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


void Game2::onKeyPolled(gfl::Key /*key*/, float /*frameLengthSecs*/)
{
}
