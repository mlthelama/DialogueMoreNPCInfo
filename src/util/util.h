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
            const auto avatar = setting::get_avatar_set() == 1 ? "var_" : "org_";
            return avatar + a_race + "_" + a_gender;
        }
    };
}
