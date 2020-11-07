//
// Nov-2020, Michael Lindner
// MIT license
//
#include "frustum.h"


namespace
{
///////////////////

constexpr Angle_t MaxFov{Angle_t::fromDegrees(60.0f)};
constexpr Angle_t MinFov{Angle_t::fromDegrees(20.0f)};

} // namespace


///////////////////

void Frustum::setupInput(InputState& input)
{
   input.addObserver([this](InputState& input, std::string_view msg,
                            const Observed<InputState>::MsgData& data) {
      onInputChanged(input, msg, data);
   });
}


void Frustum::setupUi(UiState& ui)
{
   ui.addObserver(
      [this](UiState& ui, std::string_view msg, const Observed<UiState>::MsgData& data) {
         onUiChanged(ui, msg, data);
      });
}


void Frustum::onInputChanged(InputState& /*input*/, std::string_view msg,
                             const Observed<InputState>::MsgData& data)
{
   if (msg == MouseScrolledMsg)
   {
      const auto mouseScrolledData = static_cast<const MouseScrolledMsgData&>(data);
      updateFov(mouseScrolledData.adjustedDelta.y);
   }
}


void Frustum::onUiChanged(UiState& /*ui*/, std::string_view msg,
                          const Observed<UiState>::MsgData& data)
{
   if (msg == WindowResizedMsg)
   {
      const auto resizeData = static_cast<const WindowResizedMsgData&>(data);
      m_aspect =
         static_cast<float>(resizeData.size.x) / static_cast<float>(resizeData.size.y);
   }
}


void Frustum::updateFov(float offset)
{
   m_horzFov -= sutil::radians(offset);

   if (m_horzFov > MaxFov)
      m_horzFov = MaxFov;
   if (m_horzFov < MinFov)
      m_horzFov = MinFov;
}
