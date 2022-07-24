#pragma once
#include "event/keymanager.h"

namespace event {
    inline void sink_event_handlers() {
        key_manager::sink();
        logger::info("Start listening for InputEvents."sv);
    }
}
