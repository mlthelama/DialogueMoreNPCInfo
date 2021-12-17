#pragma once
#include "hook/dialogmenu.h"

namespace Hook {
    void InstallHooks() {
        Hook::InstallDialogMenuHook();
        logger::info("Added DialogMenuHook"sv);
    }
}
