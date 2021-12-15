#pragma once
#include "scaleform/menu/dialoginfomenu.h"
#include "handle/dialogactorhandle.h"

namespace Hook {
    class DialogMenuHook : public RE::DialogueMenu {
    public:
        using DialogueProcessMessage = decltype(&RE::DialogueMenu::ProcessMessage);
        inline static REL::Relocation<DialogueProcessMessage> _process;

        using DialogueAdvanceMovie = decltype(&RE::DialogueMenu::AdvanceMovie);
        inline static REL::Relocation<DialogueAdvanceMovie> _advance;

        RE::UI_MESSAGE_RESULTS ProcessMessageHook(RE::UIMessage& a_message) {
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kShow) {
                auto dialog = RE::MenuTopicManager::GetSingleton();
                RE::TESObjectREFR* dialogTarget = nullptr;

                if (dialog->speaker && dialog->speaker.get()) {
                    dialogTarget = dialog->speaker.get().get();
                } else if (dialog->lastSpeaker && dialog->lastSpeaker.get()) {
                    dialogTarget = dialog->lastSpeaker.get().get();
                }


                if (dialogTarget && dialogTarget->formType == RE::FormType::ActorCharacter) {
                    Handle::DialogActorHandle::GetSingleton()->initActor(dialogTarget->As<RE::Actor>());
                    auto player = RE::PlayerCharacter::GetSingleton();
                    logger::trace("Player is {}, Target is {}"sv, player->GetName(), dialogTarget->GetName());

                    if (!Scaleform::DialogInfoMenu::IsMenuOpen()) {
                        Scaleform::DialogInfoMenu::Open();
                        auto task = SKSE::GetTaskInterface();
                        task->AddUITask([]() {
                            auto menu = RE::UI::GetSingleton()->GetMenu<Scaleform::DialogInfoMenu>(
                                Scaleform::DialogInfoMenu::MENU_NAME);
                            if (menu) {
                                menu->RefreshItems();
                            }
                        });
                    }
                }
            }

            return _process(this, a_message);
        }

        void AdvanceMovieHook(float a_interval, std::uint32_t a_currentTime) {
            return _advance(this, a_interval, a_currentTime);
        }
    };

    void InstallDialogMenuHook() {
        REL::Relocation<std::uintptr_t> vTable_dm(REL::ID{ 268589 });
        DialogMenuHook::_process = vTable_dm.write_vfunc(0x4, &DialogMenuHook::ProcessMessageHook);
        DialogMenuHook::_advance = vTable_dm.write_vfunc(0x5, &DialogMenuHook::AdvanceMovieHook);
    }
}
