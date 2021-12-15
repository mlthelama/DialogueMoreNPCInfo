#pragma once

namespace Handle {
    class DialogActorHandle {
    public:
        static DialogActorHandle* GetSingleton() {
            static DialogActorHandle singleton;
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
        DialogActorHandle(){};
        ~DialogActorHandle(){};
        DialogActorHandle(const DialogActorHandle&) = delete;
        DialogActorHandle& operator=(const DialogActorHandle&) = delete;

        struct DialogActorHandleData {
            RE::Actor* actor = nullptr;
        };
        DialogActorHandleData* _data;
    };
}
