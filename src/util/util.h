#pragma once
#include "setting/setting.h"

namespace util {
    class string_util {
    public:
        static std::string to_lower(std::string s) {
            for (char& c : s) c = std::tolower(c, std::locale());
            return s;
        }
    };

    class avatar_util {
    public:
        static std::string get_avatar_key(const std::string& a_race, const std::string& a_gender) {
            //got just only dragon icon
            const auto avatar = a_race == "dragon" ? "org_" : setting::get_avatar_set() == 1 ? "var_" : "org_";

            return avatar + a_race + "_" + a_gender;
        }
    };
}
