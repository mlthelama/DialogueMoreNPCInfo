#pragma once
#include "hook/dialoguemenu.h"

namespace hook {
    inline void install_hooks() {
        install_dialogue_menu_hook();
        logger::info("Added DialogMenuHook"sv);
    }
}
