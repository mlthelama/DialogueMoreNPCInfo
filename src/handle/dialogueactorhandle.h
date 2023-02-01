#pragma once

namespace handle {
    class dialogue_actor_handle {
    public:
        static dialogue_actor_handle* get_singleton() {
            static dialogue_actor_handle singleton;
            return std::addressof(singleton);
        }

        void init_actor(RE::Actor* a_actor) {
            if (!this->data_) {
                this->data_ = new dialog_actor_handle_data();
            }
            logger::trace("set target actor {}"sv, a_actor->GetName());

            dialog_actor_handle_data* data = this->data_;
            data->actor = a_actor;
        }

        [[nodiscard]] RE::Actor* get_actor() const {
            if (const dialog_actor_handle_data* data = this->data_; data && data->actor) {
                return data->actor;
            }
            return nullptr;
        }

        dialogue_actor_handle(const dialogue_actor_handle&) = delete;
        dialogue_actor_handle(dialogue_actor_handle&&) = delete;

        dialogue_actor_handle& operator=(const dialogue_actor_handle&) = delete;
        dialogue_actor_handle& operator=(dialogue_actor_handle&&) = delete;

    private:
        dialogue_actor_handle() : data_(nullptr) {}

        ~dialogue_actor_handle() = default;

        struct dialog_actor_handle_data {
            RE::Actor* actor = nullptr;
        };

        dialog_actor_handle_data* data_;
    };
}
