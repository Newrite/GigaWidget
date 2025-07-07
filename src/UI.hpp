#pragma once

#include "SKSEMenuFramework.hpp"
#include "Settings.hpp"

namespace ReUI {

// using namespace  SKSEMenuFramework;
namespace ImGui = MCP_API;

static constexpr float max_text_width = 275.0f;
static constexpr float width_checkbox = max_text_width;
static constexpr float width_value = max_text_width + 50.f;

static constexpr float X_V_MIN = -4000.f;
static constexpr float X_V_MAX = 4000.f;
static constexpr float Y_V_MIN = -2000.f;
static constexpr float Y_V_MAX = 2000.f;
static constexpr float S_V_MIN = 0.f;
static constexpr float S_V_MAX = 200.f;
static constexpr const char* FORMAT_FLOAT_02 = "%.2f";

struct WriteFloatData final
{
  float step;
  float step_fast;
  const char* format;
};

struct WriteSliderData final
{
  float vmin;
  float vmax;
  const char* format;
};

namespace UIFunction {

static auto TextLineWithValue(const std::string&& text, const std::string&& value) -> void
{
  ImGui::Text(text.c_str());
  ImGui::SameLine();
  ImGui::SetCursorPosX(width_value);
  ImGui::Text(value.c_str());
}

static auto TextValue(const std::string&& value) -> void
{
  ImGui::SameLine();
  ImGui::SetCursorPosX(width_value);
  ImGui::Text(value.c_str());
}

static auto CreateDisabledCheckbox(const std::string&& name,
                                   bool value,
                                   const bool same_line = false,
                                   const bool set_cursor_x = false) -> void
{
  if (same_line) {
    ImGui::SameLine();
  }
  if (set_cursor_x) {
    ImGui::SetCursorPosX(width_checkbox);
  }
  ImGui::BeginDisabled();
  ImGui::Checkbox(name.c_str(), &value);
  ImGui::EndDisabled();
}

static auto CreateMutableCheckbox(Settings& settings, const char* field, bool& setting_value) -> void
{
  auto field_value = setting_value;
  if (ImGui::Checkbox(field, &field_value)) {
    setting_value = field_value;
    settings.write_setting_int(field, field_value);
  }
}

static auto CreateMutableWriteFloat(Settings& settings,
                                    const char* field,
                                    double& setting_value,
                                    const WriteFloatData& data) -> void
{
  auto field_value = static_cast<float>(setting_value);
  if (ImGui::InputFloat(field, &field_value, data.step, data.step_fast, data.format)) {
    setting_value = field_value;
    settings.write_setting_float(field, field_value);
  }
}

static auto CreateMutableWriteSliderFloat(Settings& settings,
                                          const char* field,
                                          double& setting_value,
                                          const WriteSliderData& data) -> void
{
  auto field_value = static_cast<float>(setting_value);
  if (ImGui::SliderFloat(field, &field_value, data.vmin, data.vmax, data.format)) {
    setting_value = field_value;
    settings.write_setting_float(field, field_value);
  }
}

}

static void __stdcall RenderAbsorbShield()
{

  auto& settings = Settings::get_singleton();
  settings.load();

  UIFunction::CreateMutableCheckbox(settings, Settings::bAbsorbShieldWidgetVar, settings.bAbsorbShieldWidget);
  UIFunction::CreateMutableCheckbox(settings, Settings::bAbsorbShieldWidgetDebugVar, settings.bAbsorbShieldWidgetDebug);

  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fAbsorbShieldWidgetXVar,
                                          settings.fAbsorbShieldWidgetX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fAbsorbShieldWidgetYVar,
                                            settings.fAbsorbShieldWidgetY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fScaleXAbsorbShieldWidgetVar,
                                            settings.fScaleXAbsorbShieldWidget,
                                            {.vmin = S_V_MIN, .vmax = S_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fScaleYAbsorbShieldWidgetVar,
                                          settings.fScaleYAbsorbShieldWidget,
                                          {.vmin = S_V_MIN, .vmax = S_V_MAX, .format = FORMAT_FLOAT_02});
  
}

static void __stdcall RenderWoundInjury()
{

  auto& settings = Settings::get_singleton();
  settings.load();

  UIFunction::CreateMutableCheckbox(settings, Settings::bWoundInjuryDiseaseMenuWidgetVar, settings.bWoundInjuryDiseaseMenuWidget);
  UIFunction::CreateMutableCheckbox(settings, Settings::bWoundInjuryDiseaseMenuDebugVar, settings.bWoundInjuryDiseaseMenuDebug);

  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fWoundInjuryDiseaseMenuWidgetXVar,
                                          settings.fWoundInjuryDiseaseMenuWidgetX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fWoundInjuryDiseaseMenuWidgetYVar,
                                            settings.fWoundInjuryDiseaseMenuWidgetY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fScaleWoundInjuryDiseaseMenuWidgetVar,
                                            settings.fScaleWoundInjuryDiseaseMenuWidget,
                                            {.vmin = S_V_MIN, .vmax = S_V_MAX, .format = FORMAT_FLOAT_02});
  
}

static void __stdcall RenderStatsMenu()
{

  auto& settings = Settings::get_singleton();
  settings.load();

  UIFunction::CreateMutableCheckbox(settings, Settings::bStatsMenuWidgetVar, settings.bStatsMenuWidget);
  UIFunction::CreateMutableCheckbox(settings, Settings::bStatsMenuWidgetRegenFixedVar, settings.bStatsMenuWidgetRegenFixed);

  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fStatsMenuWidgetXVar,
                                          settings.fStatsMenuWidgetX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fStatsMenuWidgetYVar,
                                            settings.fStatsMenuWidgetY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fScaleStatsMenuWidgetVar,
                                            settings.fScaleStatsMenuWidget,
                                            {.vmin = S_V_MIN, .vmax = S_V_MAX, .format = FORMAT_FLOAT_02});
  
}

static void __stdcall RenderKiEnergyWidget()
{

  auto& settings = Settings::get_singleton();
  settings.load();

  UIFunction::CreateMutableCheckbox(settings, Settings::bKiEnergyVar, settings.bKiEnergy);
  UIFunction::CreateMutableCheckbox(settings, Settings::bKiEnergyTextVar, settings.bKiEnergyText);
  UIFunction::CreateMutableCheckbox(settings, Settings::bKiEnergyTextCooldownVar, settings.bKiEnergyTextCooldown);

  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fKiEnergyPoisonXVar,
                                          settings.fKiEnergyPoisonX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fKiEnergyPoisonYVar,
                                            settings.fKiEnergyPoisonY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fScaleKiEnergyVar,
                                            settings.fScaleKiEnergy,
                                            {.vmin = S_V_MIN, .vmax = S_V_MAX, .format = FORMAT_FLOAT_02});
  
}

static void __stdcall RenderPotionsPoisonWidget()
{

  auto& settings = Settings::get_singleton();
  settings.load();
  
  ImGui::SeparatorText("Potion Health");
  ImGui::Separator();

  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionHealthVar, settings.bPotionHealth);
  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionHealthFlashVar, settings.bPotionHealthFlash);
  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionHealthTextVar, settings.bPotionHealthText);
  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionHealthTextCooldownVar, settings.bPotionHealthTextCooldown);

  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fPotionHealthXVar,
                                          settings.fPotionHealthX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fPotionHealthYVar,
                                            settings.fPotionHealthY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fScalePotionHealthVar,
                                            settings.fScalePotionHealth,
                                            {.vmin = S_V_MIN, .vmax = S_V_MAX, .format = FORMAT_FLOAT_02});

  ImGui::Separator();
  ImGui::SeparatorText("Potion Stamina");
  ImGui::Separator();

  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionStaminaVar, settings.bPotionStamina);
  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionStaminaFlashVar, settings.bPotionStaminaFlash);
  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionStaminaTextVar, settings.bPotionStaminaText);
  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionStaminaTextCooldownVar, settings.bPotionStaminaTextCooldown);

  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fPotionStaminaXVar,
                                          settings.fPotionStaminaX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fPotionStaminaYVar,
                                            settings.fPotionStaminaY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fScalePotionStaminaVar,
                                            settings.fScalePotionStamina,
                                            {.vmin = S_V_MIN, .vmax = S_V_MAX, .format = FORMAT_FLOAT_02});

  ImGui::Separator();
  ImGui::SeparatorText("Potion Magicka");
  ImGui::Separator();

  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionMagickaVar, settings.bPotionMagicka);
  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionMagickaFlashVar, settings.bPotionMagickaFlash);
  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionMagickaTextVar, settings.bPotionMagickaText);
  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionMagickaTextCooldownVar, settings.bPotionMagickaTextCooldown);

  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fPotionMagickaXVar,
                                          settings.fPotionMagickaX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fPotionMagickaYVar,
                                            settings.fPotionMagickaY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fScalePotionMagickaVar,
                                            settings.fScalePotionMagicka,
                                            {.vmin = S_V_MIN, .vmax = S_V_MAX, .format = FORMAT_FLOAT_02});

  ImGui::Separator();
  ImGui::SeparatorText("Potion Other");
  ImGui::Separator();

  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionOtherVar, settings.bPotionOther);
  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionOtherFlashVar, settings.bPotionOtherFlash);
  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionOtherTextVar, settings.bPotionOtherText);
  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionOtherTextCooldownVar, settings.bPotionOtherTextCooldown);

  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fPotionOtherXVar,
                                          settings.fPotionOtherX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fPotionOtherYVar,
                                            settings.fPotionOtherY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fScalePotionOtherVar,
                                            settings.fScalePotionOther,
                                            {.vmin = S_V_MIN, .vmax = S_V_MAX, .format = FORMAT_FLOAT_02});

  ImGui::Separator();
  ImGui::SeparatorText("Potion Poison");
  ImGui::Separator();

  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionPoisonVar, settings.bPotionPoison);
  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionPoisonFlashVar, settings.bPotionPoisonFlash);
  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionPoisonTextVar, settings.bPotionPoisonText);
  UIFunction::CreateMutableCheckbox(settings, Settings::bPotionPoisonTextCooldownVar, settings.bPotionPoisonTextCooldown);

  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fPotionPoisonXVar,
                                          settings.fPotionPoisonX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fPotionPoisonYVar,
                                            settings.fPotionPoisonY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fScalePotionPoisonVar,
                                            settings.fScalePotionPoison,
                                            {.vmin = S_V_MIN, .vmax = S_V_MAX, .format = FORMAT_FLOAT_02});
  
}

static void __stdcall RenderSurvivalWidget()
{

  auto& settings = Settings::get_singleton();
  settings.load();
  
  ImGui::SeparatorText("Stress Widget");
  ImGui::Separator();

  UIFunction::CreateMutableCheckbox(settings, Settings::bStressWidgetVar, settings.bStressWidget);
  UIFunction::CreateMutableCheckbox(settings, Settings::bStressWidgetTextVar, settings.bStressWidgetText);

  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fStressWidgetXVar,
                                          settings.fStressWidgetX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fStressWidgetYVar,
                                            settings.fStressWidgetY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fScaleStressWidgetVar,
                                            settings.fScaleStressWidget,
                                            {.vmin = S_V_MIN, .vmax = S_V_MAX, .format = FORMAT_FLOAT_02});

  ImGui::Separator();
  ImGui::SeparatorText("Exhausting Widget");
  ImGui::Separator();

  UIFunction::CreateMutableCheckbox(settings, Settings::bExhaustingWidgetVar, settings.bExhaustingWidget);

  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fExhaustingWidgetXVar,
                                          settings.fExhaustingWidgetX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fExhaustingWidgetYVar,
                                            settings.fExhaustingWidgetY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fScaleExhaustingWidgetVar,
                                            settings.fScaleExhaustingWidget,
                                            {.vmin = S_V_MIN, .vmax = S_V_MAX, .format = FORMAT_FLOAT_02});

  ImGui::Separator();
  ImGui::SeparatorText("Hunger Widget");
  ImGui::Separator();

  UIFunction::CreateMutableCheckbox(settings, Settings::bHungerWidgetVar, settings.bHungerWidget);

  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fHungerWidgetXVar,
                                          settings.fHungerWidgetX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fHungerWidgetYVar,
                                            settings.fHungerWidgetY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fScaleHungerWidgetVar,
                                            settings.fScaleHungerWidget,
                                            {.vmin = S_V_MIN, .vmax = S_V_MAX, .format = FORMAT_FLOAT_02});

  ImGui::Separator();
  ImGui::SeparatorText("Weather Widget");
  ImGui::Separator();

  UIFunction::CreateMutableCheckbox(settings, Settings::bWeatherWidgetVar, settings.bWeatherWidget);

  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fWeatherWidgetXVar,
                                          settings.fWeatherWidgetX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fWeatherWidgetYVar,
                                            settings.fWeatherWidgetY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fScaleWeatherWidgetVar,
                                            settings.fScaleWeatherWidget,
                                            {.vmin = S_V_MIN, .vmax = S_V_MAX, .format = FORMAT_FLOAT_02});
  
}

static void __stdcall RenderDynamicStats()
{

  auto& settings = Settings::get_singleton();
  settings.load();

  UIFunction::CreateMutableCheckbox(settings, Settings::bDynamicStatsMenuWidgetVar, settings.bDynamicStatsMenuWidget);
  UIFunction::CreateMutableCheckbox(settings, Settings::bDynamicStatsMenuWidgetAlwaysVar, settings.bDynamicStatsMenuWidgetAlways);
  UIFunction::CreateMutableCheckbox(settings, Settings::bDynamicStatsAlwaysHideBackgroundVar, settings.bDynamicStatsAlwaysHideBackground);
  UIFunction::CreateMutableCheckbox(settings, Settings::bDynamicStatsAlwaysHideBaseBackgroundVar, settings.bDynamicStatsAlwaysHideBaseBackground);

  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fDynamicStatsMenuWidgetXVar,
                                          settings.fDynamicStatsMenuWidgetX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fDynamicStatsMenuWidgetYVar,
                                            settings.fDynamicStatsMenuWidgetY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fScaleDynamicStatsMenuWidgetVar,
                                            settings.fScaleDynamicStatsMenuWidget,
                                            {.vmin = S_V_MIN, .vmax = S_V_MAX, .format = FORMAT_FLOAT_02});
  
}

static void __stdcall RenderEquipmentWidget()
{

  auto& settings = Settings::get_singleton();
  settings.load();

  UIFunction::CreateMutableCheckbox(settings, Settings::bVisibleAllVar, settings.bVisibleAll);
  UIFunction::CreateMutableCheckbox(settings, Settings::bEquipmentWidgetVar, settings.bEquipmentWidget);
  UIFunction::CreateMutableCheckbox(settings, Settings::bAlwaysArrowTextVar, settings.bAlwaysArrowText);
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fScaleEquipmentWidgetVar,
                                          settings.fScaleEquipmentWidget,
                                          {.vmin = S_V_MIN, .vmax = S_V_MAX, .format = FORMAT_FLOAT_02});

  ImGui::Separator();
  ImGui::SeparatorText("Right Hand");
  ImGui::Separator();

  UIFunction::CreateMutableCheckbox(settings, Settings::bRightHandIconVar, settings.bRightHandIcon);
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fRightHandIconXVar,
                                          settings.fRightHandIconX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fRightHandIconYVar,
                                            settings.fRightHandIconY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});

  UIFunction::CreateMutableCheckbox(settings, Settings::bRightHandTextVar, settings.bRightHandText);
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fRightTextXVar,
                                          settings.fRightTextX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fRightTextYVar,
                                            settings.fRightTextY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});
  
  auto right_aligment_right = settings.iRightTextAligment == 2;
  auto right_aligment_center = settings.iRightTextAligment == 0;
  auto right_aligment_left = settings.iRightTextAligment == 1;
  if (ImGui::Checkbox("Right Text: Alignment Right", &right_aligment_right)) {
    settings.iRightTextAligment = 2;
    settings.write_setting_int(Settings::iRightTextAligmentVar, 2);
  }
  if (ImGui::Checkbox("Right Text: Alignment Center", &right_aligment_center)) {
    settings.iRightTextAligment = 0;
    settings.write_setting_int(Settings::iRightTextAligmentVar, 0);
  }
  if (ImGui::Checkbox("Right Text: Alignment Left", &right_aligment_left)) {
    settings.iRightTextAligment = 1;
    settings.write_setting_int(Settings::iRightTextAligmentVar, 1);
  }

  ImGui::Separator();
  ImGui::SeparatorText("Left Hand");
  ImGui::Separator();

  UIFunction::CreateMutableCheckbox(settings, Settings::bLeftHandIconVar, settings.bLeftHandIcon);
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fLeftHandIconXVar,
                                          settings.fLeftHandIconX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fLeftHandIconYVar,
                                            settings.fLeftHandIconY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});

  UIFunction::CreateMutableCheckbox(settings, Settings::bLeftHandTextVar, settings.bLeftHandText);
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fLeftTextXVar,
                                          settings.fLeftTextX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fLeftTextYVar,
                                            settings.fLeftTextY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});

  auto left_aligment_right = settings.iLeftTextAligment == 2;
  auto left_aligment_center = settings.iLeftTextAligment == 0;
  auto left_aligment_left = settings.iLeftTextAligment == 1;
  if (ImGui::Checkbox("Left Text: Alignment Right", &left_aligment_right)) {
    settings.iLeftTextAligment = 2;
    settings.write_setting_int(Settings::iLeftTextAligmentVar, 2);
  }
  if (ImGui::Checkbox("Left Text: Alignment Center", &left_aligment_center)) {
    settings.iLeftTextAligment = 0;
    settings.write_setting_int(Settings::iLeftTextAligmentVar, 0);
  }
  if (ImGui::Checkbox("Left Text: Alignment Left", &left_aligment_left)) {
    settings.iLeftTextAligment = 1;
    settings.write_setting_int(Settings::iLeftTextAligmentVar, 1);
  }

  ImGui::Separator();
  ImGui::SeparatorText("Ability");
  ImGui::Separator();

  UIFunction::CreateMutableCheckbox(settings, Settings::bCenterHandIconVar, settings.bCenterHandIcon);
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fCenterHandIconXVar,
                                          settings.fCenterHandIconX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fCenterHandIconYVar,
                                            settings.fCenterHandIconY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});

  UIFunction::CreateMutableCheckbox(settings, Settings::bCenterHandTextVar, settings.bCenterHandText);
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                          Settings::fCenterTextXVar,
                                          settings.fCenterTextX,
                                          {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fCenterTextYVar,
                                            settings.fCenterTextY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});

  auto center_aligment_right = settings.iCenterTextAligment == 2;
  auto center_aligment_center = settings.iCenterTextAligment == 0;
  auto center_aligment_left = settings.iCenterTextAligment == 1;
  if (ImGui::Checkbox("CenterText: Alignment Right", &center_aligment_right)) {
    settings.iCenterTextAligment = 2;
    settings.write_setting_int(Settings::iCenterTextAligmentVar, 2);
  }
  if (ImGui::Checkbox("CenterText: Alignment: Center", &center_aligment_center)) {
    settings.iCenterTextAligment = 0;
    settings.write_setting_int(Settings::iCenterTextAligmentVar, 0);
  }
  if (ImGui::Checkbox("CenterText: Alignment: Left", &center_aligment_left)) {
    settings.iCenterTextAligment = 1;
    settings.write_setting_int(Settings::iCenterTextAligmentVar, 1);
  }
  
}

static void __stdcall RenderTargetDynamicStats()
{

  auto& settings = Settings::get_singleton();
  settings.load();
  constexpr auto generic_write_float_data = WriteFloatData{.step = 1.f, .step_fast = 5.f, .format = "%.0f"};

  UIFunction::CreateMutableCheckbox(settings,
                                    Settings::bTargetDynamicStatsMenuWidgetVar,
                                    settings.bTargetDynamicStatsMenuWidget);
  UIFunction::CreateMutableCheckbox(settings,
                                    Settings::bTargetDynamicStatsAltColoringVar,
                                    settings.bTargetDynamicStatsAltColoring);
  UIFunction::CreateMutableCheckbox(settings,
                                    Settings::bTargetDynamicStatsAlwaysHideBackgroundVar,
                                    settings.bTargetDynamicStatsAlwaysHideBackground);
  UIFunction::CreateMutableCheckbox(settings,
                                    Settings::bTargetDynamicStatsActiveOnlyWithKeywordVar,
                                    settings.bTargetDynamicStatsActiveOnlyWithKeyword);
  UIFunction::CreateMutableCheckbox(settings,
                                    Settings::bTargetDynamicStatsActiveOnlyWithTargetLockVar,
                                    settings.bTargetDynamicStatsActiveOnlyWithTargetLock);

  UIFunction::CreateMutableWriteFloat(settings,
                                      Settings::fTargetDynamicStatBackgroundResistWhiteVar,
                                      settings.fTargetDynamicStatBackgroundResistWhite,
                                      generic_write_float_data);
  UIFunction::CreateMutableWriteFloat(settings,
                                      Settings::fTargetDynamicStatBackgroundResistBlackVar,
                                      settings.fTargetDynamicStatBackgroundResistBlack,
                                      generic_write_float_data);
  UIFunction::CreateMutableWriteFloat(settings,
                                      Settings::fTargetDynamicStatBackgroundResistNeutralVar,
                                      settings.fTargetDynamicStatBackgroundResistNeutral,
                                      generic_write_float_data);
  UIFunction::CreateMutableWriteFloat(settings,
                                      Settings::fTargetDynamicStatBackgroundResistArmorWhiteVar,
                                      settings.fTargetDynamicStatBackgroundResistArmorWhite,
                                      generic_write_float_data);
  UIFunction::CreateMutableWriteFloat(settings,
                                      Settings::fTargetDynamicStatBackgroundResistArmorBlackVar,
                                      settings.fTargetDynamicStatBackgroundResistArmorBlack,
                                      generic_write_float_data);
  UIFunction::CreateMutableWriteFloat(settings,
                                      Settings::fTargetDynamicStatBackgroundResistArmorNeutralVar,
                                      settings.fTargetDynamicStatBackgroundResistArmorNeutral,
                                      generic_write_float_data);

  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fTargetDynamicStatsMenuWidgetXVar,
                                            settings.fTargetDynamicStatsMenuWidgetX,
                                            {.vmin = X_V_MIN, .vmax = X_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fTargetDynamicStatsMenuWidgetYVar,
                                            settings.fTargetDynamicStatsMenuWidgetY,
                                            {.vmin = Y_V_MIN, .vmax = Y_V_MAX, .format = FORMAT_FLOAT_02});
  UIFunction::CreateMutableWriteSliderFloat(settings,
                                            Settings::fScaleTargetDynamicStatsMenuWidgetVar,
                                            settings.fScaleTargetDynamicStatsMenuWidget,
                                            {.vmin = S_V_MIN, .vmax = S_V_MAX, .format = FORMAT_FLOAT_02});
}

static void Register()
{

  if (!SKSEMenuFramework::IsInstalled()) {
    return;
  }
  
  SKSEMenuFramework::SetSection("GigaWidget");
  SKSEMenuFramework::AddSectionItem("Stats Menu", RenderStatsMenu);
  SKSEMenuFramework::AddSectionItem("Wound Injury", RenderWoundInjury);
  SKSEMenuFramework::AddSectionItem("Absorb Shield", RenderAbsorbShield);
  SKSEMenuFramework::AddSectionItem("Dynamic Stats", RenderDynamicStats);
  SKSEMenuFramework::AddSectionItem("Survival Widgets", RenderSurvivalWidget);
  SKSEMenuFramework::AddSectionItem("Ki Energy Widget", RenderKiEnergyWidget);
  SKSEMenuFramework::AddSectionItem("Equipment Widget", RenderEquipmentWidget);
  SKSEMenuFramework::AddSectionItem("Target Dynamic Stats", RenderTargetDynamicStats);
  SKSEMenuFramework::AddSectionItem("Potions and Poison Widget", RenderPotionsPoisonWidget);
}
}
