#include "event/event.h"
#include "hook/hook.h"
#include "mod/mod_manager.h"
#include "scaleform/scaleform.h"
#include "setting/setting.h"

void init_logger() {
    if (static bool initialized = false; !initialized) {
        initialized = true;
    } else {
        return;
    }

    try {
        auto path = logger::log_directory();
        if (!path) {
            stl::report_and_fail("failed to get standard log path"sv);
        }

        *path /= fmt::format("{}.log"sv, Version::PROJECT);
        auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
        auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

        log->set_level(spdlog::level::info);
        log->flush_on(spdlog::level::info);

        spdlog::set_default_logger(std::move(log));
        spdlog::set_pattern("[%H:%M:%S.%f] %s(%#) [%^%l%$] %v"s);

        logger::info("{} v{}"sv, Version::PROJECT, Version::NAME);

        try {
            setting::load_settings();
        } catch (const std::exception& e) {
            logger::warn("failed to load ini_setting {}"sv, e.what());
        }

        //if (setting::get_log_level()) {
        if (true) {
            spdlog::set_level(spdlog::level::trace);
            spdlog::flush_on(spdlog::level::trace);
        }

    } catch (const std::exception& e) {
        logger::critical("failed, cause {}"sv, e.what());
    }
}

void init_mod_support() {
    auto* mod_manager = mod::mod_manager::get_singleton();

    //check for mods here
    mod_manager->set_hand_to_hand(LoadLibrary(L"Data/SKSE/Plugins/HandToHand.dll"));
}

EXTERN_C [[maybe_unused]] __declspec(dllexport) bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse) {
    init_logger();

    logger::info("{} loading"sv, Version::PROJECT);
    logger::info("Game version {}"sv, a_skse->RuntimeVersion().string());

    Init(a_skse);

    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* a_msg) {
        switch (a_msg->type) {
            case SKSE::MessagingInterface::kDataLoaded:
                logger::info("Data loaded"sv);

                init_mod_support();

                hook::hook::install();
                event::event::sink_event_handler();
                scaleform::scaleform::Register();

                logger::info("Done with adding"sv);
                break;
        }
    });

    logger::info("{} loaded"sv, Version::PROJECT);
    return true;
}

EXTERN_C [[maybe_unused]] __declspec(dllexport) constinit auto SKSEPlugin_Version = []() noexcept {
    SKSE::PluginVersionData v;
    v.PluginName(Version::PROJECT);
    v.AuthorName(Version::AUTHOR);
    v.PluginVersion({ Version::MAJOR, Version::MINOR, Version::PATCH, Version::BETA });
    v.UsesAddressLibrary(true);
    v.CompatibleVersions({ SKSE::RUNTIME_SSE_LATEST });
    v.HasNoStructUse(true);
    return v;
}();

EXTERN_C [[maybe_unused]] __declspec(dllexport) bool SKSEAPI
    SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* pluginInfo) {
    pluginInfo->name = SKSEPlugin_Version.pluginName;
    pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
    pluginInfo->version = SKSEPlugin_Version.pluginVersion;


    if (a_skse->IsEditor()) {
        logger::critical("Loaded in editor, marking as incompatible"sv);
        return false;
    }

    const auto ver = a_skse->RuntimeVersion();
    if (ver < SKSE::RUNTIME_SSE_1_5_39) {
        logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
        return false;
    }

    return true;
}
