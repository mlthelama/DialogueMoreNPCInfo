#pragma once

namespace Handle {
    class DialogueActorHandle {
    public:
        static DialogueActorHandle* GetSingleton() {
            static DialogueActorHandle singleton;
            return std::addressof(singleton);
        }

        void initActor(RE::Actor* a_actor) {
            if (!this->_data) {
                this->_data = new DialogActorHandleData();
            }
            logger::trace("set target actor {}"sv, a_actor->GetName());

            DialogActorHandleData* data = this->_data;
            data->actor = a_actor;
        }

        RE::Actor* getActor() {
            DialogActorHandleData* data = this->_data;
            if (data && data->actor) {
                return data->actor;
            }
            return nullptr;
        }

    private:
        DialogueActorHandle(){};
        ~DialogueActorHandle(){};
        DialogueActorHandle(const DialogueActorHandle&) = delete;
        DialogueActorHandle& operator=(const DialogueActorHandle&) = delete;

        struct DialogActorHandleData {
            RE::Actor* actor = nullptr;
        };
        DialogActorHandleData* _data;
    };
}
