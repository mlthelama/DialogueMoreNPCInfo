#pragma once

namespace util {
    class type_util {
    public:
        template <typename T>
        static std::string int_to_hex(T a_value) {
            std::stringstream stream;
            stream << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << a_value;

            return stream.str();
        }
    };
}  // util
