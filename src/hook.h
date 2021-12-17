#pragma once
#include "hook/dialoguemenu.h"

namespace Hook {
    void InstallHooks() {
        Hook::InstallDialogueMenuHook();
        logger::info("Added DialogMenuHook"sv);
    }
}
