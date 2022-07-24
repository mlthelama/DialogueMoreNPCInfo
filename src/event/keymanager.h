#pragma once
#include "scaleform/menu/dialogueinfomenu.h"
#include "setting/setting.h"

class key_manager final : public RE::BSTEventSink<RE::InputEvent*> {
public:
    using event_result = RE::BSEventNotifyControl;

    static key_manager* get_singleton() {
        static key_manager singleton;
        return std::addressof(singleton);
    }

    static void sink() { RE::BSInputDeviceManager::GetSingleton()->AddEventSink(get_singleton()); }


    key_manager(const key_manager&) = delete;
    key_manager(key_manager&&) = delete;

    key_manager& operator=(const key_manager&) = delete;
    key_manager& operator=(key_manager&&) = delete;

protected:
    event_result ProcessEvent(RE::InputEvent* const* a_event,
        [[maybe_unused]] RE::BSTEventSource<RE::InputEvent*>* a_event_source) override {
        using event_type = RE::INPUT_EVENT_TYPE;
        using device_type = RE::INPUT_DEVICE;

        key_ = static_cast<uint32_t>(setting::get_toggle_key());

        if (key_ == k_invalid) {
            return event_result::kContinue;
        }

        if (!a_event) {
            return event_result::kContinue;
        }

        const auto ui = RE::UI::GetSingleton();
        const auto intfc_str = RE::InterfaceStrings::GetSingleton();

        if (ui->IsMenuOpen(intfc_str->console)) {
            return event_result::kContinue;
        }

        if (!ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME)) {
            return event_result::kContinue;
        }

        for (auto event = *a_event; event; event = event->next) {
            if (event->eventType != event_type::kButton) {
                continue;
            }

            //this stays static_cast
            const auto button = static_cast<RE::ButtonEvent*>(event);
            // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            if (!button->IsDown()) {
                continue;
            }

            auto key = button->idCode;
            switch (button->device.get()) {
                case device_type::kMouse:
                    key += k_mouse_offset;
                    break;
                case device_type::kKeyboard:
                    key += k_keyboard_offset;
                    break;
                case device_type::kGamepad:
                    key = get_gamepad_index(static_cast<RE::BSWin32GamepadDevice::Key>(key));
                    break;
                default:
                    continue;
            }

            if (const auto control_map = RE::ControlMap::GetSingleton(); !control_map->IsMovementControlsEnabled()) {
                continue;
            }

            /*if the game is not paused with the menu, it triggers the menu always in the background*/
            if (ui->GameIsPaused()) {
                continue;
            }


            if (key == key_) {
                logger::debug("configured Key ({}) pressed"sv, key);
                if (ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME)) {
                    if (scaleform::dialogue_info_menu::is_menu_open()) {
                        scaleform::dialogue_info_menu::close();
                        setting::set_show_window(false);
                    } else {
                        scaleform::dialogue_info_menu::open();
                        setting::set_show_window(true);
                    }
                }
                break;
            }
        }
        return event_result::kContinue;
    }

private:
    key_manager() = default;

    ~key_manager() override = default;

    static uint32_t get_gamepad_index(const RE::BSWin32GamepadDevice::Key a_key) {
        using key = RE::BSWin32GamepadDevice::Key;

        uint32_t index;
        switch (a_key) {
            case key::kUp:
                index = 0;
                break;
            case key::kDown:
                index = 1;
                break;
            case key::kLeft:
                index = 2;
                break;
            case key::kRight:
                index = 3;
                break;
            case key::kStart:
                index = 4;
                break;
            case key::kBack:
                index = 5;
                break;
            case key::kLeftThumb:
                index = 6;
                break;
            case key::kRightThumb:
                index = 7;
                break;
            case key::kLeftShoulder:
                index = 8;
                break;
            case key::kRightShoulder:
                index = 9;
                break;
            case key::kA:
                index = 10;
                break;
            case key::kB:
                index = 11;
                break;
            case key::kX:
                index = 12;
                break;
            case key::kY:
                index = 13;
                break;
            case key::kLeftTrigger:
                index = 14;
                break;
            case key::kRightTrigger:
                index = 15;
                break;
            default:
                index = k_invalid;
                break;
        }

        return index != k_invalid ? index + k_gamepad_offset : k_invalid;
    }

    enum : uint32_t {
        k_invalid = static_cast<uint32_t>(-1),
        k_keyboard_offset = 0,
        k_mouse_offset = 256,
        k_gamepad_offset = 266
    };

    uint32_t key_ = k_invalid;
};
