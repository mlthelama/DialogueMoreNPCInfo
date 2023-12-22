#include "mod_manager.h"

namespace mod {
    mod_manager* mod_manager::get_singleton() {
        static mod_manager singleton;
        return std::addressof(singleton);
    }

    void mod_manager::set_hand_to_hand(bool a_value) {
        hand_to_hand = a_value;
        logger::trace("set hand to hand to {}"sv, hand_to_hand);
    }
    bool mod_manager::get_hand_to_hand() const { return hand_to_hand; }

}
