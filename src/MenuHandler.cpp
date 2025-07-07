#include "MenuHandler.hpp"
#include "Settings.hpp"
#include "Widget.hpp"

auto MenuHandler::get_singleton() noexcept -> MenuHandler*
{
  static MenuHandler instance;
  return std::addressof(instance);
}

auto MenuHandler::register_() -> void
{
  if (const auto ui = RE::UI::GetSingleton()) {
    ui->AddEventSink(get_singleton());
  }
}

auto MenuHandler::ProcessEvent(const RE::MenuOpenCloseEvent* event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
    -> RE::BSEventNotifyControl
{

  if (event) {
    logger::info("Menu: {}  is {}", event->menuName.c_str(), event->opening ? "open" : "close");
    if (event->menuName == RE::HUDMenu::MENU_NAME) {
      if (event->opening) { GigaWidget::show(); }
      event->opening ? GigaWidget::toggle_visibility(true, event->menuName) : GigaWidget::toggle_visibility(false, event->menuName);
    }
    if (event->menuName == GigaWidget::TRUE_HUD) {
      if (event->opening) { GigaWidget::show(); }
      event->opening ? GigaWidget::toggle_visibility(true, event->menuName) : GigaWidget::toggle_visibility(false, event->menuName);
    }
    if (event->menuName == RE::TweenMenu::MENU_NAME) {
      logi("Tween Menu : Open {}", event->opening);
      GigaWidget::toggle_visibility_tween(event->opening);
    }
    
    if (event->menuName != RE::TweenMenu::MENU_NAME && event->opening && event->menuName != RE::CursorMenu::MENU_NAME && event->menuName != GigaWidget::COMBAT_ALERT_OVERLAY_MENU) {
      logi("Open some menu set tween to false");
      GigaWidget::toggle_visibility_tween(false);
    }
    // if (event->menuName == RE::LoadingMenu::MENU_NAME) {
    //   event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    // }
    if (event->menuName == RE::JournalMenu::MENU_NAME) {
      Settings::get_singleton().load();
      // event->opening ? ReflyemWidget::toggle_visibility(false) :
      // ReflyemWidget::toggle_visibility(true);
    }
    // if (event->menuName == RE::MapMenu::MENU_NAME) {
    //   event->opening ? ReflyemWidget::toggle_visibility(false) :
    //   ReflyemWidget::toggle_visibility(true);
    // }
    // if (event->menuName == RE::TweenMenu::MENU_NAME) {
    //   event->opening ? ReflyemWidget::toggle_visibility(false) :
    //   ReflyemWidget::toggle_visibility(true);
    // }
    // if (event->menuName == RE::MagicMenu::MENU_NAME) {
    //   event->opening ? ReflyemWidget::toggle_visibility(false) :
    //   ReflyemWidget::toggle_visibility(true);
    // }
    // if (event->menuName == RE::InventoryMenu::MENU_NAME) {
    //   event->opening ? ReflyemWidget::toggle_visibility(false) :
    //   ReflyemWidget::toggle_visibility(true);
    // }
    // if (event->menuName == RE::FaderMenu::MENU_NAME) {
    //   event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    // }

    // copypast from true hud

    using ContextID = RE::UserEvents::INPUT_CONTEXT_ID;
    // Hide the widgets when a menu is open
    auto control_map = RE::ControlMap::GetSingleton();
    if (control_map) {
      auto& priorityStack = control_map->contextPriorityStack;
      if (priorityStack.empty()) {
        GigaWidget::toggle_visibility(false, event->menuName);
      } else if (priorityStack.back() == ContextID::kGameplay || priorityStack.back() == ContextID::kFavorites ||
                 priorityStack.back() == ContextID::kConsole) {
        GigaWidget::toggle_visibility(true, event->menuName);
        if (event->opening) { GigaWidget::show(); }
      } else if ((priorityStack.back() == ContextID::kCursor || priorityStack.back() == ContextID::kItemMenu ||
                  priorityStack.back() == ContextID::kMenuMode || priorityStack.back() == ContextID::kInventory) &&
                 (RE::UI::GetSingleton()->IsMenuOpen(RE::DialogueMenu::MENU_NAME) ||
                  RE::UI::GetSingleton()->IsMenuOpen(RE::CraftingMenu::MENU_NAME) ||
                  (RE::UI::GetSingleton()->IsMenuOpen(RE::BarterMenu::MENU_NAME) ||
                   RE::UI::GetSingleton()->IsMenuOpen(RE::ContainerMenu::MENU_NAME) ||
                   RE::UI::GetSingleton()->IsMenuOpen(RE::GiftMenu::MENU_NAME) ||
                   RE::UI::GetSingleton()->IsMenuOpen(RE::InventoryMenu::MENU_NAME)))) {
        GigaWidget::toggle_visibility(false, event->menuName);
      } else {
        GigaWidget::toggle_visibility(false, event->menuName);
      }
    }
  }
  logger::debug("Menu event continue");
  return RE::BSEventNotifyControl::kContinue;
}
