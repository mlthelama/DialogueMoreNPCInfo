#pragma once
#include "scaleform/menu/dialogueinfomenu.h"

namespace Scaleform {
    void Register() {
        DialogueInfoMenu::Register();
        logger::info("Registered all forms");
    }
}
