#pragma once
#include "event/menuevent.h"

namespace Event {
    void SinkEventHandlers() {
        MenuEvent::Sink();
        logger::info("Added MenuEvent"sv);
    }
}
