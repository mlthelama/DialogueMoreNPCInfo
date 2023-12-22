namespace hook {
    class dialogue_menu_hook final : public RE::DialogueMenu {
    public:
        static void install();

    private:
        RE::UI_MESSAGE_RESULTS process_message_hook(RE::UIMessage& a_message);
        using dialogue_process_message = decltype(&DialogueMenu::ProcessMessage);
        inline static REL::Relocation<dialogue_process_message> process_;

        void advance_movie_hook(float a_interval, std::uint32_t a_current_time);
        using dialogue_advance_movie = decltype(&DialogueMenu::AdvanceMovie);
        inline static REL::Relocation<dialogue_advance_movie> advance_;
    };
}  // hook
