#include "ModAPI.hpp"

#include "CallbackHolder.hpp"
#include "Widget.hpp"

namespace Messaging {

GigaWidgetInterface* GigaWidgetInterface::get_singleton() noexcept

{
  static GigaWidgetInterface singleton;
  return std::addressof(singleton);
}

APIResult GigaWidgetInterface::add_set_available_potion_slot_callback(
    SKSE::PluginHandle plugin_handle,
    GIGAWIDGET_API::SetAvailablePotionSlotCallback&& callback) noexcept
{
  if (CallbackHolder::get_singleton().add_available_potion_slot_callback(plugin_handle, callback)) {
    logi("Success add callback add_available_potion_slot_callback {}"sv, plugin_handle);
    return APIResult::OK;
  }
  logi("add_set_available_potion_slot_callback callback already registered for this plugin_handle {}"sv, plugin_handle);
  return APIResult::AlreadyRegistered;
}

APIResult GigaWidgetInterface::add_set_frame_for_potion_callback(
    SKSE::PluginHandle plugin_handle,
    GIGAWIDGET_API::SetFrameForPotionCallback&& callback) noexcept
{
  if (CallbackHolder::get_singleton().add_set_frame_for_potion_callback(plugin_handle, callback)) {
    logi("Success add callback add_set_frame_for_potion_callback {}"sv, plugin_handle);
    return APIResult::OK;
  }
  logi("add_set_frame_for_potion_callback callback already registered for this plugin_handle {}"sv, plugin_handle);
  return APIResult::AlreadyRegistered;
}

APIResult GigaWidgetInterface::add_set_available_ki_energy_callback(
    SKSE::PluginHandle plugin_handle,
    GIGAWIDGET_API::SetAvailableKiEnergyCallback&& callback) noexcept
{
  if (CallbackHolder::get_singleton().add_available_ki_energy_callback(plugin_handle, callback)) {
    logi("Success add callback add_set_available_ki_energy_callback {}"sv, plugin_handle);
    return APIResult::OK;
  }
  logi("add_set_available_ki_energy_callback callback already registered for this plugin_handle {}"sv, plugin_handle);
  return APIResult::AlreadyRegistered;
}

APIResult GigaWidgetInterface::add_set_frame_for_ki_energy_callback(
    SKSE::PluginHandle plugin_handle,
    GIGAWIDGET_API::SetFrameForKiEnergyCallback&& callback) noexcept
{
  if (CallbackHolder::get_singleton().add_set_frame_for_ki_energy_callback(plugin_handle, callback)) {
    logi("Success add callback add_set_frame_for_ki_energy_callback {}"sv, plugin_handle);
    return APIResult::OK;
  }
  logi("add_set_frame_for_ki_energy_callback callback already registered for this plugin_handle {}"sv, plugin_handle);
  return APIResult::AlreadyRegistered;
}

APIResult GigaWidgetInterface::add_set_frame_for_absorb_shield_callback(
    SKSE::PluginHandle plugin_handle,
    GIGAWIDGET_API::SetFrameForAbsorbShieldCallback&& callback) noexcept
{
  if (CallbackHolder::get_singleton().add_set_frame_for_absorb_shield_callback(plugin_handle, callback)) {
    logi("Success add callback add_set_frame_for_absorb_shield_callback {}"sv, plugin_handle);
    return APIResult::OK;
  }
  logi("add_set_frame_for_absorb_shield_callback callback already registered for this plugin_handle {}"sv, plugin_handle);
  return APIResult::AlreadyRegistered;
}

}
