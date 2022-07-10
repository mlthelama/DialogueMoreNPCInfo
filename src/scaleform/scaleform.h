#pragma once
#include "scaleform/menu/dialogueinfomenu.h"

namespace scaleform {
    inline void Register() {
        dialogue_info_menu::Register();
        logger::info("Registered all forms");
    }
}
