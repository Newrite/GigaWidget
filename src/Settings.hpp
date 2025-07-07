#pragma once

#include "SimpleIni.hpp"

#define get_data(form_name, form_id, mod_name)                                                                         \
  [&]() -> form_name* {                                                                                                \
    let data_handler = RE::TESDataHandler::GetSingleton();                                                             \
    if (!data_handler) {                                                                                               \
      logi("Data handler is nul"sv);                                                                                  \
      return nullptr;                                                                                                  \
    }                                                                                                                  \
    return data_handler->LookupForm<form_name>(form_id, mod_name);                                                     \
  }()

struct WidgetData final
{

  WidgetData() = default;
  
  RE::ActorValue iActorValueIndexHealth{RE::ActorValue::kHealth};
  RE::ActorValue iActorValueIndexHealthRate{RE::ActorValue::kHealRate};
  RE::ActorValue iActorValueIndexHealthRateMult{RE::ActorValue::kHealRateMult};
  RE::ActorValue iActorValueIndexMagicka{RE::ActorValue::kMagicka};
  RE::ActorValue iActorValueIndexMagickaRate{RE::ActorValue::kMagickaRate};
  RE::ActorValue iActorValueIndexMagickaRateMult{RE::ActorValue::kMagickaRateMult};
  RE::ActorValue iActorValueIndexStamina{RE::ActorValue::kStamina};
  RE::ActorValue iActorValueIndexStaminaRate{RE::ActorValue::kStaminaRate};
  RE::ActorValue iActorValueIndexStaminaRateMult{RE::ActorValue::kStaminaRateMult};
  RE::ActorValue iActorValueIndexRestorationMod{RE::ActorValue::kRestorationModifier};
  RE::ActorValue iActorValueIndexAlterationMod{RE::ActorValue::kAlterationModifier};
  RE::ActorValue iActorValueIndexDestructionMod{RE::ActorValue::kDestructionModifier};
  RE::ActorValue iActorValueIndexConjurationMod{RE::ActorValue::kConjurationModifier};
  RE::ActorValue iActorValueIndexIllusionMod{RE::ActorValue::kIllusionModifier};
  RE::ActorValue iActorValueIndexDamageResist{RE::ActorValue::kDamageResist};
  RE::ActorValue iActorValueIndexMagickResist{RE::ActorValue::kResistMagic};
  RE::ActorValue iActorValueIndexFireResist{RE::ActorValue::kResistFire};
  RE::ActorValue iActorValueIndexFrostResist{RE::ActorValue::kResistFrost};
  RE::ActorValue iActorValueIndexShockResist{RE::ActorValue::kResistShock};
  RE::ActorValue iActorValueIndexPoisonResist{RE::ActorValue::kPoisonResist};
  RE::ActorValue iActorValueIndexDiseaseResist{RE::ActorValue::kResistDisease};
  RE::ActorValue iActorValueIndexAbsorbChance{RE::ActorValue::kAbsorbChance};
  RE::ActorValue iActorValueIndexSpeedMult{RE::ActorValue::kSpeedMult};
  RE::ActorValue iActorValueIndexNoiseMult{RE::ActorValue::kMovementNoiseMult};
  RE::ActorValue iActorValueIndexShoutRecoveryMult{RE::ActorValue::kShoutRecoveryMult};

  long iActiveKeywordId{0x0};
  std::string sPluginDataName{};

  [[nodiscard]] static auto get_singleton() noexcept -> WidgetData&
  {
    static WidgetData instance;
    return instance;
  }

  auto load() -> void
  {

    constexpr auto path_to_ini = "Data/SKSE/Plugins/GigaWidget_Data.ini";

    constexpr auto section_main = "Main";
    constexpr auto section_target_stats = "TargetDynamicStats";
    
    constexpr auto ActorValueIndexHealth = "iActorValueIndexHealth";
    constexpr auto ActorValueIndexHealthRate = "iActorValueIndexHealthRate";
    constexpr auto ActorValueIndexHealthRateMult = "iActorValueIndexHealthRateMult";
    constexpr auto ActorValueIndexMagicka = "iActorValueIndexMagicka";
    constexpr auto ActorValueIndexMagickaRate = "iActorValueIndexMagickaRate";
    constexpr auto ActorValueIndexMagickaRateMult = "iActorValueIndexMagickaRateMult";
    constexpr auto ActorValueIndexStamina = "iActorValueIndexStamina";
    constexpr auto ActorValueIndexStaminaRate = "iActorValueIndexStaminaRate";
    constexpr auto ActorValueIndexStaminaRateMult = "iActorValueIndexStaminaRateMult";
    constexpr auto ActorValueIndexRestorationMod = "iActorValueIndexRestorationMod";
    constexpr auto ActorValueIndexAlterationMod = "iActorValueIndexAlterationMod";
    constexpr auto ActorValueIndexDestructionMod = "iActorValueIndexDestructionMod";
    constexpr auto ActorValueIndexConjurationMod = "iActorValueIndexConjurationMod";
    constexpr auto ActorValueIndexIllusionMod = "iActorValueIndexIllusionMod";
    constexpr auto ActorValueIndexDamageResist = "iActorValueIndexDamageResist";
    constexpr auto ActorValueIndexMagickResist = "iActorValueIndexMagickResist";
    constexpr auto ActorValueIndexFireResist = "iActorValueIndexFireResist";
    constexpr auto ActorValueIndexFrostResist = "iActorValueIndexFrostResist";
    constexpr auto ActorValueIndexShockResist = "iActorValueIndexShockResist";
    constexpr auto ActorValueIndexPoisonResist = "iActorValueIndexPoisonResist";
    constexpr auto ActorValueIndexDiseaseResist = "iActorValueIndexDiseaseResist";
    constexpr auto ActorValueIndexAbsorbChance = "iActorValueIndexAbsorbChance";
    constexpr auto ActorValueIndexSpeedMult = "iActorValueIndexSpeedMult";
    constexpr auto ActorValueIndexNoiseMult = "iActorValueIndexNoiseMult";
    constexpr auto ActorValueIndexShoutRecoveryMult = "iActorValueIndexShoutRecoveryMult";
    constexpr auto ActiveKeywordId = "iActiveKeywordId";
    constexpr auto PluginDataName = "sPluginDataName";

    const auto read_double = [this](const CSimpleIni& ini, const char* key, double& value, const char* section) -> void {
      if (ini.GetValue(section, key)) {
        value = ini.GetDoubleValue(section, key);
      }
    };

    const auto read_int = [this](const CSimpleIni& ini, const char* key, long& value, const char* section) -> void {
      if (ini.GetValue(section, key)) {
        value = ini.GetLongValue(section, key);
      }
    };

    const auto read_bool = [this](const CSimpleIni& ini, const char* key, bool& value, const char* section) -> void {
      if (ini.GetValue(section, key)) {
        value = ini.GetBoolValue(section, key);
      }
    };

    const auto read_str = [this](const CSimpleIni& ini, const char* key, std::string& value, const char* section) -> void {
      if (ini.GetValue(section, key)) {
        value = ini.GetValue(section, key);
      }
    };

    const auto read_av = [this](const CSimpleIni& ini, const char* key, RE::ActorValue& value, const char* section) -> void {
      if (ini.GetValue(section, key)) {
        value = static_cast<RE::ActorValue>(ini.GetLongValue(section, key));
      }
    };

    const auto read_settings = [this, read_double, read_bool, read_int, read_str, read_av](CSimpleIni& ini, const char* path) -> void {
      ini.LoadFile(path);
      if (ini.IsEmpty()) {
        return;
      }
      
      read_av(ini, ActorValueIndexHealth, iActorValueIndexHealth, section_main);
      read_av(ini, ActorValueIndexHealthRate, iActorValueIndexHealthRate, section_main);
      read_av(ini, ActorValueIndexHealthRateMult, iActorValueIndexHealthRateMult, section_main);
      read_av(ini, ActorValueIndexMagicka, iActorValueIndexMagicka, section_main);
      read_av(ini, ActorValueIndexMagickaRate, iActorValueIndexMagickaRate, section_main);
      read_av(ini, ActorValueIndexMagickaRateMult, iActorValueIndexMagickaRateMult, section_main);
      read_av(ini, ActorValueIndexStamina, iActorValueIndexStamina, section_main);
      read_av(ini, ActorValueIndexStaminaRate, iActorValueIndexStaminaRate, section_main);
      read_av(ini, ActorValueIndexStaminaRateMult, iActorValueIndexStaminaRateMult, section_main);
      read_av(ini, ActorValueIndexRestorationMod, iActorValueIndexRestorationMod, section_main);
      read_av(ini, ActorValueIndexAlterationMod, iActorValueIndexAlterationMod, section_main);
      read_av(ini, ActorValueIndexDestructionMod, iActorValueIndexDestructionMod, section_main);
      read_av(ini, ActorValueIndexConjurationMod, iActorValueIndexConjurationMod, section_main);
      read_av(ini, ActorValueIndexIllusionMod, iActorValueIndexIllusionMod, section_main);
      read_av(ini, ActorValueIndexDamageResist, iActorValueIndexDamageResist, section_main);
      read_av(ini, ActorValueIndexMagickResist, iActorValueIndexMagickResist, section_main);
      read_av(ini, ActorValueIndexFireResist, iActorValueIndexFireResist, section_main);
      read_av(ini, ActorValueIndexFrostResist, iActorValueIndexFrostResist, section_main);
      read_av(ini, ActorValueIndexShockResist, iActorValueIndexShockResist, section_main);
      read_av(ini, ActorValueIndexPoisonResist, iActorValueIndexPoisonResist, section_main);
      read_av(ini, ActorValueIndexDiseaseResist, iActorValueIndexDiseaseResist, section_main);
      read_av(ini, ActorValueIndexAbsorbChance, iActorValueIndexAbsorbChance, section_main);
      read_av(ini, ActorValueIndexSpeedMult, iActorValueIndexSpeedMult, section_main);
      read_av(ini, ActorValueIndexNoiseMult, iActorValueIndexNoiseMult, section_main);
      read_av(ini, ActorValueIndexShoutRecoveryMult, iActorValueIndexShoutRecoveryMult, section_main);

      read_int(ini, ActiveKeywordId, iActiveKeywordId, section_target_stats);
      read_str(ini, PluginDataName, sPluginDataName, section_target_stats);
      //sPluginDataName = "RESimonrim.esp";
      
    };

    CSimpleIni ini;

    ini.SetUnicode();
    read_settings(ini, path_to_ini);
  }
};

struct Settings final
{

    static constexpr auto path_to_ini = L"Data/MCM/Settings/GigaWidget.ini";
    static constexpr auto path_to_ini_default = L"Data/MCM/Config/GigaWidget/settings.ini";
    static constexpr auto path_to_settings = "Data/SKSE/Plugins/GigaWidget_Settings.ini";

    static constexpr auto section = "Main";
    static constexpr auto bVisibleAllVar = "bVisibleAll";
    static constexpr auto bEquipmentWidgetVar = "bEquipmentWidget";
    static constexpr auto fScaleEquipmentWidgetVar = "fScaleEquipmentWidget";
    static constexpr auto bAlwaysArrowTextVar = "bAlwaysArrowText";
    
    static constexpr auto bWoundInjuryDiseaseMenuWidgetVar = "bWoundInjuryDiseaseMenuWidget";
    static constexpr auto bWoundInjuryDiseaseMenuDebugVar = "bWoundInjuryDiseaseMenuDebug";
    static constexpr auto fWoundInjuryDiseaseMenuWidgetYVar = "fWoundInjuryDiseaseMenuWidgetY";
    static constexpr auto fWoundInjuryDiseaseMenuWidgetXVar = "fWoundInjuryDiseaseMenuWidgetX";
    static constexpr auto fScaleWoundInjuryDiseaseMenuWidgetVar = "fScaleWoundInjuryDiseaseMenuWidget";

    static constexpr auto bAbsorbShieldWidgetVar = "bAbsorbShieldWidget";
    static constexpr auto bAbsorbShieldWidgetDebugVar = "bAbsorbShieldWidgetDebug";
    static constexpr auto fAbsorbShieldWidgetYVar = "fAbsorbShieldWidgetY";
    static constexpr auto fAbsorbShieldWidgetXVar = "fAbsorbShieldWidgetX";
    static constexpr auto fScaleYAbsorbShieldWidgetVar = "fScaleYAbsorbShieldWidget";
    static constexpr auto fScaleXAbsorbShieldWidgetVar = "fScaleXAbsorbShieldWidget";

    static constexpr auto fLeftHandIconYVar = "fLeftHandIconY";
    static constexpr auto fLeftHandIconXVar = "fLeftHandIconX";
    static constexpr auto bLeftHandIconVar = "bLeftHandIcon";

    static constexpr auto fLeftTextYVar = "fLeftTextY";
    static constexpr auto fLeftTextXVar = "fLeftTextX";
    static constexpr auto bLeftHandTextVar = "bLeftHandText";

    static constexpr auto fRightHandIconYVar = "fRightHandIconY";
    static constexpr auto fRightHandIconXVar = "fRightHandIconX";
    static constexpr auto bRightHandIconVar = "bRightHandIcon";

    static constexpr auto fRightTextYVar = "fRightTextY";
    static constexpr auto fRightTextXVar = "fRightTextX";
    static constexpr auto bRightHandTextVar = "bRightHandText";

    static constexpr auto fCenterHandIconYVar = "fCenterHandIconY";
    static constexpr auto fCenterHandIconXVar = "fCenterHandIconX";
    static constexpr auto bCenterHandIconVar = "bCenterHandIcon";

    static constexpr auto fCenterTextYVar = "fCenterTextY";
    static constexpr auto fCenterTextXVar = "fCenterTextX";
    static constexpr auto bCenterHandTextVar = "bCenterHandText";

    static constexpr auto bPotionHealthVar = "bPotionHealth";
    static constexpr auto bPotionHealthFlashVar = "bPotionHealthFlash";
    static constexpr auto fPotionHealthYVar = "fPotionHealthY";
    static constexpr auto fPotionHealthXVar = "fPotionHealthX";
    static constexpr auto fScalePotionHealthVar = "fScalePotionHealth";
    static constexpr auto bPotionHealthTextVar = "bPotionHealthText";
    static constexpr auto bPotionHealthTextCooldownVar = "bPotionHealthTextCooldown";

    static constexpr auto bPotionStaminaVar = "bPotionStamina";
    static constexpr auto bPotionStaminaFlashVar = "bPotionStaminaFlash";
    static constexpr auto fPotionStaminaYVar = "fPotionStaminaY";
    static constexpr auto fPotionStaminaXVar = "fPotionStaminaX";
    static constexpr auto fScalePotionStaminaVar = "fScalePotionStamina";
    static constexpr auto bPotionStaminaTextVar = "bPotionStaminaText";
    static constexpr auto bPotionStaminaTextCooldownVar = "bPotionStaminaTextCooldown";

    static constexpr auto bPotionMagickaVar = "bPotionMagicka";
    static constexpr auto bPotionMagickaFlashVar = "bPotionMagickaFlash";
    static constexpr auto fPotionMagickaYVar = "fPotionMagickaY";
    static constexpr auto fPotionMagickaXVar = "fPotionMagickaX";
    static constexpr auto fScalePotionMagickaVar = "fScalePotionMagicka";
    static constexpr auto bPotionMagickaTextVar = "bPotionMagickaText";
    static constexpr auto bPotionMagickaTextCooldownVar = "bPotionMagickaTextCooldown";

    static constexpr auto bPotionOtherVar = "bPotionOther";
    static constexpr auto bPotionOtherFlashVar = "bPotionOtherFlash";
    static constexpr auto fPotionOtherYVar = "fPotionOtherY";
    static constexpr auto fPotionOtherXVar = "fPotionOtherX";
    static constexpr auto fScalePotionOtherVar = "fScalePotionOther";
    static constexpr auto bPotionOtherTextVar = "bPotionOtherText";
    static constexpr auto bPotionOtherTextCooldownVar = "bPotionOtherTextCooldown";

    static constexpr auto bPotionPoisonVar = "bPotionPoison";
    static constexpr auto bPotionPoisonFlashVar = "bPotionPoisonFlash";
    static constexpr auto fPotionPoisonYVar = "fPotionPoisonY";
    static constexpr auto fPotionPoisonXVar = "fPotionPoisonX";
    static constexpr auto fScalePotionPoisonVar = "fScalePotionPoison";
    static constexpr auto bPotionPoisonTextVar = "bPotionPoisonText";
    static constexpr auto bPotionPoisonTextCooldownVar = "bPotionPoisonTextCooldown";

    static constexpr auto bKiEnergyVar = "bKiEnergy";
    static constexpr auto fKiEnergyPoisonYVar = "fKiEnergyPoisonY";
    static constexpr auto fKiEnergyPoisonXVar = "fKiEnergyPoisonX";
    static constexpr auto fScaleKiEnergyVar = "fScaleKiEnergy";
    static constexpr auto bKiEnergyTextVar = "bKiEnergyText";
    static constexpr auto bKiEnergyTextCooldownVar = "bKiEnergyTextCooldown";

    static constexpr auto bWeatherWidgetVar = "bWeatherWidget";
    static constexpr auto fWeatherWidgetYVar = "fWeatherWidgetY";
    static constexpr auto fWeatherWidgetXVar = "fWeatherWidgetX";
    static constexpr auto fScaleWeatherWidgetVar = "fScaleWeatherWidget";

    static constexpr auto bExhaustingWidgetVar = "bExhaustingWidget";
    static constexpr auto fExhaustingWidgetYVar = "fExhaustingWidgetY";
    static constexpr auto fExhaustingWidgetXVar = "fExhaustingWidgetX";
    static constexpr auto fScaleExhaustingWidgetVar = "fScaleExhaustingWidget";

    static constexpr auto bHungerWidgetVar = "bHungerWidget";
    static constexpr auto fHungerWidgetYVar = "fHungerWidgetY";
    static constexpr auto fHungerWidgetXVar = "fHungerWidgetX";
    static constexpr auto fScaleHungerWidgetVar = "fScaleHungerWidget";

    static constexpr auto bStressWidgetVar = "bStressWidget";
    static constexpr auto bStressWidgetTextVar = "bStressWidgetText";
    static constexpr auto fStressWidgetYVar = "fStressWidgetY";
    static constexpr auto fStressWidgetXVar = "fStressWidgetX";
    static constexpr auto fScaleStressWidgetVar = "fScaleStressWidget";

    static constexpr auto bStatsMenuWidgetVar = "bStatsMenuWidget";
    static constexpr auto bStatsMenuWidgetRegenFixedVar = "bStatsMenuWidgetRegenFixed";
    static constexpr auto fStatsMenuWidgetYVar = "fStatsMenuWidgetY";
    static constexpr auto fStatsMenuWidgetXVar = "fStatsMenuWidgetX";
    static constexpr auto fScaleStatsMenuWidgetVar = "fScaleStatsMenuWidget";

    static constexpr auto bDynamicStatsMenuWidgetVar = "bDynamicStatsMenuWidget";
    static constexpr auto bDynamicStatsMenuWidgetAlwaysVar = "bDynamicStatsMenuWidgetAlways";
    static constexpr auto bDynamicStatsAlwaysHideBackgroundVar = "bDynamicStatsAlwaysHideBackground";
    static constexpr auto bDynamicStatsAlwaysHideBaseBackgroundVar = "bDynamicStatsAlwaysHideBaseBackground";
    static constexpr auto fDynamicStatsMenuWidgetYVar = "fDynamicStatsMenuWidgetY";
    static constexpr auto fDynamicStatsMenuWidgetXVar = "fDynamicStatsMenuWidgetX";
    static constexpr auto fScaleDynamicStatsMenuWidgetVar = "fScaleDynamicStatsMenuWidget";

    static constexpr auto bTargetDynamicStatsMenuWidgetVar = "bTargetDynamicStatsMenuWidget";
    static constexpr auto bTargetDynamicStatsMenuWidgetAlwaysVar = "bTargetDynamicStatsMenuWidgetAlways";
    static constexpr auto bTargetDynamicStatsAlwaysHideBackgroundVar = "bTargetDynamicStatsAlwaysHideBackground";
    static constexpr auto bTargetDynamicStatsAlwaysHideBaseBackgroundVar = "bTargetDynamicStatsAlwaysHideBaseBackground";
    static constexpr auto fTargetDynamicStatsMenuWidgetYVar = "fTargetDynamicStatsMenuWidgetY";
    static constexpr auto fTargetDynamicStatsMenuWidgetXVar = "fTargetDynamicStatsMenuWidgetX";
    static constexpr auto fScaleTargetDynamicStatsMenuWidgetVar = "fScaleTargetDynamicStatsMenuWidget";
    static constexpr auto fTargetDynamicStatBackgroundResistWhiteVar = "fTargetDynamicStatBackgroundResistWhite";
    static constexpr auto fTargetDynamicStatBackgroundResistBlackVar = "fTargetDynamicStatBackgroundResistBlack";
    static constexpr auto fTargetDynamicStatBackgroundResistNeutralVar = "fTargetDynamicStatBackgroundResistNeutral";
    static constexpr auto fTargetDynamicStatBackgroundResistArmorWhiteVar = "fTargetDynamicStatBackgroundResistArmorWhite";
    static constexpr auto fTargetDynamicStatBackgroundResistArmorBlackVar = "fTargetDynamicStatBackgroundResistArmorBlack";
    static constexpr auto fTargetDynamicStatBackgroundResistArmorNeutralVar = "fTargetDynamicStatBackgroundResistArmorNeutral";
    static constexpr auto bTargetDynamicStatsAltColoringVar = "bTargetDynamicStatsAltColoring";
    static constexpr auto bTargetDynamicStatsActiveOnlyWithKeywordVar = "bTargetDynamicStatsActiveOnlyWithKeyword";
    static constexpr auto bTargetDynamicStatsActiveOnlyWithTargetLockVar = "bTargetDynamicStatsActiveOnlyWithTargetLock";
  
    static constexpr auto bResimonrimArmorToResistVar = "bResimonrimArmorToResist";

    static constexpr auto iLeftTextAligmentVar = "iLeftTextAligment";
    static constexpr auto iCenterTextAligmentVar = "iCenterTextAligment";
    static constexpr auto iRightTextAligmentVar = "iRightTextAligment";

  struct GameData final
  {
    static constexpr auto STRESS_MOD = "Stress and Fear.esp"sv;
    static constexpr auto STARFROST_MOD = "Starfrost.esp"sv;
    static constexpr auto SKYRIM_MOD = "Skyrim.esm"sv;
    static constexpr auto SURVIVAL_MOD = "ccQDRSSE001-SurvivalMode.esl"sv;
    static constexpr auto BLADE_AND_BLUNT_MOD = "BladeAndBlunt.esp"sv;
    static constexpr auto RESIMONRIM_MOD = "RESimonrim.esp"sv;

    static constexpr RE::FormID SURVIVAL_COLD0 = 0x890;
    static constexpr RE::FormID SURVIVAL_COLD1 = 0x86E;
    static constexpr RE::FormID SURVIVAL_COLD2 = 0x891;
    static constexpr RE::FormID SURVIVAL_COLD3 = 0x86D;
    static constexpr RE::FormID SURVIVAL_COLD4 = 0x870;
    static constexpr RE::FormID SURVIVAL_COLD5 = 0x871;

    static constexpr RE::FormID SURVIVAL_EXHAUSTION1 = 0x878;
    static constexpr RE::FormID SURVIVAL_EXHAUSTION2 = 0x879;
    static constexpr RE::FormID SURVIVAL_EXHAUSTION3 = 0x87A;
    static constexpr RE::FormID SURVIVAL_EXHAUSTION4 = 0x87B;
    static constexpr RE::FormID SURVIVAL_EXHAUSTION5 = 0x87D;

    static constexpr RE::FormID SKYRIM_RESTED = 0xFB981;
    static constexpr RE::FormID SKYRIM_RESTED_MARRIAGE = 0xCDA1D;
    static constexpr RE::FormID SKYRIM_RESTED_WELL = 0xFB984;

    static constexpr RE::FormID STARFROST_FED = 0x80E;
    static constexpr RE::FormID STARFROST_FED_WELL = 0x813;
    static constexpr RE::FormID STARFROST_FED_VAMPIRE = 0x83C;
    static constexpr RE::FormID STARFROST_FED_MARRIAGE = 0x817;

    static constexpr RE::FormID STARFROST_HUNGER1 = 0x84E;
    static constexpr RE::FormID STARFROST_HUNGER2 = 0x856;
    static constexpr RE::FormID STARFROST_HUNGER3 = 0x857;

    static constexpr RE::FormID STRESS_TOTAL = 0x801;
    
    static constexpr RE::FormID MINOR_WOUND = 0x84A;
    static constexpr RE::FormID MAJOR_WOUND = 0x84B;
    static constexpr RE::FormID CRITICAL_WOUND = 0x84D;
    
    static constexpr RE::FormID INJURY = 0xAB3;

    RE::TESGlobal* stress_total;

    RE::SpellItem* cold0;
    RE::SpellItem* cold1;
    RE::SpellItem* cold2;
    RE::SpellItem* cold3;
    RE::SpellItem* cold4;
    RE::SpellItem* cold5;

    RE::SpellItem* exhaustion1;
    RE::SpellItem* exhaustion2;
    RE::SpellItem* exhaustion3;
    RE::SpellItem* exhaustion4;
    RE::SpellItem* exhaustion5;

    RE::SpellItem* rested;
    RE::SpellItem* rested_well;
    RE::SpellItem* rested_marriage;

    RE::SpellItem* fed;
    RE::SpellItem* fed_well;
    RE::SpellItem* fed_vampire;
    RE::SpellItem* fed_marriage;

    RE::SpellItem* hunger1;
    RE::SpellItem* hunger2;
    RE::SpellItem* hunger3;

    RE::SpellItem* wound1;
    RE::SpellItem* wound2;
    RE::SpellItem* wound3;
    
    RE::BGSKeyword* injury_keyword;

    RE::BGSKeyword* target_resists_keyword;

    void init()
    {
      stress_total = get_data(RE::TESGlobal, STRESS_TOTAL, STRESS_MOD);

      cold0 = get_data(RE::SpellItem, SURVIVAL_COLD0, SURVIVAL_MOD);
      cold1 = get_data(RE::SpellItem, SURVIVAL_COLD1, SURVIVAL_MOD);
      cold2 = get_data(RE::SpellItem, SURVIVAL_COLD2, SURVIVAL_MOD);
      cold3 = get_data(RE::SpellItem, SURVIVAL_COLD3, SURVIVAL_MOD);
      cold4 = get_data(RE::SpellItem, SURVIVAL_COLD4, SURVIVAL_MOD);
      cold5 = get_data(RE::SpellItem, SURVIVAL_COLD5, SURVIVAL_MOD);

      exhaustion1 = get_data(RE::SpellItem, SURVIVAL_EXHAUSTION1, SURVIVAL_MOD);
      exhaustion2 = get_data(RE::SpellItem, SURVIVAL_EXHAUSTION2, SURVIVAL_MOD);
      exhaustion3 = get_data(RE::SpellItem, SURVIVAL_EXHAUSTION3, SURVIVAL_MOD);
      exhaustion4 = get_data(RE::SpellItem, SURVIVAL_EXHAUSTION4, SURVIVAL_MOD);
      exhaustion5 = get_data(RE::SpellItem, SURVIVAL_EXHAUSTION5, SURVIVAL_MOD);

      rested = get_data(RE::SpellItem, SKYRIM_RESTED, SKYRIM_MOD);
      rested_well = get_data(RE::SpellItem, SKYRIM_RESTED_WELL, SKYRIM_MOD);
      rested_marriage = get_data(RE::SpellItem, SKYRIM_RESTED_MARRIAGE, SKYRIM_MOD);

      fed = get_data(RE::SpellItem, STARFROST_FED, STARFROST_MOD);
      fed_well = get_data(RE::SpellItem, STARFROST_FED_WELL, STARFROST_MOD);
      fed_vampire = get_data(RE::SpellItem, STARFROST_FED_VAMPIRE, STARFROST_MOD);
      fed_marriage = get_data(RE::SpellItem, STARFROST_FED_MARRIAGE, STARFROST_MOD);

      hunger1 = get_data(RE::SpellItem, STARFROST_HUNGER1, STARFROST_MOD);
      hunger2 = get_data(RE::SpellItem, STARFROST_HUNGER2, STARFROST_MOD);
      hunger3 = get_data(RE::SpellItem, STARFROST_HUNGER3, STARFROST_MOD);

      wound1 = get_data(RE::SpellItem, MINOR_WOUND, BLADE_AND_BLUNT_MOD);
      wound2 = get_data(RE::SpellItem, MAJOR_WOUND, BLADE_AND_BLUNT_MOD);
      wound3 = get_data(RE::SpellItem, CRITICAL_WOUND, BLADE_AND_BLUNT_MOD);
      
      injury_keyword = get_data(RE::BGSKeyword, INJURY, RESIMONRIM_MOD);
    }
  };

  Settings() = default;

  GameData game_data{};

  bool bVisibleAll{true};
  bool bEquipmentWidget{true};
  double fScaleEquipmentWidget{50.};
  bool bAlwaysArrowText{true};

  double fLeftHandIconY{210.};
  double fLeftHandIconX{600.};
  bool bLeftHandIcon{true};

  double fLeftTextY{210.};
  double fLeftTextX{600.};
  bool bLeftHandText{true};

  double fRightHandIconY{210.};
  double fRightHandIconX{600.};
  bool bRightHandIcon{true};

  double fRightTextY{210.};
  double fRightTextX{600.};
  bool bRightHandText{true};

  double fCenterHandIconY{210.};
  double fCenterHandIconX{600.};
  bool bCenterHandIcon{true};

  double fCenterTextY{210.};
  double fCenterTextX{600.};
  bool bCenterHandText{true};

  bool bPotionHealth{true};
  bool bPotionHealthFlash{true};
  double fPotionHealthY{210.};
  double fPotionHealthX{600.};
  double fScalePotionHealth{50.};
  bool bPotionHealthText{true};
  bool bPotionHealthTextCooldown{false};

  bool bPotionStamina{true};
  bool bPotionStaminaFlash{true};
  double fPotionStaminaY{210.};
  double fPotionStaminaX{600.};
  double fScalePotionStamina{50.};
  bool bPotionStaminaText{true};
  bool bPotionStaminaTextCooldown{false};

  bool bPotionMagicka{true};
  bool bPotionMagickaFlash{true};
  double fPotionMagickaY{210.};
  double fPotionMagickaX{600.};
  double fScalePotionMagicka{50.};
  bool bPotionMagickaText{true};
  bool bPotionMagickaTextCooldown{false};

  bool bPotionOther{true};
  bool bPotionOtherFlash{true};
  double fPotionOtherY{210.};
  double fPotionOtherX{600.};
  double fScalePotionOther{50.};
  bool bPotionOtherText{true};
  bool bPotionOtherTextCooldown{false};

  bool bPotionPoison{true};
  bool bPotionPoisonFlash{true};
  double fPotionPoisonY{210.};
  double fPotionPoisonX{600.};
  double fScalePotionPoison{50.};
  bool bPotionPoisonText{true};
  bool bPotionPoisonTextCooldown{false};

  bool bKiEnergy{true};
  double fKiEnergyPoisonY{210.};
  double fKiEnergyPoisonX{600.};
  double fScaleKiEnergy{50.};
  bool bKiEnergyText{true};
  bool bKiEnergyTextCooldown{false};

  bool bWeatherWidget{true};
  double fWeatherWidgetY{210.};
  double fWeatherWidgetX{600.};
  double fScaleWeatherWidget{50.};

  bool bExhaustingWidget{true};
  double fExhaustingWidgetY{210.};
  double fExhaustingWidgetX{600.};
  double fScaleExhaustingWidget{50.};

  bool bHungerWidget{true};
  double fHungerWidgetY{210.};
  double fHungerWidgetX{600.};
  double fScaleHungerWidget{50.};

  bool bStressWidget{true};
  bool bStressWidgetText{true};
  double fStressWidgetY{210.};
  double fStressWidgetX{600.};
  double fScaleStressWidget{50.};

  bool bStatsMenuWidget{true};
  bool bStatsMenuWidgetRegenFixed{true};
  double fStatsMenuWidgetY{210.};
  double fStatsMenuWidgetX{600.};
  double fScaleStatsMenuWidget{50.};

  bool bDynamicStatsMenuWidget{true};
  bool bDynamicStatsMenuWidgetAlways{true};
  bool bDynamicStatsAlwaysHideBackground{true};
  bool bDynamicStatsAlwaysHideBaseBackground{true};
  double fDynamicStatsMenuWidgetY{210.};
  double fDynamicStatsMenuWidgetX{600.};
  double fScaleDynamicStatsMenuWidget{50.};

  bool bTargetDynamicStatsMenuWidget{true};
  bool bTargetDynamicStatsMenuWidgetAlways{true};
  bool bTargetDynamicStatsAlwaysHideBackground{true};
  bool bTargetDynamicStatsAlwaysHideBaseBackground{true};
  bool bTargetDynamicStatsAltColoring{true};
  bool bTargetDynamicStatsActiveOnlyWithKeyword{true};
  bool bTargetDynamicStatsActiveOnlyWithTargetLock{true};
  double fTargetDynamicStatsMenuWidgetY{210.};
  double fTargetDynamicStatsMenuWidgetX{600.};
  double fScaleTargetDynamicStatsMenuWidget{50.};
  double fTargetDynamicStatBackgroundResistWhite{0.};
  double fTargetDynamicStatBackgroundResistBlack{25.};
  double fTargetDynamicStatBackgroundResistNeutral{50.};
  double fTargetDynamicStatBackgroundResistArmorWhite{100.};
  double fTargetDynamicStatBackgroundResistArmorBlack{25.};
  double fTargetDynamicStatBackgroundResistArmorNeutral{50.};

  bool bWoundInjuryDiseaseMenuWidget{true};
  bool bWoundInjuryDiseaseMenuDebug{false};
  double fWoundInjuryDiseaseMenuWidgetY{250.0};
  double fWoundInjuryDiseaseMenuWidgetX{0.0};
  double fScaleWoundInjuryDiseaseMenuWidget{50.0};

  bool bAbsorbShieldWidget{true};
  bool bAbsorbShieldWidgetDebug{false};
  double fAbsorbShieldWidgetY{250.0};
  double fAbsorbShieldWidgetX{0.0};
  double fScaleYAbsorbShieldWidget{50.0};
  double fScaleXAbsorbShieldWidget{50.0};

  bool bResimonrimArmorToResist{false};

  long iLeftTextAligment{0};
  long iCenterTextAligment{1};
  long iRightTextAligment{2};

  [[nodiscard]] static auto get_singleton() noexcept -> Settings&
  {
    static Settings instance;
    return instance;
  }

  auto load() -> void
  {

    const auto read_double = [this](const CSimpleIni& ini, const char* key, double& value) -> void {
      if (ini.GetValue(section, key)) {
        value = ini.GetDoubleValue(section, key);
      }
    };

    const auto read_int = [this](const CSimpleIni& ini, const char* key, long& value) -> void {
      if (ini.GetValue(section, key)) {
        value = ini.GetLongValue(section, key);
      }
    };

    const auto read_bool = [this](const CSimpleIni& ini, const char* key, bool& value) -> void {
      if (ini.GetValue(section, key)) {
        value = ini.GetBoolValue(section, key);
      }
    };

    //const auto read_settings = [this, read_double, read_bool, read_int](CSimpleIni& ini, const wchar_t* path) -> void {
    //  ini.LoadFile(path);
    //  if (ini.IsEmpty()) {
    //    return;
    //  }
    //  
    //  
    //};

    //CSimpleIni ini;
    CSimpleIni ini_giga_settings;

    //ini.SetUnicode();
    ini_giga_settings.SetUnicode();
    //read_settings(ini, path_to_ini_default);
    //read_settings(ini, path_to_ini);

    ini_giga_settings.LoadFile(path_to_settings);
    if (!ini_giga_settings.IsEmpty()) {
      loge("Settings ini is empty.");

      // Equipment Widget
      read_int(ini_giga_settings, iLeftTextAligmentVar, iLeftTextAligment);
      read_int(ini_giga_settings, iCenterTextAligmentVar, iCenterTextAligment);
      read_int(ini_giga_settings, iRightTextAligmentVar, iRightTextAligment);

      read_bool(ini_giga_settings, bVisibleAllVar, bVisibleAll);
      read_bool(ini_giga_settings, bEquipmentWidgetVar, bEquipmentWidget);
      read_bool(ini_giga_settings, bAlwaysArrowTextVar, bAlwaysArrowText);
      read_double(ini_giga_settings, fScaleEquipmentWidgetVar, fScaleEquipmentWidget);

      // Right Hand
      read_bool(ini_giga_settings, bRightHandIconVar, bRightHandIcon);
      read_double(ini_giga_settings, fRightHandIconXVar, fRightHandIconX);
      read_double(ini_giga_settings, fRightHandIconYVar, fRightHandIconY);

      read_bool(ini_giga_settings, bRightHandTextVar, bRightHandText);
      read_double(ini_giga_settings, fRightTextXVar, fRightTextX);
      read_double(ini_giga_settings, fRightTextYVar, fRightTextY);

      // Left Hand
      read_bool(ini_giga_settings, bLeftHandIconVar, bLeftHandIcon);
      read_double(ini_giga_settings, fLeftHandIconXVar, fLeftHandIconX);
      read_double(ini_giga_settings, fLeftHandIconYVar, fLeftHandIconY);

      read_bool(ini_giga_settings, bLeftHandTextVar, bLeftHandText);
      read_double(ini_giga_settings, fLeftTextXVar, fLeftTextX);
      read_double(ini_giga_settings, fLeftTextYVar, fLeftTextY);

      // Ability
      read_bool(ini_giga_settings, bCenterHandIconVar, bCenterHandIcon);
      read_double(ini_giga_settings, fCenterHandIconXVar, fCenterHandIconX);
      read_double(ini_giga_settings, fCenterHandIconYVar, fCenterHandIconY);

      read_bool(ini_giga_settings, bCenterHandTextVar, bCenterHandText);
      read_double(ini_giga_settings, fCenterTextXVar, fCenterTextX);
      read_double(ini_giga_settings, fCenterTextYVar, fCenterTextY);
      // Equipment Widget

      // Potions and Poison Widget
      // Health
      read_bool(ini_giga_settings, bPotionHealthVar, bPotionHealth);
      read_bool(ini_giga_settings, bPotionHealthFlashVar, bPotionHealthFlash);
      read_bool(ini_giga_settings, bPotionHealthTextVar, bPotionHealthText);
      read_bool(ini_giga_settings, bPotionHealthTextCooldownVar, bPotionHealthTextCooldown);
      read_double(ini_giga_settings, fPotionHealthXVar, fPotionHealthX);
      read_double(ini_giga_settings, fPotionHealthYVar, fPotionHealthY);
      read_double(ini_giga_settings, fScalePotionHealthVar, fScalePotionHealth);

      // Stamina
      read_bool(ini_giga_settings, bPotionStaminaVar, bPotionStamina);
      read_bool(ini_giga_settings, bPotionStaminaFlashVar, bPotionStaminaFlash);
      read_bool(ini_giga_settings, bPotionStaminaTextVar, bPotionStaminaText);
      read_bool(ini_giga_settings, bPotionStaminaTextCooldownVar, bPotionStaminaTextCooldown);
      read_double(ini_giga_settings, fPotionStaminaXVar, fPotionStaminaX);
      read_double(ini_giga_settings, fPotionStaminaYVar, fPotionStaminaY);
      read_double(ini_giga_settings, fScalePotionStaminaVar, fScalePotionStamina);

      // Magicka
      read_bool(ini_giga_settings, bPotionMagickaVar, bPotionMagicka);
      read_bool(ini_giga_settings, bPotionMagickaFlashVar, bPotionMagickaFlash);
      read_bool(ini_giga_settings, bPotionMagickaTextVar, bPotionMagickaText);
      read_bool(ini_giga_settings, bPotionMagickaTextCooldownVar, bPotionMagickaTextCooldown);
      read_double(ini_giga_settings, fPotionMagickaXVar, fPotionMagickaX);
      read_double(ini_giga_settings, fPotionMagickaYVar, fPotionMagickaY);
      read_double(ini_giga_settings, fScalePotionMagickaVar, fScalePotionMagicka);

      // Other
      read_bool(ini_giga_settings, bPotionOtherVar, bPotionOther);
      read_bool(ini_giga_settings, bPotionOtherFlashVar, bPotionOtherFlash);
      read_bool(ini_giga_settings, bPotionOtherTextVar, bPotionOtherText);
      read_bool(ini_giga_settings, bPotionOtherTextCooldownVar, bPotionOtherTextCooldown);
      read_double(ini_giga_settings, fPotionOtherXVar, fPotionOtherX);
      read_double(ini_giga_settings, fPotionOtherYVar, fPotionOtherY);
      read_double(ini_giga_settings, fScalePotionOtherVar, fScalePotionOther);

      // Poison
      read_bool(ini_giga_settings, bPotionPoisonVar, bPotionPoison);
      read_bool(ini_giga_settings, bPotionPoisonFlashVar, bPotionPoisonFlash);
      read_bool(ini_giga_settings, bPotionPoisonTextVar, bPotionPoisonText);
      read_bool(ini_giga_settings, bPotionPoisonTextCooldownVar, bPotionPoisonTextCooldown);
      read_double(ini_giga_settings, fPotionPoisonXVar, fPotionPoisonX);
      read_double(ini_giga_settings, fPotionPoisonYVar, fPotionPoisonY);
      read_double(ini_giga_settings, fScalePotionPoisonVar, fScalePotionPoison);
      // Potions and Poison Widget

      // Ki Energy Widget
      read_bool(ini_giga_settings, bKiEnergyVar, bKiEnergy);
      read_bool(ini_giga_settings, bKiEnergyTextVar, bKiEnergyText);
      read_bool(ini_giga_settings, bKiEnergyTextCooldownVar, bKiEnergyTextCooldown);
      read_double(ini_giga_settings, fKiEnergyPoisonXVar, fKiEnergyPoisonX);
      read_double(ini_giga_settings, fKiEnergyPoisonYVar, fKiEnergyPoisonY);
      read_double(ini_giga_settings, fScaleKiEnergyVar, fScaleKiEnergy);

      // Absorb Shield 
      read_bool(ini_giga_settings, bAbsorbShieldWidgetVar, bAbsorbShieldWidget);
      read_bool(ini_giga_settings, bAbsorbShieldWidgetDebugVar, bAbsorbShieldWidgetDebug);
      read_double(ini_giga_settings, fAbsorbShieldWidgetYVar, fAbsorbShieldWidgetY);
      read_double(ini_giga_settings, fAbsorbShieldWidgetXVar, fAbsorbShieldWidgetX);
      read_double(ini_giga_settings, fScaleXAbsorbShieldWidgetVar, fScaleXAbsorbShieldWidget);
      read_double(ini_giga_settings, fScaleYAbsorbShieldWidgetVar, fScaleYAbsorbShieldWidget);

      // Stats Menu
      read_bool(ini_giga_settings, bStatsMenuWidgetVar, bStatsMenuWidget);
      read_bool(ini_giga_settings, bStatsMenuWidgetRegenFixedVar, bStatsMenuWidgetRegenFixed);
      read_double(ini_giga_settings, fStatsMenuWidgetYVar, fStatsMenuWidgetY);
      read_double(ini_giga_settings, fStatsMenuWidgetXVar, fStatsMenuWidgetX);
      read_double(ini_giga_settings, fScaleStatsMenuWidgetVar, fScaleStatsMenuWidget);

      // Stress Widget
      read_bool(ini_giga_settings, bStressWidgetVar, bStressWidget);
      read_bool(ini_giga_settings, bStressWidgetTextVar, bStressWidgetText);
      read_double(ini_giga_settings, fStressWidgetYVar, fStressWidgetY);
      read_double(ini_giga_settings, fStressWidgetXVar, fStressWidgetX);
      read_double(ini_giga_settings, fScaleStressWidgetVar, fScaleStressWidget);

      // Hunger Widget
      read_bool(ini_giga_settings, bHungerWidgetVar, bHungerWidget);
      read_double(ini_giga_settings, fHungerWidgetYVar, fHungerWidgetY);
      read_double(ini_giga_settings, fHungerWidgetXVar, fHungerWidgetX);
      read_double(ini_giga_settings, fScaleHungerWidgetVar, fScaleHungerWidget);

      // Weather Widget
      read_bool(ini_giga_settings, bWeatherWidgetVar, bWeatherWidget);
      read_double(ini_giga_settings, fWeatherWidgetYVar, fWeatherWidgetY);
      read_double(ini_giga_settings, fWeatherWidgetXVar, fWeatherWidgetX);
      read_double(ini_giga_settings, fScaleWeatherWidgetVar, fScaleWeatherWidget);

      // Exhausting Widget
      read_bool(ini_giga_settings, bExhaustingWidgetVar, bExhaustingWidget);
      read_double(ini_giga_settings, fExhaustingWidgetYVar, fExhaustingWidgetY);
      read_double(ini_giga_settings, fExhaustingWidgetXVar, fExhaustingWidgetX);
      read_double(ini_giga_settings, fScaleExhaustingWidgetVar, fScaleExhaustingWidget);

      // Wound Injury
      read_bool(ini_giga_settings, bWoundInjuryDiseaseMenuWidgetVar, bWoundInjuryDiseaseMenuWidget);
      read_bool(ini_giga_settings, bWoundInjuryDiseaseMenuDebugVar, bWoundInjuryDiseaseMenuDebug);
      read_double(ini_giga_settings, fWoundInjuryDiseaseMenuWidgetYVar, fWoundInjuryDiseaseMenuWidgetY);
      read_double(ini_giga_settings, fWoundInjuryDiseaseMenuWidgetXVar, fWoundInjuryDiseaseMenuWidgetX);
      read_double(ini_giga_settings, fScaleWoundInjuryDiseaseMenuWidgetVar, fScaleWoundInjuryDiseaseMenuWidget);
      
      // Dynamic Stats
      read_bool(ini_giga_settings, bDynamicStatsMenuWidgetVar, bDynamicStatsMenuWidget);
      read_bool(ini_giga_settings, bDynamicStatsMenuWidgetAlwaysVar, bDynamicStatsMenuWidgetAlways);
      read_bool(ini_giga_settings, bDynamicStatsAlwaysHideBackgroundVar, bDynamicStatsAlwaysHideBackground);
      read_bool(ini_giga_settings, bDynamicStatsAlwaysHideBaseBackgroundVar, bDynamicStatsAlwaysHideBaseBackground);
      read_double(ini_giga_settings, fDynamicStatsMenuWidgetYVar, fDynamicStatsMenuWidgetY);
      read_double(ini_giga_settings, fDynamicStatsMenuWidgetXVar, fDynamicStatsMenuWidgetX);
      read_double(ini_giga_settings, fScaleDynamicStatsMenuWidgetVar, fScaleDynamicStatsMenuWidget);
      
      // Target Dynamic Stats
      read_bool(ini_giga_settings, bTargetDynamicStatsMenuWidgetVar, bTargetDynamicStatsMenuWidget);
      read_bool(ini_giga_settings, bTargetDynamicStatsMenuWidgetAlwaysVar, bTargetDynamicStatsMenuWidgetAlways);
      read_bool(ini_giga_settings, bTargetDynamicStatsAlwaysHideBackgroundVar, bTargetDynamicStatsAlwaysHideBackground);
      read_bool(ini_giga_settings, bTargetDynamicStatsAlwaysHideBaseBackgroundVar, bTargetDynamicStatsAlwaysHideBaseBackground);
      read_bool(ini_giga_settings, bTargetDynamicStatsAltColoringVar, bTargetDynamicStatsAltColoring);
      read_bool(ini_giga_settings, bTargetDynamicStatsActiveOnlyWithKeywordVar, bTargetDynamicStatsActiveOnlyWithKeyword);
      read_bool(ini_giga_settings, bTargetDynamicStatsActiveOnlyWithTargetLockVar, bTargetDynamicStatsActiveOnlyWithTargetLock);
      read_double(ini_giga_settings, fTargetDynamicStatsMenuWidgetYVar, fTargetDynamicStatsMenuWidgetY);
      read_double(ini_giga_settings, fTargetDynamicStatsMenuWidgetXVar, fTargetDynamicStatsMenuWidgetX);
      read_double(ini_giga_settings, fScaleTargetDynamicStatsMenuWidgetVar, fScaleTargetDynamicStatsMenuWidget);
      read_double(ini_giga_settings, fTargetDynamicStatBackgroundResistWhiteVar, fTargetDynamicStatBackgroundResistWhite);
      read_double(ini_giga_settings, fTargetDynamicStatBackgroundResistBlackVar, fTargetDynamicStatBackgroundResistBlack);
      read_double(ini_giga_settings, fTargetDynamicStatBackgroundResistNeutralVar, fTargetDynamicStatBackgroundResistNeutral);
      read_double(ini_giga_settings, fTargetDynamicStatBackgroundResistArmorWhiteVar, fTargetDynamicStatBackgroundResistArmorWhite);
      read_double(ini_giga_settings, fTargetDynamicStatBackgroundResistArmorBlackVar, fTargetDynamicStatBackgroundResistArmorBlack);
      read_double(ini_giga_settings, fTargetDynamicStatBackgroundResistArmorNeutralVar, fTargetDynamicStatBackgroundResistArmorNeutral);

      // Other Settings
      read_bool(ini_giga_settings, bResimonrimArmorToResistVar, bResimonrimArmorToResist);
    }

    auto& data = WidgetData::get_singleton();
    data.load();

    game_data.init();
    game_data.target_resists_keyword = get_data(RE::BGSKeyword, data.iActiveKeywordId, data.sPluginDataName);
  }

  auto write_setting_float(const char* setting_field, const float new_setting) -> void
  {

    CSimpleIni ini;

    ini.SetUnicode();
    ini.LoadFile(path_to_settings);
    if (ini.IsEmpty()) {
      return;
    }

    if (!ini.SectionExists(section)) {
      return;
    }

    if (!ini.KeyExists(section, setting_field)) {
      return;
    }

    ini.SetDoubleValue(section, setting_field, new_setting);

    let result = ini.SaveFile(path_to_settings);
    if (result < 0) {
      loge("Error when try save: {} with value: {}", setting_field, new_setting);
    }
    
  }

  auto write_setting_int(const char* setting_field, const int new_setting) -> void
  {

    CSimpleIni ini;

    ini.SetUnicode();
    ini.LoadFile(path_to_settings);
    if (ini.IsEmpty()) {
      return;
    }

    if (!ini.SectionExists(section)) {
      return;
    }

    if (!ini.KeyExists(section, setting_field)) {
      return;
    }

    ini.SetLongValue(section, setting_field, new_setting);

    let result = ini.SaveFile(path_to_settings);
    if (result < 0) {
      loge("Error when try save: {} with value: {}", setting_field, new_setting);
    }
    
  }

  auto write_setting_bool(const char* setting_field, const bool new_setting) -> void
  {

    CSimpleIni ini;

    ini.SetUnicode();
    ini.LoadFile(path_to_settings);
    if (ini.IsEmpty()) {
      return;
    }

    if (!ini.SectionExists(section)) {
      return;
    }

    if (!ini.KeyExists(section, setting_field)) {
      return;
    }

    ini.SetBoolValue(section, setting_field, new_setting);

    let result = ini.SaveFile(path_to_settings);
    if (result < 0) {
      loge("Error when try save: {} with value: {}", setting_field, new_setting);
    }
    
  }
};

