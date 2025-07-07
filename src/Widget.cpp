#include "Widget.hpp"

#include "CallbackHolder.hpp"
#include "Settings.hpp"

enum class MenuHideResult
{
  kTrue,
  kFalse,
  kSkip
};

static const std::map<std::string, std::function<MenuHideResult(RE::BSFixedString&)>> MENUS_TO_HIDE_WHEN_OPEN_MENU{
    {"mirelgigawidget.BaseWidget._visible",
     [](RE::BSFixedString& menu_name) {
       let& settings = Settings::get_singleton();
       return settings.bEquipmentWidget ? MenuHideResult::kTrue : MenuHideResult::kFalse;
     }},
    {"mirelgigawidget.KiEnergy._visible",
     [](RE::BSFixedString& menu_name) {
       let& settings = Settings::get_singleton();
       return settings.bKiEnergy ? MenuHideResult::kTrue : MenuHideResult::kFalse;
     }},
    {"mirelgigawidget.Stress._visible",
     [](RE::BSFixedString& menu_name) {
       let& settings = Settings::get_singleton();
       return settings.bStressWidget ? MenuHideResult::kTrue : MenuHideResult::kFalse;
     }},
    {"mirelgigawidget.Rested._visible",
     [](RE::BSFixedString& menu_name) {
       let& settings = Settings::get_singleton();
       return settings.bExhaustingWidget ? MenuHideResult::kTrue : MenuHideResult::kFalse;
     }},
    {"mirelgigawidget.Weather._visible",
     [](RE::BSFixedString& menu_name) {
       let& settings = Settings::get_singleton();
       return settings.bWeatherWidget ? MenuHideResult::kTrue : MenuHideResult::kFalse;
     }},
    {"mirelgigawidget.Hunger._visible",
     [](RE::BSFixedString& menu_name) {
       let& settings = Settings::get_singleton();
       return settings.bHungerWidget ? MenuHideResult::kTrue : MenuHideResult::kFalse;
     }},
    {"mirelgigawidget.PotionPoisonHolder._visible",
     [](RE::BSFixedString& menu_name) {
       let& settings = Settings::get_singleton();
       return settings.bPotionPoison ? MenuHideResult::kTrue : MenuHideResult::kFalse;
     }},
    {"mirelgigawidget.PotionOtherHolder._visible",
     [](RE::BSFixedString& menu_name) {
       let& settings = Settings::get_singleton();
       return settings.bPotionOther ? MenuHideResult::kTrue : MenuHideResult::kFalse;
     }},
    {"mirelgigawidget.PotionMagickaHolder._visible",
     [](RE::BSFixedString& menu_name) {
       let& settings = Settings::get_singleton();
       return settings.bPotionMagicka ? MenuHideResult::kTrue : MenuHideResult::kFalse;
     }},
    {"mirelgigawidget.PotionStaminaHolder._visible",
     [](RE::BSFixedString& menu_name) {
       let& settings = Settings::get_singleton();
       return settings.bPotionStamina ? MenuHideResult::kTrue : MenuHideResult::kFalse;
     }},
    {"mirelgigawidget.DynamicStats._visible",
     [](RE::BSFixedString& menu_name) {
       let& settings = Settings::get_singleton();
       return settings.bDynamicStatsMenuWidget ? MenuHideResult::kTrue : MenuHideResult::kFalse;
     }},
    {"mirelgigawidget.TargetDynamicStats._visible",
     [](RE::BSFixedString& menu_name) {
       let& settings = Settings::get_singleton();
       if (menu_name == RE::FavoritesMenu::MENU_NAME || menu_name == RE::CursorMenu::MENU_NAME || menu_name == RE::Console::MENU_NAME) {
         return MenuHideResult::kSkip;
       }
       return settings.bTargetDynamicStatsMenuWidget ? MenuHideResult::kTrue : MenuHideResult::kFalse;
     }},
    {"mirelgigawidget.WoundInjuryDisease._visible",
     [](RE::BSFixedString& menu_name) {
       let& settings = Settings::get_singleton();
       return settings.bWoundInjuryDiseaseMenuWidget ? MenuHideResult::kTrue : MenuHideResult::kFalse;
     }},
    {"mirelgigawidget.AbsorbShieldWidget._visible",
     [](RE::BSFixedString& menu_name) {
       let& settings = Settings::get_singleton();
       return settings.bAbsorbShieldWidget ? MenuHideResult::kTrue : MenuHideResult::kFalse;
     }},
    {"mirelgigawidget.PotionHealthHolder._visible", [](RE::BSFixedString&) {
      let& settings = Settings::get_singleton();
      return settings.bPotionHealth ? MenuHideResult::kTrue : MenuHideResult::kFalse;
    }}};

auto GigaWidget::is_shout_equipped() -> bool
{

  const auto player = RE::PlayerCharacter::GetSingleton();

  const auto spell = player->selectedPower;
  if (!spell) {
    logger::debug("Null Current spell"sv);
    return false;
  }

  return spell->Is(RE::FormType::Shout);
}

auto GigaWidget::weapon_is_poisoned(const bool is_left) -> bool
{

  const auto player = RE::PlayerCharacter::GetSingleton();
  if (!player) {
    return false;
  }

  const auto hand_data = player->GetEquippedEntryData(is_left);
  if (!hand_data) {
    return false;
  }

  return hand_data->IsPoisoned();
}

auto GigaWidget::weapon_is_enchanted(const bool is_left) -> bool
{

  const auto player = RE::PlayerCharacter::GetSingleton();
  if (!player) {
    return false;
  }

  const auto hand_data = player->GetEquippedEntryData(is_left);
  if (!hand_data) {
    return false;
  }

  return hand_data->IsEnchanted();
}

enum RangeType
{
  kBow,
  kCrossbow,
  kStaff,
  kNone
};

// 4 - disable, 1 - neutral, 2 - negative, 3 - positive
auto get_effects_magnitude_sum(const std::vector<RE::ActiveEffect*>& effects) -> std::uint8_t
{
  if (effects.empty()) {
    return 4;
  }

  auto pos_value = 0.f;
  auto neg_value = 0.f;

  for (const auto active_effect : effects) {
    if (!active_effect || active_effect->flags.any(RE::ActiveEffect::Flag::kInactive) || !active_effect->effect ||
        !active_effect->effect->baseEffect) {
      continue;
    }

    const auto effect = active_effect->effect;
    const auto base_effect = effect->baseEffect;

    if (base_effect->data.flags.any(RE::EffectSetting::EffectSettingData::Flag::kDetrimental)) {
      // logger::info("Effect: {} Add {} to NegValue", base_effect->GetFullName(), active_effect->magnitude);
      neg_value += active_effect->magnitude;
    } else {
      // logger::info("Effect: {} Add {} to PosValue", base_effect->GetFullName(), active_effect->magnitude);
      pos_value += active_effect->magnitude;
    }
  }

  // logger::info("EffectsSum: PosValue: {} NegValue: {}", pos_value, neg_value);

  if (pos_value > 0.f && neg_value < 0.f) {
    return 1;
  }

  if (pos_value > 0.f) {
    return 3;
  }

  if (neg_value < 0.f) {
    return 2;
  }

  return 4;
}

auto get_wound_level(const Settings& settings) -> std::uint8_t
{
  let player = RE::PlayerCharacter::GetSingleton();
  if (settings.game_data.wound3 && player->HasSpell(settings.game_data.wound3)) {
    return 3;
  }

  if (settings.game_data.wound2 && player->HasSpell(settings.game_data.wound2)) {
    return 2;
  }

  if (settings.game_data.wound1 && player->HasSpell(settings.game_data.wound1)) {
    return 1;
  }

  return 0;
}

auto actor_has_active_mgef_with_keyword(RE::Actor* actor, const RE::BGSKeyword* keyword) -> bool
{

  if (!actor || !keyword) {
    return false;
  }

  auto active_effects = actor->GetActiveEffectList();
  if (!active_effects) {
    return false;
  }
  logger::debug("Start search mgef wigh keyword"sv);
  for (const auto active_effect : *active_effects) {
    if (!active_effect || active_effect->flags.any(RE::ActiveEffect::Flag::kInactive) || !active_effect->effect ||
        !active_effect->effect->baseEffect) {
      continue;
    }
    const auto base_effect = active_effect->effect->baseEffect;

    logger::debug("Not null effect and active"sv);

    if (base_effect->HasKeyword(keyword)) {
      logger::debug("Found keyword with id"sv);
      return true;
    }
  }
  logger::debug("Not found keyword with id"sv);
  return false;
}

auto actor_has_active_disease(RE::Actor* actor) -> bool
{

  if (!actor) {
    return false;
  }

  auto active_effects = actor->GetActiveEffectList();
  if (!active_effects) {
    return false;
  }

  logger::debug("Start search disease"sv);
  for (const auto active_effect : *active_effects) {
    if (!active_effect || active_effect->flags.any(RE::ActiveEffect::Flag::kInactive) || active_effect->spell) {
      continue;
    }
    const auto spell_type = active_effect->spell->GetSpellType();

    logger::debug("Not null effect and active"sv);

    if (spell_type == RE::MagicSystem::SpellType::kDisease) {
      logger::debug("Found disease"sv);
      return true;
    }
  }
  logger::debug("Not found disease"sv);
  return false;
}

auto get_aligment_by_int(const std::uint8_t aligment) -> std::string
{
  if (aligment == 0) {
    return "left";
  }
  if (aligment == 1) {
    return "center";
  }
  if (aligment == 2) {
    return "right";
  }
  return "none";
}

auto get_effects_by_actor_value(RE::Actor* actor, const RE::ActorValue av) -> std::vector<RE::ActiveEffect*>
{
  std::vector<RE::ActiveEffect*> effects = {};

  if (!actor) {
    return effects;
  }

  auto active_effects = actor->GetActiveEffectList();
  if (!active_effects) {
    return effects;
  }

  for (auto active_effect : *active_effects) {
    if (!active_effect || active_effect->flags.any(RE::ActiveEffect::Flag::kInactive) || !active_effect->effect ||
        !active_effect->effect->baseEffect) {
      continue;
    }

    const auto effect = active_effect->effect;
    // logger::info("Check Effect: {}, AV: {} PAV: {} DAV: {}", effect->baseEffect->GetFullName(), av,
    // effect->baseEffect->data.primaryAV, effect->baseEffect->data.secondaryAV);

    if (effect->baseEffect->data.castingType == RE::MagicSystem::CastingType::kConstantEffect) {
      // logger::info("Effect Skip: {}", effect->baseEffect->GetFullName());
      continue;
    }

    if (effect->baseEffect->HasArchetype(RE::EffectSetting::Archetype::kPeakValueModifier) ||
        effect->baseEffect->HasArchetype(RE::EffectSetting::Archetype::kValueModifier)) {
      // logger::info("PeakValue Check Effect: {}, AV: {} PAV: {} DAV: {}",
      //            effect->baseEffect->GetFullName(),
      //            av,
      //            effect->baseEffect->data.primaryAV,
      //            effect->baseEffect->data.secondaryAV);

      if (effect->baseEffect->data.primaryAV == av) {
        // logger::info("Peak or Value Modifier Effect Added: {}", effect->baseEffect->GetFullName());
        effects.push_back(active_effect);
      }
    }

    if (effect->baseEffect->HasArchetype(RE::EffectSetting::Archetype::kDualValueModifier)) {
      // logger::info("Dual Check Effect: {}, AV: {} PAV: {} DAV: {}",
      //              effect->baseEffect->GetFullName(),
      //              av,
      //              effect->baseEffect->data.primaryAV,
      //              effect->baseEffect->data.secondaryAV);

      if (effect->baseEffect->data.secondaryAV == av || effect->baseEffect->data.primaryAV == av) {
        // logger::info("DualValue Modifier Effect Added: {}", effect->baseEffect->GetFullName());
        effects.push_back(active_effect);
      }
    }
  }

  return effects;
}

auto get_weapon_range_type(const bool is_left) -> RangeType
{

  const auto player = RE::PlayerCharacter::GetSingleton();
  if (!player) {
    return RangeType::kNone;
  }

  const auto hand_object = player->GetEquippedObject(is_left);
  if (!hand_object) {
    return RangeType::kNone;
  }

  auto hand_weapon = hand_object->As<RE::TESObjectWEAP>();
  if (!hand_weapon) {
    return RangeType::kNone;
  }

  if (hand_weapon->GetWeaponType() == RE::WeaponTypes::kBow) {
    return RangeType::kBow;
  }
  if (hand_weapon->GetWeaponType() == RE::WeaponTypes::kCrossbow) {
    return RangeType::kCrossbow;
  }
  if (hand_weapon->GetWeaponType() == RE::WeaponTypes::kStaff) {
    return RangeType::kStaff;
  }

  return RangeType::kNone;
}

auto GigaWidget::enchanted_count(const bool is_left) -> double
{

  const auto player = RE::PlayerCharacter::GetSingleton();
  if (!player) {
    return 0.0;
  }

  auto object = player->GetEquippedObject(is_left);
  if (!object) {
    return 0.0;
  }

  if (!object->IsWeapon()) {
    return 0.0;
  }

  const auto entry_data = player->GetEquippedEntryData(is_left);
  if (!entry_data) {
    return 0.0;
  }

  const auto ench = entry_data->GetEnchantment();
  if (!ench) {
    return 0.0;
  }
  const auto cost = ench->CalculateMagickaCost(player);
  if (cost <= 0.0) {
    return 0.0;
  }
  const auto final_cost = (is_left
                             ? player->GetActorValue(RE::ActorValue::kLeftItemCharge)
                             : player->GetActorValue(RE::ActorValue::kRightItemCharge)) /
                          cost;

  return final_cost;
  return entry_data->GetEnchantmentCharge().value_or(0.0);

  if (is_left) {
    // return player->GetActorValue(RE::ActorValue::kLeftItemCharge);
  }
  return player->GetActorValue(RE::ActorValue::kRightItemCharge);
}

auto GigaWidget::get_poison_count(const bool is_left) -> std::uint32_t
{

  const auto player = RE::PlayerCharacter::GetSingleton();
  if (!player) {
    return 0;
  }

  auto hand_data = player->GetEquippedEntryData(is_left);
  if (!hand_data) {
    return 0;
  }

  if (!hand_data->IsPoisoned()) {
    return 0;
  }

  const auto extra_lists = hand_data->extraLists;

  if (!extra_lists) {
    return 0;
  }

  for (const auto& x_list : *extra_lists) {
    const auto x_poison = x_list->GetByType<RE::ExtraPoison>();
    if (x_poison) {
      return x_poison->count;
    }
  }

  return 0;
}

auto GigaWidget::get_ammo_count() -> std::int32_t
{
  auto player = RE::PlayerCharacter::GetSingleton();
  if (!player) {
    return 0;
  }

  auto ammo = player->GetCurrentAmmo();
  if (!ammo) {
    return 0;
  }

  let current_ammo_count = player->GetItemCount(ammo);

  return current_ammo_count;
}

auto GigaWidget::get_object_name(HandType hand_type) -> const char*
{

  auto player = RE::PlayerCharacter::GetSingleton();
  if (!player) {
    return "";
  }

  const char* object_name = "";

  switch (hand_type) {
    case HandType::kRightHand: {
      const auto hand_object = player->GetEquippedEntryData(false);
      if (hand_object) {
        object_name = hand_object->GetDisplayName();
      }
      const auto object = player->GetEquippedObject(false);
      if (object) {
        object_name = object->GetName();
      }
      break;
    }
    case HandType::kLeftHand: {
      auto object = player->GetEquippedObject(true);
      if (object) {
        let range_type = get_weapon_range_type(true);
        if (range_type == RangeType::kCrossbow || range_type == RangeType::kBow) {
          let ammo = player->GetCurrentAmmo();
          if (ammo) {
            return ammo->GetFullName();
          }
        }
        object_name = object->GetName();
      }
      auto hand_object = player->GetEquippedEntryData(true);
      if (hand_object) {
        object_name = hand_object->GetDisplayName();
      }
      if (object) {
        let weapon = object->As<RE::TESObjectWEAP>();
        if (weapon) {
          if (weapon->IsTwoHandedAxe() || weapon->IsTwoHandedSword()) {
            object_name = "";
          }
        }
      }
      break;
    }
    case HandType::kCenterHand: {

      const auto spell = player->selectedPower;
      if (!spell) {
        logger::debug("Null Current spell"sv);
        return object_name;
      }

      object_name = spell->GetName();
      break;
    }
    default: {
      return object_name;
    }
  }

  return object_name;
}

GigaWidget::GigaWidget()
{
  const auto scale_form_manager = RE::BSScaleformManager::GetSingleton();

  logger::info("Construct menu");

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

  scale_form_manager->LoadMovieEx(this,
                                  MENU_PATH,
                                  [](RE::GFxMovieDef* def) -> void {
                                    def->SetState(RE::GFxState::StateType::kLog,
                                                  RE::make_gptr<GagaWidgetLogger>().get());
                                  });

  if (auto scaleform = RE::BSScaleformManager::GetSingleton()) {
    scaleform->LoadMovie(this, this->uiMovie, MENU_PATH);
  }

  logger::info("Finish construct menu");
}

auto GigaWidget::register_() -> void
{
  if (const auto ui = RE::UI::GetSingleton()) {
    ui->Register(MENU_NAME, creator);
    logger::info("Register menu");
    show();
  }
}

auto GigaWidget::show() -> void
{
  if (const auto message_queue = RE::UIMessageQueue::GetSingleton()) {
    logger::debug("Show menu");
    message_queue->AddMessage(MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
  }
}

auto GigaWidget::hide() -> void
{
  if (const auto message_queue = RE::UIMessageQueue::GetSingleton()) {
    logger::debug("Hide menu");
    message_queue->AddMessage(MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
  }
}

auto GigaWidget::update() -> void
{

  const auto ui = RE::UI::GetSingleton();
  if (!ui || ui->GameIsPaused()) {
    return;
  }

  const auto giga_widget = ui->GetMenu<GigaWidget>(MENU_NAME);
  if (!giga_widget || !giga_widget->uiMovie) {
    return;
  }

  apply_layout(giga_widget);
}

void GigaWidget::AdvanceMovie(const float interval, const uint32_t current_time)
{
  logger::debug("AdvanceMovie");
  update();
  IMenu::AdvanceMovie(interval, current_time);
}

auto GigaWidget::toggle_visibility_tween(const bool mode) -> void
{
  const auto ui = RE::UI::GetSingleton();
  if (!ui) {
    return;
  }

  const auto overlay_menu = ui->GetMenu(MENU_NAME);
  if (!overlay_menu || !overlay_menu->uiMovie) {
    return;
  }

  in_tween_menu = mode;

  auto& settings = Settings::get_singleton();
  let stats_visible = in_tween_menu && settings.bStatsMenuWidget;
  logi("toggle_visibility_tween Stats visible: {} In Tween Menu: {} Settings: {}",
       stats_visible,
       in_tween_menu,
       settings.bStatsMenuWidget);
  overlay_menu->uiMovie->SetVariable("mirelgigawidget.Stats._visible", RE::GFxValue{stats_visible});

  // overlay_menu->uiMovie->SetVisible(mode);
}

auto GigaWidget::toggle_visibility(const bool mode, RE::BSFixedString menu) -> void
{
  const auto ui = RE::UI::GetSingleton();
  if (!ui) {
    return;
  }

  const auto overlay_menu = ui->GetMenu(MENU_NAME);
  if (!overlay_menu || !overlay_menu->uiMovie) {
    return;
  }

  in_menu = !mode;
  for (auto& [menu_path, function_bool] : MENUS_TO_HIDE_WHEN_OPEN_MENU) {
    let result = function_bool(menu);
    bool boolean;
    switch (result) {
      case MenuHideResult::kTrue: {
        boolean = true;
        break;
      }
      case MenuHideResult::kFalse: {
        boolean = false;
        break;
      }
      case MenuHideResult::kSkip:
      default: {
        boolean = false;
        break;
      }
    }
    let visible = mode && boolean;
    //logi("[GigaWidget::toggle_visibility] Menu Path: {} InMenu: {} Mode: {} VisibleResult: {} ResultVisible: {}", menu_path, in_menu, mode, visible, static_cast<int>(result));
    if (result == MenuHideResult::kSkip) {
      continue;
    }
    overlay_menu->uiMovie->SetVariable(menu_path.c_str(), RE::GFxValue{visible});
  }
  // overlay_menu->uiMovie->SetVisible(mode);
}

auto GigaWidget::evalute_visible(std::map<IconElementType, PathVisiblePair>* hand_map, bool is_left) -> void
{
  for (const auto key : *hand_map | views::keys) {
    switch (key) {
      case kPoisonIcon: {
        hand_map->at(key).is_visible = weapon_is_poisoned(is_left);
        continue;
      }
      case kPoisonText: {
        const auto poison_count = get_poison_count(is_left);
        if (poison_count > 0) {
          logger::debug("Poison Count Visible");
          hand_map->at(key).is_visible = true;
        }
        continue;
      }
      case kEnchantIcon: {
        hand_map->at(key).is_visible = weapon_is_enchanted(is_left);
        continue;
      }
      case kEnchantText: {
        const auto charge_power = enchanted_count(is_left);
        if (charge_power > 0.0) {
          hand_map->at(key).is_visible = true;
        }
        continue;
      }
      case kScrollIcon: {
        const auto player = RE::PlayerCharacter::GetSingleton();
        if (player) {
          const auto object = player->GetEquippedObject(is_left);
          if (object) {
            hand_map->at(key).is_visible = object->Is(RE::FormType::Scroll);
          }
        }
        continue;
      }
      case kWeaponIcon: {
        const auto player = RE::PlayerCharacter::GetSingleton();
        if (player) {
          auto object = player->GetEquippedObject(is_left);
          if (object) {
            if (get_weapon_range_type(is_left) != RangeType::kNone) {
              hand_map->at(key).is_visible = false;
            } else {
              const auto object_name =
                  static_cast<std::string>(get_object_name(is_left ? HandType::kLeftHand : HandType::kRightHand));
              hand_map->at(key).is_visible = object->IsWeapon() && object_name != ""s;
            }
          }
        }
        continue;
      }
      case kFistIcon: {
        auto player = RE::PlayerCharacter::GetSingleton();
        if (player) {
          const auto entry_data = player->GetEquippedEntryData(is_left);
          if (entry_data) {
            continue;
          }
          auto object = player->GetEquippedObject(is_left);
          if (object) {
            const auto object_name =
                static_cast<std::string>(get_object_name(is_left ? HandType::kLeftHand : HandType::kRightHand));
            if (object->IsMagicItem() || object_name != ""s) {
              continue;
            }
            hand_map->at(key).is_visible = true;
          }
        }
        continue;
      }
      case kShieldIcon: {
        const auto player = RE::PlayerCharacter::GetSingleton();
        if (player) {
          const auto object = player->GetEquippedObject(is_left);
          if (object) {
            hand_map->at(key).is_visible = object->IsArmor();
          }
        }
        continue;
      }
      case kMagickIcon: {
        const auto player = RE::PlayerCharacter::GetSingleton();
        if (player) {
          const auto object = player->GetEquippedObject(is_left);
          if (object) {
            hand_map->at(key).is_visible = object->Is(RE::FormType::Spell);
          }
        }
        continue;
      }
      case kArrowIcon: {

        const auto player = RE::PlayerCharacter::GetSingleton();
        if (player) {
          let range_type = get_weapon_range_type(is_left);
          if (range_type == RangeType::kCrossbow || range_type == RangeType::kBow) {
            let ammo = player->GetCurrentAmmo();
            if (ammo) {
              hand_map->at(key).is_visible = true;
            } else {
              hand_map->at(key).is_visible = false;
            }
          } else {
            hand_map->at(key).is_visible = false;
          }
        }
        continue;
      }
      case kBowIcon: {
        const auto player = RE::PlayerCharacter::GetSingleton();
        if (player) {
          let range_type = get_weapon_range_type(is_left);
          if (range_type == RangeType::kCrossbow || range_type == RangeType::kBow) {
            hand_map->at(key).is_visible = true;
          } else {
            hand_map->at(key).is_visible = false;
          }
        }
        continue;
      }
      case kStaffIcon: {
        const auto player = RE::PlayerCharacter::GetSingleton();
        if (player) {
          let range_type = get_weapon_range_type(is_left);
          if (range_type == RangeType::kStaff) {
            hand_map->at(key).is_visible = true;
          } else {
            hand_map->at(key).is_visible = false;
          }
        }
        continue;
      }
      case kTorchIcon: {
        const auto player = RE::PlayerCharacter::GetSingleton();
        if (player) {
          auto left_object = player->GetEquippedObject(true);
          if (left_object && left_object->As<RE::TESObjectLIGH>()) {
            hand_map->at(key).is_visible = true;
          } else {
            hand_map->at(key).is_visible = false;
          }
        }
        continue;
      }
    }
  }
}

struct PotionSettings final
{
  bool visible;
  bool flash;
  double x;
  double y;
  double scale;
  bool text;
};

struct KiEnergySettings final
{
  bool visible;
  double x;
  double y;
  double scale;
  bool text;
};

struct AbsorbShieldSettings final
{
  bool visible;
  double x;
  double y;
  double scaleX;
  double scaleY;
};

struct SurvivalSettings final
{
  bool visible;
  double x;
  double y;
  double scale;
  bool text;
};

enum RestedStage
{
  kRestedNone = 0,
  kExhaustion1 = 1,
  kExhaustion2 = 1,
  kExhaustion3 = 2,
  kExhaustion4 = 3,
  kExhaustion5 = 4,
  kRested = 5,
  kRestedWell = 6,
  kRestedLovers = 7
};

enum HungerStage
{
  kHungerNone = 0,
  kFed = 1,
  kFedWell = 2,
  kFedLovers = 3,
  kFedVampire = 4,
  kHunger1 = 5,
  kHunger2 = 6,
  hHunger3 = 7
};

enum WeatherStage
{
  kWeatherNone = 0,
  kCold0 = 4,
  kCold1 = 5,
  kCold2 = 5,
  kCold3 = 1,
  kCold4 = 2,
  kCold5 = 3
};

static bool player_has_spell(RE::SpellItem* spell)
{
  if (!spell) {
    return false;
  }
  return RE::PlayerCharacter::GetSingleton()->HasSpell(spell);
}

static RestedStage get_rested_stage(const Settings& settings)
{
  const auto& game_data = settings.game_data;

  if (player_has_spell(game_data.rested)) {
    return RestedStage::kRested;
  }
  if (player_has_spell(game_data.rested_well)) {
    return RestedStage::kRestedWell;
  }
  if (player_has_spell(game_data.rested_marriage)) {
    return RestedStage::kRestedLovers;
  }

  if (player_has_spell(game_data.exhaustion1)) {
    return RestedStage::kExhaustion1;
  }
  if (player_has_spell(game_data.exhaustion2)) {
    return RestedStage::kExhaustion2;
  }
  if (player_has_spell(game_data.exhaustion3)) {
    return RestedStage::kExhaustion3;
  }
  if (player_has_spell(game_data.exhaustion4)) {
    return RestedStage::kExhaustion4;
  }
  if (player_has_spell(game_data.exhaustion5)) {
    return RestedStage::kExhaustion5;
  }

  return RestedStage::kRestedNone;
}

static HungerStage get_hunger_stage(const Settings& settings)
{
  const auto& game_data = settings.game_data;

  if (player_has_spell(game_data.fed)) {
    return HungerStage::kFed;
  }
  if (player_has_spell(game_data.fed_well)) {
    return HungerStage::kFedWell;
  }
  if (player_has_spell(game_data.fed_marriage)) {
    return HungerStage::kFedLovers;
  }
  if (player_has_spell(game_data.fed_vampire)) {
    return HungerStage::kFedVampire;
  }

  if (player_has_spell(game_data.hunger1)) {
    return HungerStage::kHunger1;
  }
  if (player_has_spell(game_data.hunger2)) {
    return HungerStage::kHunger2;
  }
  if (player_has_spell(game_data.hunger3)) {
    return HungerStage::hHunger3;
  }

  return HungerStage::kHungerNone;
}

static WeatherStage get_weather_stage(const Settings& settings)
{
  const auto& game_data = settings.game_data;

  if (player_has_spell(game_data.cold0)) {
    return WeatherStage::kCold0;
  }
  if (player_has_spell(game_data.cold1)) {
    return WeatherStage::kCold1;
  }
  if (player_has_spell(game_data.cold2)) {
    return WeatherStage::kCold2;
  }
  if (player_has_spell(game_data.cold3)) {
    return WeatherStage::kCold3;
  }
  if (player_has_spell(game_data.cold4)) {
    return WeatherStage::kCold4;
  }
  if (player_has_spell(game_data.cold5)) {
    return WeatherStage::kCold5;
  }

  return WeatherStage::kWeatherNone;
}

static int get_stress_value(const Settings& settings)
{
  const auto& game_data = settings.game_data;
  if (!game_data.stress_total) {
    return -1;
  }
  return static_cast<int>(game_data.stress_total->value);
}

auto set_potion_setting(const RE::GPtr<GigaWidget>& giga_widget,
                        const PotionSettings& settings,
                        const std::string&& potion_type) -> void
{

  let variable_visible = "mirelgigawidget.Potion"s + potion_type + "Holder._visible"s;
  let variable_flash = "mirelgigawidget.Potion"s + potion_type + "Holder.FlashLight._visible"s;
  let variable_x = "mirelgigawidget.Potion"s + potion_type + "Holder._x"s;
  let variable_y = "mirelgigawidget.Potion"s + potion_type + "Holder._y"s;
  let variable_xscale = "mirelgigawidget.Potion"s + potion_type + "Holder._yscale"s;
  let variable_yscale = "mirelgigawidget.Potion"s + potion_type + "Holder._xscale"s;
  let variable_text = "mirelgigawidget.Potion"s + potion_type + "Holder.PotionCountText._visible"s;

  // logi("PotionType: {}", potion_type);
  // logi("Visible: {}", settings.visible);
  // logi("X: {}", settings.x);
  // logi("Y: {}", settings.y);
  // logi("Scale: {}", settings.scale);
  // logi("TextVisible: {}", settings.text);

  giga_widget->uiMovie->SetVariable(variable_visible.c_str(), RE::GFxValue{in_menu ? false : settings.visible});
  giga_widget->uiMovie->SetVariable(variable_flash.c_str(), RE::GFxValue{settings.flash});
  giga_widget->uiMovie->SetVariable(variable_x.c_str(), RE::GFxValue{settings.x});
  giga_widget->uiMovie->SetVariable(variable_y.c_str(), RE::GFxValue{settings.y});
  giga_widget->uiMovie->SetVariable(variable_xscale.c_str(), RE::GFxValue{settings.scale});
  giga_widget->uiMovie->SetVariable(variable_yscale.c_str(), RE::GFxValue{settings.scale});
  giga_widget->uiMovie->SetVariable(variable_text.c_str(), RE::GFxValue{settings.text});
}

auto set_ki_energy_setting(const RE::GPtr<GigaWidget>& giga_widget, const KiEnergySettings& settings) -> void
{

  let variable_visible = "mirelgigawidget.KiEnergy._visible"s;
  let variable_x = "mirelgigawidget.KiEnergy._x"s;
  let variable_y = "mirelgigawidget.KiEnergy._y"s;
  let variable_xscale = "mirelgigawidget.KiEnergy._yscale"s;
  let variable_yscale = "mirelgigawidget.KiEnergy._xscale"s;
  let variable_text = "mirelgigawidget.KiEnergy.EnergyCountText._visible"s;

  giga_widget->uiMovie->SetVariable(variable_visible.c_str(), RE::GFxValue{in_menu ? false : settings.visible});
  giga_widget->uiMovie->SetVariable(variable_x.c_str(), RE::GFxValue{settings.x});
  giga_widget->uiMovie->SetVariable(variable_y.c_str(), RE::GFxValue{settings.y});
  giga_widget->uiMovie->SetVariable(variable_xscale.c_str(), RE::GFxValue{settings.scale});
  giga_widget->uiMovie->SetVariable(variable_yscale.c_str(), RE::GFxValue{settings.scale});
  giga_widget->uiMovie->SetVariable(variable_text.c_str(), RE::GFxValue{settings.text});
}

auto set_absorb_shield_setting(const RE::GPtr<GigaWidget>& giga_widget, const AbsorbShieldSettings& settings) -> void
{

  let variable_visible = "mirelgigawidget.AbsorbShieldWidget._visible"s;
  let variable_x = "mirelgigawidget.AbsorbShieldWidget._x"s;
  let variable_y = "mirelgigawidget.AbsorbShieldWidget._y"s;
  let variable_xscale = "mirelgigawidget.AbsorbShieldWidget._yscale"s;
  let variable_yscale = "mirelgigawidget.AbsorbShieldWidget._xscale"s;

  giga_widget->uiMovie->SetVariable(variable_visible.c_str(), RE::GFxValue{in_menu ? false : settings.visible});
  giga_widget->uiMovie->SetVariable(variable_x.c_str(), RE::GFxValue{settings.x});
  giga_widget->uiMovie->SetVariable(variable_y.c_str(), RE::GFxValue{settings.y});
  giga_widget->uiMovie->SetVariable(variable_xscale.c_str(), RE::GFxValue{settings.scaleX});
  giga_widget->uiMovie->SetVariable(variable_yscale.c_str(), RE::GFxValue{settings.scaleY});
}

auto get_actor_value_max(RE::Actor* actor, const RE::ActorValue av, const bool fixed) -> float
{
  if (!actor) {
    return 0.f;
  }
  if (fixed) {
    return actor->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, av) + actor->GetPermanentActorValue(av);
  }
  return actor->GetPermanentActorValue(av);
}

float actor_value_current_percent(RE::Actor* actor, const RE::ActorValue av)
{
  if (!actor) {
    return 0.0f;
  }
  auto& settings = Settings::get_singleton();
  let max_av = get_actor_value_max(actor, av, settings.bStatsMenuWidgetRegenFixed);
  let current_av = actor->GetActorValue(av);
  let current_percent = (current_av / (max_av / 100.f));
  return current_percent;
}

auto actor_value_regeneration_value(const RE::ActorValue av,
                                    const RE::ActorValue av_rate,
                                    const RE::ActorValue av_rate_mult) -> float
{

  const auto player = RE::PlayerCharacter::GetSingleton();
  if (!player) {
    return 0.f;
  }

  const auto active_effects = player->GetActiveEffectList();
  if (!active_effects) {
    return 0.f;
  }

  const auto validate_active_effect = [](const RE::ActiveEffect* active_effect) -> bool {
    return active_effect && !active_effect->flags.any(RE::ActiveEffect::Flag::kInactive) && active_effect->effect &&
           active_effect->effect->baseEffect &&
           !active_effect->effect->baseEffect->data.flags.any(RE::EffectSetting::EffectSettingData::Flag::kRecover) &&
           !active_effect->effect->baseEffect->IsDetrimental();
  };

  const auto value_peak_mod_or_value_mod_is_av = [av](const RE::ActiveEffect* active_effect) -> bool {
    return (active_effect->effect->baseEffect->HasArchetype(RE::EffectSetting::Archetype::kPeakValueModifier) ||
            active_effect->effect->baseEffect->HasArchetype(RE::EffectSetting::Archetype::kValueModifier)) &&
           active_effect->effect->baseEffect->data.primaryAV == av;
  };

  auto restore_value_counter = 0.f;

  for (const auto active_effect : *active_effects) {

    if (!validate_active_effect(active_effect)) {
      continue;
    }

    if (value_peak_mod_or_value_mod_is_av(active_effect)) {
      restore_value_counter += active_effect->magnitude;
      continue;
    }

    if (active_effect->effect->baseEffect->HasArchetype(RE::EffectSetting::Archetype::kDualValueModifier)) {

      const auto base_effect = active_effect->effect->baseEffect;

      if (base_effect->data.primaryAV == av && base_effect->data.secondaryAV != av) {
        restore_value_counter += active_effect->magnitude;
      } else if (base_effect->data.primaryAV == av && base_effect->data.secondaryAV == av) {
        restore_value_counter += (active_effect->magnitude * base_effect->data.secondAVWeight);
        restore_value_counter += active_effect->magnitude;
      } else if (base_effect->data.secondaryAV == av) {
        restore_value_counter += (active_effect->magnitude * base_effect->data.secondAVWeight);
      }
    }
  }

  auto regeneration = 0.f;

  const auto rate = player->GetActorValue(av_rate);
  const auto mult_rate = player->GetActorValue(av_rate_mult);

  if (rate <= 0.f || mult_rate <= 0.f) {
    regeneration = 0.f;
  } else {
    auto& settings = Settings::get_singleton();
    const auto max_value = get_actor_value_max(player, av, settings.bStatsMenuWidgetRegenFixed);
    regeneration = (max_value * (rate * 0.01f)) * (mult_rate * 0.01f);
  }

  return restore_value_counter + regeneration;
}

void set_dynamic_stats_widget_values(const RE::GPtr<GigaWidget>& giga_widget, Settings& settings)
{

  auto player = RE::PlayerCharacter::GetSingleton();
  auto& wdata = WidgetData::get_singleton();

  let armor = get_effects_magnitude_sum(get_effects_by_actor_value(player, wdata.iActorValueIndexDamageResist));
  let speed = get_effects_magnitude_sum(get_effects_by_actor_value(player, wdata.iActorValueIndexSpeedMult));

  let absorb = get_effects_magnitude_sum(get_effects_by_actor_value(player, wdata.iActorValueIndexAbsorbChance));

  let magic = get_effects_magnitude_sum(get_effects_by_actor_value(player, wdata.iActorValueIndexMagickResist));
  let fire = get_effects_magnitude_sum(get_effects_by_actor_value(player, wdata.iActorValueIndexFireResist));
  let shock = get_effects_magnitude_sum(get_effects_by_actor_value(player, wdata.iActorValueIndexShockResist));
  let frost = get_effects_magnitude_sum(get_effects_by_actor_value(player, wdata.iActorValueIndexFrostResist));

  let poison = get_effects_magnitude_sum(get_effects_by_actor_value(player, wdata.iActorValueIndexPoisonResist));
  let disease = get_effects_magnitude_sum(get_effects_by_actor_value(player, wdata.iActorValueIndexDiseaseResist));

  const RE::GFxValue stats_background01_arguments[5]{absorb, poison, disease, armor, speed};
  const RE::GFxValue stats_background02_arguments[4]{magic, fire, frost, shock};
  const RE::GFxValue always_visible[1]{settings.bDynamicStatsMenuWidgetAlways};
  const RE::GFxValue always_disable[2]{settings.bDynamicStatsAlwaysHideBackground,
                                       settings.bDynamicStatsAlwaysHideBaseBackground};

  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setDynamicStatsBackground01", stats_background01_arguments, 5);
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setDynamicStatsBackground02", stats_background02_arguments, 4);
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setDynamicStatsAlwaysVisible", always_visible, 1);
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setDynamicStatsAlwaysDisable", always_disable, 2);
}

float adjust_armor_raiting(float armor)
{

  let settings = Settings::get_singleton();
  if (!settings.bResimonrimArmorToResist) {
    return armor;
  }

  auto factor = RE::GameSettingCollection::GetSingleton()->GetSetting("fArmorScalingFactor")->GetFloat();
  auto a_vanilla = armor * factor;

  static constexpr float HARD_FACTOR = 0.025f;
  static constexpr float ARMOR_RATING_SOFT_CAP = 600;
  static constexpr float ARMOR_RATING_HARD_CAP = 1200;
  static constexpr float SOFT_CAP = 75.f;
  static constexpr float HARD_CAP = 90.f;

  if (armor <= ARMOR_RATING_SOFT_CAP) {
    return a_vanilla;
  } else if (armor < ARMOR_RATING_HARD_CAP) {
    auto remainderRating = armor - ARMOR_RATING_SOFT_CAP;
    return SOFT_CAP + (remainderRating * HARD_FACTOR);
  } else {
    return HARD_CAP;
  }
}

auto worn_has_keyword(RE::Actor* actor, RE::BGSKeyword* keyword) -> bool
{
  if (actor && keyword) {
    auto inv = actor->GetInventoryChanges();
    if (!inv) {
      return false;
    }
    using FuncT = bool (*)(RE::InventoryChanges*, RE::BGSKeyword*);
    const REL::Relocation<FuncT> func{RELOCATION_ID(15808, 0)};
    return func(inv, keyword);
  }
  return false;
}

auto try_actor_has_active_mgef_with_keyword(RE::Actor* actor, const RE::BGSKeyword* keyword) -> bool
{
  if (!actor || !keyword) {
    return false;
  }

  auto active_effects = actor->GetActiveEffectList();
  if (!active_effects) {
    return false;
  }

  for (const auto active_effect : *active_effects) {
    if (!active_effect || active_effect->flags.any(RE::ActiveEffect::Flag::kInactive) || !active_effect->effect ||
        !active_effect->effect->baseEffect) {
      continue;
    }
    const auto base_effect = active_effect->effect->baseEffect;

    if (base_effect->HasKeyword(keyword)) {
      return true;
    }
  }
  return false;
}

auto try_has_absolute_keyword(RE::Actor* actor, RE::BGSKeyword* keyword) -> bool
{
  if (!actor || !keyword) {
    return false;
  }
  const auto result = actor->HasKeyword(keyword) || try_actor_has_active_mgef_with_keyword(actor, keyword) ||
                      worn_has_keyword(actor, keyword);
  return result;
}

RE::Actor* get_combat_target()
{
  auto player = RE::PlayerCharacter::GetSingleton();
  letr settings = Settings::get_singleton();
  if (settings.bTargetDynamicStatsActiveOnlyWithKeyword) {
    if (!try_has_absolute_keyword(player, settings.game_data.target_resists_keyword)) {
      return nullptr;
    }
  }

  let tdm = ExternalApi::get_tdm_api();

  if (settings.bTargetDynamicStatsActiveOnlyWithTargetLock && (!tdm || !tdm->GetCurrentTarget())) {
    return nullptr;
  }
  
  RE::ActorHandle target = tdm ? tdm->GetCurrentTarget() : player->currentCombatTarget;

  if (!target) {
    //logi("Combat target first null");
    target = player->currentCombatTarget;
  }

  if (!target && player->combatController) {
    target = player->combatController->targetHandle;
  }

  if (!target && player->combatController && player->combatController->cachedTarget) {
    target = player->combatController->cachedTarget.get();
  }

  if (!target) {
    //logi("Combat target second null");
    return nullptr;
  }

  if (!target.get()) {
    //logi("Combat target three null");
    return nullptr;
  }

  //logi("Combat target get");
  return target.get().get();

}

struct StatColors final
{
  float white;
  float black;
  float neutral;
};

// 4 - disable, 1 - neutral, 2 - negative, 3 - positive
uint8_t get_target_stat_color(RE::Actor* target, const RE::ActorValue av, const StatColors colors)
{
  if (!target) {
    return 4;
  }

  letr settings = Settings::get_singleton();

  if (!settings.bTargetDynamicStatsAltColoring) {
    return get_effects_magnitude_sum(get_effects_by_actor_value(target, av));
  }

  // let black = static_cast<float>(settings.fTargetDynamicStatBackgroundResistBlack);
  // let white = static_cast<float>(settings.fTargetDynamicStatBackgroundResistWhite);
  // let neutral = static_cast<float>(settings.fTargetDynamicStatBackgroundResistNeutral);

  auto av_value = target->GetActorValue(av);
  if (settings.bResimonrimArmorToResist && av == RE::ActorValue::kDamageResist) {
    av_value = adjust_armor_raiting(av_value);
  }
  if (av_value < colors.white) {
    return 3;
  }
  if (av_value < colors.black) {
    return 4;
  }
  if (av_value < colors.neutral) {
    return 1;
  }

  return 2;

}

void set_target_dynamic_stats_widget_values(const RE::GPtr<GigaWidget>& giga_widget, Settings& settings)
{

  auto target = get_combat_target();
  auto& wdata = WidgetData::get_singleton();

  if (!target) {
    return;
  }

  // let black = static_cast<float>(settings.fTargetDynamicStatBackgroundResistBlack);
  // let white = static_cast<float>(settings.fTargetDynamicStatBackgroundResistWhite);
  // let neutral = static_cast<float>(settings.fTargetDynamicStatBackgroundResistNeutral);

  let default_colors = StatColors{static_cast<float>(settings.fTargetDynamicStatBackgroundResistWhite),
                                  static_cast<float>(settings.fTargetDynamicStatBackgroundResistBlack),
                                  static_cast<float>(settings.fTargetDynamicStatBackgroundResistNeutral)};
  let armor_colors = StatColors{static_cast<float>(settings.fTargetDynamicStatBackgroundResistArmorWhite),
                                static_cast<float>(settings.fTargetDynamicStatBackgroundResistArmorBlack),
                                static_cast<float>(settings.fTargetDynamicStatBackgroundResistArmorNeutral)};

  let armor = get_target_stat_color(target,
                                    wdata.iActorValueIndexDamageResist,
                                    settings.bResimonrimArmorToResist ? default_colors : armor_colors);
  //let speed = get_target_stat_color(target, wdata.iActorValueIndexSpeedMult);
  let speed = 4;

  let absorb = get_target_stat_color(target, wdata.iActorValueIndexAbsorbChance, default_colors);

  let magic = get_target_stat_color(target, wdata.iActorValueIndexMagicka, default_colors);
  let fire = get_target_stat_color(target, wdata.iActorValueIndexFireResist, default_colors);
  let shock = get_target_stat_color(target, wdata.iActorValueIndexShockResist, default_colors);
  let frost = get_target_stat_color(target, wdata.iActorValueIndexFrostResist, default_colors);

  let poison = get_target_stat_color(target, wdata.iActorValueIndexPoisonResist, default_colors);
  let disease = get_target_stat_color(target, wdata.iActorValueIndexDiseaseResist, default_colors);

  const RE::GFxValue stats_background01_arguments[5]{absorb, poison, disease, armor, speed};
  const RE::GFxValue stats_background02_arguments[4]{magic, fire, frost, shock};
  const RE::GFxValue always_visible[1]{settings.bTargetDynamicStatsMenuWidgetAlways};
  const RE::GFxValue always_disable[2]{settings.bTargetDynamicStatsAlwaysHideBackground,
                                       settings.bTargetDynamicStatsAlwaysHideBaseBackground};

  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setTargetDynamicStatsBackground01",
                                       stats_background01_arguments,
                                       5);
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setTargetDynamicStatsBackground02",
                                       stats_background02_arguments,
                                       4);
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setTargetDynamicStatsAlwaysVisible", always_visible, 1);
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setTargetDynamicStatsAlwaysDisable", always_disable, 2);

  let magic_resist = std::format("{}", static_cast<int32_t>(target->GetActorValue(wdata.iActorValueIndexMagickResist)));
  let fire_resist = std::format("{}", static_cast<int32_t>(target->GetActorValue(wdata.iActorValueIndexFireResist)));
  let frost_resist = std::format("{}", static_cast<int32_t>(target->GetActorValue(wdata.iActorValueIndexFrostResist)));
  let electric_resist = std::format("{}",
                                    static_cast<int32_t>(target->GetActorValue(wdata.iActorValueIndexShockResist)));

  const RE::GFxValue resist_base_arguments[4]{
      magic_resist.c_str(), fire_resist.c_str(), frost_resist.c_str(), electric_resist.c_str()};
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setTargetDynamicStatsResistsBase", resist_base_arguments, 4);

  let absorb_chance = std::format("{}",
                                  static_cast<int32_t>(target->GetActorValue(wdata.iActorValueIndexAbsorbChance)));
  let poison_resist = std::format("{}",
                                  static_cast<int32_t>(target->GetActorValue(wdata.iActorValueIndexPoisonResist)));
  let disease_resist = std::format("{}",
                                   static_cast<int32_t>(target->GetActorValue(wdata.iActorValueIndexDiseaseResist)));

  const RE::GFxValue resist_extra_arguments[3]{absorb_chance.c_str(), poison_resist.c_str(), disease_resist.c_str()};
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setTargetDynamicStatsResistsExtra", resist_extra_arguments, 3);

  let armor_stat = std::format("{}",
                               static_cast<int32_t>(adjust_armor_raiting(
                                   target->GetActorValue(wdata.iActorValueIndexDamageResist))));
  let speed_stat = std::format("{}", static_cast<int32_t>(target->GetActorValue(wdata.iActorValueIndexSpeedMult)));

  let level_stat = std::format("{}", target->GetLevel());
  
  const RE::GFxValue base_stats_arguments[3]{
      armor_stat.c_str(), speed_stat.c_str(), level_stat.c_str()};
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setTargetDynamicStatsBaseStats", base_stats_arguments, 3);

}

/*float adjust_armor_raiting(float armor)
{
  auto factor = RE::GameSettingCollection::GetSingleton()->GetSetting("fArmorScalingFactor")->GetFloat();
  auto a_vanilla = armor * factor;

  static constexpr float HARD_FACTOR = 0.025f;
  static constexpr float ARMOR_RATING_SOFT_CAP = 600;
  static constexpr float ARMOR_RATING_HARD_CAP = 1200;
  static constexpr float SOFT_CAP = 75.f;
  static constexpr float HARD_CAP = 90.f;

  if (armor <= ARMOR_RATING_SOFT_CAP) {
    return a_vanilla;
  } else if (armor < ARMOR_RATING_HARD_CAP) {
    auto remainderRating = armor - ARMOR_RATING_SOFT_CAP;
    return SOFT_CAP + (remainderRating * HARD_FACTOR);
  } else {
    return HARD_CAP;
  }
}*/

void set_stats_widget_values(const RE::GPtr<GigaWidget>& giga_widget, Settings& settings)
{

  auto player = RE::PlayerCharacter::GetSingleton();
  auto& wdata = WidgetData::get_singleton();

  let health_cuurent_max = std::format(
      "C/M: {} / {}",
      static_cast<int32_t>(player->GetActorValue(wdata.iActorValueIndexHealth)),
      static_cast<int32_t>(get_actor_value_max(player,
                                               wdata.iActorValueIndexHealth,
                                               settings.bStatsMenuWidgetRegenFixed)));

  let magicka_cuurent_max = std::format(
      "C/M: {} / {}",
      static_cast<int32_t>(player->GetActorValue(wdata.iActorValueIndexMagicka)),
      static_cast<int32_t>(get_actor_value_max(player,
                                               wdata.iActorValueIndexMagicka,
                                               settings.bStatsMenuWidgetRegenFixed)));

  let stamina_cuurent_max = std::format(
      "C/M: {} / {}",
      static_cast<int32_t>(player->GetActorValue(wdata.iActorValueIndexStamina)),
      static_cast<int32_t>(get_actor_value_max(player,
                                               wdata.iActorValueIndexStamina,
                                               settings.bStatsMenuWidgetRegenFixed)));

  let health_regen =
      std::format("FR: {}",
                  static_cast<int32_t>(actor_value_regeneration_value(
                      wdata.iActorValueIndexHealth,
                      wdata.iActorValueIndexHealthRate,
                      wdata.iActorValueIndexHealthRateMult)));

  let magicka_regen =
      std::format("FR: {}",
                  static_cast<int32_t>(actor_value_regeneration_value(
                      wdata.iActorValueIndexMagicka,
                      wdata.iActorValueIndexMagickaRate,
                      wdata.iActorValueIndexMagickaRateMult)));

  let stamina_regen =
      std::format("FR: {}",
                  static_cast<int32_t>(actor_value_regeneration_value(
                      wdata.iActorValueIndexStamina,
                      wdata.iActorValueIndexStaminaRate,
                      wdata.iActorValueIndexStaminaRateMult)));

  let health_percent =
      std::format("P: {}", static_cast<int32_t>(actor_value_current_percent(player, wdata.iActorValueIndexHealth)));
  let magicka_percent =
      std::format("P: {}", static_cast<int32_t>(actor_value_current_percent(player, wdata.iActorValueIndexMagicka)));
  let stamina_percent =
      std::format("P: {}", static_cast<int32_t>(actor_value_current_percent(player, wdata.iActorValueIndexStamina)));

  const RE::GFxValue health_arguments[4]{0, health_cuurent_max.c_str(), health_regen.c_str(), health_percent.c_str()};
  const RE::GFxValue magicka_arguments[4]{
      1, magicka_cuurent_max.c_str(), magicka_regen.c_str(), magicka_percent.c_str()};
  const RE::GFxValue stamina_arguments[4]{
      2, stamina_cuurent_max.c_str(), stamina_regen.c_str(), stamina_percent.c_str()};

  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setStatsValueStats", health_arguments, 4);
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setStatsValueStats", magicka_arguments, 4);
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setStatsValueStats", stamina_arguments, 4);

  let armor_stat = std::format("{}",
                               static_cast<int32_t>(adjust_armor_raiting(
                                   (player->GetActorValue(wdata.iActorValueIndexDamageResist)))));
  let speed_stat = std::format("{}", static_cast<int32_t>(player->GetActorValue(wdata.iActorValueIndexSpeedMult)));
  let noise_stat =
      std::format("{}", static_cast<int32_t>(player->GetActorValue(wdata.iActorValueIndexNoiseMult) * 100.f));
  let shout_stat =
      std::format("{}", static_cast<int32_t>(player->GetActorValue(wdata.iActorValueIndexShoutRecoveryMult) * 100.f));

  const RE::GFxValue base_stats_arguments[4]{
      armor_stat.c_str(), speed_stat.c_str(), noise_stat.c_str(), shout_stat.c_str()};
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setStatsBaseStats", base_stats_arguments, 4);

  let destruction_cost =
      std::format("{}", static_cast<int32_t>(player->GetActorValue(wdata.iActorValueIndexDestructionMod)));
  let restoration_cost =
      std::format("{}", static_cast<int32_t>(player->GetActorValue(wdata.iActorValueIndexRestorationMod)));
  let conjuration_cost =
      std::format("{}", static_cast<int32_t>(player->GetActorValue(wdata.iActorValueIndexConjurationMod)));
  let illusion_cost = std::format("{}", static_cast<int32_t>(player->GetActorValue(wdata.iActorValueIndexIllusionMod)));
  let alteration_cost =
      std::format("{}", static_cast<int32_t>(player->GetActorValue(wdata.iActorValueIndexAlterationMod)));

  const RE::GFxValue cost_stats_arguments[5]{destruction_cost.c_str(),
                                             restoration_cost.c_str(),
                                             conjuration_cost.c_str(),
                                             illusion_cost.c_str(),
                                             alteration_cost.c_str()};
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setStatsCostStats", cost_stats_arguments, 5);

  let magic_resist = std::format("{}", static_cast<int32_t>(player->GetActorValue(wdata.iActorValueIndexMagickResist)));
  let fire_resist = std::format("{}", static_cast<int32_t>(player->GetActorValue(wdata.iActorValueIndexFireResist)));
  let frost_resist = std::format("{}", static_cast<int32_t>(player->GetActorValue(wdata.iActorValueIndexFrostResist)));
  let electric_resist = std::format("{}",
                                    static_cast<int32_t>(player->GetActorValue(wdata.iActorValueIndexShockResist)));

  const RE::GFxValue resist_base_arguments[4]{
      magic_resist.c_str(), fire_resist.c_str(), frost_resist.c_str(), electric_resist.c_str()};
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setStatsResistsBase", resist_base_arguments, 4);

  let absorb_chance = std::format("{}",
                                  static_cast<int32_t>(player->GetActorValue(wdata.iActorValueIndexAbsorbChance)));
  let poison_resist = std::format("{}",
                                  static_cast<int32_t>(player->GetActorValue(wdata.iActorValueIndexPoisonResist)));
  let disease_resist = std::format("{}",
                                   static_cast<int32_t>(player->GetActorValue(wdata.iActorValueIndexDiseaseResist)));

  const RE::GFxValue resist_extra_arguments[3]{absorb_chance.c_str(), poison_resist.c_str(), disease_resist.c_str()};
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setStatsResistsExtra", resist_extra_arguments, 3);
}

auto set_survival_widget_setting(const RE::GPtr<GigaWidget>& giga_widget,
                                 const SurvivalSettings& settings,
                                 const std::string&& widget_name,
                                 bool is_stress) -> void
{
  let variable_visible = "mirelgigawidget."s + widget_name + "._visible"s;
  let variable_x = "mirelgigawidget."s + widget_name + "._x"s;
  let variable_y = "mirelgigawidget."s + widget_name + "._y"s;
  let variable_xscale = "mirelgigawidget."s + widget_name + "._yscale"s;
  let variable_yscale = "mirelgigawidget."s + widget_name + "._xscale"s;
  if (is_stress) {
    let variable_text = "mirelgigawidget."s + widget_name + ".StressCountText._visible"s;
    giga_widget->uiMovie->SetVariable(variable_text.c_str(), RE::GFxValue{settings.text});
  }

  giga_widget->uiMovie->SetVariable(variable_visible.c_str(), RE::GFxValue{in_menu ? false : settings.visible});
  giga_widget->uiMovie->SetVariable(variable_x.c_str(), RE::GFxValue{settings.x});
  giga_widget->uiMovie->SetVariable(variable_y.c_str(), RE::GFxValue{settings.y});
  giga_widget->uiMovie->SetVariable(variable_xscale.c_str(), RE::GFxValue{settings.scale});
  giga_widget->uiMovie->SetVariable(variable_yscale.c_str(), RE::GFxValue{settings.scale});
}

auto GigaWidget::apply_layout(const RE::GPtr<GigaWidget>& giga_widget) -> void
{
  if (!giga_widget || !giga_widget->uiMovie) {
    return;
  }

  static const RE::GFxValue update_delta{0.02};

  std::map<IconElementType, PathVisiblePair> right_map{
      {kEnchantIcon, PathVisiblePair{"mirelgigawidget.BaseWidget.rightHandIcon.rightLightningIcon._visible"}},
      {kPoisonIcon, PathVisiblePair{"mirelgigawidget.BaseWidget.rightHandIcon.rightPoisonIcon._visible"}},
      {kScrollIcon, PathVisiblePair{"mirelgigawidget.BaseWidget.rightHandIcon.rightScrollIcon._visible"}},
      {kWeaponIcon, PathVisiblePair{"mirelgigawidget.BaseWidget.rightHandIcon.rightWeaponIcon._visible"}},
      {kFistIcon, PathVisiblePair{"mirelgigawidget.BaseWidget.rightHandIcon.rightFistIcon._visible"}},
      {kShieldIcon, PathVisiblePair{"mirelgigawidget.BaseWidget.rightHandIcon.rightShieldIcon._visible"}},
      {kMagickIcon, PathVisiblePair{"mirelgigawidget.BaseWidget.rightHandIcon.rightMagickIcon._visible"}},
      {kEnchantText, PathVisiblePair{"mirelgigawidget.BaseWidget.rightHandIcon.rightEnchantText._visible"}},
      {kPoisonText, PathVisiblePair{"mirelgigawidget.BaseWidget.rightHandIcon.rightPoisonText._visible"}},
      {kBowIcon, PathVisiblePair{"mirelgigawidget.BaseWidget.rightHandIcon.BowIcon._visible"}},
      {kStaffIcon, PathVisiblePair{"mirelgigawidget.BaseWidget.rightHandIcon.StaffIcon._visible"}},
  };

  std::map<IconElementType, PathVisiblePair> left_map{
      {kEnchantIcon, PathVisiblePair{"mirelgigawidget.BaseWidget.leftHandIcon.leftLightningIcon._visible"}},
      {kPoisonIcon, PathVisiblePair{"mirelgigawidget.BaseWidget.leftHandIcon.leftPoisonIcon._visible"}},
      {kScrollIcon, PathVisiblePair{"mirelgigawidget.BaseWidget.leftHandIcon.leftScrollIcon._visible"}},
      {kWeaponIcon, PathVisiblePair{"mirelgigawidget.BaseWidget.leftHandIcon.leftWeaponIcon._visible"}},
      {kFistIcon, PathVisiblePair{"mirelgigawidget.BaseWidget.leftHandIcon.leftFistIcon._visible"}},
      {kShieldIcon, PathVisiblePair{"mirelgigawidget.BaseWidget.leftHandIcon.leftShieldIcon._visible"}},
      {kMagickIcon, PathVisiblePair{"mirelgigawidget.BaseWidget.leftHandIcon.leftMagickIcon._visible"}},
      {kEnchantText, PathVisiblePair{"mirelgigawidget.BaseWidget.leftHandIcon.leftEnchantText._visible"}},
      {kPoisonText, PathVisiblePair{"mirelgigawidget.BaseWidget.leftHandIcon.leftPoisonText._visible"}},
      {kArrowIcon, PathVisiblePair{"mirelgigawidget.BaseWidget.leftHandIcon.ArrowIcon._visible"}},
      {kStaffIcon, PathVisiblePair{"mirelgigawidget.BaseWidget.leftHandIcon.StaffIcon._visible"}},
      {kTorchIcon, PathVisiblePair{"mirelgigawidget.BaseWidget.leftHandIcon.TorchIcon._visible"}},
  };

  evalute_visible(&right_map, false);
  evalute_visible(&left_map, true);

  for (auto [path_to_variable, is_visible] : right_map | std::views::values) {
    giga_widget->uiMovie->SetVariable(path_to_variable, RE::GFxValue{is_visible});
  }

  for (auto [path_to_variable, is_visible] : left_map | std::views::values) {
    giga_widget->uiMovie->SetVariable(path_to_variable, RE::GFxValue{is_visible});
  }

  auto& settings = Settings::get_singleton();
  bool left_hand_text_visible = settings.bLeftHandText;

  if (left_map[IconElementType::kArrowIcon].is_visible && settings.bAlwaysArrowText) {
    left_hand_text_visible = true;
  }

  const RE::GFxValue is_right_hand_poisoned = weapon_is_poisoned(false);
  const RE::GFxValue is_left_hand_poisoned = weapon_is_poisoned(true);

  const RE::GFxValue left_poison_count = get_poison_count(true);
  const RE::GFxValue right_poison_count = get_poison_count(false);

  const RE::GFxValue left_enchant_count = static_cast<std::int32_t>(enchanted_count(true));
  const RE::GFxValue right_enchant_count = static_cast<std::int32_t>(enchanted_count(false));

  const RE::GFxValue right_hand_text = get_object_name(HandType::kRightHand);
  const RE::GFxValue left_hand_text = get_object_name(HandType::kLeftHand);
  const RE::GFxValue center_hand_text = get_object_name(HandType::kCenterHand);
  const RE::GFxValue arrow_count_text = get_ammo_count();

  const RE::GFxValue rested_stage{static_cast<int>(get_rested_stage(settings))};
  const RE::GFxValue hunger_stage{static_cast<int>(get_hunger_stage(settings))};
  const RE::GFxValue weather_stage{static_cast<int>(get_weather_stage(settings))};
  const RE::GFxValue stress_data[2]{get_stress_value(settings), RE::PlayerCharacter::GetSingleton()->IsInCombat()};

  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.update", &update_delta, 1);

  giga_widget->uiMovie->SetVariable("mirelgigawidget._visible", RE::GFxValue{settings.bVisibleAll});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget._visible",
                                    in_menu ? RE::GFxValue{false} : RE::GFxValue{settings.bEquipmentWidget});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget._yscale", RE::GFxValue{settings.fScaleEquipmentWidget});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget._xscale", RE::GFxValue{settings.fScaleEquipmentWidget});

  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.RightHandText._x", RE::GFxValue{settings.fRightTextX});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.RightHandText._y", RE::GFxValue{settings.fRightTextY});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.RightHandText._visible",
                                    RE::GFxValue{settings.bRightHandText});

  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.rightHandIcon._x",
                                    RE::GFxValue{settings.fRightHandIconX});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.rightHandIcon._y",
                                    RE::GFxValue{settings.fRightHandIconY});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.rightHandIcon._visible",
                                    RE::GFxValue{settings.bRightHandIcon});

  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.LeftHandText._x", RE::GFxValue{settings.fLeftTextX});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.LeftHandText._y", RE::GFxValue{settings.fLeftTextY});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.LeftHandText._visible",
                                    RE::GFxValue{left_hand_text_visible});

  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.leftHandIcon._x",
                                    RE::GFxValue{settings.fLeftHandIconX});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.leftHandIcon._y",
                                    RE::GFxValue{settings.fLeftHandIconY});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.leftHandIcon._visible",
                                    RE::GFxValue{settings.bLeftHandIcon});

  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.CenterText._x",
                                    RE::GFxValue{settings.fCenterTextX});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.CenterText._y",
                                    RE::GFxValue{settings.fCenterTextY});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.CenterText._visible",
                                    RE::GFxValue{settings.bCenterHandText});

  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.centerHandIcon._x",
                                    RE::GFxValue{settings.fCenterHandIconX});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.centerHandIcon._y",
                                    RE::GFxValue{settings.fCenterHandIconY});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.centerHandIcon._visible",
                                    RE::GFxValue{settings.bCenterHandIcon});

  const auto is_shout = is_shout_equipped();

  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.centerHandIcon.centerAbilityIcon._visible",
                                    RE::GFxValue{!is_shout});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.centerHandIcon.centerDragonIcon._visible",
                                    RE::GFxValue{is_shout});

  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.rightHandIcon.rightEnchantText.text",
                                    right_enchant_count);
  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.leftHandIcon.leftEnchantText.text", left_enchant_count);

  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.rightHandIcon.rightPoisonText.text",
                                    right_poison_count);
  giga_widget->uiMovie->SetVariable("mirelgigawidget.BaseWidget.leftHandIcon.leftPoisonText.text", left_poison_count);

  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setRightHandText", &right_hand_text, 1);
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setLeftHandText", &left_hand_text, 1);
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setCenterHandText", &center_hand_text, 1);
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setArrowCountText", &arrow_count_text, 1);

  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setRestedStage", &rested_stage, 1);
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setHungerStage", &hunger_stage, 1);
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setWeatherStage", &weather_stage, 1);

  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setStressStage", stress_data, 2);

  std::uint8_t potion_health_frame_callback_result = 100;
  std::uint8_t potion_stamina_frame_callback_result = 100;
  std::uint8_t potion_magicka_frame_callback_result = 100;
  std::uint8_t potion_other_frame_callback_result = 100;
  std::uint8_t potion_poison_frame_callback_result = 100;

  for (const auto& callback_frame : CallbackHolder::get_singleton().set_frame_for_potion_callback() | views::values) {
    potion_health_frame_callback_result = callback_frame(GIGAWIDGET_API::PotionType::Health);
    potion_stamina_frame_callback_result = callback_frame(GIGAWIDGET_API::PotionType::Stamina);
    potion_magicka_frame_callback_result = callback_frame(GIGAWIDGET_API::PotionType::Magicka);
    potion_other_frame_callback_result = callback_frame(GIGAWIDGET_API::PotionType::Other);
    potion_poison_frame_callback_result = callback_frame(GIGAWIDGET_API::PotionType::Poison);

    GIGAWIDGET_API::AvailablePotionSlotResult potion_health_count_callback_result =
        GIGAWIDGET_API::AvailablePotionSlotResult();
    GIGAWIDGET_API::AvailablePotionSlotResult potion_stamina_count_callback_result =
        GIGAWIDGET_API::AvailablePotionSlotResult();
    GIGAWIDGET_API::AvailablePotionSlotResult potion_magicka_count_callback_result =
        GIGAWIDGET_API::AvailablePotionSlotResult();
    GIGAWIDGET_API::AvailablePotionSlotResult potion_other_count_callback_result =
        GIGAWIDGET_API::AvailablePotionSlotResult();
    GIGAWIDGET_API::AvailablePotionSlotResult potion_poison_count_callback_result =
        GIGAWIDGET_API::AvailablePotionSlotResult();
    for (const auto& callback_slot :
         CallbackHolder::get_singleton().set_available_potion_slot_callback() | views::values) {
      potion_health_count_callback_result = callback_slot(GIGAWIDGET_API::PotionType::Health);
      potion_stamina_count_callback_result = callback_slot(GIGAWIDGET_API::PotionType::Stamina);
      potion_magicka_count_callback_result = callback_slot(GIGAWIDGET_API::PotionType::Magicka);
      potion_other_count_callback_result = callback_slot(GIGAWIDGET_API::PotionType::Other);
      potion_poison_count_callback_result = callback_slot(GIGAWIDGET_API::PotionType::Poison);
    }

    // logi("Health Slots D:{} S:{}",
    //      potion_health_count_callback_result.duration,
    //      potion_health_count_callback_result.slots);

    const RE::GFxValue potion_health_frame[2]{potion_health_frame_callback_result,
                                              static_cast<std::uint8_t>(GIGAWIDGET_API::PotionType::Health)};
    const RE::GFxValue potion_stamina_frame[2]{potion_stamina_frame_callback_result,
                                               static_cast<std::uint8_t>(GIGAWIDGET_API::PotionType::Stamina)};
    const RE::GFxValue potion_magicka_frame[2]{potion_magicka_frame_callback_result,
                                               static_cast<std::uint8_t>(GIGAWIDGET_API::PotionType::Magicka)};
    const RE::GFxValue potion_other_frame[2]{potion_other_frame_callback_result,
                                             static_cast<std::uint8_t>(GIGAWIDGET_API::PotionType::Other)};
    const RE::GFxValue potion_poison_frame[2]{potion_poison_frame_callback_result,
                                              static_cast<std::uint8_t>(GIGAWIDGET_API::PotionType::Poison)};

    const RE::GFxValue potion_health_count[3]{(settings.bPotionHealthTextCooldown
                                                 ? potion_health_count_callback_result.duration
                                                 : potion_health_count_callback_result.slots),
                                              (potion_health_count_callback_result.slots > 0),
                                              static_cast<std::uint16_t>(GIGAWIDGET_API::PotionType::Health)};
    const RE::GFxValue potion_stamina_count[3]{(settings.bPotionStaminaTextCooldown
                                                  ? potion_stamina_count_callback_result.duration
                                                  : potion_stamina_count_callback_result.slots),
                                               (potion_stamina_count_callback_result.slots > 0),
                                               static_cast<std::uint16_t>(GIGAWIDGET_API::PotionType::Stamina)};
    const RE::GFxValue potion_magicka_count[3]{(settings.bPotionMagickaTextCooldown
                                                  ? potion_magicka_count_callback_result.duration
                                                  : potion_magicka_count_callback_result.slots),
                                               (potion_magicka_count_callback_result.slots > 0),
                                               static_cast<std::uint16_t>(GIGAWIDGET_API::PotionType::Magicka)};
    const RE::GFxValue potion_other_count[3]{(settings.bPotionOtherTextCooldown
                                                ? potion_other_count_callback_result.duration
                                                : potion_other_count_callback_result.slots),
                                             (potion_other_count_callback_result.slots > 0),
                                             static_cast<std::uint16_t>(GIGAWIDGET_API::PotionType::Other)};

    let poison_have_slots = potion_poison_count_callback_result.slots > 0;
    auto poison_slots = potion_poison_count_callback_result.slots;
    if (settings.bPotionPoisonTextCooldown) {
      poison_slots = potion_poison_count_callback_result.duration;
    }

    const RE::GFxValue potion_poison_count[3]{
        poison_slots, poison_have_slots, static_cast<std::uint16_t>(GIGAWIDGET_API::PotionType::Poison)};

    giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setPotionCount", potion_health_count, 3);
    giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setPotionCount", potion_stamina_count, 3);
    giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setPotionCount", potion_magicka_count, 3);
    giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setPotionCount", potion_other_count, 3);
    giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setPotionCount", potion_poison_count, 3);

    giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setPotionFrame", potion_health_frame, 2);
    giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setPotionFrame", potion_stamina_frame, 2);
    giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setPotionFrame", potion_magicka_frame, 2);
    giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setPotionFrame", potion_other_frame, 2);
    giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setPotionFrame", potion_poison_frame, 2);

    auto text_visible = settings.bPotionHealthText;
    if (potion_health_count_callback_result.duration <= 0 && settings.bPotionHealthTextCooldown) {
      text_visible = false;
    }

    let potion_health_settings = PotionSettings{settings.bPotionHealth,
                                                settings.bPotionHealthFlash,
                                                settings.fPotionHealthX,
                                                settings.fPotionHealthY,
                                                settings.fScalePotionHealth,
                                                text_visible};

    text_visible = settings.bPotionStaminaText;
    if (potion_stamina_count_callback_result.duration <= 0 && settings.bPotionStaminaTextCooldown) {
      text_visible = false;
    }

    let potion_stamina_settings = PotionSettings{settings.bPotionStamina,
                                                 settings.bPotionStaminaFlash,
                                                 settings.fPotionStaminaX,
                                                 settings.fPotionStaminaY,
                                                 settings.fScalePotionStamina,
                                                 text_visible};

    text_visible = settings.bPotionMagickaText;
    if (potion_magicka_count_callback_result.duration <= 0 && settings.bPotionMagickaTextCooldown) {
      text_visible = false;
    }

    let potion_magicka_settings = PotionSettings{settings.bPotionMagicka,
                                                 settings.bPotionMagickaFlash,
                                                 settings.fPotionMagickaX,
                                                 settings.fPotionMagickaY,
                                                 settings.fScalePotionMagicka,
                                                 text_visible};

    text_visible = settings.bPotionOtherText;
    if (potion_other_count_callback_result.duration <= 0 && settings.bPotionOtherTextCooldown) {
      text_visible = false;
    }

    let potion_other_settings = PotionSettings{settings.bPotionOther,
                                               settings.bPotionOtherFlash,
                                               settings.fPotionOtherX,
                                               settings.fPotionOtherY,
                                               settings.fScalePotionOther,
                                               text_visible};

    text_visible = settings.bPotionPoisonText;
    if (potion_poison_count_callback_result.duration <= 0 && settings.bPotionPoisonTextCooldown) {
      text_visible = false;
    }

    let potion_poison_settings = PotionSettings{settings.bPotionPoison,
                                                settings.bPotionPoisonFlash,
                                                settings.fPotionPoisonX,
                                                settings.fPotionPoisonY,
                                                settings.fScalePotionPoison,
                                                text_visible};

    set_potion_setting(giga_widget, potion_health_settings, "Health");
    set_potion_setting(giga_widget, potion_stamina_settings, "Stamina");
    set_potion_setting(giga_widget, potion_magicka_settings, "Magicka");
    set_potion_setting(giga_widget, potion_other_settings, "Other");
    set_potion_setting(giga_widget, potion_poison_settings, "Poison");
  }

  std::uint8_t ki_energy_frame_callback_result = 100;

  for (const auto& callback_frame :
       CallbackHolder::get_singleton().set_frame_for_ki_energy_callback() | views::values) {
    ki_energy_frame_callback_result = callback_frame();

    GIGAWIDGET_API::AvailableKiEnergyResult ki_energy_count_callback_result = GIGAWIDGET_API::AvailableKiEnergyResult();
    for (const auto& callback_slot :
         CallbackHolder::get_singleton().set_available_ki_energy_callback() | views::values) {
      ki_energy_count_callback_result = callback_slot();
    }

    // logi("Health Slots D:{} S:{}",
    //      potion_health_count_callback_result.duration,
    //      potion_health_count_callback_result.slots);

    const RE::GFxValue ki_energy_frame[1]{ki_energy_frame_callback_result};

    auto ki_energy_slots = ki_energy_count_callback_result.slots;
    if (settings.bKiEnergyTextCooldown) {
      ki_energy_slots = ki_energy_count_callback_result.duration;
    }

    const RE::GFxValue ki_energy_count[1]{ki_energy_slots};

    giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setKiEnergyCount", ki_energy_count, 1);

    giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setKiEnergyFrame", ki_energy_frame, 1);

    auto text_visible = settings.bKiEnergyText;
    if (ki_energy_count_callback_result.duration <= 0 && settings.bKiEnergyTextCooldown) {
      text_visible = false;
    }

    let ki_energy_settings = KiEnergySettings{settings.bKiEnergy,
                                              settings.fKiEnergyPoisonX,
                                              settings.fKiEnergyPoisonY,
                                              settings.fScaleKiEnergy,
                                              text_visible};

    set_ki_energy_setting(giga_widget, ki_energy_settings);
  }

  std::uint8_t absorb_shield_frame_callback_result = 50;
  for (const auto& callback_frame :
       CallbackHolder::get_singleton().set_frame_for_absorb_shield_callback() | views::values) {
    absorb_shield_frame_callback_result = callback_frame();
  }
  if (settings.bAbsorbShieldWidgetDebug) {
    absorb_shield_frame_callback_result = 100;
  }
  const RE::GFxValue absorb_shield_frame[1]{absorb_shield_frame_callback_result};
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setAbsorbShieldFrame", absorb_shield_frame, 1);
  let absorb_shield_settings = AbsorbShieldSettings{settings.bAbsorbShieldWidget,
                                                    settings.fAbsorbShieldWidgetX,
                                                    settings.fAbsorbShieldWidgetY,
                                                    settings.fScaleXAbsorbShieldWidget,
                                                    settings.fScaleYAbsorbShieldWidget};

  set_absorb_shield_setting(giga_widget, absorb_shield_settings);

  let stats_visible = in_tween_menu && settings.bStatsMenuWidget;
  // logi("apply_layout Stats visible: {} In Tween Menu: {} Settings: {}", stats_visible, in_tween_menu,
  // settings.bStatsMenuWidget);
  giga_widget->uiMovie->SetVariable("mirelgigawidget.Stats._visible", RE::GFxValue{stats_visible});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.Stats._x", RE::GFxValue{settings.fStatsMenuWidgetX});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.Stats._y", RE::GFxValue{settings.fStatsMenuWidgetY});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.Stats._xscale", RE::GFxValue{settings.fScaleStatsMenuWidget});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.Stats._yscale", RE::GFxValue{settings.fScaleStatsMenuWidget});
  set_stats_widget_values(giga_widget, settings);

  giga_widget->uiMovie->SetVariable("mirelgigawidget.DynamicStats._visible",
                                    RE::GFxValue{in_menu ? false : settings.bDynamicStatsMenuWidget});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.DynamicStats._x", RE::GFxValue{settings.fDynamicStatsMenuWidgetX});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.DynamicStats._y", RE::GFxValue{settings.fDynamicStatsMenuWidgetY});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.DynamicStats._xscale",
                                    RE::GFxValue{settings.fScaleDynamicStatsMenuWidget});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.DynamicStats._yscale",
                                    RE::GFxValue{settings.fScaleDynamicStatsMenuWidget});
  set_dynamic_stats_widget_values(giga_widget, settings);

  auto combat_target = get_combat_target();

  giga_widget->uiMovie->SetVariable("mirelgigawidget.TargetDynamicStats._visible",
                                    RE::GFxValue{
                                        (in_menu || !combat_target) ? false : settings.bTargetDynamicStatsMenuWidget});
  //giga_widget->uiMovie->SetVariable("mirelgigawidget.TargetDynamicStats._visible",
  //                              RE::GFxValue{true});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.TargetDynamicStats._x",
                                    RE::GFxValue{settings.fTargetDynamicStatsMenuWidgetX});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.TargetDynamicStats._y",
                                    RE::GFxValue{settings.fTargetDynamicStatsMenuWidgetY});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.TargetDynamicStats._xscale",
                                    RE::GFxValue{settings.fScaleTargetDynamicStatsMenuWidget});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.TargetDynamicStats._yscale",
                                    RE::GFxValue{settings.fScaleTargetDynamicStatsMenuWidget});
  set_target_dynamic_stats_widget_values(giga_widget, settings);

  giga_widget->uiMovie->SetVariable("mirelgigawidget.WoundInjuryDisease._visible",
                                    RE::GFxValue{in_menu ? false : settings.bWoundInjuryDiseaseMenuWidget});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.WoundInjuryDisease._x",
                                    RE::GFxValue{settings.fWoundInjuryDiseaseMenuWidgetX});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.WoundInjuryDisease._y",
                                    RE::GFxValue{settings.fWoundInjuryDiseaseMenuWidgetY});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.WoundInjuryDisease._xscale",
                                    RE::GFxValue{settings.fScaleWoundInjuryDiseaseMenuWidget});
  giga_widget->uiMovie->SetVariable("mirelgigawidget.WoundInjuryDisease._yscale",
                                    RE::GFxValue{settings.fScaleWoundInjuryDiseaseMenuWidget});

  const RE::GFxValue wound_injury_disease[3]{
      settings.bWoundInjuryDiseaseMenuDebug
        ? true
        : actor_has_active_mgef_with_keyword(RE::PlayerCharacter::GetSingleton(), settings.game_data.injury_keyword),
      settings.bWoundInjuryDiseaseMenuDebug ? true : actor_has_active_disease(RE::PlayerCharacter::GetSingleton()),
      settings.bWoundInjuryDiseaseMenuDebug ? 3 : get_wound_level(settings)};
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setWoundInjuryDisease", wound_injury_disease, 3);

  const RE::GFxValue aligments[3]{settings.iCenterTextAligment,
                                  settings.iLeftTextAligment,
                                  settings.iRightTextAligment};
  giga_widget->uiMovie->InvokeNoReturn("mirelgigawidget.setEquipmentTextAligment", aligments, 3);

  let survival_weather_settings = SurvivalSettings{
      settings.bWeatherWidget, settings.fWeatherWidgetX, settings.fWeatherWidgetY, settings.fScaleWeatherWidget, false};
  let survival_ex_settings = SurvivalSettings{settings.bExhaustingWidget,
                                              settings.fExhaustingWidgetX,
                                              settings.fExhaustingWidgetY,
                                              settings.fScaleExhaustingWidget,
                                              false};
  let survival_hunger_settings = SurvivalSettings{
      settings.bHungerWidget, settings.fHungerWidgetX, settings.fHungerWidgetY, settings.fScaleHungerWidget, false};
  let survival_stress_settings = SurvivalSettings{settings.bStressWidget,
                                                  settings.fStressWidgetX,
                                                  settings.fStressWidgetY,
                                                  settings.fScaleStressWidget,
                                                  settings.bStressWidgetText};

  set_survival_widget_setting(giga_widget, survival_weather_settings, "Weather", false);
  set_survival_widget_setting(giga_widget, survival_ex_settings, "Rested", false);
  set_survival_widget_setting(giga_widget, survival_hunger_settings, "Hunger", false);
  set_survival_widget_setting(giga_widget, survival_stress_settings, "Stress", true);
}
