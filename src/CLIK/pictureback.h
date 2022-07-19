#pragma once
#include "Object.h"

namespace CLIK {
    class picture_back : public Object {
        using super = Object;

    public:
        picture_back() = default;
        picture_back(const picture_back&) = default;
        picture_back(picture_back&&) = default;
        using super::super;

        explicit picture_back(const super& a_rhs)
            : super(a_rhs) {}

        explicit picture_back(super&& a_rhs)
            : super(std::move(a_rhs)) {}

        ~picture_back() = default;

        picture_back& operator=(const picture_back&) = default;
        picture_back& operator=(picture_back&&) = default;
        using super::operator=;

        picture_back& operator=(const super& a_rhs) {
            super::operator=(a_rhs);
            return *this;
        }

        picture_back& operator=(super&& a_rhs) {
            super::operator=(std::move(a_rhs));
            return *this;
        }

        [[nodiscard]] Object data_provider() const { return GetObject("dataProvider"); }
        void data_provider(const Object& a_data_provider) { SetObject("dataProvider", a_data_provider); }
    };
}
