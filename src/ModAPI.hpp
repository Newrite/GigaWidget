#pragma once
#include "GigaWidgetAPI.hpp"

namespace Messaging {
using APIResult = ::GIGAWIDGET_API::APIResult;
using InterfaceVersion1 = ::GIGAWIDGET_API::IVGigaWidget1;

class GigaWidgetInterface final : public InterfaceVersion1
{
public:
  GigaWidgetInterface() = default;
  GigaWidgetInterface(const GigaWidgetInterface&) = delete;
  GigaWidgetInterface(GigaWidgetInterface&&) = delete;
  virtual ~GigaWidgetInterface() = default;

  GigaWidgetInterface& operator=(const GigaWidgetInterface&) = delete;
  GigaWidgetInterface& operator=(GigaWidgetInterface&&) = delete;
  
  static GigaWidgetInterface* get_singleton() noexcept;

  APIResult add_set_available_potion_slot_callback(
      SKSE::PluginHandle plugin_handle,
      GIGAWIDGET_API::SetAvailablePotionSlotCallback&& callback) noexcept override;

  APIResult add_set_frame_for_potion_callback(SKSE::PluginHandle plugin_handle,
                                              GIGAWIDGET_API::SetFrameForPotionCallback&& callback) noexcept override;

  APIResult add_set_available_ki_energy_callback(
    SKSE::PluginHandle plugin_handle,
    GIGAWIDGET_API::SetAvailableKiEnergyCallback&& callback) noexcept override;

  APIResult add_set_frame_for_ki_energy_callback(SKSE::PluginHandle plugin_handle,
                                              GIGAWIDGET_API::SetFrameForKiEnergyCallback&& callback) noexcept override;

  APIResult add_set_frame_for_absorb_shield_callback(SKSE::PluginHandle plugin_handle,
                                            GIGAWIDGET_API::SetFrameForKiEnergyCallback&& callback) noexcept override;
};
}
