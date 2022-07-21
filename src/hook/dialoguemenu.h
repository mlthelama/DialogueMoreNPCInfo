#pragma once
#include "handle/dialogueactorhandle.h"
#include "scaleform/menu/dialogueinfomenu.h"

namespace hook {
    class dialogue_menu_hook final : public RE::DialogueMenu {
    public:
        using dialogue_process_message = decltype(&DialogueMenu::ProcessMessage);
        inline static REL::Relocation<dialogue_process_message> process;

        using dialogue_advance_movie = decltype(&DialogueMenu::AdvanceMovie);
        inline static REL::Relocation<dialogue_advance_movie> advance;

        RE::UI_MESSAGE_RESULTS process_message_hook(RE::UIMessage& a_message) {
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kShow) {
                const auto dialog = RE::MenuTopicManager::GetSingleton();
                RE::TESObjectREFR* dialog_target = nullptr;

                if (dialog->speaker && dialog->speaker.get()) {
                    dialog_target = dialog->speaker.get().get();
                } else if (dialog->lastSpeaker && dialog->lastSpeaker.get()) {
                    dialog_target = dialog->lastSpeaker.get().get();
                }

                if (dialog_target && dialog_target->formType == RE::FormType::ActorCharacter) {
                    handle::dialogue_actor_handle::get_singleton()->init_actor(dialog_target->As<RE::Actor>());
                    const auto player = RE::PlayerCharacter::GetSingleton();
                    logger::trace("Player is {}, Target is {}"sv, player->GetName(), dialog_target->GetName());

                    if (!scaleform::dialogue_info_menu::is_menu_open()) {
                        scaleform::dialogue_info_menu::open();
                    }
                }
            } else if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kHide) {
                scaleform::dialogue_info_menu::close();
            }

            return process(this, a_message);
        }

        void advance_movie_hook(float a_interval, std::uint32_t a_current_time) {
            if (scaleform::dialogue_info_menu::is_menu_open()) {
                const auto task = SKSE::GetTaskInterface();
                task->AddUITask([] {
                    if (const auto menu = RE::UI::GetSingleton()->GetMenu<scaleform::dialogue_info_menu>(
                        scaleform::dialogue_info_menu::menu_name)) {
                        menu->refresh_items();
                    }
                });
            }
            return advance(this, a_interval, a_current_time);
        }
    };

    inline void install_dialogue_menu_hook() {
        //Dialoguemenu
        REL::Relocation<std::uintptr_t> v_table_dm(REL::ID{ offset::get_dialog_menu }); //215255 AE
        dialogue_menu_hook::process = v_table_dm.write_vfunc(0x4, &dialogue_menu_hook::process_message_hook);
        dialogue_menu_hook::advance = v_table_dm.write_vfunc(0x5, &dialogue_menu_hook::advance_movie_hook);
    }
}
