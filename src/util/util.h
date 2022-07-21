#pragma once
#include "setting/setting.h"

namespace util {
    class string_util {
    public:
        template <typename T>
        static std::string int_to_hex(T a_i) {
            std::stringstream stream;
            stream << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << a_i;

            return stream.str();
        }

        static std::string to_lower(std::string s) {
            for (char& c : s)
                c = std::tolower(c, std::locale());
            return s;
        }
    };

    class avatar_util {
        inline static std::map<std::string_view, std::string_view> mer_name_map_ = {
            { "wood elf", "bosmer" },
            { "high elf", "altmer" },
            { "dark elf", "dunmer" },
        };

        static std::string_view get_mer_name(std::string_view a_name) {
            if (!mer_name_map_.contains(a_name)) {
                logger::warn("can not find name {}"sv, a_name);
                return a_name;
            }
            return mer_name_map_.find(a_name)->second;
        }

    public:
        static std::string get_avatar_key(const std::string& a_race, const std::string& a_gender) {
            //to remove the text key, not the cleanest but it brings us the value we need
            const auto gender = std::regex_replace(a_gender, std::regex("\\$DMNI"), "");
            auto race = string_util::to_lower(a_race);

            //need map some race names
            //if not found, same value will be returned
            race = get_mer_name(race);

            const auto avatar = *setting::avatar_set ? "var_" : "org_";

            return avatar + race + "_" + string_util::to_lower(gender);
        }
    };
}
