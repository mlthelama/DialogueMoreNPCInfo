#pragma once

struct Setting {
    using ISetting = AutoTOML::ISetting;
    using bSetting = AutoTOML::bSetting;
    using sSetting = AutoTOML::sSetting;
    using iSetting = AutoTOML::iSetting;

    static void load() {
        try {
            const auto table = toml::parse_file("Data/SKSE/Plugins/DialogueMoreNPCInfo.toml"s);
            for (const auto& setting : ISetting::get_settings()) { setting->load(table); }
        } catch (const toml::parse_error& ex) {
            std::ostringstream ss;
            ss << "Error parsing file \'" << *ex.source().path << "\':\n"
               << '\t' << ex.description() << '\n'
               << "\t\t(" << ex.source().begin << ')';
            logger::error(ss.str());
            throw std::runtime_error("failed to load settings"s);
        }
    }

    //static inline const char* _constUndefined = "<undef>";

    static inline iSetting logLevel{ "General"s, "logLevel"s, 0 };
};
