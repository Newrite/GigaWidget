#pragma once
#include "GigaWidgetAPI.hpp"

class CallbackHolder final
{
  public:
  static CallbackHolder& get_singleton() noexcept
  {
    static CallbackHolder instance;
    return instance;
  }

  bool add_set_frame_for_potion_callback(const SKSE::PluginHandle plugin_handle,
                                         const GIGAWIDGET_API::SetFrameForPotionCallback callback)
  {
    let lock = std::lock_guard{callbacks_mutex_};
    if (set_frame_for_potion_callback_.contains(plugin_handle)) {
      return false;
    }
    set_frame_for_potion_callback_.emplace(plugin_handle, callback);
    return true;
  }

  bool add_available_potion_slot_callback(const SKSE::PluginHandle plugin_handle,
                                          const GIGAWIDGET_API::SetAvailablePotionSlotCallback callback)
  {
    let lock = std::lock_guard{callbacks_mutex_};
    if (set_available_potion_slot_callback_.contains(plugin_handle)) {
      return false;
    }
    set_available_potion_slot_callback_.emplace(plugin_handle, callback);
    return true;
  }

  bool add_set_frame_for_ki_energy_callback(const SKSE::PluginHandle plugin_handle,
                                            const GIGAWIDGET_API::SetFrameForKiEnergyCallback callback)
  {
    let lock = std::lock_guard{callbacks_mutex_};
    if (set_frame_for_ki_energy_callback_.contains(plugin_handle)) {
      return false;
    }
    set_frame_for_ki_energy_callback_.emplace(plugin_handle, callback);
    return true;
  }

  bool add_available_ki_energy_callback(const SKSE::PluginHandle plugin_handle,
                                        const GIGAWIDGET_API::SetAvailableKiEnergyCallback callback)
  {
    let lock = std::lock_guard{callbacks_mutex_};
    if (set_available_ki_energy_callback_.contains(plugin_handle)) {
      return false;
    }
    set_available_ki_energy_callback_.emplace(plugin_handle, callback);
    return true;
  }

  bool add_set_frame_for_absorb_shield_callback(const SKSE::PluginHandle plugin_handle,
                                            const GIGAWIDGET_API::SetFrameForAbsorbShieldCallback callback)
  {
    let lock = std::lock_guard{callbacks_mutex_};
    if (set_frame_for_absorb_shield_callback_.contains(plugin_handle)) {
      return false;
    }
    set_frame_for_absorb_shield_callback_.emplace(plugin_handle, callback);
    return true;
  }

  [[nodiscard]] std::unordered_map<SKSE::PluginHandle, GIGAWIDGET_API::SetFrameForPotionCallback>&
  set_frame_for_potion_callback()
  {
    return set_frame_for_potion_callback_;
  }
  [[nodiscard]] std::unordered_map<SKSE::PluginHandle, GIGAWIDGET_API::SetAvailablePotionSlotCallback>&
  set_available_potion_slot_callback()
  {
    return set_available_potion_slot_callback_;
  }

  [[nodiscard]] std::unordered_map<SKSE::PluginHandle, GIGAWIDGET_API::SetFrameForKiEnergyCallback>&
  set_frame_for_ki_energy_callback()
  {
    return set_frame_for_ki_energy_callback_;
  }
  [[nodiscard]] std::unordered_map<SKSE::PluginHandle, GIGAWIDGET_API::SetAvailableKiEnergyCallback>&
  set_available_ki_energy_callback()
  {
    return set_available_ki_energy_callback_;
  }

  [[nodiscard]] std::unordered_map<SKSE::PluginHandle, GIGAWIDGET_API::SetFrameForAbsorbShieldCallback>&
  set_frame_for_absorb_shield_callback()
  {
    return set_frame_for_absorb_shield_callback_;
  }

  private:
  std::mutex callbacks_mutex_;
  std::unordered_map<SKSE::PluginHandle, GIGAWIDGET_API::SetFrameForPotionCallback> set_frame_for_potion_callback_;
  std::unordered_map<SKSE::PluginHandle, GIGAWIDGET_API::SetAvailablePotionSlotCallback>
      set_available_potion_slot_callback_;
  std::unordered_map<SKSE::PluginHandle, GIGAWIDGET_API::SetFrameForKiEnergyCallback> set_frame_for_ki_energy_callback_;
  std::unordered_map<SKSE::PluginHandle, GIGAWIDGET_API::SetFrameForAbsorbShieldCallback> set_frame_for_absorb_shield_callback_;
  std::unordered_map<SKSE::PluginHandle, GIGAWIDGET_API::SetAvailableKiEnergyCallback>
      set_available_ki_energy_callback_;
};
