#pragma once
#include "Object.h"

namespace CLIK {
    class key_value : public Object {
        using super = Object;

    public:
        key_value() = default;
        key_value(const key_value&) = default;
        key_value(key_value&&) = default;
        using super::super;

        explicit key_value(const super& a_rhs)
            : super(a_rhs) {}

        explicit key_value(super&& a_rhs)
            : super(std::move(a_rhs)) {}

        ~key_value() = default;

        key_value& operator=(const key_value&) = default;
        key_value& operator=(key_value&&) = default;
        using super::operator=;

        key_value& operator=(const super& a_rhs) {
            super::operator=(a_rhs);
            return *this;
        }

        key_value& operator=(super&& a_rhs) {
            super::operator=(std::move(a_rhs));
            return *this;
        }

        [[nodiscard]] Object data_provider() const { return GetObject("dataProvider"); }
        void data_provider(const Object& a_data_provider) { SetObject("dataProvider", a_data_provider); }
    };
}
