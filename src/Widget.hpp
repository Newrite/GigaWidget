#pragma once
#include "GigaWidgetAPI.hpp"
#include "TrueHUDAPI.hpp"
#include "TrueDirectionalMovementAPI.hpp"

static bool in_menu;
static bool in_tween_menu;

namespace ExternalApi {

static auto get_tdm_api()
{
  static TDM_API::IVTDM3* tdm_api = nullptr;
  if (!tdm_api) {
    tdm_api =
        reinterpret_cast<TDM_API::IVTDM3*>(TDM_API::RequestPluginAPI());
    if (tdm_api) {
      logger::info("Enabled compatibility with true hud");
    } else {
      logger::info("true hud not found");
    }
  }
  return tdm_api;
}

static auto get_true_hud_api()
{
  static TRUEHUD_API::IVTrueHUD4* true_hud_api = nullptr;
  if (!true_hud_api) {
    true_hud_api =
        reinterpret_cast<TRUEHUD_API::IVTrueHUD4*>(TRUEHUD_API::RequestPluginAPI());
    if (true_hud_api) {
      logger::info("Enabled compatibility with true hud");
    } else {
      logger::info("true hud not found");
    }
  }
  return true_hud_api;
}

}

struct GigaWidget final : public RE::IMenu
{


  enum class HandType
  {
    kRightHand,
    kLeftHand,
    kCenterHand
  };

  enum IconElementType
  {
    kPoisonIcon,
    kPoisonText,
    kEnchantIcon,
    kEnchantText,
    kScrollIcon,
    kWeaponIcon,
    kFistIcon,
    kShieldIcon,
    kMagickIcon,
    kArrowIcon,
    kBowIcon,
    kStaffIcon,
    kTorchIcon
  };

  struct PathVisiblePair
  {
    const char* path_to_variable = "";
    bool is_visible = false;
  };

  static auto is_shout_equipped() -> bool;
  static auto weapon_is_poisoned(bool is_left) -> bool;
  static auto get_poison_count(const bool is_left) -> std::uint32_t;
  static auto get_ammo_count() -> std::int32_t;
  static auto weapon_is_enchanted(const bool is_left) -> bool;
  static auto enchanted_count(const bool is_left) -> double;
  static auto get_object_name(HandType hand_type) -> const char*;
  static auto evalute_visible(std::map<IconElementType, PathVisiblePair>* hand_map, bool is_left) -> void;

public:
  static constexpr const char* MENU_PATH = "GigaWidget";
  static constexpr const char* MENU_NAME = "GigaWidget";
  static constexpr std::string_view TRUE_HUD = "TrueHUD";
  static constexpr std::string_view COMBAT_ALERT_OVERLAY_MENU = "CombatAlertOverlayMenu";

  GigaWidget();

  static auto register_() -> void;
  static auto show() -> void;
  static auto hide() -> void;
  static auto update() -> void;
  static auto apply_layout(const RE::GPtr<GigaWidget>& giga_widget) -> void;
  static auto toggle_visibility(bool mode, RE::BSFixedString menu = "") -> void;
  static auto toggle_visibility_tween(bool mode) -> void;

  // static auto creator() -> RE::stl::owner<RE::IMenu*> { return new ReflyemWidget(); }
  static auto creator() -> RE::IMenu* { return new GigaWidget(); }

  auto AdvanceMovie(float interval, uint32_t current_time) -> void override;

private:
  class GagaWidgetLogger final : public RE::GFxLog
  {
  public:
    void LogMessageVarg(LogMessageType, const char* fmt, const std::va_list arg_list) override
    {
      std::string format(fmt ? fmt : "");
      while (!format.empty() && format.back() == '\n') {
        format.pop_back();
      }

      std::va_list args;
      va_copy(args, arg_list);
      std::vector<char> buf(
          static_cast<std::size_t>(std::vsnprintf(0, 0, format.c_str(), arg_list) + 1));
      std::vsnprintf(buf.data(), buf.size(), format.c_str(), args);
      va_end(args);

      logger::info("{}"sv, buf.data());
    }
  };
};
