#pragma once

namespace actor {
    class actor_data {
    public:
        actor_data(const actor_data&) = delete;
        actor_data(actor_data&&) = delete;

        actor_data& operator=(const actor_data&) = delete;
        actor_data& operator=(actor_data&&) = delete;

    private:
        actor_data() = default;
        ~actor_data() = default;
    };
}  // actor
