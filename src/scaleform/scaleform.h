#pragma once
#include "scaleform/menu/dialoginfomenu.h"

namespace Scaleform {
    void Register() {
        DialogInfoMenu::Register();
        logger::info("Registered all forms");
    }
}
