#include "event.h"
#include "event/input_event.h"

namespace event {
    void event::sink_event_handler() {
        input_event::sink();
        logger::info("added input event sink"sv);
    }
}  // event
