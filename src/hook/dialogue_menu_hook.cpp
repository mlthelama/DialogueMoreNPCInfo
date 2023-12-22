#include "dialogue_menu_hook.h"
#include "handle/dialogueactorhandle.h"
#include "scaleform/menu/dialogue_info_menu.h"
#include "setting/setting.h"

namespace hook {
    void dialogue_menu_hook::install() {
        logger::info("Hooking ..."sv);

        REL::Relocation<std::uintptr_t> dialog_menu_table(RE::VTABLE_DialogueMenu[0]);
        process_ = dialog_menu_table.write_vfunc(0x4, &dialogue_menu_hook::process_message_hook);
        advance_ = dialog_menu_table.write_vfunc(0x5, &dialogue_menu_hook::advance_movie_hook);

        logger::info("Hooked."sv);
    }

    RE::UI_MESSAGE_RESULTS dialogue_menu_hook::process_message_hook(RE::UIMessage& a_message) {
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

                //only show if we are allowed to
                if (setting::get_show_window()) {
                    if (!scaleform::dialogue_info_menu::is_menu_open()) {
                        scaleform::dialogue_info_menu::open();
                    }
                }
            }
        } else if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kHide) {
            scaleform::dialogue_info_menu::close();
        }

        return process_(this, a_message);
    }

    void dialogue_menu_hook::advance_movie_hook(float a_interval, std::uint32_t a_current_time) {
        if (scaleform::dialogue_info_menu::is_menu_open()) {
            const auto task = SKSE::GetTaskInterface();
            task->AddUITask([] {
                if (const auto menu = RE::UI::GetSingleton()->GetMenu<scaleform::dialogue_info_menu>(
                        scaleform::dialogue_info_menu::menu_name)) {
                    menu->refresh_items();
                }
            });
        }
        return advance_(this, a_interval, a_current_time);
    }
    
}  // hook
