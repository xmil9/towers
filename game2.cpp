//
// Nov-2020, Michael Lindner
// MIT license
//
#include "game2.h"
#include "animation_factory.h"
#include "animation_tags.h"
#include "attacker_models.h"
#include "defender_models.h"
#include "map_data.h"
#include "mesh2.h"
#include "texture_tags.h"
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
   return (setupUi() && setupInput() && setupOutput() && setupTextures() &&
           setupRenderer() && setupTerrain() && setupSpriteData() && setupAnimations() &&
           setupAttackers() && setupDefenders() && setupBackground() && setupDashboard());
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

   if (m_mainWnd.create(WndWidth, WndHeight, "towers") != GLFW_NO_ERROR)
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


bool Game2::setupTextures()
{
   struct TextureSpec
   {
      std::string tag;
      std::filesystem::path filename;
   };
   const std::vector<TextureSpec> textures{
      {AatTexture, "aat.png"},
      {MhcTexture, "mhc.png"},
      {LtTexture, "lt.png"},
      {LtFiring1Texture, "lt_firing1.png"},
      {LtFiring2Texture, "lt_firing2.png"},
      {Explosion1TTag, "explosion1.png"},
      {Explosion2TTag, "explosion2.png"},
      {Explosion3TTag, "explosion3.png"},
      {Explosion4TTag, "explosion4.png"},
      {Map1TTag, "map1.png"},
      {DashboardTTag, "dashboard.png"},
   };

   for (const auto& spec : textures)
      if (!m_resources.loadTexture(spec.tag, m_resources.texturePath() / spec.filename))
         return false;
   return true;
}


bool Game2::setupRenderer()
{
   return m_renderer.setup(&m_resources, WndWidth, WndHeight);
}


bool Game2::setupTerrain()
{
   m_coordSys = std::make_unique<MapCoordSys>(PixDim{MapWidth, MapHeight});

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
   m_spriteRenderer = std::make_unique<SpriteRenderer>(&m_resources);
   m_spriteRenderer->setMesh(mesh);

   return true;
}


bool Game2::setupAnimations()
{
   assert(!!m_coordSys);
   assert(!!m_spriteRenderer);

   AnimationFactory factory(m_spriteRenderer.get());

   m_resources.addAnimation(
      ExplosionATag,
      factory.make(ExplosionATag, m_coordSys->toRenderCoords(NormDim{.05, .05})));

   const PixDim firingSize = m_coordSys->toRenderCoords(
      m_coordSys->makeEquivalentMapSize(LtSize, m_resources.getTextureSize(LtTexture)));
   m_resources.addAnimation(LtFiringAnimation,
                            factory.make(LtFiringAnimation, firingSize));

   return true;
}


static void addAttacker(const std::optional<Attacker>& attacker,
                        std::vector<Attacker>& coll)
{
   if (attacker)
      coll.push_back(*attacker);
}


bool Game2::setupAttackers()
{
   assert(!!m_coordSys);
   assert(!!m_map);
   assert(!!m_spriteRenderer);

   m_attackFactory = std::make_unique<AttackerFactory>(m_coordSys.get());

   m_attackFactory->registerModel(
      AatModel, AttackerLook{Sprite{m_spriteRenderer.get(), SpriteLook{AatTexture},
                                    SpriteForm{m_resources.getTextureSize(AatTexture)}},
                             m_resources.getAnimation(ExplosionATag)});
   m_attackFactory->registerModel(
      MhcModel, AttackerLook{Sprite{m_spriteRenderer.get(), SpriteLook{MhcTexture},
                                    SpriteForm{m_resources.getTextureSize(MhcTexture)}},
                             m_resources.getAnimation(ExplosionATag)});

   addAttacker(
      m_attackFactory->makeAttacker(AatModel, .03f, Attacker::Attribs{2000, .001f, 0},
                                    OffsetPath{&m_map->path(), NormVec{0.001, 0.002}}),
      m_attackers);
   addAttacker(
      m_attackFactory->makeAttacker(MhcModel, .012f, Attacker::Attribs{800, .002f, 0},
                                    OffsetPath{&m_map->path(), NormVec{-0.001, 0.003}}),
      m_attackers);
   addAttacker(
      m_attackFactory->makeAttacker(AatModel, .03f, Attacker::Attribs{2000, .001f, 100},
                                    OffsetPath{&m_map->path(), NormVec{0.001, 0.002}}),
      m_attackers);
   addAttacker(
      m_attackFactory->makeAttacker(MhcModel, .012f, Attacker::Attribs{800, .002f, 10},
                                    OffsetPath{&m_map->path(), NormVec{-0.001, 0.01}}),
      m_attackers);

   return true;
}


static void addDefender(const std::optional<Defender>& defender,
                        std::vector<Defender>& coll)
{
   if (defender)
      coll.push_back(*defender);
}


bool Game2::setupDefenders()
{
   assert(!!m_coordSys);
   assert(!!m_map);
   assert(!!m_spriteRenderer);

   m_defenseFactory = std::make_unique<DefenderFactory>(m_coordSys.get(), &m_attackers);

   m_defenseFactory->registerModel(
      LtModel, DefenderLook{Sprite{m_spriteRenderer.get(), SpriteLook{LtTexture},
                                   SpriteForm{m_resources.getTextureSize(LtTexture)}},
                            m_resources.getAnimation(LtFiringAnimation)});

   Defender::Attribs ltAttribs{LtRange, LtDamage};
   addDefender(
      m_defenseFactory->makeDefender(LtModel, LtSize, NormPos{.387f, .476f}, ltAttribs),
      m_defenders);
   addDefender(
      m_defenseFactory->makeDefender(LtModel, LtSize, NormPos{.45f, .276f}, ltAttribs),
      m_defenders);

   return true;
}


bool Game2::setupBackground()
{
   assert(!!m_spriteRenderer);
   m_background = std::make_unique<Sprite>(m_spriteRenderer.get(), SpriteLook{Map1TTag},
                                           SpriteForm{{MapWidth, MapHeight}});

   return true;
}


bool Game2::setupDashboard()
{
   assert(!!m_spriteRenderer);

   m_dashboard =
      std::make_unique<Sprite>(m_spriteRenderer.get(), SpriteLook{DashboardTTag},
                               SpriteForm{{DashboardWidth, DashboardHeight}});

   constexpr PixDim dashPixDim{DashboardWidth, DashboardHeight};
   constexpr NormDim buttonDim{.375f, .0625f};
   m_ltButton = std::make_unique<Sprite>(m_spriteRenderer.get(), SpriteLook{LtTexture},
                                         SpriteForm{buttonDim * dashPixDim});

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
   renderMap();
   renderDashboard();

   for (auto& attacker : m_attackers)
      attacker.render(m_renderer.shaders());
   for (auto& defender : m_defenders)
      defender.render(m_renderer.shaders());

   m_mainWnd.swapBuffers();
}


void Game2::renderMap()
{
   m_background->render(m_renderer.shaders(), {0.f, 0.f});
}


void Game2::renderDashboard()
{
   constexpr PixPos dashLeftTop{MapWidth - 1, 0.f};
   m_dashboard->render(m_renderer.shaders(), dashLeftTop);

   constexpr NormDim ltPos{.075f, .0167f};
   constexpr NormDim ltPixPos = ltPos * PixDim{DashboardWidth, DashboardHeight};
   m_ltButton->render(m_renderer.shaders(), dashLeftTop + ltPixPos);
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
