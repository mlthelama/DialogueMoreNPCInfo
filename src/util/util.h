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
    public:
        static std::string get_avatar_key(const std::string& a_race, const std::string& a_gender) {
            const auto gender = std::regex_replace(a_gender, std::regex("\\$DMNI"), "");
            auto race = string_util::to_lower(a_race);

            //to map it to the icons
            if (race == "wood elf") {
                race = "bosmer";
            } else if (race == "high elf") {
                race = "altmer";
            } else if (race == "dark elf") {
                race = "dunmer";
            }

            const auto avatar = *setting::avatar_set ? "var_" : "org_";

            return avatar + race + "_" + string_util::to_lower(gender);
        }
    };
}
