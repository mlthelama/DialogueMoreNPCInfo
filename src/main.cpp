#include "event.h"
#include "hook.h"
#include "scaleform/scaleform.h"
#include "setting/setting.h"
#include "util/constant.h"

void init_logger() {
    if (static bool initialized = false; !initialized) {
        initialized = true;
    } else {
        return;
    }

    try {
#ifndef NDEBUG
        auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
        auto path = logger::log_directory();
        if (!path) {
            stl::report_and_fail("failed to get standard log path"sv);
        }

        *path /= Version::PROJECT;
        *path += ".log"sv;
        auto sink = make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif
        auto log = make_shared<spdlog::logger>("global log"s, move(sink));

#ifndef NDEBUG
        log->set_level(spdlog::level::trace);
#else
        log->set_level(spdlog::level::trace);
        log->flush_on(spdlog::level::trace);
#endif

        set_default_logger(move(log));
        spdlog::set_pattern("[%H:%M:%S.%f] %s(%#) [%^%l%$] %v"s);

        logger::info("{} v{}"sv, Version::PROJECT, Version::NAME);

        try {
            setting::load_settings();
        } catch (const std::exception& e) { logger::warn("failed to load setting {}"sv, e.what()); }

        switch (setting::get_log_level()) {
            case util::const_log_trace:
                spdlog::set_level(spdlog::level::trace);
                spdlog::flush_on(spdlog::level::trace);
                break;
            case util::const_log_debug:
                spdlog::set_level(spdlog::level::debug);
                spdlog::flush_on(spdlog::level::debug);
                break;
            case util::const_log_info:
                spdlog::set_level(spdlog::level::info);
                spdlog::flush_on(spdlog::level::info);
                break;
        }
    } catch (const std::exception& e) { logger::critical("failed, cause {}"sv, e.what()); }
}

extern "C" DLLEXPORT constexpr auto SKSEPlugin_Version = []() {
    SKSE::PluginVersionData v{};
    v.PluginVersion(Version::MAJOR);
    v.PluginName(Version::PROJECT.data());
    v.AuthorName(Version::AUTHOR);
    v.UsesAddressLibrary(true);
    v.CompatibleVersions({ SKSE::RUNTIME_LATEST });
    return v;
}();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse) {
    try {
        init_logger();

        logger::info("DialogMoreNPCInfo loading"sv);

        Init(a_skse);

        SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* a_msg) {
            switch (a_msg->type) {
                case SKSE::MessagingInterface::kDataLoaded:
                    logger::info("Data loaded"sv);
                    hook::install_hooks();
		    event::sink_event_handlers();		    
                    scaleform::Register();
                    break;
            }
        });

        logger::info("DialogMoreNPCInfo loaded"sv);
    } catch (const std::exception& e) {
        logger::critical("failed, cause {}"sv, e.what());
        return false;
    }

    return true;
}
