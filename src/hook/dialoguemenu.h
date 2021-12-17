#pragma once
#include "handle/dialogueactorhandle.h"
#include "scaleform/menu/dialogueinfomenu.h"

namespace Hook {
    class DialogueMenuHook : public RE::DialogueMenu {
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
                    Handle::DialogueActorHandle::GetSingleton()->initActor(dialogTarget->As<RE::Actor>());
                    auto player = RE::PlayerCharacter::GetSingleton();
                    logger::trace("Player is {}, Target is {}"sv, player->GetName(), dialogTarget->GetName());

                    if (!Scaleform::DialogueInfoMenu::IsMenuOpen()) {
                        Scaleform::DialogueInfoMenu::Open();
                    }
                }
            } else if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kHide) {
                Scaleform::DialogueInfoMenu::Close();
            }

            return _process(this, a_message);
        }

        void AdvanceMovieHook(float a_interval, std::uint32_t a_currentTime) {
            return _advance(this, a_interval, a_currentTime);
        }
    };

    void InstallDialogueMenuHook() {
        //Dialoguemenu
        REL::Relocation<std::uintptr_t> vTable_dm(REL::ID{ 268589 });  //215255 AE
        DialogueMenuHook::_process = vTable_dm.write_vfunc(0x4, &DialogueMenuHook::ProcessMessageHook);
        DialogueMenuHook::_advance = vTable_dm.write_vfunc(0x5, &DialogueMenuHook::AdvanceMovieHook);
    }
}