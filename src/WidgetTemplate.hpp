#pragma once

struct WidgetTemplate : public RE::IMenu
{
  static constexpr std::string_view TRUE_HUD = "TrueHUD";

  WidgetTemplate(const std::string menu_path, const std::string menu_name)
  {

    menu_path_ = menu_path;
    menu_name_ = menu_name;

    // const auto scale_form_manager = RE::BSScaleformManager::GetSingleton();

    logger::info("Construct menu: {}"sv, menu_name_);

    inputContext = Context::kNone;
    depthPriority = 0;

    menuFlags.set(RE::UI_MENU_FLAGS::kAlwaysOpen);
    menuFlags.set(RE::UI_MENU_FLAGS::kRequiresUpdate);
    menuFlags.set(RE::UI_MENU_FLAGS::kAllowSaving);
    // menuFlags.set(RE::UI_MENU_FLAGS::kCustomRendering);
    // menuFlags.set(RE::UI_MENU_FLAGS::kAssignCursorToRenderer);

    if (uiMovie) {
      uiMovie->SetMouseCursorCount(0);
      uiMovie->SetVisible(true);
    }

    // scale_form_manager->LoadMovieEx(this, MENU_PATH, [](RE::GFxMovieDef* def) -> void {
    //   def->SetState(RE::GFxState::StateType::kLog, RE::make_gptr<ReflyemLogger>().get());
    // });

    if (auto scaleform = RE::BSScaleformManager::GetSingleton()) {
      scaleform->LoadMovie(this, this->uiMovie, menu_path_.c_str());
    }

    logger::info("Finish construct menu: {}"sv, menu_name_);
  }

  // auto register_widget() -> void {
  //   const auto ui = RE::UI::GetSingleton();
  //   if (!ui) {
  //     logger::error("Null UI when try register widget menu: {}"sv, menu_name_);
  //     return;
  //   }
  //   ui->Register(menu_name_, creator)
  // }

  private:
  std::string menu_path_;
  std::string menu_name_;
};
