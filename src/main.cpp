#include "GigaWidgetAPI.hpp"
#include "MenuHandler.hpp"
#include "ModAPI.hpp"
#include "Settings.hpp"
#include "UI.hpp"
#include "Widget.hpp"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

extern "C" DLLEXPORT void* SKSEAPI RequestPluginAPI(const GIGAWIDGET_API::InterfaceVersion a_interfaceVersion)
{
  //auto api = Messaging::TrueHUDInterface::GetSingleton();
  auto api = Messaging::GigaWidgetInterface::get_singleton();
  logger::info("GIGAWIDGET_API::RequestPluginAPI called");

  switch (a_interfaceVersion) {
    case GIGAWIDGET_API::InterfaceVersion::V1: {
      logger::info("GIGAWIDGET_API::RequestPluginAPI returned the API singleton");
      return static_cast<void*>(api);
    }
  }
  logger::info("GIGAWIDGET_API::RequestPluginAPI requested the wrong interface version");
  return nullptr;
}

void init_logging()
{
  auto path = logger::log_directory();
  if (!path)
    return;

  const auto plugin = SKSE::PluginDeclaration::GetSingleton();
  *path /= std::format("{}.log", plugin->GetName());

  std::vector<spdlog::sink_ptr> sinks{std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true),
                                      std::make_shared<spdlog::sinks::msvc_sink_mt>()};

  auto logger = std::make_shared<spdlog::logger>("global", sinks.begin(), sinks.end());
  logger->set_level(spdlog::level::info);
  logger->flush_on(spdlog::level::info);

  spdlog::set_default_logger(std::move(logger));
  spdlog::set_pattern("[%^%L%$] %v");
}

static void message_handler(SKSE::MessagingInterface::Message* message)
{
  switch (message->type) {
    case SKSE::MessagingInterface::kDataLoaded: {
      // It is now safe to access form data.
      Settings::get_singleton().load();
      GigaWidget::register_();
      MenuHandler::register_();
      GigaWidget::show();
      logger::info("Get data loaded message"sv);
      break;
    }
    case SKSE::MessagingInterface::kNewGame:
    case SKSE::MessagingInterface::kPostLoadGame: {
      // loading. Data will be a boolean indicating
      // whether the load was successful.
      Settings::get_singleton().load();
      GigaWidget::show();
      break;
    }
  }
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
  init_logging();

  const auto plugin = SKSE::PluginDeclaration::GetSingleton();
  logger::info("{} v{} is loading...", plugin->GetName(), plugin->GetVersion());

  SKSE::Init(a_skse);
  if (!SKSE::GetMessagingInterface()->RegisterListener(message_handler)) {
    logger::critical("Failed to load messaging interface! This error is fatal, plugin will not load.");
    return false;
  }

  logger::info("{} loaded.", plugin->GetName());
  ReUI::Register();

  return true;
}

