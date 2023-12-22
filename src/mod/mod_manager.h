#pragma once

namespace mod {
    class mod_manager {
    public:
        [[nodiscard]] static mod_manager* get_singleton();

        void set_hand_to_hand(bool a_value);
        [[nodiscard]] bool get_hand_to_hand() const;


    private:
        bool hand_to_hand{};
    };
}
