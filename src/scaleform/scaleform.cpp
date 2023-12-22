#include "scaleform.h"
#include "scaleform/menu/dialogue_info_menu.h"

namespace scaleform {
    void scaleform::Register() {
        dialogue_info_menu::Register();
        logger::info("registered all forms");
    }
}  // scaleform
