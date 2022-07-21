#pragma once
#include "Object.h"

namespace CLIK {
    class lazy_data_setter : public Object {
        using super = Object;

    public:
        lazy_data_setter() = default;
        lazy_data_setter(const lazy_data_setter&) = default;
        lazy_data_setter(lazy_data_setter&&) = default;
        using super::super;

        explicit lazy_data_setter(const super& a_rhs)
            : super(a_rhs) {}

        explicit lazy_data_setter(super&& a_rhs)
            : super(std::move(a_rhs)) {}

        ~lazy_data_setter() = default;

        lazy_data_setter& operator=(const lazy_data_setter&) = default;
        lazy_data_setter& operator=(lazy_data_setter&&) = default;
        using super::operator=;

        lazy_data_setter& operator=(const super& a_rhs) {
            super::operator=(a_rhs);
            return *this;
        }

        lazy_data_setter& operator=(super&& a_rhs) {
            super::operator=(std::move(a_rhs));
            return *this;
        }

        [[nodiscard]] Object data_provider() const { return GetObject("dataProvider"); }
        void data_provider(const Object& a_data_provider) { SetObject("dataProvider", a_data_provider); }
    };
}
