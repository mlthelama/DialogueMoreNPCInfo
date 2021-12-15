#pragma once
#include "scaleform/menu/dialoginfomenu.h"

class MenuEvent : public RE::BSTEventSink<RE::MenuOpenCloseEvent> {
public:
    using EventResult = RE::BSEventNotifyControl;

    static MenuEvent* GetSingleton() {
        static MenuEvent singleton;
        return std::addressof(singleton);
    }

    static void Sink() {
        RE::UI::GetSingleton()->AddEventSink(
            static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(MenuEvent::GetSingleton()));
    }

protected:
    auto ProcessEvent(RE::MenuOpenCloseEvent const* a_event,
        [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) -> EventResult {
        if (!a_event) {
            return EventResult::kContinue;
        }

        if (a_event->opening && a_event->menuName == RE::DialogueMenu::MENU_NAME) {
            logger::trace("{} is opening"sv, RE::DialogueMenu::MENU_NAME);
            //Scaleform::DialogInfoMenu::Open();
        } else if (!a_event->opening && a_event->menuName == RE::DialogueMenu::MENU_NAME) {
            logger::trace("{} is closing"sv, RE::DialogueMenu::MENU_NAME);
            Scaleform::DialogInfoMenu::Close();
        }

        return EventResult::kContinue;
    }

private:
    MenuEvent() = default;
    MenuEvent(const MenuEvent&) = delete;
    MenuEvent(MenuEvent&&) = delete;
    virtual ~MenuEvent() = default;

    MenuEvent& operator=(const MenuEvent&) = delete;
    MenuEvent& operator=(MenuEvent&&) = delete;
};
